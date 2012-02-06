#!/bin/sh

error()
{
  if [ -n "$1" ]; then
    echo "$@"
  fi
  echo "An error has occurred; aborting.  The upload FAILED."
  exit 1
}

run()
{
  echo "$@"
  "$@"
}

rexec()
{
  run ssh fabric-engine.com "$@"
}

rcp()
{
  if [ "$1" = "-r" ]; then
    shift
    run rsync -rze ssh --exclude=.git --exclude=.gitignore --exclude=.DS_Store --exclude=Apps/Private --delete --delete-excluded --progress "$1" fabric-engine.com:"$2"
  else
    run rsync -e ssh --progress "$1" fabric-engine.com:"$2"
  fi
}

[ -d "$FABRIC_CORE_PATH" ] || error "The FABRIC_CORE_PATH environment variable must be set to the path to the Fabric NativeCore checkout"
[ -f "$FABRIC_CORE_PATH/index.html" ] || error "Sanity check failed: $FABRIC_CORE_PATH/index.html does not exist or is not a file"
[ -d "$FABRIC_CORE_PATH/Web" ] || error "Sanity check failed: $FABRIC_CORE_PATH/Web does not exist or is not a directory"

FORCE=0
if [ "$1" = "-f" ]; then
  FORCE=1
  shift
fi

VERSION="$1"
[ -n "$VERSION" ] || error "Usage: $0 [-f] <version>"
(echo "$VERSION" | egrep -q '^[0-9]{1,2}\.[0-9]{1,2}\.[0-9]{1,2}$') || error "Version number must be of the form 1.0.19, 2.1.0, etc."
DIST_DIR="/fabric-distribution/$VERSION"

if [ "$FORCE" = 1 ]; then
  rexec mkdir -p $DIST_DIR || error
else
  rexec mkdir $DIST_DIR || error "If you are sure you want to overwrite the existing version, specify the -f flag"
  rexec cp -r "/fabric-distribution/stable/*" "$DIST_DIR/" || error
fi

SG_DIR="$DIST_DIR/sg"
rexec mkdir -p "$SG_DIR" || error
rcp "$FABRIC_CORE_PATH/index.html" "$SG_DIR/" || error
FABRIC_DIR="$SG_DIR/Fabric"
rexec mkdir -p "$FABRIC_DIR" || error
for DIR in "$FABRIC_CORE_PATH/Web/"*; do
  rcp -r "$DIR" "$FABRIC_DIR/" || error
done

BIN_DIR="$DIST_DIR/bin"
rexec mkdir -p "$BIN_DIR" || error
LOCAL_DIST_DIR="$FABRIC_CORE_PATH/Native/dist"
for PLATFORM in Windows-x86 Darwin-universal Linux-i686 Linux-x86_64; do
  rcp "$LOCAL_DIST_DIR/FabricEngine-$PLATFORM.crx" "$BIN_DIR/" || error
  rcp "$LOCAL_DIST_DIR/FabricEngine-$PLATFORM.xpi" "$BIN_DIR/" || error
  rcp "$LOCAL_DIST_DIR/crx-update-$PLATFORM.xml" "$BIN_DIR/" || error
  rcp "$LOCAL_DIST_DIR/xpi-update-$PLATFORM.rdf" "$BIN_DIR/" || error
done

rexec ln -snf "$VERSION" "/fabric-distribution/testing" || error
rexec ln -snf "$DIST_DIR/bin" "/var/www/dist.testing.fabric-engine.com/$VERSION" || error
echo "The upload was SUCCESSFUL."
