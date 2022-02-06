#pragma once

#include <iostream>
#include <vector>

#include "PxPhysicsAPI.h"

// Current collision callback class is just for learning purposes and cannot process more than two actors.
class CollisionCallback : public physx::PxSimulationEventCallback
{
public:
	//Trigger callback function that called when rigid dynamics enters trigger zone.
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
	//Collision callback function that called when 2 rigid dynamics collide with each other.
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);

	//PxSimulationEventCallback overrides. Basically do nothing on events.
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {};
	void onWake(physx::PxActor** actors, physx::PxU32 count) {};
	void onSleep(physx::PxActor** actors, physx::PxU32 count) {};
	void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {};
};

