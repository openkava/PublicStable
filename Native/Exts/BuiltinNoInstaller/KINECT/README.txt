Fabric Engine Kinect Extension README.txt
Copyright 2012 Fabric Engine Inc.  All rights reserved.

Introduction
------------

This archive contains the Fabric Kinect extensions, which
provides the ability to interface with Kinect devices to Fabric
programs.  This extension is not installed along with the
Fabric browser plugin since it requires that both the
Microsoft Visual C++ 2010 Redistributable Package and the
Kinect for Windows SDK be installed.

Installation
------------

First, install the Microsoft Visual C++ 2010 Redistributable
Package.  As of this writing, it is available at:

  http://www.microsoft.com/download/en/details.aspx?id=5555

Next, install the Kinect for Windows SDK.  As of this writing
it is available at:

  http://www.microsoft.com/download/en/details.aspx?id=28782

Finally, to install the Fabric Kinect extension, just copy
the contents of the Exts subdirectory into the directory

  %APPDATA%\Fabric\Exts

where %APPDATA% is the value of the APPDATA environment
variable.  Note that you may need to create the directory
first.

Note that you will need to restart any browsers running
Fabric for the extension to be loaded.
