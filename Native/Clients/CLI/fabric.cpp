#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "LogCollector.h"
#include <Fabric/Core/Build.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/DG/Debug.h>
#include <Fabric/Core/RT/NumericDesc.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/V8Ext/Extension.h>
#include <Fabric/Core/MT/IdleTaskQueue.h>
#include <Fabric/Core/IO/Helpers.h>

#include <getopt.h>

#if defined( FABRIC_POSIX )
#include <unistd.h>
#include <libedit/histedit.h>
#elif defined( FABRIC_WIN32 )
#include <iostream>
#include <string>
#endif

using namespace Fabric;

void RunShell();
bool ExecuteString(v8::Handle<v8::String> source,
                   v8::Handle<v8::Value> name,
                   bool print_result,
                   bool report_exceptions);
v8::Handle<v8::Value> Print(const v8::Arguments& args);
v8::Handle<v8::Value> PrintDeep(const v8::Arguments& args);
v8::Handle<v8::Value> Read(const v8::Arguments& args);
v8::Handle<v8::Value> Load(const v8::Arguments& args);
v8::Handle<v8::Value> Quit(const v8::Arguments& args);
v8::Handle<v8::Value> Version(const v8::Arguments& args);
v8::Handle<v8::String> ReadFile(const char* name);
void ReportException(v8::TryCatch* handler);

int RunMain( std::vector< std::string > const &filesToRun, std::string const &pluginDir, std::vector<std::string> const &loadFiles )
{
  static const int numExtensionNames = 1;
  static char const *extensionNames[numExtensionNames] = { "Fabric" };
  v8::ExtensionConfiguration extensionConfiguration( numExtensionNames, extensionNames );

  V8Ext::Extension extension( pluginDir );
  v8::DeclareExtension declareExtension( &extension );
    
  v8::HandleScope handleScope;

  v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
  global->Set(v8::String::New("print"), v8::FunctionTemplate::New(Print));
  global->Set(v8::String::New("printDeep"), v8::FunctionTemplate::New(PrintDeep));
  global->Set(v8::String::New("read"), v8::FunctionTemplate::New(Read));
  global->Set(v8::String::New("load"), v8::FunctionTemplate::New(Load));
  global->Set(v8::String::New("quit"), v8::FunctionTemplate::New(Quit));
  global->Set(v8::String::New("version"), v8::FunctionTemplate::New(Version));
  
  for ( size_t i=0; i<filesToRun.size(); ++i )
  {
    char const *fileToRun = filesToRun[i].c_str();
    v8::HandleScope handleScope;
    v8::Handle<v8::String> file_name = v8::String::New( fileToRun );
    v8::Handle<v8::String> source = ReadFile( fileToRun );
    if (source.IsEmpty())
    {
      printf("Error reading '%s'\n", fileToRun );
      return 1;
    }

    v8::Persistent<v8::Context> context = v8::Context::New( &extensionConfiguration, global );
    context->Enter();
    for ( size_t i=0; i<loadFiles.size(); ++i )
    {
      v8::HandleScope handleScope;
      v8::Handle<v8::String> file_name = v8::String::New( loadFiles[i].data(), loadFiles[i].length() );
      v8::Handle<v8::String> source = ReadFile( loadFiles[i].data() );
      if (source.IsEmpty())
      {
        printf("Error reading '%s'\n", loadFiles[i].data() );
        return 1;
      }
      ExecuteString(source, file_name, false, false);
    }
    bool executeResult = ExecuteString(source, file_name, false, true);
    context->Exit();
    context.Dispose();
    if ( !executeResult )
      return 1;
  }

  if ( filesToRun.empty() )
  {
    v8::HandleScope handleScope;
    v8::Persistent<v8::Context> context = v8::Context::New( &extensionConfiguration, global );
    context->Enter();
    RunShell();
    context->Exit();
    context.Dispose();
  }
  
  MT::IdleTaskQueue::Instance()->wait();

  return 0;
}


