import os
import json
import ctypes
import collections

# FIXME fix object hierarchy, static variables, visibility

# FIXME Windows
if os.name == 'posix':
  fabric = ctypes.CDLL( os.path.dirname( __file__ ) + '/libFabricPython.so' )
else:
  raise Exception('not implemented for Windows yet!')

def createClient():
  return INTERFACE( fabric )

# this is the interface object that gets returned to the user
class INTERFACE( object ):
  def __init__( self, fabric ):
    self.__client = CLIENT( fabric )
    self.KLC = KLC( self.__client )
    self.MR = MR( self.__client )
    self.RT = RT( self.__client )

  def flush( self ):
    self.__client.executeQueuedCommands();

  def close( self ):
    self.__client.close()

  def getMemoryUsage( self ):
    # dictionary hack to simulate Python 3.x nonlocal
    memoryUsage = { '_': None }
    def __getMemoryUsage( result ):
      memoryUsage[ '_' ] = result

    self.__client.queueCommand( [], 'getMemoryUsage', None, None, __getMemoryUsage )
    self.flush()

    return memoryUsage[ '_' ]

class CLIENT( object ):
  def __init__( self, fabric ):
    self.__fabricClient = fabric.createClient()
    self.__queuedCommands = [];
    self.__queuedUnwinds = [];
    self.__queuedCallbacks = [];
    self.GC = GC( self )
    self.__registerNotifyCallback()

  def __del__( self ):
    self.close()

  def __jsonExec( self, data, length ):
    result = ctypes.c_char_p()
    rlength = ctypes.c_size_t()

    fabric.jsonExec(
      self.__fabricClient,
      data,
      length,
      ctypes.pointer( result ),
      ctypes.pointer( rlength )
    )

    # FIXME we're ignoring rlength for now
    return result

  def close( self ):
    if self.__fabricClient != None:
      fabric.close( self.__fabricClient )
      self.__fabricClient = None

  def queueCommand( self, dst, cmd, arg = None, unwind = None, callback = None ):
    command = { 'dst': dst, 'cmd': cmd }
    if ( arg is not None ):
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
    results = json.loads( jsonEncodedResults.value )

    for i in range(len(results)):
      result = results[i]
      callback = callbacks[i]
      #if ( 'exception' in result ):
        # FIXME do unwinds
      if ( callback is not None ):
        callback( result[ 'result' ] )

    fabric.freeString( jsonEncodedResults )

  def __route( self, src, cmd, arg ):
    if len(src) == 0:
      self.__handle( cmd, arg )
    else:
      src = collections.deque( src )
      firstSrc = src.popleft()

      if firstSrc == 'RT':
        pass
      elif firstSrc == 'DG':
        pass
      elif firstSrc == 'EX':
        pass
      elif firstSrc == 'IO':
        pass
      elif firstSrc == 'VP':
        pass
      elif firstSrc == 'GC':
        pass
      else:
        raise Exception( 'unroutable src: ' + firstSrc )
        
  def __notifyCallback( self, jsonEncodedNotifications ):
    try:
      notifications = json.loads( jsonEncodedNotifications )
    except Exception:
      raise Exception( 'unable to parse JSON notifications' )

    for i in range( 0, len( notifications ) ):
      n = notifications[ i ]
      self.__route( n[ 'src' ], n[ 'cmd' ], n[ 'arg' ] )

  def __getNotifyCallback( self ):
    # use a closure here so that 'self' is maintained without us
    # explicitly passing it
    NOTIFYCALLBACK = ctypes.CFUNCTYPE( None, ctypes.c_char_p )
    def notifyCallback( jsonEncodedNotifications ):
      self.__notifyCallback( jsonEncodedNotifications )
    return NOTIFYCALLBACK( notifyCallback )

  def __registerNotifyCallback( self ):
    fabric.setJSONNotifyCallback( self.__fabricClient, self.__getNotifyCallback() )

