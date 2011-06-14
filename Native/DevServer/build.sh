#!/bin/sh
arch -i386 py2applet-2.7 FabricDevServer.py
cp -r /opt/local/lib/Resources/qt_menu.nib FabricDevServer.app/Contents/Resources/
cp -r Fabric.icns FabricDevServer.app/Contents/Resources/PythonApplet.icns
