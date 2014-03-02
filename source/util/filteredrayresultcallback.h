#pragma once

class FilteredRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
public:
	explicit FilteredRayResultCallback(const btCollisionObject* ignoredObject, const btVector3 &rayFromWorld, const btVector3& rayToWorld) :
		ClosestRayResultCallback(rayFromWorld, rayToWorld)
	{
		m_ignoredObject = ignoredObject;
	}
	virtual ~FilteredRayResultCallback() {}


	virtual bool needsCollision(btBroadphaseProxy* proxy0) const
	{
		if (proxy0 == nullptr)
			return false;

		void* collisionPartner = proxy0->m_clientObject;
		
		if (collisionPartner) {
			btCollisionObject *collobj = static_cast<btCollisionObject*>(collisionPartner);
			return m_ignoredObject != collobj;
		}
		else {
			// this should prevent some crashes when an object was removed from the world and a ray intersection test is performed
			return false;
		}
	}
private:
	const btCollisionObject* m_ignoredObject;
};