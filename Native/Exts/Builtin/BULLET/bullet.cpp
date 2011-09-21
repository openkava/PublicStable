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

struct fabricBulletShape {
	btCollisionShape * mShape;
};

struct fabricBulletRigidBody {
  fabricBulletShape * mShape;
	btRigidBody * mBody;
  btTransform mInitialTransform;
};

// implement the callbacks
FABRIC_EXT_EXPORT void FabricBULLET_World_Create(
  KL::Data & worldData
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

    // do the reset as well
    world->mDynamicsWorld->getBroadphase()->resetPool(world->mDynamicsWorld->getDispatcher());
    world->mDynamicsWorld->getConstraintSolver()->reset();
    world->mDynamicsWorld->getSolverInfo().m_splitImpulse = false;
    world->mSoftBodyWorldInfo.m_sparsesdf.Reset();
    world->mSoftBodyWorldInfo.m_sparsesdf.GarbageCollect();

    world->mSoftBodyWorldInfo.air_density = 0;
    world->mSoftBodyWorldInfo.water_density = 0;
    world->mSoftBodyWorldInfo.water_offset = 0;
    world->mSoftBodyWorldInfo.water_normal = btVector3(0,0,0);
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_Delete(
  KL::Data & worldData
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

FABRIC_EXT_EXPORT void FabricBULLET_World_SetGravity(
  KL::Data & worldData,
  KL::Vec3 & gravity
)
{
  if(worldData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    world->mDynamicsWorld->setGravity(btVector3(gravity.x,gravity.y,gravity.z));
    world->mSoftBodyWorldInfo.m_gravity = world->mDynamicsWorld->getGravity();
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_Step(
  KL::Data & worldData,
  KL::Size & worldStep,
  KL::Size & worldSubsteps,
  KL::Scalar & timeStep
)
{
  if(worldData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    KL::Scalar frameStep = 1.0f / 30.0f;
    KL::Scalar dt = frameStep / KL::Scalar(worldSubsteps);
    KL::Size nbTimeSteps = KL::Size(floorf(timeStep / frameStep));
    nbTimeSteps *= worldSubsteps;

    for(KL::Size step=0; step<nbTimeSteps; step++, worldStep++) {
      world->mDynamicsWorld->stepSimulation(dt,0,frameStep);
      timeStep -= dt;
    }
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_Reset(
  KL::Data & worldData,
  KL::Size & worldStep
)
{
  if(worldData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    worldStep = 0;

    world->mDynamicsWorld->getBroadphase()->resetPool(world->mDynamicsWorld->getDispatcher());
    world->mDynamicsWorld->getConstraintSolver()->reset();
    world->mDynamicsWorld->getSolverInfo().m_splitImpulse = false;
    world->mSoftBodyWorldInfo.m_sparsesdf.Reset();
    world->mSoftBodyWorldInfo.m_sparsesdf.GarbageCollect();
    
    // loop over all rigid bodies and reset their transform
    btCollisionObjectArray & collisionObjects = world->mDynamicsWorld->getCollisionObjectArray();
    for(size_t i=0;i<collisionObjects.size();i++)
    {
      fabricBulletRigidBody * fabricBody = (fabricBulletRigidBody *)collisionObjects[i]->getUserPointer();
      if(!fabricBody)
        continue;
      btRigidBody * body = static_cast<btRigidBody*>(collisionObjects[i]);
      if(!body)
        continue;

      body->setLinearVelocity(btVector3(0.0f,0.0f,0.0f));
      body->setAngularVelocity(btVector3(0.0f,0.0f,0.0f));
      body->setWorldTransform(fabricBody->mInitialTransform);
    }
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_AddRigidBody(
  KL::Data & worldData,
  KL::Data & bodyData
)
{
  if(worldData != NULL && bodyData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    fabricBulletRigidBody * body = (fabricBulletRigidBody *)bodyData;
    world->mDynamicsWorld->addRigidBody(body->mBody);
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_RemoveRigidBody(
  KL::Data & worldData,
  KL::Data & bodyData
)
{
  if(worldData != NULL && bodyData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    fabricBulletRigidBody * body = (fabricBulletRigidBody *)bodyData;
    world->mDynamicsWorld->removeRigidBody(body->mBody);
  }
}

// implement the callbacks
FABRIC_EXT_EXPORT void FabricBULLET_Shape_Create(
  KL::Data & shapeData,
  KL::Integer & shapeType,
  KL::VariableArray<KL::Scalar> & shapeParameters,
  KL::VariableArray<KL::Vec3> & shapeVertices
)
{
  if(shapeData == NULL) {
    
    // validate the shape type first
    btCollisionShape * collisionShape = NULL;
    if(shapeType == BOX_SHAPE_PROXYTYPE) {
      
      if(shapeParameters.size() != 3) {
        printf("   { FabricBULLET } ERROR: For the box shape you need to specify three parameters.\n");
        return;
      }
      collisionShape = new btBoxShape(btVector3(shapeParameters[0],shapeParameters[1],shapeParameters[2]));
      
    } else if(shapeType == CONVEX_HULL_SHAPE_PROXYTYPE) {
      
      if(shapeParameters.size() != 0) {
        printf("   { FabricBULLET } ERROR: For the convex hull shape you need to specify zero parameters.\n");
        return;
      }
      if(shapeVertices.size() <= 3) {
        printf("   { FabricBULLET } ERROR: For the convex hull shape you need to specify at least 3 vertices.\n");
        return;
      }
      
      btConvexHullShape * hullShape = new btConvexHullShape();
      for(KL::Size i=0;i<shapeVertices.size();i++)
         hullShape->addPoint(btVector3(shapeVertices[i].x,shapeVertices[i].y,shapeVertices[i].z));
      collisionShape = hullShape;

    } else if(shapeType == SPHERE_SHAPE_PROXYTYPE) {

      if(shapeParameters.size() != 1) {
        printf("   { FabricBULLET } ERROR: For the sphere shape you need to specify one parameter.\n");
        return;
      }
      collisionShape = new btSphereShape(shapeParameters[0]);

    //} else if(shapeType == CAPSULE_SHAPE_PROXYTYPE) {
    //} else if(shapeType == CONE_SHAPE_PROXYTYPE) {
    //} else if(shapeType == CYLINDER_SHAPE_PROXYTYPE) {
    //} else if(shapeType == GIMPACT_SHAPE_PROXYTYPE) {
    } else if(shapeType == STATIC_PLANE_PROXYTYPE) {

      if(shapeParameters.size() != 4) {
        printf("   { FabricBULLET } ERROR: For the plane shape you need to specify four parameters.\n");
        return;
      }
      collisionShape = new btStaticPlaneShape(btVector3(shapeParameters[0],shapeParameters[1],shapeParameters[2]),shapeParameters[3]);
      
    //} else if(shapeType == COMPOUND_SHAPE_PROXYTYPE) {
    //} else if(shapeType == SOFTBODY_SHAPE_PROXYTYPE) {
    }
    
    // if we don't have a shape, we can't do this
    if(collisionShape == NULL) {
      printf("   { FabricBULLET } ERROR: For the shape type %d is not supported.\n",int(shapeType));
      return;
    }
    
    collisionShape->setMargin(0.0);

    fabricBulletShape * shape = new fabricBulletShape();
    shapeData = shape;
    shape->mShape = collisionShape;
    
    shape->mShape->setUserPointer(shape);
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_Shape_Delete(
  KL::Data & shapeData
)
{
  if(shapeData != NULL) {
    fabricBulletShape * shape = (fabricBulletShape *)shapeData;
    shapeData = NULL;

    delete( shape->mShape );
    delete( shape );
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_Create(
  KL::Data & bodyData,
  KL::Data & shapeData,
  KL::Xfo & bodyTransform,
  KL::Scalar & bodyMass,
  KL::Scalar & bodyFriction,
  KL::Scalar & bodyRestitution
)
{
  if(bodyData == NULL) {
    if(shapeData == NULL) {
      printf("   { FabricBULLET } ERROR: Cannot create a RigidBody with a NULL shape.\n");
      return;
    }
    
    fabricBulletShape * shape = (fabricBulletShape *)shapeData;

    // convert the transform.
    btTransform transform;
    transform.setOrigin(btVector3(bodyTransform.tr.x,bodyTransform.tr.y,bodyTransform.tr.z));
    transform.setRotation(btQuaternion(bodyTransform.ori.v.x,bodyTransform.ori.v.y,bodyTransform.ori.v.z,bodyTransform.ori.w));
    
    btMotionState* motionState = new btDefaultMotionState(transform);

    btVector3 inertia(0,0,0);
    if(bodyMass > 0.0f)
      shape->mShape->calculateLocalInertia(bodyMass,inertia);
      
    shape->mShape->setLocalScaling(btVector3(bodyTransform.sc.x,bodyTransform.sc.y,bodyTransform.sc.z));

    btRigidBody::btRigidBodyConstructionInfo info(bodyMass,motionState,shape->mShape,inertia);
    info.m_friction = bodyFriction;
    info.m_restitution = bodyRestitution;
    
    fabricBulletRigidBody * body = new fabricBulletRigidBody();
    bodyData = body;
    
    body->mInitialTransform = transform;
    body->mBody = new btRigidBody(info);
    body->mShape = shape;

    if(bodyMass == 0.0f)
    {
      body->mBody->setCollisionFlags( body->mBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
      if(shape->mShape->getShapeType() == STATIC_PLANE_PROXYTYPE)
        body->mBody->setActivationState(ISLAND_SLEEPING);
      else
        body->mBody->setActivationState(DISABLE_DEACTIVATION);
    }
    else
    {
      body->mBody->setCollisionFlags( body->mBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
      body->mBody->setActivationState(DISABLE_DEACTIVATION);
    }
    
    body->mBody->setSleepingThresholds(0.8f,1.0f);
    body->mBody->setDamping(0.3f,0.3f);

    body->mBody->setUserPointer(body);
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_Delete(
  KL::Data & bodyData
)
{
  if(bodyData == NULL) {
    fabricBulletRigidBody * body = (fabricBulletRigidBody * )bodyData;
    delete(body);
    bodyData = NULL;
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_SetMass(
  KL::Data & bodyData,
  KL::Scalar & bodyMass
)
{
  if(bodyData != NULL) {
    fabricBulletRigidBody * body = (fabricBulletRigidBody * )bodyData;
    btVector3 inertia(0,0,0);
    if(bodyMass > 0.0f)
       body->mBody->getCollisionShape()->calculateLocalInertia(bodyMass,inertia);
    body->mBody->setMassProps(bodyMass,inertia);
    if(bodyMass == 0.0f)
      body->mBody->setCollisionFlags( body->mBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    else
      body->mBody->setCollisionFlags( body->mBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_GetTransform(
  KL::Data & bodyData,
  KL::Xfo & bodyTransform
)
{
  if(bodyData != NULL) {
    fabricBulletRigidBody * body = (fabricBulletRigidBody * )bodyData;
    btTransform & transform = body->mBody->getWorldTransform();
    bodyTransform.tr.x = transform.getOrigin().getX();
    bodyTransform.tr.y = transform.getOrigin().getY();
    bodyTransform.tr.z = transform.getOrigin().getZ();
    bodyTransform.ori.v.x = transform.getRotation().getX();
    bodyTransform.ori.v.y = transform.getRotation().getY();
    bodyTransform.ori.v.z = transform.getRotation().getZ();
    bodyTransform.ori.w = transform.getRotation().getW();
    bodyTransform.sc.x = body->mShape->mShape->getLocalScaling().getX();
    bodyTransform.sc.y = body->mShape->mShape->getLocalScaling().getY();
    bodyTransform.sc.z = body->mShape->mShape->getLocalScaling().getZ();
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_SetTransform(
  KL::Data & bodyData,
  KL::Xfo & bodyTransform
)
{
  if(bodyData != NULL) {
    fabricBulletRigidBody * body = (fabricBulletRigidBody * )bodyData;
    if(body->mBody->getInvMass() == 0.0f) {
      btTransform transform;
      transform.setOrigin(btVector3(bodyTransform.tr.x,bodyTransform.tr.y,bodyTransform.tr.z));
      transform.setRotation(btQuaternion(bodyTransform.ori.v.x,bodyTransform.ori.v.y,bodyTransform.ori.v.z,bodyTransform.ori.w));
      body->mBody->setWorldTransform(transform);
      body->mShape->mShape->setLocalScaling(btVector3(bodyTransform.sc.x,bodyTransform.sc.y,bodyTransform.sc.z));
    }
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_GetLinearVelocity(
  KL::Data & bodyData,
  KL::Vec3 & bodyVelocity
)
{
  if(bodyData != NULL) {
    fabricBulletRigidBody * body = (fabricBulletRigidBody * )bodyData;
    btVector3 velocity = body->mBody->getLinearVelocity();
    bodyVelocity.x = velocity.getX();
    bodyVelocity.y = velocity.getY();
    bodyVelocity.z = velocity.getZ();
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_SetLinearVelocity(
  KL::Data & bodyData,
  KL::Vec3 & bodyVelocity
)
{
  if(bodyData != NULL) {
    fabricBulletRigidBody * body = (fabricBulletRigidBody * )bodyData;
    body->mBody->setLinearVelocity(btVector3(bodyVelocity.x,bodyVelocity.y,bodyVelocity.z));
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_GetAngularVelocity(
  KL::Data & bodyData,
  KL::Vec3 & bodyVelocity
)
{
  if(bodyData != NULL) {
    fabricBulletRigidBody * body = (fabricBulletRigidBody * )bodyData;
    btVector3 velocity = body->mBody->getAngularVelocity();
    bodyVelocity.x = velocity.getX();
    bodyVelocity.y = velocity.getY();
    bodyVelocity.z = velocity.getZ();
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_SetAngularVelocity(
  KL::Data & bodyData,
  KL::Vec3 & bodyVelocity
)
{
  if(bodyData != NULL) {
    fabricBulletRigidBody * body = (fabricBulletRigidBody * )bodyData;
    body->mBody->setAngularVelocity(btVector3(bodyVelocity.x,bodyVelocity.y,bodyVelocity.z));
  }  
}

