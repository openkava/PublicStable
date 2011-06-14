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
  Native/Core \
  Native/Tools \
  Native/Clients \
  Native/Exts/Builtin \
  Native/Test \
  Native/Installer \
  Native/DevServer \
  Web/Apps/Private \
  Util \
  Fabric.xcodeproj' -- --all
if false; then
  git filter-branch -f --tree-filter '\
  find . -wholename ./.git -prune \
    -o -name ThirdParty -prune \
    -o -type f -print0 \
    | xargs -0 ~/Fabric/Util/fix-copyright.sh \
'
fi
git gc --prune=now
git push -f origin master
popd
rm -rf "$TMPDIR"
