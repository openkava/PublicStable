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
SConscript( dirs=['Native'] )
SConscript( dirs=['Documentation'], variant_dir=os.path.join( buildDir, 'Documentation' ) )
