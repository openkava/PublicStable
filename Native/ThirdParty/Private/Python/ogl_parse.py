import sys
import os
import exceptions
import glob
import platform

# construct the filepath based on this python script's path
basePath = __file__.replace('\\','/').rpartition('/')[0]
files = []
if platform.system() == "Linux":
  # the order actually matters here
  files.append('/usr/include/GL/glew.h')
  files.append('/usr/include/GL/glxew.h')
  files.append('/usr/include/GL/gl.h')
  files.append('/usr/include/GL/glx.h')
  files.append('/usr/include/GL/glext.h')
  files.append('/usr/include/GL/glu.h')
else:
  raise(Exception("THIS PLATFORM HAS NOT YET BEEN IMPLEMENTED."))
  
templatePath = basePath+'/../../../Core/OGL/OGL.cpp_template'
sourcePath = basePath+'/../../../Core/OGL/OGL.cpp'

verbose = False

# RUN THE PARSER
def main():
  
  # LET'S FIND THE CONSTANTS AND FUNCTIONS BASED ON KEYWORDS
  constants = []
  functions = []
  exports = {}
  for file in files:
    lines = open(file).read().split('\n')
    i = 0
    ifdefCount = 0
    insideProtoTypes = 0
    while(i < len(lines)):
      line = lines[i]
      line = line.replace('#if defined','#ifdef')
      if line.startswith("#ifndef"):
        ifdefCount += 1
      if line.startswith("#ifdef"):
        ifdefCount += 1
        if line.endswith('PROTOTYPES'):
          insideProtoTypes = ifdefCount
      elif line.startswith("#endif"):
        if insideProtoTypes == ifdefCount:
          insideProtoTypes = 0
        ifdefCount -= 1
      if insideProtoTypes > 0:
        i += 1
        continue
      
      while(line.strip().endswith(',')):
        i += 1
        line += lines[i]
      if line.startswith("GLAPI "):
        functions.append(line)
      elif(line.startswith("#define GL_") or
           line.startswith("#define GLX_") or
           line.startswith("#define WGL_")):
        constants.append(line)
      elif line.find('GET_VAR') > 15:
        constants.append(line)
      elif not line.startswith("#define") and line.find('_EXPORT')  > -1:
        export = line.strip(';').split(' ');
        if(len(export) == 3):
          exports[export[2]] = export[1]
      elif line.startswith('extern') and line.find('"C"') == -1:
        if line.find('(') > -1:
          functions.append(line)
        else:
          export = line.strip(';').split(' ');
          if(len(export) == 3):
            if not exports.has_key(export[2]):
              exports[export[2]] = export[1]
      elif line.startswith('GLAPI'):
        functions.append(line)
      elif line.find('GET_FUN') > 15:
        functions.append(line)
      elif line.startswith('typedef') and line.find('(') > -1:
        type = line.split(' ')[1]
        name = line.partition(')')[0].partition('(')[2].strip().rpartition(' ')[2]
        if not exports.has_key(name):
          value = '(' + line.rpartition('(')[2].partition(')')[0]+')'
          exports[name] = [type,value]
      i += 1
      
  # define suffixes to skip
  suffixes = [
    'MESA',
    'SUN',
    'SUNX',
    'NV',
    'ARB',
    'ATI',
    'GREMEDY',
    'IBM',
    'SGIX',
    'AMD',
    'APPLE',
    'SGIS',
    'WIN',
    'INTEL',
    'OES',
    'SGI',
    'HP'
  ]
  
  # LOOP OVER ALL DEFINED FUNCTIONS, AND REGISTER THEM
  registeredConstants = {}
  for i in range(len(constants)):
    # REPLACE MACROS WITH THEIR DEFINED FUNCTIONS
    if constants[i].find('(') > -1:
      name = constants[i].split(' ')[1]
      macro = constants[i].partition('(')[2].partition(')')[0]
      if(not exports.has_key(macro)):
        raise(Exception("Macro "+macro+" has not been found."))
      while(exports.has_key(macro)):
        macro = exports[macro]
      constants[i] = '#define '+name+' '+macro

    split = constants[i].strip(';').split(' ')
    if(len(split) == 3):
      name = split[1]
      hardwareSpecific = False
      for j in range(len(suffixes)):
        if name.find('_'+suffixes[j]+'_') > -1:
          hardwareSpecific = True
          break
      value = split[2]
      if not hardwareSpecific and not registeredConstants.has_key(name) and not value == '0xFFFFFFFFFFFFFFFF':
        registeredConstants[name] = value;
        
  # CREATE THE SOURCE CODE FOR EACH CONSTANT
  addConstantsCode = []
  for name in registeredConstants:
    value = registeredConstants[name]
    addConstantsCode.append('      #if defined( '+name+' )')
    if value.startswith('0x') or value.isdigit:
      addConstantsCode.append('        ADD_CONST_INT( "'+name+'", '+name+' );')
    else:
      addConstantsCode.append('        ADD_CONST_BOOL( "'+name+'", '+name+' );')
    addConstantsCode.append('      #endif')
    
  # DEFINE A MAPPING FOR THE DATATYPE
  # oglTYPE: [C++type, trace format, trace cast, KL Type]
  knownCTypes = {
    'void':['void','void*','',''],
    'bool':['bool','%b','bool','Boolean'],
    'int':['int','%d','int','Integer'],
    'char':['char','%s','const char*','String'],
    'int64_t':['int64_t','%d','int','Integer'],
    'uint':['unsigned','0x%04X','unsigned','Size'],
    'GLfloat':['GLfloat','%f','float','Scalar'],
    'GLint':['GLint','%d','int','Integer'],
    'GLsizei':['GLsizei','%d','int','Size'],
    'GLushort':['unsigned','0x%04X','unsigned','Integer'],
    'GLvoid':['GLvoid','void*','',''],
    'GLboolean':['GLboolean','%b','bool','Boolean'],
    'GLint':['GLint','%d','int','Integer'],
    'GLuint':['GLuint','0x%04X','unsigned','Size'],
    'GLenum':['GLenum','%d','int','Size'],
    'GLdouble':['GLdouble','%f','float','Scalar'],
    'GLbyte':['GLbyte','%d','int','Byte'],
    'GLubyte':['GLubyte','0x%04X','unsigned','Size'],
    'GLhalf':['GLhalf','%d','integer','Integer'],
    'GLshort':['GLshort','%d','int','Integer'],
    'GLchar':['GLchar','%s','const char*','String'],
    'GLintptr':['GLintptr','%d','int','Integer'],
    'GLsizeiptr':['GLsizeiptr','%d','int','Size'],
    'GLclampf':['GLclampf','%f','float','Scalar'],
    'GLclampd':['GLclampd','%f','float','Scalar'],
    'GLbitfield':['GLbitfield','0x%04X','unsigned','Integer'],
  }
  
  # LOOP ALL FUNCTIONS AND DEFINE THEIR CODE
  functionsCode = []
  addFunctionsCode = []
  knownFunctions = {}
  for i in range(len(functions)):

    # FIRST CHECK IF THE FUNCTION USES A MACRO
    if functions[i].find("GET_FUN") > -1:
      name = functions[i].partition('(')[2].partition(')')[0]
      macro = name
      while(exports.has_key(str(macro))):
        macro = exports[str(macro)]
      if not(len(macro) == 2):
        continue
      functions[i] = macro[0]+" "+name.strip('_')+" "+macro[1]+';'
      
    # APPLY SOME FORMATTING
    function = functions[i]
    if function.startswith('extern '):
      function = function[7:1000]
    if function.startswith('GLAPI '):
      function = function[6:1000]
    function = function.replace('BOOL','bool')
    function = function.replace('*','* ').replace(' *','*').replace('*  ','* ')
    function = function.replace(', ',',').replace('( ','(').replace(' )',')').replace(' (','(')
    function = function.replace('GLAPIENTRY ','')
    function = function.replace('APIENTRY ','')

    # EXTRACT THE FUNCTION'S NAME
    name = function.partition(' ')[2].partition('(')[0].rpartition(' ')[2]
    
    # IF WE HAVE A GLEW NAME, REPLACE IT WITH GL
    if name.startswith('glew'):
      name = 'gl'+name[4:1000]
    if knownFunctions.has_key(name):
      continue
    if not name.startswith('gl'):
      continue
    
    # CHECK IF THIS IS A HARDWARE SPECIFIC FUNCTION AND SKIP THOSE
    hardwareSpecific = False
    for i in range(len(suffixes)):
      if name.endswith(suffixes[i]):
        hardwareSpecific = True
        break
    if hardwareSpecific:
      continue
          
    # LET'S FIGURE OUT THE RETURN TYPE
    returnType = function.partition('(')[0].rpartition(' ')[0]
    returnTypeKey = returnType.rpartition(' ')[2].strip('*')
    if not knownCTypes.has_key(returnTypeKey):
      if verbose:
        print("Warning: Skipped function '"+name+"' due to unknown return type '"+returnType+ "'.")
      continue

    # DEFINE THE PARAMETER LAYOUT, STARTING WITH THE RETURN TYPE
    parameterLayout = []
    if returnType.find('void') == -1:
      parameterLayout.append('='+knownCTypes[returnTypeKey][3])
    
    # EXTRACT ALL OF THE VARIABLES (BETWEEN BRACES)
    variables = function.partition('(')[2].rpartition(')')[0].split(',')
    if len(variables) == 1 and variables[0] == 'void':
      variables = []
      
    # LOOP OVER ALL VARIABLES
    allTypesValid = True
    traceFormat = []        # THE PRINTF FORMAT TOKENS (USED FOR TRACE)
    traceVars = []          # THE NAME OF THE VARIABLES TO PASS TO PRINTF (TRACE)
    varTypes = []           # THE TYPES OF THE VARIABLES
    varNamesHead = []       # THE NAMES OF THE VARIABLES FOR THE FUNCTION DEFINITION
    varNames = []           # THE NAMES OF THE VARIABLES FOR THE GL CALL
    additionalCodePre = []  # CODE TO RUN PRE THE GL CALL
    additionalCodePost = [] # CODE TO RUN POST THE GL CALL
    for i in range(len(variables)):
      
      # STRIP OFF WHITESPACES
      variables[i] = variables[i].strip()
      
      # DEFINE A NAME FOR THE VARIABLE IF THERE IS NONE
      if variables[i].find(' ') == -1 or variables[i].endswith('*'):
        variables[i] = variables[i] + ' var'+str(i)
        
      # EXTRACT THE TYPE FOR EACH VARIABLE
      fulltype = variables[i].rpartition(' ')[0].strip()
      
      # GET THE SIMPLE TYPE, AKA char OUT OF const char*
      type = fulltype.strip('*').strip()
      if type.startswith('const '):
        type = type[6:1000]
        
      # ENSURE THE VARIABLE TYPE IS KNOWN
      if not knownCTypes.has_key(type) or variables[i].find('[') > -1:
        allTypesValid = False
        if verbose:
          print("Warning: Skipped function '"+name+"' due to unknown parameter type '"+variables[i]+ "'.")
        break

      # EXTRACT THE NAME OF THE VARIABLE
      varname = variables[i].rpartition(' ')[2]
      
      # DEAL WITH STRINGS OR DUAL POINTERS
      if variables[i].count('*') == 2 or type == 'char' or type == 'GLchar':
        if type == 'char' or type == 'GLchar':

          # DEAL WITH A STIRNG ARRAY
          traceFormat.append('char*')
          fulltype = fulltype.replace('**','*')
          if(variables[i].find('const') > -1):
            
            # IF WE HAVE A CONST CHAR, WE WANT TO JUST READ IT
            varTypes.append('void*')
            additionalCodePre.append('      '+fulltype.strip()+' '+varname+'Str = ('+fulltype+')stringDesc->getValueData( &'+varname+' );')
            varTypes.append('void*')
            if variables[i].find('**') > -1:
              varNames.append('&'+varname+'Str')
            else:
              varNames.append(varname+'Str')
            parameterLayout.append('<String '+varname)
          else:
            
            # IF WE DON'T HAVE A CONST CHAR, WE NEED TO WRITE TO IT
            varTypes.append('void*')
            if variables[i].find('**') > -1:
              varNames.append('&'+varname+'Str')
            else:
              varNames.append(varname+'Str')
            additionalCodePre.append('      char * '+varname+'Str = new char[1024];')
            additionalCodePost.append('      stringDesc->setValue( '+varname+'Str, strlen('+varname+'Str), '+varname+' );')
            additionalCodePost.append('      delete( '+varname+'Str );')
            parameterLayout.append('>String '+varname)
        else:
          # BAIL OUT, NO DUAL POINTERS ARE ALLOWED. SKIP THIS FUNCTION
          if verbose:
            print("Warning: Skipped function '"+name+"' due to invalid double pointr parameter '"+variables[i]+ "'.")
          allTypesValid = False
          break
      else:
        
        # IF WE HAVE A TRACE FORMAT TYPE, AND WE ARE NOT A POINTER, LET'S TRACE US
        if len(knownCTypes[type][2]) > 0 and variables[i].find('*') == -1:
          traceFormat.append(knownCTypes[type][1])
          traceVars.append('('+knownCTypes[type][2]+')'+varname)
        else:
          traceFormat.append(type+'*')
          
        # IF WE HAVE A POINTER
        if variables[i].find('*') > -1:
          if variables[i].startswith('const'):
            # IF WE ARE CONST, LET'S CREATE A DATA PTR
            parameterLayout.append('<Data '+varname)
          else:
            # this is really flaky, it should be >Data, but that's not possible.
            # the rvalue is still able to write to though.
            parameterLayout.append('<Data '+varname)
        else:
          parameterLayout.append('<'+knownCTypes[type][3]+' '+varname)
        varTypes.append(fulltype)
        varNames.append(varname)
      varNamesHead.append(varname)
    if not allTypesValid:
      continue

    # REMEMBER THE FUNCTION
    knownFunctions[name] = function
    
    # IF WE HAVE VARIABLES
    if len(varNames) > 0:
      functionsCode.append('    static '+returnType+' '+name[2:1000]+'(')
      for i in range(len(varNamesHead)-1):
        functionsCode.append('      '+varTypes[i]+' '+varNamesHead[i]+',')
      functionsCode.append('      '+varTypes[len(varTypes)-1]+' '+varNamesHead[len(varNamesHead)-1])
      functionsCode.append('    ){')
      if len(traceVars) > 0:
        functionsCode.append('      FABRIC_OGL_TRACE( "'+name+'( '+str(', ').join(traceFormat)+' )",')
        for i in range(len(traceVars)-1):
          functionsCode.append('        '+traceVars[i]+',')
        functionsCode.append('        '+traceVars[len(traceVars)-1])
        functionsCode.append('      );')
      else:
        functionsCode.append('      FABRIC_OGL_TRACE( "'+name+'( '+str(', ').join(traceFormat)+' )" );')
    else:
      functionsCode.append('    static '+returnType+' '+name[2:1000]+'()')
      functionsCode.append('    {')
      functionsCode.append('      FABRIC_OGL_TRACE( "'+name+'()" );')
    functionsCode.append('      ErrorReporter errorReporter( "'+name+'" );')
    functionsCode.extend(additionalCodePre)
    prefix = ''
    if returnType.find('void') == -1:
      prefix = returnType+' result = '
    if len(varNames) > 0:
      functionsCode.append('      '+prefix+name+'( '+str(', ').join(varNames)+' );')
    else:
      functionsCode.append('      '+prefix+name+'();')
    functionsCode.extend(additionalCodePost)

    # IF WE HAVE A RETURN TYPE
    if returnType.find('void') == -1:
      traceFormat = knownCTypes[returnTypeKey][1]
      traceVar = knownCTypes[returnTypeKey][2]
      if len(traceVar) > 0 and returnType.find('*') == -1:
        functionsCode.append('      FABRIC_OGL_TRACE_NOTE( "Returning '+traceFormat+'", ('+traceVar+')result );')
      functionsCode.append('      return result;')
    
    functionsCode.append('    }')
    addFunctionsCode.append('      ADD_FUNC( '+name[2:1000]+', "'+str(',').join(parameterLayout)+'" );')
    
  # LOAD THE TEMPLATE AND FILL IT
  template = open(templatePath).read();
  template = template.replace("####FUNCTIONS####",str('\n').join(functionsCode))
  template = template.replace("####CONSTANTADDS####",str('\n').join(addConstantsCode))
  template = template.replace("####FUNCTIONADDS####",str('\n').join(addFunctionsCode))
  # WRITE OUT TO OUR SOURCE FILE
  open(sourcePath,"w").write(template)
  
if __name__ == '__main__':
  main()