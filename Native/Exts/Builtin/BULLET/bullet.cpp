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
  void * mWorld;
};

struct fabricBulletSoftBody {
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
  void * mWorld;
};

struct fabricBulletConstraint {
	btTypedConstraint * mConstraint;
  void * mWorld;
};

struct fabricBulletForce {
  KL::Vec3 origin;
  KL::Vec3 direction;
  KL::Scalar radius;
  KL::Scalar factor;
  KL::Boolean useTorque;
  KL::Boolean useFalloff;
  KL::Boolean enabled;
  KL::Boolean autoDisable;
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
    world->mSoftBodyWorldInfo.m_sparsesdf.GarbageCollect();
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
    
    // loop over all softbodies and reset their positions
    btSoftBodyArray  & softBodies = world->mDynamicsWorld->getSoftBodyArray();
    for(size_t i=0;i<softBodies.size();i++)
    {
      fabricBulletSoftBody * fabricBody = (fabricBulletSoftBody *)softBodies[i]->getUserPointer();
      if(!fabricBody)
        continue;
      btSoftBody * body = static_cast<btSoftBody*>(softBodies[i]);
      if(!body)
        continue;
      
      btTransform identity;
      identity.setIdentity();
      body->transform(identity);
      for(KL::Size j=0;j<body->m_nodes.size();j++)
      {
        body->m_nodes[j].m_x = fabricBody->mInitialPositions[j];
        body->m_nodes[j].m_q = fabricBody->mInitialPositions[j];
        body->m_nodes[j].m_n = fabricBody->mInitialNormals[j];
        body->m_nodes[j].m_v = btVector3(0.0f,0.0f,0.0f);
        body->m_nodes[j].m_f = btVector3(0.0f,0.0f,0.0f);
      }
      body->updateBounds();
      body->setPose(true,true);
      
      body->m_materials[0]->m_kLST = fabricBody->kLST;
      body->m_materials[0]->m_kAST = fabricBody->kLST;
      body->m_materials[0]->m_kVST = fabricBody->kLST;
      
      body->m_cfg.kDP = fabricBody->kDP;
      body->m_cfg.kDF = fabricBody->kDF;
      body->m_cfg.kDG = fabricBody->kDG;
      body->m_cfg.kVC = fabricBody->kVC;
      body->m_cfg.kPR = fabricBody->kPR;
      body->m_cfg.kMT = fabricBody->kMT;
      body->m_cfg.kCHR = fabricBody->kCHR;
      body->m_cfg.kKHR = fabricBody->kCHR;
      body->m_cfg.kSHR = fabricBody->kCHR;
      body->m_cfg.kAHR = fabricBody->kCHR;
      
      if(body->m_clusters.size() > 0)
      {
        for(int j=0;j<body->m_clusters.size();j++)
        {
          body->m_clusters[j]->m_ldamping = body->m_cfg.kDP;
          body->m_clusters[j]->m_adamping = body->m_cfg.kDP;
        }
      }
      
      body->m_cfg.piterations = fabricBody->piterations;
      body->m_cfg.citerations = fabricBody->piterations;
    }
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_Raycast(
  KL::Data & worldData,
  KL::Vec3 & rayOrigin,
  KL::Vec3 & rayDirection,
  KL::Boolean & hit,
  KL::Vec3 & hitPosition
)
{
  if(worldData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    
    btVector3 from(rayOrigin.x,rayOrigin.y,rayOrigin.z);
    btVector3 to = from + btVector3(rayDirection.x,rayDirection.y,rayDirection.z) * 10000.0f;
    btCollisionWorld::ClosestRayResultCallback callback(from,to);
    world->mDynamicsWorld->rayTest(from,to,callback);
    hit = callback.hasHit();
    if(hit) {
      // filter out passive object
      if(callback.m_collisionObject) {
        btRigidBody * body = static_cast<btRigidBody*>(callback.m_collisionObject);
        if(body) {
          if(body->getInvMass() == 0.0f) {
            hit = false;
            return;
          }
        }
      }
      btVector3 position = from + (to-from) * callback.m_closestHitFraction;
      hitPosition.x = position.getX();
      hitPosition.y = position.getY();
      hitPosition.z = position.getZ();
    }
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_ApplyForce(
  KL::Data & worldData,
  fabricBulletForce & force
)
{
  if(worldData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    
    btVector3 origin(force.origin.x,force.origin.y,force.origin.z);
    btVector3 direction(force.direction.x,force.direction.y,force.direction.z);
    
    // loop over all rigid bodies introduce a new force!
    btCollisionObjectArray & collisionObjects = world->mDynamicsWorld->getCollisionObjectArray();
    for(size_t i=0;i<collisionObjects.size();i++)
    {
      fabricBulletRigidBody * fabricBody = (fabricBulletRigidBody *)collisionObjects[i]->getUserPointer();
      if(!fabricBody)
        continue;
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
  KL::Data & worldData,
  KL::Data & bodyData
)
{
  if(worldData != NULL && bodyData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    fabricBulletRigidBody * body = (fabricBulletRigidBody *)bodyData;
    world->mDynamicsWorld->addRigidBody(body->mBody);
    body->mWorld = world;
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
    body->mWorld = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_AddSoftBody(
  KL::Data & worldData,
  KL::Data & bodyData
)
{
  if(worldData != NULL && bodyData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    fabricBulletSoftBody * body = (fabricBulletSoftBody *)bodyData;
    world->mDynamicsWorld->addSoftBody(body->mBody);
    body->mWorld = world;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_RemoveSoftBody(
  KL::Data & worldData,
  KL::Data & bodyData
)
{
  if(worldData != NULL && bodyData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    fabricBulletSoftBody * body = (fabricBulletSoftBody *)bodyData;
    world->mDynamicsWorld->removeSoftBody(body->mBody);
    body->mWorld = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_AddConstraint(
  KL::Data & worldData,
  KL::Data & constraintData
)
{
  if(worldData != NULL && constraintData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    fabricBulletConstraint * constraint = (fabricBulletConstraint *)constraintData;
    world->mDynamicsWorld->addConstraint(constraint->mConstraint);
    constraint->mWorld = world;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_World_RemoveConstraint(
  KL::Data & worldData,
  KL::Data & constraintData
)
{
  if(worldData != NULL && constraintData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    fabricBulletConstraint * constraint = (fabricBulletConstraint *)constraintData;
    world->mDynamicsWorld->removeConstraint(constraint->mConstraint);
    constraint->mWorld = NULL;
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
        throwException( "{FabricBULLET} ERROR: For the box shape you need to specify three parameters." );
        return;
      }
      collisionShape = new btBoxShape(btVector3(shapeParameters[0],shapeParameters[1],shapeParameters[2]));
      
    } else if(shapeType == CONVEX_HULL_SHAPE_PROXYTYPE) {
      
      if(shapeParameters.size() != 0) {
        throwException( "{FabricBULLET} ERROR: For the convex hull shape you need to specify zero parameters." );
        return;
      }
      if(shapeVertices.size() <= 3) {
        throwException( "{FabricBULLET} ERROR: For the convex hull shape you need to specify at least 3 vertices." );
        return;
      }
      
      btConvexHullShape * hullShape = new btConvexHullShape();
      for(KL::Size i=0;i<shapeVertices.size();i++)
         hullShape->addPoint(btVector3(shapeVertices[i].x,shapeVertices[i].y,shapeVertices[i].z));
      collisionShape = hullShape;

    } else if(shapeType == SPHERE_SHAPE_PROXYTYPE) {

      if(shapeParameters.size() != 1) {
        throwException( "{FabricBULLET} ERROR: For the sphere shape you need to specify one parameter." );
        return;
      }
      collisionShape = new btSphereShape(shapeParameters[0]);

    //} else if(shapeType == CAPSULE_SHAPE_PROXYTYPE) {
    //} else if(shapeType == CONE_SHAPE_PROXYTYPE) {
    //} else if(shapeType == CYLINDER_SHAPE_PROXYTYPE) {
    //} else if(shapeType == GIMPACT_SHAPE_PROXYTYPE) {
    } else if(shapeType == STATIC_PLANE_PROXYTYPE) {

      if(shapeParameters.size() != 4) {
        throwException( "{FabricBULLET} ERROR: For the plane shape you need to specify four parameters." );
        return;
      }
      collisionShape = new btStaticPlaneShape(btVector3(shapeParameters[0],shapeParameters[1],shapeParameters[2]),shapeParameters[3]);
      
    //} else if(shapeType == COMPOUND_SHAPE_PROXYTYPE) {
    //} else if(shapeType == SOFTBODY_SHAPE_PROXYTYPE) {
    }
    
    // if we don't have a shape, we can't do this
    if(collisionShape == NULL) {
      printf("   { FabricBULLET } ERROR: Shape type %d is not supported.\n",int(shapeType));
      throwException( "{FabricBULLET} ERROR: Shape type is not supported." );
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
      throwException( "{FabricBULLET} ERROR: Cannot create a RigidBody with a NULL shape." );
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
    body->mWorld = NULL;

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
  if(bodyData != NULL) {
    fabricBulletRigidBody * body = (fabricBulletRigidBody * )bodyData;
    if(body->mWorld != NULL)
      FabricBULLET_World_RemoveRigidBody(body->mWorld,bodyData);
    delete(body->mBody);
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

FABRIC_EXT_EXPORT void FabricBULLET_SoftBody_Create(
  KL::Data & bodyData,
  KL::Data & worldData,
  KL::SlicedArray<KL::Vec3> & positions,
  KL::SlicedArray<KL::Vec3> & normals,
  KL::VariableArray<KL::Integer> & indices,
  KL::Xfo & bodyTransform,
  KL::Integer & bodyClusters,
  KL::Integer & bodyConstraints,
  KL::Scalar & bodyMass,
  KL::Scalar & bodyStiffness,
  KL::Scalar & bodyFriction,
  KL::Scalar & bodyConservation,
  KL::Scalar & bodyPressure,
  KL::Scalar & bodyRecover
)
{
  if(bodyData == NULL && worldData != NULL) {
    fabricBulletWorld * world = (fabricBulletWorld *)worldData;
    
    // convert the transform.
    btTransform transform;
    transform.setOrigin(btVector3(bodyTransform.tr.x,bodyTransform.tr.y,bodyTransform.tr.z));
    transform.setRotation(btQuaternion(bodyTransform.ori.v.x,bodyTransform.ori.v.y,bodyTransform.ori.v.z,bodyTransform.ori.w));
    btVector3 scaling(bodyTransform.sc.x,bodyTransform.sc.y,bodyTransform.sc.z);
    btVector3 axis = transform.getRotation().getAxis();
    KL::Scalar angle = transform.getRotation().getAngle();
    
    fabricBulletSoftBody * body = new fabricBulletSoftBody();
    bodyData = body;

    body->mInitialPositions.resize(positions.size());
    body->mInitialNormals.resize(normals.size());
    for(size_t i=0;i<body->mInitialPositions.size();i++) {
      body->mInitialPositions[i] = transform * btVector3(positions[i].x,positions[i].y,positions[i].z);
      body->mInitialNormals[i] = btVector3(normals[i].x,normals[i].y,normals[i].z).rotate(axis,angle);
    }
    
    body->mBody = new btSoftBody(&world->mSoftBodyWorldInfo,body->mInitialPositions.size(),&body->mInitialPositions[0],0);
    body->mWorld = NULL;
    
    for(KL::Size i=0;i<indices.size()-2;i+=3)
    {
      body->mBody->appendFace(indices[i],indices[i+1],indices[i+2]);
      body->mBody->appendLink(indices[i],indices[i+1]);
      body->mBody->appendLink(indices[i+1],indices[i+2]);
      body->mBody->appendLink(indices[i+2],indices[i]);
    }

    KL::Scalar massPart = bodyMass / KL::Scalar(body->mBody->m_nodes.size());
    for(KL::Size i=0;i<body->mBody->m_nodes.size();i++)
    {
      body->mBody->setMass(i,massPart);
      body->mBody->m_nodes[i].m_n = body->mInitialNormals[i];
    }
    
    body->mBody->getCollisionShape()->setMargin(0.0f);
    if(bodyClusters >= 0)
    {
      body->mBody->generateClusters(bodyClusters);
      body->mBody->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS + btSoftBody::fCollision::CL_SELF;
    }
    else
      body->mBody->m_cfg.collisions += btSoftBody::fCollision::VF_SS;

    body->mBody->m_cfg.aeromodel = btSoftBody::eAeroModel::V_TwoSided;

    if(bodyConstraints > 0)
      body->mBody->generateBendingConstraints(bodyConstraints,body->mBody->m_materials[0]);

    body->mBody->setPose(true,true);
    
    body->mBody->m_materials[0]->m_kLST = bodyStiffness;
    body->mBody->m_materials[0]->m_kAST = bodyStiffness;
    body->mBody->m_materials[0]->m_kVST = bodyStiffness;
    
    body->mBody->m_cfg.kDP = 0.01f;
    body->mBody->m_cfg.kDF = bodyFriction;
    body->mBody->m_cfg.kDG = 0.1f;
    body->mBody->m_cfg.kVC = bodyConservation;
    body->mBody->m_cfg.kPR = bodyPressure;
    body->mBody->m_cfg.kMT = bodyRecover;
    body->mBody->m_cfg.kCHR = 0.25f;
    body->mBody->m_cfg.kKHR = 0.25f;
    body->mBody->m_cfg.kSHR = 0.25f;
    body->mBody->m_cfg.kAHR = 0.25f;
    
    if(body->mBody->m_clusters.size() > 0)
    {
      for(int j=0;j<body->mBody->m_clusters.size();j++)
      {
        body->mBody->m_clusters[j]->m_ldamping = body->mBody->m_cfg.kDP;
        body->mBody->m_clusters[j]->m_adamping = body->mBody->m_cfg.kDP;
      }
    }
    
    body->mBody->m_cfg.piterations = 4;
    body->mBody->m_cfg.citerations = 4;
    
    body->kLST = body->mBody->m_materials[0]->m_kLST;
    body->kDP = body->mBody->m_cfg.kDP;
    body->kDF = body->mBody->m_cfg.kDF;
    body->kDG = body->mBody->m_cfg.kDG;
    body->kVC = body->mBody->m_cfg.kVC;
    body->kPR = body->mBody->m_cfg.kPR;
    body->kMT = body->mBody->m_cfg.kMT;
    body->kCHR = body->mBody->m_cfg.kCHR;
    body->piterations = body->mBody->m_cfg.piterations;
    
    body->mBody->setUserPointer(body);
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_SoftBody_Delete(
  KL::Data & bodyData
)
{
  if(bodyData != NULL) {
    fabricBulletSoftBody * body = (fabricBulletSoftBody * )bodyData;
    if(body->mWorld != NULL)
      FabricBULLET_World_RemoveSoftBody(body->mWorld,bodyData);
    delete(body->mBody);
    delete(body);
    bodyData = NULL;
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_SoftBody_GetPosition(
  KL::Size index,
  KL::Data & bodyData,
  KL::Vec3 & position,
  KL::Vec3 & normal
)
{
  if(bodyData != NULL) {
    fabricBulletSoftBody * body = (fabricBulletSoftBody * )bodyData;
    position.x = body->mBody->m_nodes[index].m_x.getX();
    position.y = body->mBody->m_nodes[index].m_x.getY();
    position.z = body->mBody->m_nodes[index].m_x.getZ();
    normal.x = body->mBody->m_nodes[index].m_n.getX();
    normal.y = body->mBody->m_nodes[index].m_n.getY();
    normal.z = body->mBody->m_nodes[index].m_n.getZ();
  }  
}

FABRIC_EXT_EXPORT void FabricBULLET_Constraint_Create(
  KL::Data & constraintData,
  KL::Data & bodyDataA,
  KL::Data & bodyDataB,
  KL::Integer & constraintType,
  KL::Xfo & pivotA,
  KL::Xfo & pivotB,
  KL::VariableArray<KL::Scalar> & constraintParameters
)
{
  if(constraintData == NULL) {
    
    // check the bodies
    fabricBulletRigidBody * bodyA = (fabricBulletRigidBody *)bodyDataA;
    if(!bodyA)
    {
      throwException( "{FabricBULLET} ERROR: bodyA is NULL when creating constraint." );
      return;
    }
    fabricBulletRigidBody * bodyB = (fabricBulletRigidBody *)bodyDataB;
    if(!bodyB)
    {
      throwException( "{FabricBULLET} ERROR: bodyB is NULL when creating constraint." );
      return;
    }

    // convert the transforms    
    btTransform pivotTransformA;
    pivotTransformA.setOrigin(btVector3(pivotA.tr.x,pivotA.tr.y,pivotA.tr.z));
    pivotTransformA.setRotation(btQuaternion(pivotA.ori.v.x,pivotA.ori.v.y,pivotA.ori.v.z,pivotA.ori.w));
    btTransform pivotTransformB;
    pivotTransformB.setOrigin(btVector3(pivotB.tr.x,pivotB.tr.y,pivotB.tr.z));
    pivotTransformB.setRotation(btQuaternion(pivotB.ori.v.x,pivotB.ori.v.y,pivotB.ori.v.z,pivotB.ori.w));

    // validate the shape type first
    btTypedConstraint * constraint = NULL;
    if(constraintType == POINT2POINT_CONSTRAINT_TYPE) {
      
      if(constraintParameters.size() != 0) {
        throwException( "{FabricBULLET} ERROR: For the point2point constraint you need to specify zero parameters." );
        return;
      }
      
      constraint = new btPoint2PointConstraint(
         *bodyA->mBody,*bodyB->mBody,pivotTransformA.getOrigin(),pivotTransformB.getOrigin());
    } else if(constraintType == HINGE_CONSTRAINT_TYPE) {
      
      if(constraintParameters.size() != 0) {
        throwException( "{FabricBULLET} ERROR: For the hinge constraint you need to specify zero parameters." );
        return;
      }

      btVector3 axisA = pivotTransformA.getBasis() * btVector3(1,0,0);
      btVector3 axisB = pivotTransformB.getBasis() * btVector3(1,0,0);

      constraint = new btHingeConstraint(
         *bodyA->mBody,*bodyB->mBody,pivotTransformA.getOrigin(),pivotTransformB.getOrigin(),axisA,axisB,true);
    } else if(constraintType == SLIDER_CONSTRAINT_TYPE) {
      
      if(constraintParameters.size() != 0) {
        throwException( "{FabricBULLET} ERROR: For the slider constraint you need to specify zero parameters." );
        return;
      }

      constraint = new btSliderConstraint(
         *bodyA->mBody,*bodyB->mBody,pivotTransformA,pivotTransformB,true);
    }
    
    // if we don't have a shape, we can't do this
    if(constraint == NULL) {
      printf("   { FabricBULLET } ERROR: Constraint type %d is not supported.\n",int(constraintType));
      throwException( "{FabricBULLET} ERROR: Constraint type is not supported." );
      return;
    }

    fabricBulletConstraint * fabricConstraint = new fabricBulletConstraint();
    constraintData = fabricConstraint;
    
    fabricConstraint->mConstraint = constraint;
    fabricConstraint->mWorld = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricBULLET_Constraint_Delete(
  KL::Data & constraintData
)
{
  if(constraintData != NULL) {
    fabricBulletConstraint * constraint = (fabricBulletConstraint*)constraintData;
    if(constraint->mWorld != NULL)
      FabricBULLET_World_RemoveConstraint(constraint->mWorld,constraintData);
    delete( constraint->mConstraint );
    delete( constraint );
    constraintData = NULL;
  }
}
