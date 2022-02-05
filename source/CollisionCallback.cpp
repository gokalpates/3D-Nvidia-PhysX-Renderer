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
				dynamic->setGlobalPose(transform);
			}
		}
	}
}
