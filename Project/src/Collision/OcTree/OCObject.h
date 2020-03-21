#pragma once

#include <mge/core/GameObject.hpp>
#include "mge/core/AABB.h"
#include "mge/core/OBB.h"

class OCObject : public GameObject
{
public:
	
	OCObject(glm::vec3 pPosition, Collider* pCollider, bool pPlayer);
	bool Collides(OCObject* other);
	void update(float pStep) override;
	Collider* collider;
	glm::vec3 velocity;
	~OCObject();
private:
	void getInput(float pStep);
	bool _player;
	void ApplyVelocity();
};