class MR( object ):
  def __init__( self, client ):
    self.__client = client

  def createConstArray( self, elementType, data = None ):
    valueArray = ARRAYPRODUCER( self.__client )

    arg = { 'id': valueArray.getID() }
    if type( elementType ) is str:
      arg[ 'elementType' ] = elementType
      arg[ 'data' ] = data
    elif type ( elementType ) is dict:
      inputArg = elementType
      arg[ 'elementType' ] = inputArg[ 'elementType' ]
      if 'data' in inputArg:
        arg[ 'data' ] = inputArg[ 'data' ]
      if 'jsonData' in inputArg:
        arg[ 'jsonData' ] = inputArg[ 'jsonData' ]
    else:
      raise Exception( "createConstArray: first argument must be str or dict" )

    self.__client.queueCommand( ['MR'], 'createConstArray', arg, valueArray.unwind )
    return valueArray

  def createConstValue( self, valueType, data ):
    value = VALUEPRODUCER( self.__client )
    arg = {
      'id': value.getID(),
      'valueType': valueType,
      'data': data
    }
    self.__client.queueCommand( ['MR'], 'createConstValue', arg, value.unwind )
    return value

  def createValueCache( self, input ):
    return self.__createMRCommand( VALUEPRODUCER( self.__client ), 'createValueCache', input, None, None )

  def createValueGenerator( self, operator ):
    return self.__createMRCommand( VALUEPRODUCER( self.__client ), 'createValueGenerator', None, operator, None )

  def createValueMap( self, input, operator, shared = None ):
    return self.__createMRCommand( VALUEPRODUCER( self.__client ), 'createValueMap', input, operator, shared )

  def createValueTransform( self, input, operator, shared = None ):
    return self.__createMRCommand( VALUEPRODUCER( self.__client ), 'createValueTransform', input, operator, shared )

  def createArrayCache( self, input ):
    return self.__createMRCommand( ARRAYPRODUCER( self.__client ), 'createArrayCache', input, None, None )

  def createArrayGenerator( self, count, operator, shared = None ):
    obj = ARRAYPRODUCER( self.__client )
    arg = {
      'id': obj.getID(),
      'countID': count.getID(),
      'operatorID': operator.getID()
    }
    if ( shared is not None ):
      arg[ 'sharedID' ] = shared.getID()

    self.__client.queueCommand( ['MR'], 'createArrayGenerator', arg, obj.unwind )
    return obj

  def createArrayMap( self, input, operator, shared = None ):
    return self.__createMRCommand( ARRAYPRODUCER( self.__client ), 'createArrayMap', input, operator, shared )

  def createArrayTransform( self, input, operator, shared = None ):
    return self.__createMRCommand( ARRAYPRODUCER( self.__client ), 'createArrayTransform', input, operator, shared )

  def createReduce( self, inputArrayProducer, reduceOperator, sharedValueProducer = None ):
    reduce = VALUEPRODUCER( self.__client )
    arg = {
      'id': reduce.getID(),
      'inputArrayProducerID': inputArrayProducer.getID(),
      'reduceOperatorID': reduceOperator.getID()
    }
    if ( sharedValueProducer is not None ):
      arg[ 'sharedValueProducerID' ] = sharedValueProducer.getID()

    self.__client.queueCommand( ['MR'], 'createReduce', arg, reduce.unwind )
    return reduce

  def __createMRCommand( self, obj, cmd, input, operator, shared ):
    arg = {
      'id': obj.getID()
    }
    if ( input is not None ):
      arg[ 'inputID' ] = input.getID()
    if ( operator is not None ):
      arg[ 'operatorID' ] = operator.getID()
    if ( shared is not None ):
      arg[ 'sharedID' ] = shared.getID()

    self.__client.queueCommand( ['MR'], cmd, arg, obj.unwind )
    return obj

