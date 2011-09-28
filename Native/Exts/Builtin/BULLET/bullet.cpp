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

#pragma pack(1)

// ====================================================================
// KL structs
struct BulletWorld {
  struct LocalData {
    btSoftRigidDynamicsWorld * mDynamicsWorld;
    btBroadphaseInterface*	mBroadphase;
    btCollisionDispatcher*	mDispatcher;
    btConstraintSolver*	mSolver;
    btSoftBodyRigidBodyCollisionConfiguration* mCollisionConfiguration;
    btSoftBodyWorldInfo	mSoftBodyWorldInfo;
  };

  LocalData * localData;
  KL::Vec3 gravity;
  KL::Size step;
  KL::Size substeps;
  KL::Boolean hit;
  KL::Vec3 hitPosition;
};

struct BulletShape {
  struct LocalData {
  	btCollisionShape * mShape;
  };

  LocalData * localData;
  KL::Integer type;
  KL::String name;
  KL::VariableArray<KL::Scalar> parameters;
  KL::VariableArray<KL::Vec3> vertices;
};

struct BulletRigidBody {
  struct LocalData {
    BulletShape::LocalData * mShape;
    btRigidBody * mBody;
    btTransform mInitialTransform;
    BulletWorld::LocalData * mWorld;
  };

  LocalData * localData;
  KL::String name;
  KL::Xfo transform;
  KL::Scalar mass;
  KL::Scalar friction;
  KL::Scalar restitution;
};

struct BulletSoftBody {
  struct LocalData {
    btSoftBody * mBody;
    btAlignedObjectArray<btVector3> mInitialPositions;
    btAlignedObjectArray<btVector3> mInitialNormals;
    KL::Scalar kLST;
    KL::Scalar kDP;
    KL::Scalar kDF;
    KL::Scalar kDG;
    KL::Scalar kVC;
    KL::Scalar kPR;
    KL::Scalar kMT;
    KL::Scalar kCHR;
    KL::Integer piterations;
    BulletWorld::LocalData * mWorld;
  };

  LocalData * localData;
  KL::String name;
  KL::Xfo transform;
  KL::Integer clusters;
  KL::Integer constraints;
  KL::Scalar mass;
  KL::Scalar stiffness;
  KL::Scalar friction;
  KL::Scalar conservation;
  KL::Scalar pressure;
  KL::Scalar recover;
};

struct BulletConstraint {
  struct LocalData {
    btTypedConstraint * mConstraint;
    BulletWorld::LocalData * mWorld;
  };

  LocalData * localData;
  BulletRigidBody::LocalData * bodyLocalDataA;
  BulletRigidBody::LocalData * bodyLocalDataB;
  KL::Integer type;
  KL::String name;
  KL::Xfo pivotA;
  KL::Xfo pivotB;
  KL::String nameA;
  KL::String nameB;
  KL::Integer indexA;
  KL::Integer indexB;
  KL::VariableArray<KL::Scalar> parameters;
};

struct BulletForce {
  KL::String name;
  KL::Vec3 origin;
  KL::Vec3 direction;
  KL::Scalar radius;
  KL::Scalar factor;
  KL::Boolean useTorque;
  KL::Boolean useFalloff;
  KL::Boolean enabled;
  KL::Boolean autoDisable;
};

