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
  return _INTERFACE( fabric )

# this is the interface object that gets returned to the user
class _INTERFACE( object ):
  def __init__( self, fabric ):
    self.__client = _CLIENT( fabric )
    self.KLC = self.__client.klc
    self.MR = self.__client.mr
    self.RT = self.__client.rt
    self.DG = self.__client.dg
    self.DependencyGraph = self.DG

  def flush( self ):
    self.__client.executeQueuedCommands()

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

class _CLIENT( object ):
  def __init__( self, fabric ):
    self.__fabricClient = fabric.createClient()
    self.__queuedCommands = []
    self.__queuedUnwinds = []
    self.__queuedCallbacks = []

    self.gc = _GC( self )
    self.klc = _KLC( self )
    self.mr = _MR( self )
    self.rt = _RT( self )
    self.dg = _DG( self )

    self.__NOTIFYCALLBACK = ctypes.CFUNCTYPE( None, ctypes.c_char_p )
    self.__registerNotifyCallback()

  def __del__( self ):
    self.close()

  def __jsonExec( self, data, length ):
    result = ctypes.c_char_p()

    fabric.jsonExec(
      self.__fabricClient,
      data,
      length,
      ctypes.pointer( result )
    )

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

    # FIXME leaving this in for debug only
    self.executeQueuedCommands()

  def executeQueuedCommands( self ):
    commands = self.__queuedCommands
    self.__queuedCommands = []
    unwinds = self.__queuedUnwinds
    self.__queuedUnwinds = []
    callbacks = self.__queuedCallbacks
    self.__queuedCallbacks = []

    jsonEncodedCommands = json.dumps( commands )
    jsonEncodedResults = self.__jsonExec( jsonEncodedCommands, len( jsonEncodedCommands ) )

    try:
      results = json.loads( jsonEncodedResults.value )
    except Exception:
      raise Exception( 'unable to parse JSON results: ' + jsonEncodedResults )

    for i in range(len(results)):
      result = results[i]
      callback = callbacks[i]

      if ( 'exception' in result ):
        for j in reverse( range( i+1, j ) ):
          unwind = unwinds[ j ]
          if ( unwind is not None ):
            unwind()
      elif ( callback is not None ):
        callback( result[ 'result' ] )

    fabric.freeString( self.__fabricClient, jsonEncodedResults )

  def _route( self, src, cmd, arg ):
    if len(src) == 0:
      self.__handle( cmd, arg )
    else:
      src = collections.deque( src )
      firstSrc = src.popleft()

      if firstSrc == 'RT':
        self.rt._route( src, cmd, arg )
      elif firstSrc == 'DG':
        self.dg._route( src, cmd, arg )
      elif firstSrc == 'EX':
        pass
      elif firstSrc == 'IO':
        pass
      elif firstSrc == 'VP':
        pass
      elif firstSrc == 'GC':
        self.gc._route( src, cmd, arg )
      else:
        raise Exception( 'unroutable src: ' + firstSrc )
        
  def __notifyCallback( self, jsonEncodedNotifications ):
    try:
      notifications = json.loads( jsonEncodedNotifications )
    except Exception:
      raise Exception( 'unable to parse JSON notifications' )

    for i in range( 0, len( notifications ) ):
      n = notifications[ i ]
      self._route( n[ 'src' ], n[ 'cmd' ], n[ 'arg' ] )

  def __getNotifyCallback( self ):
    # use a closure here so that 'self' is maintained without us
    # explicitly passing it
    def notifyCallback( jsonEncodedNotifications ):
      self.__notifyCallback( jsonEncodedNotifications )

    # this is important, we have to maintain a reference to the CFUNCTYPE
    # ptr and not just return it, otherwise it will be garbage collected
    # and callbacks will fail
    self.__CFUNCTYPE_notifyCallback = self.__NOTIFYCALLBACK ( notifyCallback )
    return self.__CFUNCTYPE_notifyCallback

  def __registerNotifyCallback( self ):
    fabric.setJSONNotifyCallback( self.__fabricClient, self.__getNotifyCallback() )

