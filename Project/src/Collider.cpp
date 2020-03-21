#include "Collider.h"
#include <iostream>
#include "mge/core/AABB.h"
#include "mge/core/OBB.h"
#include "OCObject.h"

Collider::Collider(glm::vec3 pCenter)
{
	center = pCenter;
}

Collider::~Collider()
{
}

glm::vec3 Collider::Center()
{
	return center;
}

bool Collider::collides(AABB * one, AABB * other)
{
	bool collides = one->Max().x > other->Min().x
		&& one->Min().x < other->Max().x
		&& one->Max().y > other->Min().y
		&& one->Min().y < other->Max().y
		&& one->Max().z > other->Min().z
		&& one->Min().z < other->Max().z;

	if (collides == true)
	{
		one->isColliding = collides;
		other->isColliding = collides;
	}

	return collides;
}

bool Collider::collides(AABB * one, OBB * other)
{
	bool collision = true;

	//Check if the addition of the longest line from center to a corner of both objects is less than the glm::length of the distance between the 2 object centers
	//If yes then there is no collision
	if (glm::length(one->LocalMax()) + glm::length(other->LocalMax()) < glm::length(one->owner->getLocalPosition() - other->owner->getLocalPosition()))
	{
		one->isColliding = false;
		other->isColliding = false;
		return false;
	}

	//AXIS
	//Create a list of Axes from both of the Objects to project corner points upon
	std::vector<glm::vec3> axes = one->GetAxis();
	std::vector<glm::vec3> otherAxes = other->GetAxis();
	for (int i = 0; i < otherAxes.size(); i++)
	{
		axes.push_back(otherAxes[i]);
	}

	//CORNERS
	//Create a list of Corners that will be projected upon the Axes
	std::vector<glm::vec3> corners = one->GetCorners();
	std::vector<glm::vec3> otherCorners = other->GetCorners();

	for (int j = 0; j < axes.size(); j++)
	{
		//Get a list of projections of the Corners upon the Axes from both objects
		std::vector<float> projections1;
		std::vector<float> projections2;
		for (int i = 0; i < corners.size(); i++)
		{
			projections1.push_back(glm::dot(corners[i], axes[j]));
			projections2.push_back(glm::dot(otherCorners[i], axes[j]));
		}
		/*for (int i = 0; i < corners.size(); i++)
		{
			projections2.push_back(glm::dot(otherCorners[i], axes[j]));
		}*/

		//Get the Min and Max projections of both Objects
		float minProjection1 = INFINITY;
		float maxProjection1 = -INFINITY;
		for (int i = 0; i< projections1.size(); i++)
		{
			if (projections1[i] < minProjection1)
			{
				minProjection1 = projections1[i];
			}
			if (projections1[i] > maxProjection1)
			{
				maxProjection1 = projections1[i];
			}
		}
		float minProjection2 = INFINITY;
		float maxProjection2 = -INFINITY;
		for (int i = 0; i< projections2.size(); i++)
		{
			if (projections2[i] < minProjection2)
			{
				minProjection2 = projections2[i];
			}
			if (projections2[i] > maxProjection2)
			{
				maxProjection2 = projections2[i];
			}
		}
		//Check for intersection
		bool intersection = false;
		for (int i = 0; i < projections2.size(); i++)
		{
			if (projections2[i] >= minProjection1)
			{
				if (projections2[i] <= maxProjection1)
				{
					intersection = true;
				}
			}
		}
		//Check intersections for both objects, in case one object encases the other from both sides
		//Where the min and max points are both less and more than that of the other object
		for (int i = 0; i < corners.size(); i++)
		{
			if (projections1[i] >= minProjection2)
			{
				if (projections1[i] <= maxProjection2)
				{
					intersection = true;
				}
			}
		}

		//if theres is no intersection there is no collision
		if (intersection == false)
		{
			collision = false;
		}
	}

	if (collision == true)
	{
		one->isColliding = collision;
		other->isColliding = collision;
	}

	return collision;
}

bool Collider::collides(OBB * one, AABB * other)
{
	return Collider::collides(other, one);
}

bool Collider::collides(OBB * one, OBB * other)
{
	if (glm::length(one->LocalMax()) + glm::length(other->LocalMax()) < glm::length(one->owner->getLocalPosition() - other->owner->getLocalPosition()))
	{
		one->isColliding = false;
		other->isColliding = false;
		return false;
	}

	bool collision = true;

	//Get Axes
	std::vector<glm::vec3> axes = one->GetAxis();
	std::vector<glm::vec3> otherAxes = other->GetAxis();
	for (int i = 0; i < otherAxes.size(); i++)
	{
		axes.push_back(otherAxes[i]);
	}

	//Get Corners
	std::vector<glm::vec3> corners = one->GetCorners();
	std::vector<glm::vec3> otherCorners = other->GetCorners();

	for (int j = 0; j < axes.size(); j++)
	{
		//Get projections
		std::vector<float> projections1;
		std::vector<float> projections2;
		for (int i = 0; i < corners.size(); i++)
		{
			projections1.push_back(glm::dot(corners[i], axes[j]));
			projections2.push_back(glm::dot(otherCorners[i], axes[j]));
		}
		/*for (int i = 0; i < corners.size(); i++)
		{
			projections2.push_back(glm::dot(otherCorners[i], axes[j]));
		}*/
		//Get min and max projection
		float minProjection1 = INFINITY;
		float maxProjection1 = -INFINITY;
		for (int i = 0; i< projections1.size(); i++)
		{
			if (projections1[i] < minProjection1)
			{
				minProjection1 = projections1[i];
			}
			if (projections1[i] > maxProjection1)
			{
				maxProjection1 = projections1[i];
			}
		}
		float minProjection2 = INFINITY;
		float maxProjection2 = -INFINITY;
		for (int i = 0; i< projections2.size(); i++)
		{
			if (projections2[i] < minProjection2)
			{
				minProjection2 = projections2[i];
			}
			if (projections2[i] > maxProjection2)
			{
				maxProjection2 = projections2[i];
			}
		}

		//Check for intersection
		bool intersection = false;
		for (int i = 0; i < projections2.size(); i++)
		{
			if (projections2[i] >= minProjection1)
			{
				if (projections2[i] <= maxProjection1)
				{
					intersection = true;
				}
			}
		}
		for (int i = 0; i < corners.size(); i++)
		{
			if (projections1[i] >= minProjection2)
			{
				if (projections1[i] <= maxProjection2)
				{
					intersection = true;
				}
			}
		}

		//if theres is no intersection there is no collision
		if (intersection == false)
		{
			collision = false;
		}
	}

	if (collision == true)
	{
		one->isColliding = collision;
		other->isColliding = collision;
	}

	return collision;
}

bool Collider::collides(Collider * other)
{
	std::cout << "dead end" << std::endl;
	return false;
}

bool Collider::collides(AABB * other)
{
	std::cout << "dead end" << std::endl;
	return false;
}

bool Collider::collides(OBB * other)
{
	std::cout << "dead end" << std::endl;
	return false;
}

void Collider::renderWireFrame()
{
	std::cout << "dead end" << std::endl;
}

void Collider::SetOwner(OCObject * pOwner)
{
	owner = pOwner;
}
