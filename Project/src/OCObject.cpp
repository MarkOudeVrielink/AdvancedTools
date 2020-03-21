#include "OCObject.h"
#include <SFML/Window/Keyboard.hpp>


OCObject::OCObject(glm::vec3 pPosition, Collider* pCollider, bool pPlayer) : GameObject("OcObject", pPosition)
{
	_player = pPlayer;
	collider = pCollider;
	collider->SetOwner(this);
}

bool OCObject::Collides(OCObject * other)
{
	return collider->collides(other->collider);
}

void OCObject::update(float pStep)
{
	GameObject::update(pStep);
	
	if (_player)
	{
		getInput(pStep);
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		ApplyVelocity();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		rotate(0.01f, glm::vec3(1, 0, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
	{
		rotate(0.01f, glm::vec3(0, 1, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		rotate(0.01f, glm::vec3(0, 0, 1));
	}
}


OCObject::~OCObject()
{
}

void OCObject::getInput(float pStep)
{
	float speed = 4;

	glm::vec3 position = getWorldPosition();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		position = position + glm::vec3(0, 0, -pStep*speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		position = position + glm::vec3(0, 0, pStep*speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		position = position + glm::vec3(pStep*speed, 0, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		position = position + glm::vec3(-pStep*speed, 0, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Home))
	{
		position = position + glm::vec3(0, pStep*speed, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::End))
	{
		position = position + glm::vec3(0, -pStep*speed, 0);
	}

	setLocalPosition(position);
}

void OCObject::ApplyVelocity()
{
	translate(velocity);

	glm::vec3 position = getLocalPosition();

	float size = 20;

	if (position.x > size)
	{
		setLocalPosition(glm::vec3(-size, position.y, position.z));
	}
	if (position.x < -size)
	{
		setLocalPosition(glm::vec3(size, position.y, position.z));
	}
	if (position.y > size)
	{
		setLocalPosition(glm::vec3(position.x, -size, position.z));
	}
	if (position.y < -size)
	{
		setLocalPosition(glm::vec3(position.x, size, position.z));
	}
	if (position.z > size)
	{
		setLocalPosition(glm::vec3(position.x, position.y, -size));
	}
	if (position.z < -size)
	{
		setLocalPosition(glm::vec3(position.x, position.y, size));
	}
}
