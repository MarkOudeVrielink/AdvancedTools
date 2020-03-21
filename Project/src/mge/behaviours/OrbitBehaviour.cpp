#include <glm.hpp>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>

#include "OrbitBehaviour.h"
#include "mge/core/GameObject.hpp"

OrbitBehaviour::OrbitBehaviour(GameObject* pTarget, sf::RenderWindow* pWindow, float pDistance = 5.f, float pMoveSpeed = 50.f, float pTiltAngle = 45.f) :
	AbstractBehaviour(), 
	_target(pTarget),
	_window(pWindow),
	_distance(pDistance),
	_moveSpeed(pMoveSpeed),
	_tiltAngle(pTiltAngle)
{	
	_rotationMatrix = glm::mat4();
	_rotateYaw = 0.f;
	_rotatePitch = 0.f;
}

OrbitBehaviour::~OrbitBehaviour()
{
}

void OrbitBehaviour::update(float pStep) {
			
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
		_distance++;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		_distance--;
	}	

	glm::vec2 mousePosition = glm::vec2(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y);

	/// Window is (800,600)
	if (mousePosition.x > 800/4 * 3) {		
		_rotateYaw++;
	} else if(mousePosition.x < 800 / 4){		
		_rotateYaw--;
	} else {
		_rotateYaw = _rotateYaw;
	}
	
	if ((mousePosition.y > 600/4 * 3) && _rotatePitch < _tiltAngle) {
		_rotatePitch++;			
	} else if ((mousePosition.y < 600/ 4) && _rotatePitch > -_tiltAngle){
		_rotatePitch--;
	} else {
		_rotatePitch = _rotatePitch;
	}	

	/// Calculate the right rotation for the obiting object.
	_rotationMatrix = glm::rotate(glm::radians(_rotateYaw * _moveSpeed), glm::vec3(0, 1, 0)); // Rotate matrix over the Yaw.
	glm::mat3 yaw = glm::mat3(_rotationMatrix); // Save the yaw.

	_rotationMatrix = glm::rotate(glm::radians(_rotatePitch * _moveSpeed), glm::vec3(1,0,0)); // Rotate over pitch.
	glm::mat3 pitch = glm::mat3(_rotationMatrix);// Save the pitch.
	
	glm::vec3 offset = glm::vec3(0,0, _distance); // Set the object back from the target with given distance.
		
	_owner->setLocalPosition((yaw * pitch * offset) + _target->getLocalPosition()); // Set the new position of the owner with the right rotation and distance from the target.

	/// Get our lookAt vectors and create a cameraMatrix.
	glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - _target->getLocalPosition());
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, right));

	glm::mat4 cameraMatrix = glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1));
		
	_owner->setTransform(cameraMatrix); // Make object look at the target.	
}