class _GCOBJECT( object ):
  def __init__( self, nsobj ):
    self.__id = "GC_" + str(nsobj._getClient().gc.getNextID())
    self.__nextCallbackID = 0
    self.__callbacks = {}
    self._nsobj = nsobj
    nsobj._getClient().gc.addObject( self )

  def __del__( self ):
    self.__dispose()

  def __dispose( self ):
    self._gcObjQueueCommand( 'dispose' )
    self.__nsobj._getClient().gc.disposeObject( self )
    self.__id = None
 
  def _gcObjQueueCommand( self, cmd, arg = None, unwind = None, callback = None ):
    if self.__id is None:
      raise Exception( "GC object has already been disposed" )
    self._nsobj._objQueueCommand( self.__id, cmd, arg, unwind, callback )

  def _registerCallback( self, callback ):
    self.__nextCallbackID = self.__nextCallbackID + 1
    callbackID = self.__nextCallbackID
    self.__callbacks[ callbackID ] = callback
    return callbackID

  def _route( self, src, cmd, arg ):
    callback = self.__callbacks[ arg[ 'serial' ] ]
    del self.__callbacks[ arg[ 'serial' ] ]
    callback( arg[ 'result' ] )

  def getID( self ):
    return self.__id

  def setID( self, id ):
    self.__id = id

  def unwind( self ):
    self.setID( None )

class _NAMESPACE( object ):
  def __init__( self, client, name ):
    self.__client = client
    self.__name = name

  def _getClient( self ):
    return self.__client

  def _getName( self ):
    return self.__namespace

  def _objQueueCommand( self, dst, cmd, arg = None, unwind = None, callback = None ):
    if dst is not None:
      dst = [ self.__name, dst ]
    else:
      dst = [ self.__name ]
    self.__client.queueCommand( dst, cmd, arg, unwind, callback )

  def _queueCommand( self, cmd, arg = None, unwind = None, callback = None ):
    self._objQueueCommand( None, cmd, arg, unwind, callback )

  def _executeQueuedCommands( self ):
    self.__client.executeQueuedCommands()