int main( int argc, char **argv )
{
  std::string pluginDir;
  std::vector<std::string> loadFiles;

  while ( true )
  {
    static struct option longOptions[] =
    {
      { "version", 0, NULL, 'v' },
      { "exts", required_argument, NULL, 'p' },
      { "load", required_argument, NULL, 'l' },
#if defined(FABRIC_BUILD_DEBUG)
      { "debug", 0, NULL, 'd' },
#endif
      { NULL, 0, NULL, 0 }
    };
    int option_index = 0;

    int c = getopt_long( argc, argv, "", longOptions, &option_index );
    if ( c == -1 )
      break;
    switch ( c )
    {
      case 'v':
        printf( "%s version %s\n", buildName, buildFullVersion );
        return 0;
        
      case 'p':
        pluginDir = optarg;
        break;
      
      case 'l':
        loadFiles.push_back( optarg );
        break;
      
      default:
        break;
    }
  }

  if ( pluginDir.length() == 0 )
    pluginDir = IO::JoinPath( IO::getRootPath(), "Exts" );
    
  std::vector< std::string > filesToRun;
  for ( ; optind < argc; ++optind )
    filesToRun.push_back( argv[optind] );

  return RunMain( filesToRun, pluginDir, loadFiles );
}

static std::string quoteV8Value( v8::Handle<v8::Value> const &v8Value, size_t depth )
{
  std::string result;
  v8::HandleScope handleScope;
  if ( v8Value->IsString() )
  {
    v8::Handle<v8::String> v8String = v8::Handle<v8::String>::Cast( v8Value );
    v8::String::Utf8Value utf8Value( v8String );
    char const *data = *utf8Value;
    int length = utf8Value.length();
    result += '"';
    for ( int i=0; i<length; ++i )
    {
      char ch = data[i];
      switch ( ch )
      {
        case '"':
          result += "\\\"";
          break;
        case '\\':
          result += "\\\\";
          break;
        case '\n':
          result += "\\n";
          break;
        case '\r':
          result += "\\r";
          break;
        case '\t':
          result += "\\t";
          break;
        case '\v':
          result += "\\v";
          break;
        case '\a':
          result += "\\a";
          break;
        case '\b':
          result += "\\b";
          break;
        default:
          result += ch;
          break;
      }
    }
    result += '"';
  }
  else if ( v8Value->IsObject() )
  {
    if ( v8Value->IsFunction() )
      result += "<Function>";
    else if ( v8Value->IsArray() )
    {
      if ( depth == 0 )
        result += "<Array>";
      else
      {
        v8::Handle<v8::Array> v8Array = v8::Handle<v8::Array>::Cast( v8Value );
        uint32_t length = v8Array->Length();
        uint32_t lengthToDisplay = length;
        if ( lengthToDisplay > 16 )
          lengthToDisplay = 16;
        result += '[';
        for ( uint32_t i=0; i<length; ++i )
        {
          if ( i > 0 )
            result += ',';
          v8::Handle<v8::Value> v8Element = v8Array->Get( v8::Uint32::New(i) );
          result += quoteV8Value( v8Element, depth-1 );
        }
        if ( lengthToDisplay < length )
          result += ",...";
        result += ']';
      }
    }
    else
    {
      if ( depth == 0 )
        result += "<Object>";
      else
      {
        v8::Handle<v8::Object> v8Object = v8::Handle<v8::Object>::Cast( v8Value );
        v8::Handle<v8::Array> propertyNames = v8Object->GetPropertyNames();
        uint32_t numPropertyNames = propertyNames->Length();
        uint32_t numPropertyNamesToDisplay = numPropertyNames;
        if ( numPropertyNamesToDisplay > 16 )
          numPropertyNamesToDisplay = 16;
        result += '{';
        bool first = true;
        for ( uint32_t i=0; i<numPropertyNamesToDisplay; ++i )
        {
          if ( !first )
            result += ',';
            
          v8::Handle<v8::Value> propertyName = propertyNames->Get( v8::Uint32::New(i) );
          v8::Handle<v8::Value> propertyValue;
          if ( propertyName->IsString() )
          {
            v8::Handle<v8::String> propertyNameString = v8::Handle<v8::String>::Cast( propertyName );
            propertyValue = v8Object->Get( propertyNameString );
          }
          
          if ( !propertyValue.IsEmpty() )
          {
            v8::String::Utf8Value propertyNameUTF8Value( propertyName );
            result += std::string( *propertyNameUTF8Value, propertyNameUTF8Value.length() );
            result += ':';
            result += quoteV8Value( propertyValue, depth - 1 );
            first = false;
          }
        }
        if ( numPropertyNamesToDisplay < numPropertyNames )
          result += ",...";
        result += '}';
      }
    }
  }
  else
  {
    v8::String::Utf8Value utf8Value( v8Value );
    result = std::string( *utf8Value, utf8Value.length() );
  }
  return result;
}

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

