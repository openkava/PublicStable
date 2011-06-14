#!/bin/bash
#
# Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
#

TMPDIR=/tmp/make-public-from-private.$$

git clone git@github.com:fabric-engine/Private.git "$TMPDIR"
pushd "$TMPDIR"
git remote rm origin
git remote add origin git@github.com:fabric-engine/Public.git
git filter-branch -f --prune-empty --index-filter 'git rm -r --cached --ignore-unmatch \
  Native/ThirdParty/Private \
  Native/JSON/ThirdParty/yajl-1.0.9 \
  Native/BuiltinExts \
  Native/NPAPI \
  Native/ThirdParty/build.bat \
  Native/ThirdParty/getopt \
  Native/ThirdParty/md5-1.4 \
  Native/ThirdParty/Patches \
  Native/ThirdParty/Makefile \
  Native/ThirdParty/GLEW \
  Native/Core \
  Native/Tools \
  Native/Clients \
  Native/Exts/Builtin \
  Native/Exts/Wrapper \
  Native/Test \
  Native/Installer \
  Native/DevServer \
  Web/PrivateApps \
  Util \
  Fabric.xcodeproj' -- --all
git filter-branch -f --tree-filter '\
  find . -wholename ./.git -prune \
    -o -name ThirdParty -prune \
    -o -type f -print0 \
    | xargs -0 ~/Fabric/Util/fix-copyright.sh \
'
git gc --prune=now
git push -f origin master
popd
rm -rf "$TMPDIR"
