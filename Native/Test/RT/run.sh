#!/bin/sh
. ../helpers.sh

if [ -n "$FABRIC_TEST_WITH_VALGRIND" ]; then
  VALGRIND_CMD="valgrind --suppressions=../valgrind.suppressions.$FABRIC_BUILD_OS --leak-check=full -q"
else
  VALGRIND_CMD=
fi

REPLACE=0
if [ "$1" = "-r" ]; then
  REPLACE=1
  shift
fi

if [ "$FABRIC_BUILD_OS" = "Windows" ]; then
  OUTPUT_FILTER="dos2unix --d2u"
else
  OUTPUT_FILTER=cat
fi

ERROR=0
for f in "$@"; do
  TMPFILE=$(tmpfilename)

  CMD="node $f"
  
  NODE_PATH="$NODE_PATH" $VALGRIND_CMD $CMD 2>&1 \
    | grep -v '^\[FABRIC\] Fabric Engine version' \
    | grep -v '^\[FABRIC\] This build of Fabric' \
    | grep -v '^\[FABRIC\] .*Extension registered' \
    | grep -v '^\[FABRIC\] .*Searching extension directory' \
    | grep -v '^\[FABRIC\] .*unable to open extension directory' \
    | $OUTPUT_FILTER >$TMPFILE

  if [ "$REPLACE" -eq 1 ]; then
    mv $TMPFILE ${f%.js}.out
    echo "REPL $(basename $f)"
  else
    EXPFILE=${f%.js}.$FABRIC_BUILD_OS.$FABRIC_BUILD_ARCH.out
    [ -f "$EXPFILE" ] || EXPFILE=${f%.js}.out
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
      echo "To debug:"
      echo NODE_PATH="$NODE_PATH" "gdb --args" $CMD
      ERROR=1
      break
    else
      echo "PASS $(basename $f)";
      rm $TMPFILE
    fi
  fi
done
if [ "$ERROR" -eq 1 ]; then
	exit 1
fi