v8::Handle<v8::Value> _Print( const v8::Arguments& args, size_t depth )
{
  bool first = true;
  for (int i = 0; i < args.Length(); i++)
  {
    v8::HandleScope handle_scope;
    if (first) {
      first = false;
    } else {
      printf(" ");
    }
    
    if ( args[i]->IsString() )
    {
      v8::String::Utf8Value str(args[i]);
      const char* cstr = ToCString(str);
      printf("%s", cstr);
    }
    else
    {
      std::string quotedV8Value = quoteV8Value( args[i], depth );
      fwrite( quotedV8Value.data(), 1, quotedV8Value.length(), stdout );
    }
  }
  printf("\n");
  fflush(stdout);
  return v8::Undefined();
}

v8::Handle<v8::Value> Print( const v8::Arguments& args )
{
  return _Print( args, 1 );
}

v8::Handle<v8::Value> PrintDeep( const v8::Arguments& args )
{
  return _Print( args, 16 );
}


// The callback that is invoked by v8 whenever the JavaScript 'read'
// function is called.  This function loads the content of the file named in
// the argument into a JavaScript string.
v8::Handle<v8::Value> Read(const v8::Arguments& args) {
  if (args.Length() != 1) {
    return v8::ThrowException(v8::String::New("Bad parameters"));
  }
  v8::String::Utf8Value file(args[0]);
  if (*file == NULL) {
    return v8::ThrowException(v8::String::New("Error loading file"));
  }
  v8::Handle<v8::String> source = ReadFile(*file);
  if (source.IsEmpty()) {
    return v8::ThrowException(v8::String::New("Error loading file"));
  }
  return source;
}


// The callback that is invoked by v8 whenever the JavaScript 'load'
// function is called.  Loads, compiles and executes its argument
// JavaScript file.
v8::Handle<v8::Value> Load(const v8::Arguments& args) {
  for (int i = 0; i < args.Length(); i++) {
    v8::HandleScope handle_scope;
    v8::String::Utf8Value file(args[i]);
    if (*file == NULL) {
      return v8::ThrowException(v8::String::New("Error loading file"));
    }
    v8::Handle<v8::String> source = ReadFile(*file);
    if (source.IsEmpty()) {
      return v8::ThrowException(v8::String::New("Error loading file"));
    }
    if (!ExecuteString(source, v8::String::New(*file), false, false)) {
      return v8::ThrowException(v8::String::New("Error executing file"));
    }
  }
  return v8::Undefined();
}


// The callback that is invoked by v8 whenever the JavaScript 'quit'
// function is called.  Quits.
v8::Handle<v8::Value> Quit(const v8::Arguments& args) {
  // If not arguments are given args[0] will yield undefined which
  // converts to the integer value 0.
  int exit_code = args[0]->Int32Value();
  exit(exit_code);
  return v8::Undefined();
}


v8::Handle<v8::Value> Version(const v8::Arguments& args) {
  return v8::String::New(v8::V8::GetVersion());
}


