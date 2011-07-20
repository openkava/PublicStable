#!/bin/sh

. ../helpers.sh

BUILD_OS=$(uname -s)
BUILD_ARCH=$(uname -m)
BUILD_TYPE=Debug

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

for f in "$@"; do
  TMPFILE=$(tmpfilename)

  echo ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Tools/KL/kl --ast --run $f
  if ! $VALGRIND_CMD ../../build/$BUILD_OS/$BUILD_ARCH/$BUILD_TYPE/Fabric/Tools/KL/kl --ast --run $f >$TMPFILE 2>&1 ; then
    echo "FAIL $(basename $f)"
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
    exit 1
  else
    echo "PASS $(basename $f)";
    rm $TMPFILE
  fi
done
