#pragma once

#include <mge/core/GameObject.hpp>
#include "OCObject.h"

class Pair
{
public:
	OCObject* one;
	OCObject* other;
};

class OcTree
{
public:
	OcTree(glm::vec3 pCenter, glm::vec3 pHalfSize, int pLayer, OcTree* pRoot, World* pWorld);
	~OcTree();

	OcTree* Add(OCObject* obj);
	bool Remove(OCObject* obj);
	std::vector<Pair*>* Collisions(std::vector<Pair*>* pairs);
	void Collides(std::vector<Pair*>* pairs, OCObject* one);
	
	static int count;
	int id;

	void Update();
	void Render(World* pWorld);
	void ResetColliders();
	void Clear();
private:
	int _layer;
	glm::vec3 _center;
	glm::vec3 _halfSize;
	OcTree* _root;
	OcTree* _children[8];
	std::vector<OCObject*> _objects;

	World* _world;

	void _render();
};

