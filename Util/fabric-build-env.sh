#!/bin/sh

if [ -z "$FABRIC_CORE_PATH" ]; then
  cat >&2 <<EOF
The FABRIC_CORE_PATH environment variable must be set
to the path to the Fabric NativeCore checkout
EOF
else
  FABRIC_NPAPI_LINK=0
  FABRIC_BUILD_OS=$(uname -s)
  if [ "$FABRIC_BUILD_OS" = "Darwin" ]; then
    FABRIC_BUILD_ARCH=universal
  else
    FABRIC_BUILD_ARCH=$(uname -m)
  fi
  FABRIC_BUILD_TYPE=

  while getopts ":n" opt; do
    case $opt in
      n)
        FABRIC_NPAPI_LINK=1
        ;;
      \?)
        echo "Invalid option: -$OPTARG" >&2
        usage
        ;;
    esac
  done
  shift $(($OPTIND-1))
  FABRIC_BUILD_TYPE=$1

  if [ "$FABRIC_BUILD_TYPE" != "Debug" -a "$FABRIC_BUILD_TYPE" != "Release" ]; then
    cat >&2 <<EOF
Usage: souce fabric-build-env.sh [-n] (Debug|Release)
EOF
  else
    FABRIC_DIST_PATH="$FABRIC_CORE_PATH/Native/dist/$FABRIC_BUILD_OS/$FABRIC_BUILD_ARCH/$FABRIC_BUILD_TYPE"

    echo "Exporting FABRIC_BUILD_OS=$FABRIC_BUILD_OS"
    export FABRIC_BUILD_OS="$FABRIC_BUILD_OS"
    echo "Exporting FABRIC_BUILD_ARCH=$FABRIC_BUILD_ARCH"
    export FABRIC_BUILD_ARCH="$FABRIC_BUILD_ARCH"
    echo "Exporting FABRIC_BUILD_TYPE=$FABRIC_BUILD_TYPE"
    export FABRIC_BUILD_TYPE="$FABRIC_BUILD_TYPE"

    echo "Setting up ~/Library/Fabric -> .../FabricEngine/Library/Fabric"
    FABRIC_LIBRARY_FABRIC_SRC_PATH="$FABRIC_DIST_PATH/FabricEngine/Library/Fabric"
    FABRIC_LIBRARY_FABRIC_DST_PATH="$HOME/Library/Fabric"
    ln -shf "$FABRIC_LIBRARY_FABRIC_SRC_PATH" "$FABRIC_LIBRARY_FABRIC_DST_PATH"

    echo "Setting up ~/node_modules/Fabric -> .../FabricEngine/node_modules/Fabric"
    FABRIC_NODE_MODULE_SRC_PATH="$FABRIC_DIST_PATH/FabricEngine/node_modules/Fabric"
    FABRIC_NODE_MODULE_DST_PATH="$HOME/node_modules/Fabric"
    ln -shf "$FABRIC_NODE_MODULE_SRC_PATH" "$FABRIC_NODE_MODULE_DST_PATH"

    FABRIC_NPAPI_DST="$HOME/Library/Internet Plug-Ins/Fabric.$FABRIC_BUILD_ARCH.plugin"
    if [ "$FABRIC_NPAPI_LINK" -eq 1 ]; then
      FABRIC_NPAPI_SRC="$FABRIC_DIST_PATH/FabricEngine/Library/Internet Plug-Ins/Fabric.$FABRIC_BUILD_ARCH.plugin"
      echo "Setting up ~/Library/Internet Plug-Ins/Fabric.$FABRIC_BUILD_ARCH.plugin -> .../FabricEngine/Library/Internet Plug-Ins/Fabric.$FABRIC_BUILD_ARCH.plugin"
      ln -shf "$FABRIC_NPAPI_SRC" "$FABRIC_NPAPI_DST"
    else
      echo "Removing ~/Library/Internet Plug-Ins/Fabric.$FABRIC_BUILD_ARCH.plugin"
      rm -f "$FABRIC_NPAPI_DST"
    fi
  fi
fi