class _DG( _NAMESPACE ):
  def __init__( self, client ):
    super( _DG, self ).__init__( client, 'DG' )
    self._namedObjects = []

  def createBinding( self ):
    return self._BINDING()

  def createBindingList( self, dst ):
    return self._BINDINGLIST( self, dst )

  def __createNamedObject( self, name, cmd, objType ):
    if name in self._namedObjects:
      raise Exception( 'a NamedObject named "' + name + '" already exists' )
    obj = objType( self, name )
    self._namedObjects[ name ] = obj
    
    def __unwind():
      obj._destroy()

    self._queueCommand( cmd, name, __unwind )

    return obj

  def createNamedObject( self, name ):
    return self.__createNamedObject( name, self._NAMEDOBJECT )

  def createOperator( self, name ):
    return self.__createNamedObject( name, self._OPERATOR )
   
  def createNode( self, name ):
    return self.__createNamedObject( name, self._NODE )

  def _route( self, src, cmd, arg ):
    pass

  class _NAMEDOBJECT( object ):
    def __init__( self, dg, name ):
      self.__name = name
      self.__errors = None
      self._dg = dg
  
    def _nObjQueueCommand( self, cmd, arg, unwind, callback ):
      if self.__name is None:
        raise Exception( 'NamedObject "' + name + '" has been deleted' )
      self._dg._objQueueCommand( [ self.__name ], cmd, arg, unwind, callback )
  
    def _patch( self, diff ):
      if 'errors' in diff:
        self.__errors = diff[ 'errors' ]
  
    def _destroy( self ):
      del self._dg._namedObjects[ name ]
      self.__name = None
  
    def _handle( self, cmd, arg ):
      if cmd == 'delta':
        self._patch( arg )
      else:
        raise Exception( 'command "' + cmd + '" not recognized' )
  
    def _route( self, src, cmd, arg ):
      if len( src ) == 0:
        self.__handle( cmd, arg )
      else:
        raise Exception( 'unroutable' )
  
    def getName( self ):
      return self.__name
  
    def getErrors( self ):
      self._dg._executeQueuedCommands()
      return self.__errors
  
  class _BINDINGLIST( object ):
    def __init__( self, dg, dst ):
      self.__bindings = []
      self._dg = dg
      self.__dst = dst
   
    def empty( self ):
      if self.__bindings is None:
        self._dg._executeQueuedCommands()
      return len( self.__bindings ) == 0
  
    def getLength( self ):
      if self.__bindings is None:
        self._dg._executeQueuedCommands()
      return len( self.__bindings )
  
    def getOperator( self, index ):
      if self.__bindings is None:
        self._dg._executeQueuedCommands()
      return self.__bindings[ index ].getOperator()
      
    def append( self, binding ):
      operatorName = None
      try:
        operatorName = binding.getOperator().getName()
      except Exception:
        raise 'operator: not an operator'
  
      oldBindings = self.__bindings
      self.__bindings = None
  
      def __unwind():
        self.__bindings = oldBindings
      args = {
        'operatorName': operatorName,
        'parameterLayout': binding.getParameterLayout()
      }
      self._dg._objQueueCommand( self.__dst, 'append', args, __unwind )
  
    def insert( self, binding, beforeIndex ):
      operatorName = None
      try:
        operatorName = binding.getOperator().getName()
      except Exception:
        raise 'operator: not an operator'
  
      if type( beforeIndex ) is not int:
        raise Exception( 'beforeIndex: must be an integer' )
  
      oldBindings = self.__bindings
      self.__bindings = None
      
      def __unwind():
        self.__bindings = oldBindings
      args = {
        'beforeIndex': beforeIndex,
        'operatorName': operatorName,
        'parameterLayout': binding.getParameterLayout()
      }
      self._dg._objQueueCommand( self.__dst, 'insert', args, __unwind )
  
    def remove( self, index ):
      oldBindings = self.__bindings
      self.__bindings = None
      
      def __unwind():
        self.__bindings = oldBindings
      args = {
        'index': index,
      }
      self._dg._objQueueCommand( self.__dst, 'remove', args, __unwind )
  
  class _BINDING( object ):
    def __init__( self ):
      self.__operator = None
      self.__parameterLayout = None
  
    def getOperator( self ):
      return self.__operator
  
    def setOperator( self, operator ):
      self.__operator = operator
  
    def getParameterLayout( self ):
      return self.__parameterLayout
  
    def setParameterLayout( self, parameterLayout ):
      self.__parameterLayout = parameterLayout
  
  class _OPERATOR( _NAMEDOBJECT ):
    def __init__( self, dg, name ):
      super( _DG._OPERATOR, self ).__init__( dg, name )
      self.__diagnostics = []
      self.__filename = None
      self.__sourceCode = None
      self.__entryFunctionName = None
      self.__mainThreadOnly = None

    def _patch( self, diff ):
      super( _OPERATOR, self )._patch( diff )

      if 'filename' in diff:
        self.__filename = diff.filename

      if 'sourceCode' in diff:
        self.__sourceCode = diff.sourceCode

      if 'entryFunctionName' in diff:
        self.__entryFunctionName = diff.entryFunctionName

      if 'diagnostics' in diff:
        self.__diagnostics = diff.diagnostics

      if 'mainThreadOnly' in diff:
        self.__mainThreadOnly = diff.mainThreadOnly

    def getMainThreadOnly( self ):
      if self.__mainThreadOnly is None:
        self._dg._executeQueuedCommands()
      return self.__mainThreadOnly

    def setMainThreadOnly( self, mainThreadOnly ):
      oldMainThreadOnly = self.__mainThreadOnly
      self.__mainThreadOnly = mainThreadOnly

      def __unwind():
        self.__mainThreadOnly = oldMainThreadOnly

      self._nObjQueueCommand( 'setMainThreadOnly', mainThreadOnly, __unwind )

    def getFilename( self ):
      if self.__filename is None:
        self._dg._executeQueuedCommands()
      return self.__filename

    def getSourceCode( self ):
      if self.__sourceCode is None:
        self._dg._executeQueuedCommands()
      return self.__sourceCode

    def setSourceCode( self, filename, sourceCode = None ):
      # this is legacy usage, sourceCode only
      if sourceCode is None:
        sourceCode = filename
        filename = "(unknown)"

      oldFilename = self.__filename
      self.__filename = filename
      oldSourceCode = self.__sourceCode
      self.__sourceCode = sourceCode
      oldDiagnostics = self.__diagnostics
      self.__diagnostics = None

      def __unwind():
        self.__filename = oldFilename
        self.__sourceCode = oldSourceCode
        self.__diagnostics = oldDiagnostics

      args = {
        'filename': filename,
        'sourceCode': sourceCode
      }
      self._nObjQueueCommand( 'setSourceCode', args, __unwind )

    def getEntryFunctionName( self ):
      if self.__entryFunctionName is None:
        self._dg._executeQueuedCommands()
      return self.__entryFunctionName

    def setEntryFunctionName( self, entryFunctionName ):
      oldEntryFunctionName = self.__entryFunctionName
      self.__entryFunctionName = entryFunctionName

      def __unwind():
        self.__entryFunctionName = oldEntryFunctionName
      self._nObjQueueCommand( 'setEntryFunctionName', entryFunctionName, __unwind )
      self.__diagnostics = None

    def getDiagnostics( self ):
      if self.__diagnostics is None:
        self._dg._executeQueuedCommands()
      return self.__diagnostics

  class _CONTAINER( _NAMEDOBJECT ):
    def __init__( self, dg, name ):
      super( _DG._CONTAINER, self ).__init__( dg, name )
      self.__members = None
      self.__count = None

    def _patch( self, diff ):
      super( _CONTAINER, self )._patch( diff )

      if 'members' in diff:
        self.__members = diff.members

      if 'count' in diff:
        self.__count = diff.count

    def _handle( self, cmd, arg ):
      if cmd == 'dataChange':
        memberName = arg[ 'memberName' ]
        sliceIndex = arg[ 'sliceIndex' ]
        # FIXME invalidate cache here, see pzion comment in node.js
      else:
        # FIXME what happens if this calls _patch?
        super( _CONTAINER, self )._handle( cmd, arg )

    def getCount( self ):
      if self.__count is None:
        self._dg._executeQueuedCommands()
      return self.__count

    def setCount( self, count ):
      self._nObjQueueCommand( 'setCount', count )
      self.__count = None

    def getMembers( self ):
      if self.__members is None:
        self._dg._executeQueuedCommands()
      return self.__members

    def addMember( self, memberName, memberType, defaultValue ):
      if self.__members is None:
        self.__members = {}
      if memberName in self.__members:
        raise Exception( 'there is already a member named "' + memberName + '"' )

      arg = { 'name': memberName, 'type': memberType }
      if defaultValue is not None:
        arg[ 'defaultValue' ] = defaultValue

      self.__members[ memberName ] = arg

      def __unwind():
        del self.__members[ memberName ]

      self._nObjQueueCommand( 'addMember', arg, __unwind )

    def removeMember( self, memberName ):
      if self.__members is None or memberName not in self.__members:
        raise Exception( 'there is no member named "' + memberName + '"' )

      oldMember = self.__members[ memberName ]
      del self.__members[ memberName ]

      def __unwind():
        self.__members[ memberName ] = oldMember

      self._nObjQueueCommand( 'removeMember', memberName, __unwind )

  class _NODE( _CONTAINER ):
    def __init__( self, dg, name ):
      super( _DG._NODE, self ).__init__( dg, name )
      self.__dependencies = {}
      self.__bindings = dg.createBindingList( [ name, 'bindings' ] )
  
