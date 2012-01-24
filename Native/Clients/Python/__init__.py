import os
import ctypes
import json
import collections

# FIXME fix object hierarchy, static variables, visibility

# FIXME Windows
fabric = ctypes.CDLL( os.path.dirname( __file__ ) + '/libFabricPython.so' )

def createClient():
  return CLIENT( fabric )
 
class CLIENT(object):
  def __init__( self, fabric ):
    self.__client_ptr = fabric.createClient()
    self.__queuedCommands = [];
    self.__queuedUnwinds = [];
    self.__queuedCallbacks = [];

    self.GC = GC( self )
    self.MR = MR( self )

  def __del__( self ):
    self.__close()

  def __jsonExec( self, data, length ):
    result = ctypes.c_char_p()
    rlength = ctypes.c_size_t()

    fabric.jsonExec( self.__client_ptr, data, length, ctypes.pointer( result ), ctypes.pointer( rlength ) )

    return result

  def close( self ):
    if self.__client_ptr != None:
      fabric.close( self.__client_ptr )
      self.__client_ptr = None

  def queueCommand( self, dst, cmd, arg = None, unwind = None, callback = None ):
    command = { 'dst': dst, 'cmd': cmd }
    if ( arg != None ):
      command[ 'arg' ] = arg

    self.__queuedCommands.append( command )
    self.__queuedUnwinds.append( unwind )    
    self.__queuedCallbacks.append( callback )    

    self.executeQueuedCommands()

  def executeQueuedCommands( self ):
    commands = self.__queuedCommands;
    self.__queuedCommands = []
    unwinds = self.__queuedUnwinds;
    self.__queuedUnwinds = []
    callbacks = self.__queuedCallbacks;
    self.__queuedCallbacks = []

    jsonEncodedCommands = json.dumps( commands )
    jsonEncodedResults = self.__jsonExec( jsonEncodedCommands, len( jsonEncodedCommands ) )

    # FIXME exception handling
    print "RESULTS: "+jsonEncodedResults.value
    results = json.loads( jsonEncodedResults.value )

    for i in range(len(results)):
      result = results[i]
      callback = callbacks[i]
      #if ( 'exception' in result ):
        # FIXME do unwinds
      if ( callback != None ):
        callback( result[ 'result' ] )

    fabric.freeString( jsonEncodedResults )

#  def test( self ):
#    self.__jsonExec( '[{"dst":["MR"],"cmd":"createConstValue","arg":{"id":"GC_0","valueType":"Size","data":10000}},{"dst":["MR","GC_0"],"cmd":"produce","arg":null}]', 142 )

class MR( object ):
  def __init__( self, client ):
    self.__client = client

  def createConstValue( self, valueType, data ):
    value = VALUEPRODUCER( self.__client )
    arg = { 'id': value.getID(), 'valueType': valueType, 'data': data }
    self.__client.queueCommand( ['MR'], 'createConstValue', arg, value.unwind )
    return value

  def createValueCache( self, input ):
    value = VALUEPRODUCER( self.__client )
    arg = { 'id': value.getID(), 'inputID': input.getID() }
    self.__client.queueCommand( ['MR'], 'createValueCache', arg, value.unwind )
    return value

class GCOBJECT( object ):
  def __init__( self, client, namespace ):
    self.__id = "GC_" + str(client.GC.getNextID())
    self.__nextCallbackID = 0
    self.__callbacks = {}
    self.__namespace = namespace
    self._client = client
    client.GC.addObject( self )

  def __del__( self ):
    self.__dispose()

  def __dispose( self ):
    self._queueCommand( 'dispose' )
    self._client.GC.disposeObject( self )
    self.__id = None
 
  def _queueCommand( self, cmd, arg, unwind, callback ):
    if self.__id == None:
      raise Exception( "GC object has already been disposed" )
    self._client.queueCommand( [self.__namespace, self.__id], cmd, arg, unwind, callback )

  def __registerCallback( self, callback ):
    self.__nextCallbackID = self.__nextCallbackID + 1
    callbackID = self.__nextCallbackID
    self.__callbacks[ callbackID ] = callback
    return callbackID

  # FIXME what is the visibility of route?
  def route( self, cmd, arg ):
    callback = self.__callbacks[ arg[ 'serial' ] ]
    del self.__callbacks[ arg[ 'serial' ] ]
    callback( arg[ 'result' ] )

  def getID( self ):
    return self.__id

class PRODUCER( GCOBJECT ):
  def __init__( self, client ):
    super( PRODUCER, self ).__init__( client, 'MR' )

  def __toJSON( self, result ):
    self.__json = result

  def toJSON( self ):
    # dictionary hack to simulate Python 3.x nonlocal
    json = { '_': None }
    def __toJSON( result ):
      json[ '_' ] = data

    self._queueCommand( 'toJSON', None, None, __toJSON )
    return json[ '_' ]

class VALUEPRODUCER( PRODUCER ):
  def __init__( self, client ):
    super( VALUEPRODUCER, self ).__init__( client )

  def produce( self ):
    # dictionary hack to simulate Python 3.x nonlocal
    result = { '_': None }
    def __produce( data ):
      result[ '_' ] = data

    self._queueCommand( 'produce', None, None, __produce )
    self._client.executeQueuedCommands()

    return result[ '_' ]

  def produceAsync( self, callback ):
    self._queueCommand( 'produceAsync', self.__registerCallback( callback ) )
    self._client.executeQueuedCommands()

  def flush( self ):
    self._queueCommand( 'flush' )

  # FIXME this doesn't make sense, should this be disposing?
  def unwind( self ):
    self.__id = None

class GC( object ):
  def __init__( self, client ):
    self.__nextID = 0
    self.__objects = {}
    self.__client = client

  def getNextID( self ):
    id = self.__nextID
    self.__nextID = self.__nextID + 1
    return id

  def addObject( self, obj ):
    self.__objects[ obj.getID() ] = obj

  def disposeObject( self, obj ):
    del self.__objects[ obj.getID() ]

  def route( src, cmd, arg ):
    src = collections.deque( src )
    id = src.popleft()
    obj = self.__objects[ id ]
    obj.route( src, cmd, arg )

