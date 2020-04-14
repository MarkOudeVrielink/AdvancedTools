#pragma once
#include <glm/detail/type_vec3.hpp>

class OCObject;
class AABB;
class OBB;

class Collider {
public:
	Collider(glm::vec3 pCenter);
	virtual ~Collider();

	glm::vec3 Center();

	static bool collides(AABB* one, AABB* other);
	static bool collides(AABB* one, OBB* other);
	static bool collides(OBB* one, AABB* other);
	static bool collides(OBB* one, OBB* other);

	virtual bool collides(Collider* other);
	virtual bool collides(AABB* other);
	virtual bool collides(OBB* other);

	virtual void renderWireFrame();

	void SetOwner(OCObject* pOwner);

	bool isColliding = false;
	
protected:
	glm::vec3 center;
	OCObject* owner;
};