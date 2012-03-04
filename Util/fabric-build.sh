#!/bin/sh
scons -C $FABRIC_CORE_PATH -k "$@" 2>&1 \
  | sed -l "s,$FABRIC_CORE_PATH/build/,$FABRIC_CORE_PATH/,g"
