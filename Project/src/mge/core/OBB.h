#pragma once

#include <mge/core/GameObject.hpp>
#include "Collision/Collider.h"

class OBB : public Collider
{
public:
	OBB(glm::vec3 pCenter, glm::vec3 pHalfSize);
	~OBB();

	glm::vec3 HalfSize();
	glm::vec3 Size();
	glm::vec3 Max();
	glm::vec3 Min();
	glm::vec3 LocalMin();
	glm::vec3 LocalMax();

	bool collides(Collider* other) override;
	bool collides(AABB* other) override;
	bool collides(OBB* other) override;

	void renderWireFrame() override;

	std::vector<glm::vec3> GetCorners();
	std::vector<glm::vec3> GetAxis();

protected:
	glm::vec3 halfSize;
};

