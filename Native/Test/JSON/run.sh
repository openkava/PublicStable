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
OS_SPEC=0
if [ "$1" = "-r" ]; then
  REPLACE=1
  shift
fi
if [ "$1" = "-s" ]; then
  REPLACE=1
  OS_SPEC=1
  shift
fi

for f in "$@"; do
  TMPFILE=$(tmpfilename)

  if ! $VALGRIND_CMD ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Tools/ParseJSON/parseJSON $f >$TMPFILE 2>&1 ; then
    echo "FAIL $(basename $f)"
    echo "To debug, run:"
    echo "gdb --args" $VALGRIND_CMD ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Tools/ParseJSON/parseJSON $f
    exit 1
  fi

  if [ "$REPLACE" -eq 1 ]; then
    if [ "$OS_SPEC" -eq 1 ]; then
      OUTFILE=${f%.json}.$BUILD_OS.$BUILD_ARCH.out
    else
      OUTFILE=${f%.json}.out
    fi
    mv "$TMPFILE" "$OUTFILE"
    echo "REPL $(basename $f)";
  else
    EXPFILE=${f%.json}.$BUILD_OS.$BUILD_ARCH.out
    [ -f "$EXPFILE" ] || EXPFILE=${f%.json}.out
    if ! cmp $TMPFILE $EXPFILE; then
      echo "FAIL $(basename $f)"
      echo "Expected output:"
      if [ -f $EXPFILE ]; then
        cat $EXPFILE
      else
        echo "(missing $EXPFILE)"
      fi
      echo "Actual output ($TMPFILE):"
      cat $TMPFILE
      echo "diff -u:"
      diff -u $EXPFILE $TMPFILE
      echo "To debug, run:"
      echo "gdb --args" $VALGRIND_CMD ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Tools/ParseJSON/parseJSON $f
      exit 1
    else
      echo "PASS $(basename $f)";
      rm $TMPFILE
    fi
  fi
done