// Reads a file into a v8 string.
v8::Handle<v8::String> ReadFile(const char* name) {
  FILE* file = fopen(name, "rb");
  if (file == NULL) return v8::Handle<v8::String>();

  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  rewind(file);

  char* chars = new char[size + 1];
  chars[size] = '\0';
  for (int i = 0; i < size;) {
    int read = fread(&chars[i], 1, size - i, file);
    i += read;
  }
  fclose(file);
  v8::Handle<v8::String> result = v8::String::New(chars, size);
  delete[] chars;
  return result;
}

static void printBanner( )
{
  printf( "%s version %s\n%s\n%s\n", Fabric::buildName, Fabric::buildFullVersion, Fabric::buildDesc, Fabric::buildCopyright );
}

#if defined( FABRIC_POSIX )
char const *prompt( EditLine *el )
{
  return "> ";
}

// The read-eval-execute loop of the shell.
void RunShell()
{
  EditLine *el = el_init( "CLI", stdin, stdout, stderr );
  el_set( el, EL_PROMPT, &prompt );
  el_set( el, EL_EDITOR, "emacs" );

  History *hist = history_init();
  HistEvent ev;
  history( hist, &ev, H_SETSIZE, 800 );
  el_set( el, EL_HIST, history, hist );
  
  printBanner();
  while (true)
  {
    int count;
    const char *line = el_gets(el, &count);
    if ( count == 0 )
      break;
    history( hist, &ev, H_ENTER, line );
      
    v8::HandleScope handle_scope;
    ExecuteString( v8::String::New(line), v8::String::New("(shell)"), true, true );
  }
  printf("\n");

  history_end( hist );
  el_end( el );
}
#elif defined( FABRIC_WIN32 )
void RunShell()
{
   printBanner();
   for( ;; )
   {
     std::cout << "> ";
     std::cout.flush();
     std::string    str;
     std::getline( std::cin, str );
     if( str.empty() )
       break;

    v8::HandleScope handle_scope;
    ExecuteString( v8::String::New(str.data(), str.size()), v8::String::New("(shell)"), true, true );
   }
   std::cout << std::endl;
}
#else
#error Undefined platform...
#endif

// Executes a string within the current v8 context.
bool ExecuteString(v8::Handle<v8::String> source,
                   v8::Handle<v8::Value> name,
                   bool print_result,
                   bool report_exceptions) {
  v8::HandleScope handle_scope;
  v8::TryCatch try_catch;
  v8::Handle<v8::Script> script = v8::Script::Compile(source, name);
  if (script.IsEmpty()) {
    // Print errors that happened during compilation.
    if (report_exceptions)
      ReportException(&try_catch);
    return false;
  }
  else
  {
    v8::Handle<v8::Value> result = script->Run();
    
    if (result.IsEmpty()) {
      // Print errors that happened during execution.
      if (report_exceptions)
        ReportException(&try_catch);
      return false;
    } else {
      if (print_result && !result->IsUndefined())
      {
        std::string quotedResult = quoteV8Value( result, 1 );
        fwrite( quotedResult.data(), 1, quotedResult.length(), stdout );
        fputc( '\n', stdout );
      }
      return true;
    }
  }
}


void ReportException(v8::TryCatch* try_catch) {
  v8::HandleScope handle_scope;
  v8::String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception);
  v8::Handle<v8::Message> message = try_catch->Message();
  if (message.IsEmpty()) {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    printf("%s\n", exception_string);
  } else {
    // Print (filename):(line number): (message).
    v8::String::Utf8Value filename(message->GetScriptResourceName());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber();
    printf("%s:%i: %s\n", filename_string, linenum, exception_string);
    // Print line of source code.
    v8::String::Utf8Value sourceline(message->GetSourceLine());
    const char* sourceline_string = ToCString(sourceline);
    printf("%s\n", sourceline_string);
    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn();
    for (int i = 0; i < start; i++) {
      printf(" ");
    }
    int end = message->GetEndColumn();
    for (int i = start; i < end; i++) {
      printf("^");
    }
    printf("\n");
  }
}
