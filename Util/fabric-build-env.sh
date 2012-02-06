#!/bin/sh

if [ -z "$FABRIC_CORE_ROOT" ]; then
  cat >&2 <<EOF
The FABRIC_CORE_ROOT environment variable must be set
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

  OPTIND=0
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
    FABRIC_DIST_PATH="$FABRIC_CORE_ROOT/Native/dist/$FABRIC_BUILD_OS/$FABRIC_BUILD_ARCH/$FABRIC_BUILD_TYPE"

    echo "Exporting FABRIC_BUILD_OS=$FABRIC_BUILD_OS"
    export FABRIC_BUILD_OS="$FABRIC_BUILD_OS"
    echo "Exporting FABRIC_BUILD_ARCH=$FABRIC_BUILD_ARCH"
    export FABRIC_BUILD_ARCH="$FABRIC_BUILD_ARCH"
    echo "Exporting FABRIC_BUILD_TYPE=$FABRIC_BUILD_TYPE"
    export FABRIC_BUILD_TYPE="$FABRIC_BUILD_TYPE"

    case "$FABRIC_BUILD_OS" in
      Darwin)
        FABRIC_LIBRARY_DST_DIR="$HOME/Library/Fabric"
        ;;
      Linux)
        FABRIC_LIBRARY_DST_DIR="$HOME/.fabric"
        ;;
    esac
    echo "Creating directory $FABRIC_LIBRARY_DST_DIR"
    mkdir -p "$FABRIC_LIBRARY_DST_DIR"

    case "$FABRIC_BUILD_OS" in
      Darwin)
        FABRIC_EXTS_SRC="$FABRIC_DIST_PATH/FabricEngine/Library/Fabric/Exts"
        ;;
      Linux)
        FABRIC_EXTS_SRC="$FABRIC_DIST_PATH/Exts"
        ;;
    esac
    FABRIC_EXTS_DST="$FABRIC_LIBRARY_DST_DIR/Exts"
    echo "Setting up $FABRIC_EXTS_DST -> $FABRIC_EXTS_SRC"
    ln -snf "$FABRIC_EXTS_SRC" "$FABRIC_EXTS_DST"

    FABRIC_NODE_MODULE_SRC="$FABRIC_DIST_PATH/FabricEngine/node_modules/Fabric"
    FABRIC_NODE_MODULE_DST="$HOME/node_modules/Fabric"
    echo "Linking $FABRIC_NODE_MODULE_DST -> $FABRIC_NODE_MODULE_SRC"
    ln -snf "$FABRIC_NODE_MODULE_SRC" "$FABRIC_NODE_MODULE_DST"

    FABRIC_PYTHON_MODULE_SRC="$FABRIC_DIST_PATH/FabricEngine/python_modules/fabric"
    FABRIC_PYTHON_MODULE_DIR="$HOME/python_modules"
    FABRIC_PYTHON_MODULE_DST="$FABRIC_PYTHON_MODULE_DIR/fabric"
    echo "Linking $FABRIC_PYTHON_MODULE_DST -> $FABRIC_PYTHON_MODULE_SRC"
    mkdir -p "$FABRIC_PYTHON_MODULE_DIR"
    ln -snf "$FABRIC_PYTHON_MODULE_SRC" "$FABRIC_PYTHON_MODULE_DST"

    case "$FABRIC_BUILD_OS" in
      Darwin)
        FABRIC_NPAPI_SRC="$FABRIC_DIST_PATH/FabricEngine/Library/Internet Plug-Ins/Fabric.$FABRIC_BUILD_ARCH.plugin"
        FABRIC_NPAPI_DST_DIR="$HOME/Library/Internet Plug-Ins"
        FABRIC_NPAPI_DST="$FABRIC_NPAPI_DST_DIR/Fabric.$FABRIC_BUILD_ARCH.plugin"
        ;;
      Linux)
        FABRIC_NPAPI_SRC="$FABRIC_DIST_PATH/NPAPI/libFabricPlugin.so"
        FABRIC_NPAPI_DST_DIR="$HOME/.mozilla/plugins"
        FABRIC_NPAPI_DST="$FABRIC_NPAPI_DST_DIR/libFabricPlugin.so"
        ;;
    esac
    if [ "$FABRIC_NPAPI_LINK" -eq 1 ]; then
      echo "Creating directory $FABRIC_NPAPI_DST_DIR"
      mkdir -p "$FABRIC_NPAPI_DST_DIR"
      echo "Linking $FABRIC_NPAPI_DST -> $FABRIC_NPAPI_SRC"
      ln -snf "$FABRIC_NPAPI_SRC" "$FABRIC_NPAPI_DST"
    else
      echo "Removing $FABRIC_NPAPI_DST"
      rm -f "$FABRIC_NPAPI_DST"
    fi
  fi
fi
