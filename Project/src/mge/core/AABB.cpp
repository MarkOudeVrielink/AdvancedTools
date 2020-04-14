#include "AABB.h"
#include "Collision/OcTree/OCObject.h"
#include "mge/materials/ColorMaterial.hpp"


AABB::AABB(glm::vec3 pCenter, glm::vec3 pHalfSize) : Collider(pCenter)
{
	halfSize = pHalfSize;
}


AABB::~AABB()
{
}

glm::vec3 AABB::HalfSize()
{
	return halfSize;
}

glm::vec3 AABB::Size()
{
	return 2.0f * halfSize;
}

glm::vec3 AABB::Max()
{
	return center + halfSize + owner->getLocalPosition();
}

glm::vec3 AABB::Min()
{
	return center - halfSize + owner->getLocalPosition();
}

glm::vec3 AABB::LocalMin()
{
	return center - halfSize;
}

glm::vec3 AABB::LocalMax()
{
	return center + halfSize;
}

bool AABB::collides(Collider * other)
{
	return other->collides(this);
}

bool AABB::collides(AABB * other)
{
	return Collider::collides(this, other);
}

bool AABB::collides(OBB * other)
{
	return Collider::collides(this, other);
}

//TODO: change colours.
void AABB::renderWireFrame()
{
	GLfloat const blue[3] = { 0.1f,0.1f,1.0f };
	GLfloat const pink[3] = { 1.0f,0.078f,0.576f };

	glLineWidth(1.0f);

	if (isColliding)
		glColor3fv(pink);
	else
		glColor3fv(blue);

	glBegin(GL_LINES);
	glVertex3f(Min().x, Max().y, Max().z);
	glVertex3f(Max().x, Max().y, Max().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Min().x, Min().y, Max().z);
	glVertex3f(Max().x, Min().y, Max().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Min().x, Max().y, Max().z);
	glVertex3f(Min().x, Min().y, Max().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Max().x, Max().y, Max().z);
	glVertex3f(Max().x, Min().y, Max().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Min().x, Max().y, Min().z);
	glVertex3f(Max().x, Max().y, Min().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Min().x, Min().y, Min().z);
	glVertex3f(Max().x, Min().y, Min().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Max().x, Max().y, Min().z);
	glVertex3f(Max().x, Min().y, Min().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Min().x, Max().y, Min().z);
	glVertex3f(Min().x, Min().y, Min().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Min().x, Max().y, Max().z);
	glVertex3f(Min().x, Max().y, Min().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Min().x, Min().y, Max().z);
	glVertex3f(Min().x, Min().y, Min().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Max().x, Max().y, Max().z);
	glVertex3f(Max().x, Max().y, Min().z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(Max().x, Min().y, Max().z);
	glVertex3f(Max().x, Min().y, Min().z);
	glEnd();
}

// return corners of the aabb object
// it's used to project the corners on the axis.
std::vector<glm::vec3> AABB::GetCorners()
{
	std::vector<glm::vec3> corners;

	glm::mat3 rotaionMatrix = owner->getTransform();
	glm::vec3 min = Min();
	glm::vec3 max = Max();

	corners.push_back(glm::vec3(min.x, min.y, min.z));
	corners.push_back(glm::vec3(max.x, min.y, min.z));

	corners.push_back(glm::vec3(max.x, min.y, max.z));
	corners.push_back(glm::vec3(min.x, min.y, max.z));

	corners.push_back(glm::vec3(min.x, max.y, min.z));
	corners.push_back(glm::vec3(max.x, max.y, min.z));

	corners.push_back(glm::vec3(max.x, max.y, max.z));
	corners.push_back(glm::vec3(min.x, max.y, max.z));

	return corners;
}

std::vector<glm::vec3> AABB::GetAxis()
{
	std::vector<glm::vec3> axes;

	axes.push_back(glm::vec3(1, 0, 0));
	axes.push_back(glm::vec3(0, 1, 0));
	axes.push_back(glm::vec3(0, 0, 1));

	return axes;
}