class _MR( _NAMESPACE ):
  def __init__( self, client ):
    super( _MR, self ).__init__( client, 'MR' )

  def createConstArray( self, elementType, data = None ):
    valueArray = self._ARRAYPRODUCER( self )

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

    self._queueCommand( 'createConstArray', arg, valueArray.unwind )
    return valueArray

  def createConstValue( self, valueType, data ):
    value = self._VALUEPRODUCER( self )
    arg = {
      'id': value.getID(),
      'valueType': valueType,
      'data': data
    }
    self._queueCommand( 'createConstValue', arg, value.unwind )
    return value

  def createValueCache( self, input ):
    return self.__createMRCommand( self._VALUEPRODUCER( self ), 'createValueCache', input, None, None )

  def createValueGenerator( self, operator ):
    return self.__createMRCommand( self._VALUEPRODUCER( self ), 'createValueGenerator', None, operator, None )

  def createValueMap( self, input, operator, shared = None ):
    return self.__createMRCommand( self._VALUEPRODUCER( self ), 'createValueMap', input, operator, shared )

  def createValueTransform( self, input, operator, shared = None ):
    return self.__createMRCommand( self._VALUEPRODUCER( self ), 'createValueTransform', input, operator, shared )

  def createArrayCache( self, input ):
    return self.__createMRCommand( self._ARRAYPRODUCER( self ), 'createArrayCache', input, None, None )

  def createArrayGenerator( self, count, operator, shared = None ):
    obj = self._ARRAYPRODUCER( self )
    arg = {
      'id': obj.getID(),
      'countID': count.getID(),
      'operatorID': operator.getID()
    }
    if ( shared is not None ):
      arg[ 'sharedID' ] = shared.getID()

    self._queueCommand( 'createArrayGenerator', arg, obj.unwind )
    return obj

  def createArrayMap( self, input, operator, shared = None ):
    return self.__createMRCommand( self._ARRAYPRODUCER( self ), 'createArrayMap', input, operator, shared )

  def createArrayTransform( self, input, operator, shared = None ):
    return self.__createMRCommand( self._ARRAYPRODUCER( self ), 'createArrayTransform', input, operator, shared )

  def createReduce( self, inputArrayProducer, reduceOperator, sharedValueProducer = None ):
    reduce = self._VALUEPRODUCER( self )
    arg = {
      'id': reduce.getID(),
      'inputArrayProducerID': inputArrayProducer.getID(),
      'reduceOperatorID': reduceOperator.getID()
    }
    if ( sharedValueProducer is not None ):
      arg[ 'sharedValueProducerID' ] = sharedValueProducer.getID()

    self._queueCommand( 'createReduce', arg, reduce.unwind )
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

    self._queueCommand( cmd, arg, obj.unwind )
    return obj

  class _PRODUCER( _GCOBJECT ):
    def __init__( self, mr ):
      super( _MR._PRODUCER, self ).__init__( mr )
  
    def toJSON( self ):
      # dictionary hack to simulate Python 3.x nonlocal
      json = { '_': None }
      def __toJSON( result ):
        json[ '_' ] = result
  
      self._gcObjQueueCommand( 'toJSON', None, None, __toJSON )
      return json[ '_' ]
  
  class _ARRAYPRODUCER( _PRODUCER ):
    def __init__( self, mr ):
      super( _MR._ARRAYPRODUCER, self ).__init__( mr )
  
    def getCount( self ):
      # dictionary hack to simulate Python 3.x nonlocal
      count = { '_': None }
      def __getCount( result ):
        count[ '_' ] = result
  
      self._gcObjQueueCommand( 'getCount', None, None, __getCount )
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
  
      self._gcObjQueueCommand( 'produce', arg, None, __produce )
      self._nsobj._executeQueuedCommands()
      return result[ '_' ]
  
    def flush( self ):
      self._gcObjQueueCommand( 'flush' )
  
    def produceAsync( self, arg1, arg2 = None, arg3 = None ):
      arg = { }
      callback = None
      if arg3 is None and arg2 is None:
        callback = arg1
      elif arg3 is None:
        arg[ 'index' ] = arg1
        callback = arg2
      else:
        arg[ 'index' ] = arg1
        arg[ 'count' ] = arg2
        callback = arg3
  
      arg[ 'serial' ] = self._registerCallback( callback )
      self._gcObjQueueCommand( 'produceAsync', arg )
      self._nsobj._executeQueuedCommands()
  
  class _VALUEPRODUCER( _PRODUCER ):
    def __init__( self, client ):
      super( _MR._VALUEPRODUCER, self ).__init__( client )
  
    def produce( self ):
      # dictionary hack to simulate Python 3.x nonlocal
      result = { '_': None }
      def __produce( data ):
        result[ '_' ] = data
  
      self._gcObjQueueCommand( 'produce', None, None, __produce )
      self._nsobj._executeQueuedCommands()
      return result[ '_' ]
  
    def produceAsync( self, callback ):
      self._gcObjQueueCommand( 'produceAsync', self._registerCallback( callback ) )
      self._nsobj._executeQueuedCommands()
  
    def flush( self ):
      self._gcObjQueueCommand( 'flush' )
  
