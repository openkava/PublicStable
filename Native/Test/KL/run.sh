#!/bin/sh

. ../helpers.sh

BUILD_OS=$(uname -s)
BUILD_ARCH=$(uname -m)
BUILD_TYPE=Debug

if [ "${BUILD_OS#MINGW}" != "$BUILD_OS" ]; then
  BUILD_OS=Windows
  BUILD_ARCH=x86
fi
if [ "$BUILD_OS" = "Darwin" ]; then
  BUILD_ARCH=universal
fi

if [ -n "$FABRIC_TEST_WITH_VALGRIND" ]; then
  VALGRIND_CMD="valgrind --suppressions=../valgrind.suppressions.$BUILD_OS --leak-check=full -q"
else
  VALGRIND_CMD=
fi

REPLACE=0
if [ "$1" = "-r" ]; then
  REPLACE=1
  shift
fi

for f in "$@"; do
  TMPFILE=$(tmpfilename)

  if ! $VALGRIND_CMD ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Tools/KL/kl --run $f >$TMPFILE 2>&1 ; then
    echo "FAIL $(basename $f)"
    echo "To debug, run:"
    echo "gdb --args" $VALGRIND_CMD ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Tools/KL/kl --run $f
    exit 1
  fi

  if [ "$REPLACE" -eq 1 ]; then
    mv $TMPFILE ${f%.kl}.out
    echo "REPL $(basename $f)";
  elif ! cmp $TMPFILE ${f%.kl}.out; then
    echo "FAIL $(basename $f)"
    echo "Expected output:"
    if [ -f ${f%.kl}.out ]; then
      cat ${f%.kl}.out
    else
      echo "(missing ${f%.kl}.out)"
    fi
    echo "Actual output ($TMPFILE):"
    cat $TMPFILE
    echo "To debug, run:"
    echo "gdb --args" $VALGRIND_CMD ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Tools/KL/kl --run $f
    exit 1
  else
    echo "PASS $(basename $f)";
    rm $TMPFILE
  fi
done