class KLC( object ):
  def __init__( self, client ):
    self.__client = client

  def __createOperator( self, sourceName, sourceCode, operatorName, cmd ):
    operator = OPERATOR( self.__client )
    arg = {
      'id': operator.getID(),
      'sourceName': sourceName,
      'sourceCode': sourceCode,
      'operatorName': operatorName
    }

    self.__client.queueCommand( ['KLC'], cmd, arg, operator.unwind )
    return operator

  def createReduceOperator( self, sourceName, sourceCode, operatorName ):
    return self.__createOperator( sourceName, sourceCode, operatorName, 'createReduceOperator' )

  def createValueGeneratorOperator( self, sourceName, sourceCode, operatorName ):
    return self.__createOperator( sourceName, sourceCode, operatorName, 'createValueGeneratorOperator' )

  def createValueMapOperator( self, sourceName, sourceCode, operatorName ):
    return self.__createOperator( sourceName, sourceCode, operatorName, 'createValueMapOperator' )

  def createValueTransformOperator( self, sourceName, sourceCode, operatorName ):
    return self.__createOperator( sourceName, sourceCode, operatorName, 'createValueTransformOperator' )

  def createArrayGeneratorOperator( self, sourceName, sourceCode, operatorName ):
    return self.__createOperator( sourceName, sourceCode, operatorName, 'createArrayGeneratorOperator' )

  def createArrayMapOperator( self, sourceName, sourceCode, operatorName ):
    return self.__createOperator( sourceName, sourceCode, operatorName, 'createArrayMapOperator' )

  def createArrayTransformOperator( self, sourceName, sourceCode, operatorName ):
    return self.__createOperator( sourceName, sourceCode, operatorName, 'createArrayTransformOperator' )

class RT( object ):
  def __init__( self, client ):
    self.__client = client

  #def getRegisteredTypes( self ):

  def registerType( self, name, desc ):
    members = [ ]
    for descMemberName in desc[ 'members' ]:
      member = {
        'name': descMemberName,
        'type': desc[ 'members' ][ descMemberName ]
      }
      members.append( member )

    defaultValue = desc[ 'constructor' ]()

    arg = {
      'name': name,
      'members': members,
      'defaultValue': defaultValue
    }
    if ( 'klBindings' in desc ):
      arg[ 'klBindings' ] = desc[ 'klBindings' ]

    def __queueCommandUnwind():
      pass
      # FIXME unwind
      #del RT.prototypes[ name ]

    self.__client.queueCommand( ['RT'], 'registerType', arg, __queueCommandUnwind )

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
 
  def _queueCommand( self, cmd, arg = None, unwind = None, callback = None ):
    if self.__id is None:
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

  def setID( self, id ):
    self.__id = id

class PRODUCER( GCOBJECT ):
  def __init__( self, client ):
    super( PRODUCER, self ).__init__( client, 'MR' )

  # FIXME this doesn't make sense, should this be disposing?
  def unwind( self ):
    self.setID( None )

  def toJSON( self ):
    # dictionary hack to simulate Python 3.x nonlocal
    json = { '_': None }
    def __toJSON( result ):
      json[ '_' ] = result

    self._queueCommand( 'toJSON', None, None, __toJSON )
    return json[ '_' ]

class ARRAYPRODUCER( PRODUCER ):
  def __init__( self, client ):
    super( ARRAYPRODUCER, self ).__init__( client )

  def getCount( self ):
    # dictionary hack to simulate Python 3.x nonlocal
    count = { '_': None }
    def __getCount( result ):
      count[ '_' ] = result

    self._queueCommand( 'getCount', None, None, __getCount )
    return count[ '_' ]

  def produce( self, index = None, count = None ):
    arg = { }
    if ( index is not None ):
      if ( count is not None ):
        arg[ 'count' ] = count
      arg[ 'index' ] = index

    # dictionary hack to simulate Python 3.x nonlocal
    result = { '_': None }
    def __produce( data ):
      result[ '_' ] = data

    self._queueCommand( 'produce', arg, None, __produce )
    self._client.executeQueuedCommands()
    return result[ '_' ]

  def flush( self ):
    self._queueCommand( 'flush' )

  # FIXME implement produceAsync
  #def produceAsync( self, b0

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

class OPERATOR( GCOBJECT ):
  def __init__( self, client ):
    super( OPERATOR, self ).__init__( client )

  # FIXME this is identical to producer's toJSON method
  def toJSON( self ):
    # dictionary hack to simulate Python 3.x nonlocal
    json = { '_': None }
    def __toJSON( result ):
      json[ '_' ] = result

    self._queueCommand( 'toJSON', None, None, __toJSON )
    return json[ '_' ]

  def getDiagnostics( self ):
    # dictionary hack to simulate Python 3.x nonlocal
    diagnostics = { '_': None }
    def __getDiagnostics( result ):
      diagnostics[ '_' ] = result

    self._queueCommand( 'getDiagnostics', None, None, __getDiagnostics )
    return diagnostics[ '_' ]

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