class _KLC( _NAMESPACE ):
  def __init__( self, client ):
    super( _KLC, self ).__init__( client, 'KLC' )

  def __createOperator( self, sourceName, sourceCode, operatorName, cmd ):
    operator = self._OPERATOR( self )
    arg = {
      'id': operator.getID(),
      'sourceName': sourceName,
      'sourceCode': sourceCode,
      'operatorName': operatorName
    }

    self._queueCommand( cmd, arg, operator.unwind )
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

  class _OPERATOR( _GCOBJECT ):
    def __init__( self, klc ):
      super( _KLC._OPERATOR, self ).__init__( klc )

    def toJSON( self ):
      # dictionary hack to simulate Python 3.x nonlocal
      json = { '_': None }
      def __toJSON( result ):
        json[ '_' ] = result
  
      self._gcObjQueueCommand( 'toJSON', None, None, __toJSON )
      return json[ '_' ]
 
    def getDiagnostics( self ):
      # dictionary hack to simulate Python 3.x nonlocal
      diagnostics = { '_': None }
      def __getDiagnostics( result ):
        diagnostics[ '_' ] = result
  
      self._gcObjQueueCommand( 'getDiagnostics', None, None, __getDiagnostics )
      return diagnostics[ '_' ]

class _RT( _NAMESPACE ):
  def __init__( self, client ):
    super( _RT, self ).__init__( client, 'RT' )

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

    def __unwind():
      pass
      # FIXME unwind
      #del RT.prototypes[ name ]

    self._queueCommand( 'registerType', arg, __unwind )

  def _route( self, src, cmd, arg ):
    pass

class _GC( _NAMESPACE ):
  def __init__( self, client ):
    super( _GC, self ).__init__( client, 'GC' )
    self.__nextID = 0
    self.__objects = {}

  def getNextID( self ):
    id = self.__nextID
    self.__nextID = self.__nextID + 1
    return id

  def addObject( self, obj ):
    self.__objects[ obj.getID() ] = obj

  def disposeObject( self, obj ):
    del self.__objects[ obj.getID() ]

  def _route( self, src, cmd, arg ):
    src = collections.deque( src )
    id = src.popleft()
    obj = self.__objects[ id ]
    obj._route( src, cmd, arg )

