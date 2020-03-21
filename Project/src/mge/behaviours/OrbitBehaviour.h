#ifndef ORBITBEHAVIOUR_H
#define ORBITBEHAVIOUR_H

#include <SFML/Graphics.hpp>

#include "mge/behaviours/AbstractBehaviour.hpp"

class OrbitBehaviour : public AbstractBehaviour
{
	public:
		OrbitBehaviour(GameObject* pTarget, sf::RenderWindow* pWindow,  float pDistance, float pMoveSpeed, float pTiltAgnle);
		virtual ~OrbitBehaviour();

		virtual void update(float pStep);

	private:
		GameObject* _target = nullptr;
		sf::RenderWindow* _window = nullptr;
		float _distance;
		float _moveSpeed;
		float _tiltAngle;	

		glm::mat4 _rotationMatrix;

		float _rotateYaw;
		float _rotatePitch;
	
};

#endif //ORBITBEHAVIOUR_H
