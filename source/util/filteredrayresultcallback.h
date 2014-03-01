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
		btCollisionObject *collobj = static_cast<btCollisionObject*>(proxy0->m_clientObject);
		return m_ignoredObject != collobj;
	}
private:
	const btCollisionObject* m_ignoredObject;
};