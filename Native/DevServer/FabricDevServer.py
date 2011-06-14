import sys, os, getopt, json
from PySide.QtCore import *
from PySide.QtGui import *
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer

def mimeTypeForPath( path ):
  if path.endswith(".html"):
    return "text/html"
  elif path.endswith(".js"):
    return "application/javascript"
  elif path.endswith(".kl"):
    return "application/kl"
  elif path.endswith(".css"):
    return "text/css"
  else:
    return "text/plain"

def configFilename():
  return os.path.join( os.environ['HOME'], "FabricDevServer.config.json" )

def loadConfig():
  try:
    file = open( configFilename() )
    try:
      config = json.load( file )
    except:
      config = {}
    finally:
      file.close()
  except:
    config = {}
      
  if not 'host' in config:
    config['host'] = 'localhost'
  if not 'port' in config:
    config['port'] = 8080
  if not 'dir' in config:
    config['dir'] = os.path.join( os.environ['HOME'], 'Fabric', 'SceneGraph' )
  return config

def saveConfig( config ):
  filename = configFilename()
  try:
    file = open( filename, 'w' )
    try:
      json.dump( config, file )
      print "Wrote " + filename
    except:
      print "Unable to write " + filename
    finally:
      file.close()
  except:
    print "Unable to write " + filename

class FabricDevServer(BaseHTTPRequestHandler):
  def do_GET(self):
    try:
      full_path = os.getcwd()
      components = self.path.split("/")
      index = 0
      while index < len(components) and components[index] == '':
        index += 1
      if index == len(components) or components[index].lower() != "fabric":
        raise IOError
      index += 1
      while index < len(components):
        c = components[index]
        if c == "..":
          raise IOError
        elif c != ".":
          full_path = os.path.join( full_path, c )
        index += 1
      if os.path.isdir( full_path ):
        self.serveDir( full_path )
      else:
        f = open( full_path )
        self.send_response( 200 )
        self.send_header( "Content-type", mimeTypeForPath( full_path ) )
        self.end_headers()
        self.wfile.write( f.read() )
        f.close()
    except IOError:
      self.send_error( 404, "no such file" )

  def log_message( self, format, *args ):
    self.server.log_message( format, *args )

  def serveDir( self, full_path ):
    self.send_response( 200 )
    self.send_header( "Content-type", "text/html" )
    self.end_headers()
    self.wfile.write( "<html><head><title>Contents of "+self.path+"</title></head><body><ul>" )
    for entry in os.listdir( full_path ):
      full_entry = os.path.join( full_path, entry )
      if os.path.isdir( full_entry ):
        desc = entry + "/"
      else:
        desc = entry
      self.wfile.write( '<li><a href="'+desc+'">'+desc+'</a></li>' )
    self.wfile.write( "</ul></body></html>" )

class FabricHTTPServer(HTTPServer):
  def __init__( self, a, b, cb ):
    HTTPServer.__init__( self, a, b )
    self.cb = cb
  def log_message( self, format, *args ):
    self.cb( format % args )

class Server(QThread):
  logSignal = Signal(str)
  
  def __init__( self, host, port, dir ):
    QThread.__init__( self )
    self.host = host
    self.port = port
    self.dir = dir
    os.chdir( self.dir )
    self.httpServer = FabricHTTPServer( ( self.host, self.port ), FabricDevServer, self.log )
    self.httpServer.timeout = 1
  
  def run( self ):
    self.log( "Serving from "+self.dir )
    self.log( "Listening on %s:%d" % (self.host,self.port) )
    self.httpServer.serve_forever()

  def stop( self ):
    self.log( "Exiting" )
    self.httpServer.shutdown()
  
  def log( self, line ):
    self.logSignal.emit( line )

class Window(QDialog):
  
  def __init__( self, parent=None ):
    super( Window, self ).__init__( parent )
    
    self.config = loadConfig()
    
    self.setWindowTitle( "FabricDevServer" )
    self.resize( 640, 480 )
    
    self.portLineEdit = QLineEdit( str( self.config['port'] ) )
    self.dirLineEdit = QLineEdit( self.config['dir'] )
    self.logTextEdit = QTextEdit("")
    self.logTextEdit.setReadOnly( True )
    
    self.startButton = QPushButton("Start")
    self.startButton.clicked.connect( self.start )
    self.stopButton = QPushButton("Stop")
    self.stopButton.clicked.connect( self.stop )
    self.stopButton.setEnabled( False )
    
    layout = QGridLayout()
    layout.addWidget( self.startButton, 0, 0 )
    layout.addWidget( QLabel("Port:"), 0, 1 )
    layout.addWidget( self.portLineEdit, 0, 2 )
    layout.addWidget( self.stopButton, 1, 0 )
    layout.addWidget( QLabel("Directory:"), 1, 1 )
    layout.addWidget( self.dirLineEdit, 1, 2 )
    layout.addWidget( self.logTextEdit, 2, 0, 1, 3 )
    self.setLayout( layout )
  
  def start( self ):
    self.config['host'] = "localhost"
    self.config['port'] = int( self.portLineEdit.text() )
    self.config['dir'] = self.dirLineEdit.text()
    
    try:
      self.thread = Server( self.config['host'], self.config['port'], self.config['dir'] )
      self.thread.logSignal.connect( self.log, Qt.QueuedConnection )
      self.thread.start()
      self.portLineEdit.setEnabled( False )
      self.dirLineEdit.setEnabled( False )
      self.startButton.setEnabled( False )
      self.stopButton.setEnabled( True )
      saveConfig( self.config )
    except:
      self.log( "error" )
  
  def stop( self ):
    self.thread.stop()
    self.thread.wait()
    self.thread = None
    self.portLineEdit.setEnabled( True )
    self.dirLineEdit.setEnabled( True )
    self.startButton.setEnabled( True )
    self.stopButton.setEnabled( False )
    
  def log( self, string ):
    self.logTextEdit.append( string )

if __name__ == '__main__':
  app = QApplication( sys.argv )
  window = Window()
  window.show()
  sys.exit( app.exec_() )

