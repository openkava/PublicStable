#
# Copyright 2010-2011 Fabric Engine Inc. All rights reserved.
#

import os, platform, posixpath, glob, sys

CacheDir( '.sconscache' )

SetOption('num_jobs', int(os.environ.get('FABRIC_BUILD_JOBS', 1)))
print "Running with -j" + str(GetOption('num_jobs'))

env = Environment()

buildDir = '#build'
Export('buildDir')
distDir = env.Dir('#dist')
Export('distDir')

Export( 'env' )

SConscript( dirs=['SConsUtils'], variant_dir=os.path.join( buildDir, 'SConsUtils' ) )

nativeTargets = [
  ['kl', "The 'kl' command-line tool"],
  ['parseJSON', "The parseJSON command-line tool"],
  ['cliTools', 'All command-line tools'],
  ['node', 'The Node.js module command-line client'],
  ['python', 'The Python module command-line client'],
  ['cliClients', 'All command-line clients'],
  ['cli', 'All command-line clients and tools'],
  ['npapi', 'The NPAPI plugin browser client'],
  ['exts', 'All the extensions'],
  ['all', 'All clients and tools (command-line and browser)'],
  ['dist', 'Build packages for distribution'],
  ['upload', 'Upload distribution packages to dist.fabric-engine.com']
  ]
for nativeTarget in nativeTargets:
  if nativeTarget[0] in COMMAND_LINE_TARGETS:
    SConscript( dirs=['Native'] )
    break

docsTargets = [
  ['klpgPDF', 'KLPG in PDF form'],
  ['iopgPDF', 'IOPG in PDF form'],
  ['guides', 'All guides']
  ]
for docsTarget in docsTargets:
  if docsTarget[0] in COMMAND_LINE_TARGETS:
    SConscript( dirs=['Documentation'], variant_dir=os.path.join( buildDir, 'Documentation' ) )
    break
targetGroups = [
  ["Native Targets", nativeTargets],
  ["Documentation Targets", docsTargets]
  ]

options = [
  ['VERBOSE=1', "Show commands that are executed"]
  ]

def displayHelp(target, source, env):
  print """
Fabric Build System
Copyright (C) 2010-2012 Fabric Engine Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

USAGE: scons [<option>...] <target>...

<target> can be among the following:
"""
  for targetGroup in targetGroups:
    print "  %s:" % targetGroup[0]
    print
    for target in targetGroup[1]:
      print "  %12s  %s" % (target[0], target[1])
    print
  print "<option> can be among the following:"
  print
  for option in options:
    print "  %12s  %s" % (option[0], option[1])
  print
  print "For more information refer to http://fabric-engine.com/"
  print
Default(
  Alias('help', Command(['phony'], [], displayHelp))
  )
