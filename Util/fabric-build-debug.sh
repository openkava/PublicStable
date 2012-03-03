#!/bin/sh
source $FABRIC_CORE_PATH/Util/fabric-build-env.sh Debug
scons -C $FABRIC_CORE_PATH -j8 2>&1 \
  | sed -l "s,build/Native/Darwin/universal/Release/Fabric/,$FABRIC_CORE_PATH/Native/,g" \
  | sed -l "s,build/Native/Darwin/universal/Debug/Fabric/,$FABRIC_CORE_PATH/Native/,g"