// ====================================================================
// world implementation
FABRIC_EXT_EXPORT void FabricBULLET_World_Create(
  BulletWorld & world
)
{
  if(world.localData == NULL) {
    world.localData = new BulletWorld::LocalData();
    
    // iniate the world which can deal with softbodies and rigid bodies
    world.localData->mCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    world.localData->mDispatcher = new btCollisionDispatcher(world.localData->mCollisionConfiguration);
    btVector3 worldAabbMin(-10000,-10000,-10000);
    btVector3 worldAabbMax(10000,10000,10000);
    world.localData->mBroadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
    world.localData->mSolver = sol;
    world.localData->mDynamicsWorld = new btSoftRigidDynamicsWorld(world.localData->mDispatcher,world.localData->mBroadphase,world.localData->mSolver,world.localData->mCollisionConfiguration);
    world.localData->mSoftBodyWorldInfo.m_dispatcher = world.localData->mDispatcher;
    world.localData->mSoftBodyWorldInfo.m_broadphase = world.localData->mBroadphase;
    world.localData->mSoftBodyWorldInfo.m_dispatcher = world.localData->mDispatcher;
    btGImpactCollisionAlgorithm::registerAlgorithm(world.localData->mDispatcher);
    
    world.localData->mSoftBodyWorldInfo.m_sparsesdf.Initialize();
    world.localData->mDynamicsWorld->getDispatchInfo().m_enableSPU = true;

    // do the reset as well
    world.localData->mDynamicsWorld->getBroadphase()->resetPool(world.localData->mDynamicsWorld->getDispatcher());
    world.localData->mDynamicsWorld->getConstraintSolver()->reset();
    world.localData->mDynamicsWorld->getSolverInfo().m_splitImpulse = false;
    world.localData->mSoftBodyWorldInfo.m_sparsesdf.Reset();
    world.localData->mSoftBodyWorldInfo.m_sparsesdf.GarbageCollect();

    world.localData->mSoftBodyWorldInfo.air_density = 0;
    world.localData->mSoftBodyWorldInfo.water_density = 0;
    world.localData->mSoftBodyWorldInfo.water_offset = 0;
    world.localData->mSoftBodyWorldInfo.water_normal = btVector3(0,0,0);
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_Delete(
  BulletWorld & world
)
{
  if(world.localData != NULL) {
    delete( world.localData->mDynamicsWorld );
    delete( world.localData->mSolver );
    delete( world.localData->mBroadphase );
    delete( world.localData->mDispatcher );
    delete( world.localData->mCollisionConfiguration );
    delete( world.localData );
    world.localData = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_SetGravity(
  BulletWorld & world
)
{
  if(world.localData != NULL) {
    world.localData->mDynamicsWorld->setGravity(btVector3(world.gravity.x,world.gravity.y,world.gravity.z));
    world.localData->mSoftBodyWorldInfo.m_gravity = world.localData->mDynamicsWorld->getGravity();
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_Step(
  BulletWorld & world,
  KL::Scalar & timeStep
)
{
  if(world.localData != NULL) {
    KL::Scalar frameStep = 1.0f / 30.0f;
    KL::Scalar dt = frameStep / KL::Scalar(world.substeps);
    KL::Size nbTimeSteps = KL::Size(floorf(timeStep / frameStep));
    nbTimeSteps *= world.substeps;

    for(KL::Size step=0; step<nbTimeSteps; step++, world.step++) {
      world.localData->mDynamicsWorld->stepSimulation(dt,0,frameStep);
      timeStep -= dt;
    }
    world.localData->mSoftBodyWorldInfo.m_sparsesdf.GarbageCollect();
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_Reset(
  BulletWorld & world
)
{
  if(world.localData != NULL) {
    world.step = 0;

    world.localData->mDynamicsWorld->getBroadphase()->resetPool(world.localData->mDynamicsWorld->getDispatcher());
    world.localData->mDynamicsWorld->getConstraintSolver()->reset();
    world.localData->mDynamicsWorld->getSolverInfo().m_splitImpulse = false;
    world.localData->mSoftBodyWorldInfo.m_sparsesdf.Reset();
    world.localData->mSoftBodyWorldInfo.m_sparsesdf.GarbageCollect();
    
    // loop over all rigid bodies and reset their transform
    btCollisionObjectArray & collisionObjects = world.localData->mDynamicsWorld->getCollisionObjectArray();
    for(size_t i=0;i<collisionObjects.size();i++)
    {
      BulletRigidBody::LocalData * localData = (BulletRigidBody::LocalData *)collisionObjects[i]->getUserPointer();
      if(!localData)
        continue;
      btRigidBody * body = static_cast<btRigidBody*>(collisionObjects[i]);
      if(!body)
        continue;

      body->setLinearVelocity(btVector3(0.0f,0.0f,0.0f));
      body->setAngularVelocity(btVector3(0.0f,0.0f,0.0f));
      body->setWorldTransform(localData->mInitialTransform);
    }
    
    // loop over all softbodies and reset their positions
    btSoftBodyArray  & softBodies = world.localData->mDynamicsWorld->getSoftBodyArray();
    for(size_t i=0;i<softBodies.size();i++)
    {
      BulletSoftBody::LocalData * localData = (BulletSoftBody::LocalData *)softBodies[i]->getUserPointer();
      if(!localData)
        continue;
      btSoftBody * body = static_cast<btSoftBody*>(softBodies[i]);
      if(!body)
        continue;
      
      btTransform identity;
      identity.setIdentity();
      body->transform(identity);
      for(KL::Size j=0;j<body->m_nodes.size();j++)
      {
        body->m_nodes[j].m_x = localData->mInitialPositions[j];
        body->m_nodes[j].m_q = localData->mInitialPositions[j];
        body->m_nodes[j].m_n = localData->mInitialNormals[j];
        body->m_nodes[j].m_v = btVector3(0.0f,0.0f,0.0f);
        body->m_nodes[j].m_f = btVector3(0.0f,0.0f,0.0f);
      }
      body->updateBounds();
      body->setPose(true,true);
      
      body->m_materials[0]->m_kLST = localData->kLST;
      body->m_materials[0]->m_kAST = localData->kLST;
      body->m_materials[0]->m_kVST = localData->kLST;
      
      body->m_cfg.kDP = localData->kDP;
      body->m_cfg.kDF = localData->kDF;
      body->m_cfg.kDG = localData->kDG;
      body->m_cfg.kVC = localData->kVC;
      body->m_cfg.kPR = localData->kPR;
      body->m_cfg.kMT = localData->kMT;
      body->m_cfg.kCHR = localData->kCHR;
      body->m_cfg.kKHR = localData->kCHR;
      body->m_cfg.kSHR = localData->kCHR;
      body->m_cfg.kAHR = localData->kCHR;
      
      if(body->m_clusters.size() > 0)
      {
        for(int j=0;j<body->m_clusters.size();j++)
        {
          body->m_clusters[j]->m_ldamping = body->m_cfg.kDP;
          body->m_clusters[j]->m_adamping = body->m_cfg.kDP;
        }
      }
      
      body->m_cfg.piterations = localData->piterations;
      body->m_cfg.citerations = localData->piterations;
    }
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_Raycast(
  BulletWorld & world,
  KL::Vec3 & rayOrigin,
  KL::Vec3 & rayDirection
)
{
  if(world.localData != NULL) {
    
    btVector3 from(rayOrigin.x,rayOrigin.y,rayOrigin.z);
    btVector3 to = from + btVector3(rayDirection.x,rayDirection.y,rayDirection.z) * 10000.0f;
    btCollisionWorld::ClosestRayResultCallback callback(from,to);
    world.localData->mDynamicsWorld->rayTest(from,to,callback);
    world.hit = callback.hasHit();
    if(world.hit) {
      // filter out passive object
      if(callback.m_collisionObject) {
        btRigidBody * body = static_cast<btRigidBody*>(callback.m_collisionObject);
        if(body) {
          if(body->getInvMass() == 0.0f) {
            world.hit = false;
            return;
          }
        }
      }
      btVector3 position = from + (to-from) * callback.m_closestHitFraction;
      world.hitPosition.x = position.getX();
      world.hitPosition.y = position.getY();
      world.hitPosition.z = position.getZ();
    }
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_ApplyForce(
  BulletWorld & world,
  BulletForce & force
)
{
  if(world.localData != NULL) {
    
    btVector3 origin(force.origin.x,force.origin.y,force.origin.z);
    btVector3 direction(force.direction.x,force.direction.y,force.direction.z);
    
    // loop over all rigid bodies introduce a new force!
    btCollisionObjectArray & collisionObjects = world.localData->mDynamicsWorld->getCollisionObjectArray();
    for(size_t i=0;i<collisionObjects.size();i++)
    {
      btRigidBody * body = static_cast<btRigidBody*>(collisionObjects[i]);
      if(!body)
        continue;

      // first compute the distance
      float distance = fabs((body->getWorldTransform().getOrigin() - origin).length());
      if(distance > force.radius)
        continue;
      float factor = force.factor * (force.useFalloff ? 1.0f - distance / force.radius : 1.0f);
      
      // either apply central or torque force
      if(force.useTorque) {
        body->applyForce(direction * factor, body->getWorldTransform().inverse() * origin);
      } else
        body->applyCentralForce(direction * factor);
    }
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_AddRigidBody(
  BulletWorld & world,
  BulletRigidBody & body
)
{
  if(world.localData != NULL && body.localData != NULL) {
    world.localData->mDynamicsWorld->addRigidBody(body.localData->mBody);
    body.localData->mWorld = world.localData;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_RemoveRigidBody(
  BulletWorld & world,
  BulletRigidBody & body
)
{
  if(world.localData != NULL && body.localData != NULL) {
    world.localData->mDynamicsWorld->removeRigidBody(body.localData->mBody);
    body.localData->mWorld = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_AddSoftBody(
  BulletWorld & world,
  BulletSoftBody & body
)
{
  if(world.localData != NULL && body.localData != NULL) {
    world.localData->mDynamicsWorld->addSoftBody(body.localData->mBody);
    body.localData->mWorld = world.localData;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_RemoveSoftBody(
  BulletWorld & world,
  BulletSoftBody & body
)
{
  if(world.localData != NULL && body.localData != NULL) {
    world.localData->mDynamicsWorld->removeSoftBody(body.localData->mBody);
    body.localData->mWorld = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_AddConstraint(
  BulletWorld & world,
  BulletConstraint & constraint
)
{
  if(world.localData != NULL && constraint.localData != NULL) {
    world.localData->mDynamicsWorld->addConstraint(constraint.localData->mConstraint);
    constraint.localData->mWorld = world.localData;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_RemoveConstraint(
  BulletWorld & world,
  BulletConstraint & constraint
)
{
  if(world.localData != NULL && constraint.localData != NULL) {
    world.localData->mDynamicsWorld->removeConstraint(constraint.localData->mConstraint);
    constraint.localData->mWorld = NULL;
  }
}

// ====================================================================
// shape implementation
FABRIC_EXT_EXPORT void FabricBULLET_Shape_Create(
  BulletShape & shape
)
{
  if(shape.localData == NULL) {
    
    // validate the shape type first
    btCollisionShape * collisionShape = NULL;
    if(shape.type == BOX_SHAPE_PROXYTYPE) {
      
      if(shape.parameters.size() != 3) {
        throwException( "{FabricBULLET} ERROR: For the box shape you need to specify three parameters." );
        return;
      }
      collisionShape = new btBoxShape(btVector3(shape.parameters[0],shape.parameters[1],shape.parameters[2]));
    } else if(shape.type  == CONVEX_HULL_SHAPE_PROXYTYPE) {
      
      if(shape.parameters.size() != 0) {
        throwException( "{FabricBULLET} ERROR: For the convex hull shape you need to specify zero parameters." );
        return;
      }
      if(shape.vertices.size() <= 3) {
        throwException( "{FabricBULLET} ERROR: For the convex hull shape you need to specify at least 3 vertices." );
        return;
      }
      
      btConvexHullShape * hullShape = new btConvexHullShape();
      for(KL::Size i=0;i<shape.vertices.size();i++)
         hullShape->addPoint(btVector3(shape.vertices[i].x,shape.vertices[i].y,shape.vertices[i].z));
      collisionShape = hullShape;

    } else if(shape.type  == SPHERE_SHAPE_PROXYTYPE) {

      if(shape.parameters.size() != 1) {
        throwException( "{FabricBULLET} ERROR: For the sphere shape you need to specify one parameter." );
        return;
      }
      collisionShape = new btSphereShape(shape.parameters[0]);

    //} else if(shape.type  == CAPSULE_SHAPE_PROXYTYPE) {
    //} else if(shape.type  == CONE_SHAPE_PROXYTYPE) {
    //} else if(shape.type  == CYLINDER_SHAPE_PROXYTYPE) {
    //} else if(shape.type  == GIMPACT_SHAPE_PROXYTYPE) {
    } else if(shape.type  == STATIC_PLANE_PROXYTYPE) {

      if(shape.parameters.size() != 4) {
        throwException( "{FabricBULLET} ERROR: For the plane shape you need to specify four parameters." );
        return;
      }
      collisionShape = new btStaticPlaneShape(btVector3(shape.parameters[0],shape.parameters[1],shape.parameters[2]),shape.parameters[3]);
      
    //} else if(shape.type  == COMPOUND_SHAPE_PROXYTYPE) {
    //} else if(shape.type  == SOFTBODY_SHAPE_PROXYTYPE) {
    }
    
    // if we don't have a shape, we can't do this
    if(collisionShape == NULL) {
      printf("   { FabricBULLET } ERROR: Shape type %d is not supported.\n",int(shape.type ));
      throwException( "{FabricBULLET} ERROR: Shape type is not supported." );
      return;
    }
    
    collisionShape->setMargin(0.0);
    
    shape.localData = new BulletShape::LocalData();
    shape.localData->mShape = collisionShape;
    shape.localData->mShape->setUserPointer(shape.localData);
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_Shape_Delete(
  BulletShape & shape
)
{
  if(shape.localData != NULL) {
    delete( shape.localData->mShape );
    delete( shape.localData );
    shape.localData = NULL;
  }
}

// ====================================================================
// rigidbody implementation
FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_Create(
  BulletRigidBody & body,
  BulletShape & shape
)
{
  if(body.localData == NULL) {
    if(shape.localData == NULL) {
      throwException( "{FabricBULLET} ERROR: Cannot create a RigidBody with an uninitialized shape." );
      return;
    }
    
    // convert the transform.
    btTransform transform;
    transform.setOrigin(btVector3(body.transform.tr.x,body.transform.tr.y,body.transform.tr.z));
    transform.setRotation(btQuaternion(body.transform.ori.v.x,body.transform.ori.v.y,body.transform.ori.v.z,body.transform.ori.w));
    
    btMotionState* motionState = new btDefaultMotionState(transform);

    btVector3 inertia(0,0,0);
    if(body.mass > 0.0f)
      shape.localData->mShape->calculateLocalInertia(body.mass,inertia);
      
    shape.localData->mShape->setLocalScaling(btVector3(body.transform.sc.x,body.transform.sc.y,body.transform.sc.z));

    btRigidBody::btRigidBodyConstructionInfo info(body.mass,motionState,shape.localData->mShape,inertia);
    info.m_friction = body.friction;
    info.m_restitution = body.restitution;
    
    body.localData = new BulletRigidBody::LocalData();
    body.localData->mInitialTransform = transform;
    body.localData->mBody = new btRigidBody(info);
    body.localData->mShape = shape.localData;
    body.localData->mWorld = NULL;

    if(body.mass == 0.0f)
    {
      body.localData->mBody->setCollisionFlags( body.localData->mBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
      if(shape.localData->mShape->getShapeType() == STATIC_PLANE_PROXYTYPE)
        body.localData->mBody->setActivationState(ISLAND_SLEEPING);
      else
        body.localData->mBody->setActivationState(DISABLE_DEACTIVATION);
    }
    else
    {
      body.localData->mBody->setCollisionFlags( body.localData->mBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
      body.localData->mBody->setActivationState(DISABLE_DEACTIVATION);
    }
    
    body.localData->mBody->setSleepingThresholds(0.8f,1.0f);
    body.localData->mBody->setDamping(0.3f,0.3f);

    body.localData->mBody->setUserPointer(body.localData);
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_Delete(
  BulletRigidBody & body
)
{
  if(body.localData != NULL) {
    if(body.localData->mWorld != NULL)
      body.localData->mWorld->mDynamicsWorld->removeRigidBody(body.localData->mBody);
    delete(body.localData->mBody);
    delete(body.localData);
    body.localData = NULL;
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_SetMass(
  BulletRigidBody & body
)
{
  if(body.localData != NULL) {
    btVector3 inertia(0,0,0);
    if(body.mass > 0.0f)
       body.localData->mBody->getCollisionShape()->calculateLocalInertia(body.mass,inertia);
    body.localData->mBody->setMassProps(body.mass,inertia);
    if(body.mass == 0.0f)
      body.localData->mBody->setCollisionFlags( body.localData->mBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    else
      body.localData->mBody->setCollisionFlags( body.localData->mBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_GetTransform(
  BulletRigidBody & body
)
{
  if(body.localData != NULL) {
    btTransform & transform = body.localData->mBody->getWorldTransform();
    body.transform.tr.x = transform.getOrigin().getX();
    body.transform.tr.y = transform.getOrigin().getY();
    body.transform.tr.z = transform.getOrigin().getZ();
    body.transform.ori.v.x = transform.getRotation().getX();
    body.transform.ori.v.y = transform.getRotation().getY();
    body.transform.ori.v.z = transform.getRotation().getZ();
    body.transform.ori.w = transform.getRotation().getW();
    body.transform.sc.x = body.localData->mShape->mShape->getLocalScaling().getX();
    body.transform.sc.y = body.localData->mShape->mShape->getLocalScaling().getY();
    body.transform.sc.z = body.localData->mShape->mShape->getLocalScaling().getZ();
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_SetTransform(
  BulletRigidBody & body
)
{
  if(body.localData != NULL) {
    if(body.localData->mBody->getInvMass() == 0.0f) {
      btTransform transform;
      transform.setOrigin(btVector3(body.transform.tr.x,body.transform.tr.y,body.transform.tr.z));
      transform.setRotation(btQuaternion(body.transform.ori.v.x,body.transform.ori.v.y,body.transform.ori.v.z,body.transform.ori.w));
      body.localData->mBody->setWorldTransform(transform);
      body.localData->mShape->mShape->setLocalScaling(btVector3(body.transform.sc.x,body.transform.sc.y,body.transform.sc.z));
    }
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_GetLinearVelocity(
  BulletRigidBody & body,
  KL::Vec3 & bodyVelocity
)
{
  if(body.localData != NULL) {
    btVector3 velocity = body.localData->mBody->getLinearVelocity();
    bodyVelocity.x = velocity.getX();
    bodyVelocity.y = velocity.getY();
    bodyVelocity.z = velocity.getZ();
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_SetLinearVelocity(
  BulletRigidBody & body,
  KL::Vec3 & bodyVelocity
)
{
  if(body.localData != NULL) {
    body.localData->mBody->setLinearVelocity(btVector3(bodyVelocity.x,bodyVelocity.y,bodyVelocity.z));
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_GetAngularVelocity(
  BulletRigidBody & body,
  KL::Vec3 & bodyVelocity
)
{
  if(body.localData != NULL) {
    btVector3 velocity = body.localData->mBody->getAngularVelocity();
    bodyVelocity.x = velocity.getX();
    bodyVelocity.y = velocity.getY();
    bodyVelocity.z = velocity.getZ();
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_RigidBody_SetAngularVelocity(
  BulletRigidBody & body,
  KL::Vec3 & bodyVelocity
)
{
  if(body.localData != NULL) {
    body.localData->mBody->setAngularVelocity(btVector3(bodyVelocity.x,bodyVelocity.y,bodyVelocity.z));
  }  
}

// ====================================================================
// softbody implementation
FABRIC_EXT_EXPORT void FabricBULLET_SoftBody_Create(
  BulletSoftBody & body,
  BulletWorld & world,
  KL::SlicedArray<KL::Vec3> & positions,
  KL::SlicedArray<KL::Vec3> & normals,
  KL::VariableArray<KL::Integer> & indices
)
{
  if(body.localData == NULL && world.localData != NULL) {
    // convert the transform.
    btTransform transform;
    transform.setOrigin(btVector3(body.transform.tr.x,body.transform.tr.y,body.transform.tr.z));
    transform.setRotation(btQuaternion(body.transform.ori.v.x,body.transform.ori.v.y,body.transform.ori.v.z,body.transform.ori.w));
    btVector3 scaling(body.transform.sc.x,body.transform.sc.y,body.transform.sc.z);
    btVector3 axis = transform.getRotation().getAxis();
    KL::Scalar angle = transform.getRotation().getAngle();

    body.localData = new BulletSoftBody::LocalData();  

    body.localData->mInitialPositions.resize(positions.size());
    body.localData->mInitialNormals.resize(normals.size());
    for(size_t i=0;i<body.localData->mInitialPositions.size();i++) {
      body.localData->mInitialPositions[i] = transform * btVector3(positions[i].x,positions[i].y,positions[i].z);
      body.localData->mInitialNormals[i] = btVector3(normals[i].x,normals[i].y,normals[i].z).rotate(axis,angle);
    }
    
    body.localData->mBody = new btSoftBody(&world.localData->mSoftBodyWorldInfo,body.localData->mInitialPositions.size(),&body.localData->mInitialPositions[0],0);
    body.localData->mWorld = NULL;
    
    for(KL::Size i=0;i<indices.size()-2;i+=3)
    {
      body.localData->mBody->appendFace(indices[i],indices[i+1],indices[i+2]);
      body.localData->mBody->appendLink(indices[i],indices[i+1]);
      body.localData->mBody->appendLink(indices[i+1],indices[i+2]);
      body.localData->mBody->appendLink(indices[i+2],indices[i]);
    }

    KL::Scalar massPart = body.mass / KL::Scalar(body.localData->mBody->m_nodes.size());
    for(KL::Size i=0;i<body.localData->mBody->m_nodes.size();i++)
    {
      body.localData->mBody->setMass(i,massPart);
      body.localData->mBody->m_nodes[i].m_n = body.localData->mInitialNormals[i];
    }
    
    body.localData->mBody->getCollisionShape()->setMargin(0.0f);
    if(body.clusters >= 0)
    {
      body.localData->mBody->generateClusters(body.clusters);
      body.localData->mBody->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS + btSoftBody::fCollision::CL_SELF;
    }
    else
      body.localData->mBody->m_cfg.collisions += btSoftBody::fCollision::VF_SS;

    body.localData->mBody->m_cfg.aeromodel = btSoftBody::eAeroModel::V_TwoSided;

    if(body.constraints > 0)
      body.localData->mBody->generateBendingConstraints(body.constraints,body.localData->mBody->m_materials[0]);

    body.localData->mBody->setPose(true,true);
    
    body.localData->mBody->m_materials[0]->m_kLST = body.stiffness;
    body.localData->mBody->m_materials[0]->m_kAST = body.stiffness;
    body.localData->mBody->m_materials[0]->m_kVST = body.stiffness;
    
    body.localData->mBody->m_cfg.kDP = 0.01f;
    body.localData->mBody->m_cfg.kDF = body.friction;
    body.localData->mBody->m_cfg.kDG = 0.1f;
    body.localData->mBody->m_cfg.kVC = body.conservation;
    body.localData->mBody->m_cfg.kPR = body.pressure;
    body.localData->mBody->m_cfg.kMT = body.recover;
    body.localData->mBody->m_cfg.kCHR = 0.25f;
    body.localData->mBody->m_cfg.kKHR = 0.25f;
    body.localData->mBody->m_cfg.kSHR = 0.25f;
    body.localData->mBody->m_cfg.kAHR = 0.25f;
    
    if(body.localData->mBody->m_clusters.size() > 0)
    {
      for(int j=0;j<body.localData->mBody->m_clusters.size();j++)
      {
        body.localData->mBody->m_clusters[j]->m_ldamping = body.localData->mBody->m_cfg.kDP;
        body.localData->mBody->m_clusters[j]->m_adamping = body.localData->mBody->m_cfg.kDP;
      }
    }
    
    body.localData->mBody->m_cfg.piterations = 4;
    body.localData->mBody->m_cfg.citerations = 4;
    
    body.localData->kLST = body.localData->mBody->m_materials[0]->m_kLST;
    body.localData->kDP = body.localData->mBody->m_cfg.kDP;
    body.localData->kDF = body.localData->mBody->m_cfg.kDF;
    body.localData->kDG = body.localData->mBody->m_cfg.kDG;
    body.localData->kVC = body.localData->mBody->m_cfg.kVC;
    body.localData->kPR = body.localData->mBody->m_cfg.kPR;
    body.localData->kMT = body.localData->mBody->m_cfg.kMT;
    body.localData->kCHR = body.localData->mBody->m_cfg.kCHR;
    body.localData->piterations = body.localData->mBody->m_cfg.piterations;
    
    body.localData->mBody->setUserPointer(body.localData);
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_SoftBody_Delete(
  BulletSoftBody & body
)
{
  if(body.localData != NULL) {
    if(body.localData->mWorld != NULL)
      body.localData->mWorld->mDynamicsWorld->removeSoftBody(body.localData->mBody);
    delete(body.localData->mBody);
    delete(body.localData);
    body.localData = NULL;
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_SoftBody_GetPosition(
  KL::Size index,
  BulletSoftBody & body,
  KL::Vec3 & position,
  KL::Vec3 & normal
)
{
  if(body.localData != NULL) {
    position.x = body.localData->mBody->m_nodes[index].m_x.getX();
    position.y = body.localData->mBody->m_nodes[index].m_x.getY();
    position.z = body.localData->mBody->m_nodes[index].m_x.getZ();
    normal.x = body.localData->mBody->m_nodes[index].m_n.getX();
    normal.y = body.localData->mBody->m_nodes[index].m_n.getY();
    normal.z = body.localData->mBody->m_nodes[index].m_n.getZ();
  }  
}

// ====================================================================
// constraint implementation
FABRIC_EXT_EXPORT void FabricBULLET_Constraint_Create(
  BulletConstraint & constraint
)
{
  if(constraint.localData == NULL) {
    
    // check the bodies
    if(!constraint.bodyLocalDataA)
    {
      throwException( "{FabricBULLET} ERROR: bodyLocalDataA is NULL when creating constraint." );
      return;
    }
    if(!constraint.bodyLocalDataB)
    {
      throwException( "{FabricBULLET} ERROR: bodyLocalDataB is NULL when creating constraint." );
      return;
    }

    // convert the transforms    
    btTransform pivotTransformA;
    pivotTransformA.setOrigin(btVector3(constraint.pivotA.tr.x,constraint.pivotA.tr.y,constraint.pivotA.tr.z));
    pivotTransformA.setRotation(btQuaternion(constraint.pivotA.ori.v.x,constraint.pivotA.ori.v.y,constraint.pivotA.ori.v.z,constraint.pivotA.ori.w));
    btTransform pivotTransformB;
    pivotTransformB.setOrigin(btVector3(constraint.pivotB.tr.x,constraint.pivotB.tr.y,constraint.pivotB.tr.z));
    pivotTransformB.setRotation(btQuaternion(constraint.pivotB.ori.v.x,constraint.pivotB.ori.v.y,constraint.pivotB.ori.v.z,constraint.pivotB.ori.w));

    // validate the shape type first
    btTypedConstraint * typedConstraint = NULL;
    if(constraint.type == POINT2POINT_CONSTRAINT_TYPE) {
      
      if(constraint.parameters.size() != 0) {
        throwException( "{FabricBULLET} ERROR: For the point2point constraint you need to specify zero parameters." );
        return;
      }
      
      typedConstraint = new btPoint2PointConstraint(
         *constraint.bodyLocalDataA->mBody,*constraint.bodyLocalDataB->mBody,pivotTransformA.getOrigin(),pivotTransformB.getOrigin());
    } else if(constraint.type == HINGE_CONSTRAINT_TYPE) {
      
      if(constraint.parameters.size() != 0) {
        throwException( "{FabricBULLET} ERROR: For the hinge constraint you need to specify zero parameters." );
        return;
      }

      btVector3 axisA = pivotTransformA.getBasis() * btVector3(1,0,0);
      btVector3 axisB = pivotTransformB.getBasis() * btVector3(1,0,0);

      typedConstraint = new btHingeConstraint(
         *constraint.bodyLocalDataA->mBody,*constraint.bodyLocalDataA->mBody,pivotTransformA.getOrigin(),pivotTransformB.getOrigin(),axisA,axisB,true);
    } else if(constraint.type == SLIDER_CONSTRAINT_TYPE) {
      
      if(constraint.parameters.size() != 0) {
        throwException( "{FabricBULLET} ERROR: For the slider constraint you need to specify zero parameters." );
        return;
      }

      typedConstraint = new btSliderConstraint(
         *constraint.bodyLocalDataA->mBody,*constraint.bodyLocalDataB->mBody,pivotTransformA,pivotTransformB,true);
    }
    
    // if we don't have a shape, we can't do this
    if(typedConstraint == NULL) {
      printf("   { FabricBULLET } ERROR: Constraint type %d is not supported.\n",int(constraint.type));
      throwException( "{FabricBULLET} ERROR: Constraint type is not supported." );
      return;
    }

    constraint.localData = new BulletConstraint::LocalData();
    constraint.localData->mConstraint = typedConstraint;
    constraint.localData->mWorld = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_Constraint_Delete(
  BulletConstraint & constraint
)
{
  if(constraint.localData != NULL) {
    if(constraint.localData->mWorld != NULL)
      constraint.localData->mWorld->mDynamicsWorld->removeConstraint(constraint.localData->mConstraint);
    delete( constraint.localData->mConstraint );
    delete( constraint.localData );
    constraint.localData = NULL;
  }
}
