Fabric Engine FileSystem Extension README.txt
Copyright 2012 Fabric Engine Inc.  All rights reserved.

Introduction
------------

This archive contains the Fabric FileSystem extensions, which
provides direct filesystem access to Fabric programs.  This
extension is not installed along with the Fabric browser plugin
since it creates a security risk; it should only be installed
on systems where the Fabric applications that are going to
be run are known and trusted.

Installation
------------

To install the extension, just copy the contents of the Exts
subdirectory into the following directory, creating the directory
if necessary.  The directory is platform-dependent:

Windows: %APPDATA%\Fabric\Exts
  (where %APPDATA% is the value of the APPDATA environment variable)
OS X: ~/Library/Fabric/Exts
Linux: ~/.fabric/Exts

Note that you will need to restart any browsers running 
Fabric for the extension to be loaded.
