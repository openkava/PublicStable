/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

#include <string>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletSoftBody/btSoftSoftCollisionAlgorithm.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBody.h>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES
const int maxProxies = 32766;
const int maxOverlap = 65535;

// define structs for the different types of data we support
struct fabricBulletWorld {
	btSoftRigidDynamicsWorld * mDynamicsWorld;
	btBroadphaseInterface*	mBroadphase;
	btCollisionDispatcher*	mDispatcher;
	btConstraintSolver*	mSolver;
	btSoftBodyRigidBodyCollisionConfiguration* mCollisionConfiguration;
  btSoftBodyWorldInfo	mSoftBodyWorldInfo;
};

FABRIC_EXT_EXPORT void FabricBULLET_CreateWorld(
  KL::Data worldData
)
{
  if(worldData == NULL) {
    fabricBulletWorld * world = new fabricBulletWorld();
    worldData = world;
    
    // iniate the world which can deal with softbodies and rigid bodies
    world->mCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    world->mDispatcher = new btCollisionDispatcher(world->mCollisionConfiguration);
    btVector3 worldAabbMin(-10000,-10000,-10000);
    btVector3 worldAabbMax(10000,10000,10000);
    world->mBroadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
    world->mSolver = sol;
    world->mDynamicsWorld = new btSoftRigidDynamicsWorld(world->mDispatcher,world->mBroadphase,world->mSolver,world->mCollisionConfiguration);
    world->mSoftBodyWorldInfo.m_dispatcher = world->mDispatcher;
    world->mSoftBodyWorldInfo.m_broadphase = world->mBroadphase;
    world->mSoftBodyWorldInfo.m_dispatcher = world->mDispatcher;
    btGImpactCollisionAlgorithm::registerAlgorithm(world->mDispatcher);
    world->mSoftBodyWorldInfo.m_sparsesdf.Initialize();
    world->mDynamicsWorld->getDispatchInfo().m_enableSPU = true;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_DeleteWorld(
  KL::Data worldData
)
{
  if(worldData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    worldData = NULL;

    delete( world->mDynamicsWorld );
    delete( world->mSolver );
    delete( world->mBroadphase );
    delete( world->mDispatcher );
    delete( world->mCollisionConfiguration );
    delete( world );
  }
}
