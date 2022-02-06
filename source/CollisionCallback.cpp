#include "CollisionCallback.h"

void CollisionCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++)
	{
		const physx::PxTriggerPair pair = pairs[i];
		if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			std::cout << "Something entered trigger volume. Teleporting!\n";
			physx::PxActor* actor = pair.otherActor;
			physx::PxRigidDynamic* dynamic = actor->is<physx::PxRigidDynamic>();
			if (dynamic)
			{
				const physx::PxVec3 teleportPosition = physx::PxVec3(0.f, 15.f, 15.f);
				const physx::PxTransform transform = physx::PxTransform(teleportPosition);
				//dynamic->setGlobalPose(transform);
			}
		}
	}
}

void CollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	for (size_t i = 0; i < nbPairs; i++)
	{
		const physx::PxContactPair* curContactPair = &pairs[i];
		
		const unsigned int contactPointCount = curContactPair->contactCount;
		physx::PxContactPairPoint* contactPoints = new physx::PxContactPairPoint[contactPointCount];
		curContactPair->extractContacts(contactPoints, contactPointCount);

		for (size_t j = 0; j < contactPointCount; j++)
		{
			physx::PxVec3 pos = contactPoints[j].position;
			std::cout << "Contact occured at global pos: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		}

		delete[] contactPoints;
	}
}
