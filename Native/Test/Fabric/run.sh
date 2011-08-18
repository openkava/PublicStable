#!/bin/sh

. ../helpers.sh

BUILD_OS=$(uname -s)
BUILD_ARCH=$(uname -m)
BUILD_TYPE=Debug

if [ "${BUILD_OS#MINGW}" != "$BUILD_OS" ]; then
  BUILD_OS=Windows
  BUILD_ARCH=x86
fi

WRAPPERS_FILE="../../../Web/Core/FABRIC.Wrappers.js"
if [ "$BUILD_OS" = "Darwin" ]; then
  EXTS_DIR="../../dist/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/FabricEngine/Library/Fabric/Exts"
elif [ "$BUILD_OS" = "Windows" ]; then
  EXTS_DIR="../../dist/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/FabricEngine/Exts"
else
  EXTS_DIR="../../dist/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Exts"
fi

if [ -n "$FABRIC_TEST_WITH_VALGRIND" -a "$BUILD_OS" = "Linux" ]; then
  VALGRIND_CMD="valgrind --suppressions=../valgrind.suppressions.linux --leak-check=full -q"
else
  VALGRIND_CMD=
fi

REPLACE=0
if [ "$1" = "-r" ]; then
  REPLACE=1
  shift
fi

if [ "$BUILD_OS" = "Windows" ]; then
  OUTPUT_FILTER="dos2unix --d2u"
else
  OUTPUT_FILTER=cat
fi

for f in "$@"; do
  TMPFILE=$(tmpfilename)

  #echo $VALGRIND_CMD ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Clients/CLI/fabric --load="'$WRAPPERS_FILE'" --exts="'$EXTS_DIR'" $f
  LD_LIBRARY_PATH=build/ $VALGRIND_CMD ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Clients/CLI/fabric --load="$WRAPPERS_FILE" --exts="$EXTS_DIR" $f 2>&1 | grep -v '^\[FABRIC\] .*Extension registered' | grep -v '^\[FABRIC\] .*Searching extension directory' | $OUTPUT_FILTER >$TMPFILE

  if [ "$REPLACE" -eq 1 ]; then
    mv $TMPFILE ${f%.js}.out
    echo "REPL $(basename $f)"
  else
    if ! cmp $TMPFILE ${f%.js}.out; then
      echo "FAIL $(basename $f)"
      echo "Expected output:"
      cat ${f%.js}.out
      echo "Actual output ($TMPFILE):"
      cat $TMPFILE
      exit 1
    else
      echo "PASS $(basename $f)";
      rm $TMPFILE
    fi
  fi
done
