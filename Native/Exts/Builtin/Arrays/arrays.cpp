/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>
#include <string>

using namespace Fabric::EDK;

FABRIC_EXT_EXPORT void incShiftVec3Array
(
  KL::VariableArray<KL::Vec3> & inputArray,
  KL::VariableArray<KL::Vec3> & outputArray
)
{
  if(inputArray.size() != outputArray.size())
    return; 
  memcpy(&outputArray[1],&inputArray[0],sizeof(KL::Vec3) * (outputArray.size()-1));
}

FABRIC_EXT_EXPORT void decShiftVec3Array
(
  KL::VariableArray<KL::Vec3> & inputArray,
  KL::VariableArray<KL::Vec3> & outputArray
)
{
  if(inputArray.size() != outputArray.size())
    return; 
  memcpy(&outputArray[0],&inputArray[1],sizeof(KL::Vec3) * (outputArray.size()-1));
}
