#include "OcTree.h"
#include "mge/core/World.hpp"
#include "mge/config.hpp"
#include "mge/core/Camera.hpp"
#include <GL/glew.h>

int OcTree::count = 0;

OcTree::OcTree(glm::vec3 pCenter, glm::vec3 pHalfSize, int pLayer, OcTree* pRoot, World* pWorld)
{
	// point to the root OctTree, or set it to null.
	pRoot != nullptr ? _root = pRoot : _root = this;
	id = OcTree::count++;
	_layer = pLayer;
	_center = pCenter;
	_halfSize = pHalfSize;
	
	_world = pWorld;

	if (_layer > 0) {
		glm::vec3 childHalfSize = _halfSize * 0.5f;

		_children[0] = new OcTree(_center + childHalfSize, childHalfSize, _layer - 1, _root, pWorld);
		_children[1] = new OcTree(_center + glm::vec3(+childHalfSize.x, +childHalfSize.y, -childHalfSize.z), childHalfSize, _layer - 1, _root, pWorld);
		_children[2] = new OcTree(_center + glm::vec3(+childHalfSize.x, -childHalfSize.y, +childHalfSize.z), childHalfSize, _layer - 1, _root, pWorld);
		_children[3] = new OcTree(_center + glm::vec3(-childHalfSize.x, +childHalfSize.y, +childHalfSize.z), childHalfSize, _layer - 1, _root, pWorld);
		_children[4] = new OcTree(_center + glm::vec3(+childHalfSize.x, -childHalfSize.y, -childHalfSize.z), childHalfSize, _layer - 1, _root, pWorld);
		_children[5] = new OcTree(_center + glm::vec3(-childHalfSize.x, -childHalfSize.y, +childHalfSize.z), childHalfSize, _layer - 1, _root, pWorld);
		_children[6] = new OcTree(_center + glm::vec3(-childHalfSize.x, +childHalfSize.y, -childHalfSize.z), childHalfSize, _layer - 1, _root, pWorld);
		_children[7] = new OcTree(_center - childHalfSize, childHalfSize, _layer - 1, _root, pWorld);
	}
}

OcTree::~OcTree()
{
}

OcTree * OcTree::Add(OCObject * obj)
{
	if (_layer > 0)
	{
		//When you add a new AABB or OBB object you don't know which one it is,
		//so you dynamic_cast to figure out which one it is
		AABB * aabbCollider = dynamic_cast<AABB*>(obj->collider);
		OBB * obbCollider = dynamic_cast<OBB*>(obj->collider);

		glm::vec3 max;
		glm::vec3 min;

		//Either the AABB or the OBB collider will be nullptr, so you know which one you are dealing with
		if (aabbCollider != nullptr)
		{
			max = aabbCollider->Max();
			min = aabbCollider->Min();
		}
		else if (obbCollider != nullptr)
		{
			max = obbCollider->Max();
			min = obbCollider->Min();
		}

		//Check if the Object fits in a child of the OcTreeNode
		if (max.x < _center.x) {
			if (max.y < _center.y) {
				if (max.z < _center.z) {
					return _children[7]->Add(obj); // - - -
				}
				else if (min.z > _center.z) {
					return _children[5]->Add(obj); // - - +
				}
			}
			else if (min.y > _center.y) {
				if (max.z < _center.z) {
					return _children[6]->Add(obj); // - + -
				}
				else if (min.z > _center.z) {
					return _children[3]->Add(obj); // - + +
				}
			}
		}
		else if (min.x > _center.x) {
			if (max.y < _center.y) {
				if (max.z < _center.z) {
					return _children[4]->Add(obj); // + - -
				}
				else if (min.z > _center.z) {
					return _children[2]->Add(obj); // + - +
				}
			}
			else if (min.y > _center.y) {
				if (max.z < _center.z) {
					return _children[1]->Add(obj); // + + -
				}
				else if (min.z > _center.z) {
					return _children[0]->Add(obj); // + + +
				}
			}
		}
	}
	//Place the object in this OcTreeNode if it's the bottom layer or no child holds it
	if (obj != nullptr)
		_objects.push_back(obj);

	return this;
}

bool OcTree::Remove(OCObject * obj)
{
	if (_layer > 0)
	{
		AABB * aabbCollider = dynamic_cast<AABB*>(obj->collider);

		OBB * obbCollider = dynamic_cast<OBB*>(obj->collider);

		glm::vec3 max;
		glm::vec3 min;

		if (aabbCollider != nullptr)
		{
			max = aabbCollider->Max();
			min = aabbCollider->Min();
		}
		else if (obbCollider != nullptr)
		{
			max = obbCollider->Max();
			min = obbCollider->Min();
		}

		if (max.x < _center.x) {
			if (max.y < _center.y) {
				if (max.z < _center.z) {
					return _children[7]->Remove(obj); // - - -
				}
				else if (min.z > _center.z) {
					return _children[5]->Remove(obj); // - - +
				}
			}
			else if (min.y > _center.y) {
				if (max.z < _center.z) {
					return _children[6]->Remove(obj); // - + -
				}
				else if (min.z > _center.z) {
					return _children[3]->Remove(obj); // - + +
				}
			}
		}
		else if (min.x > _center.x) {
			if (max.y < _center.y) {
				if (max.z < _center.z) {
					return _children[4]->Remove(obj); // + - -
				}
				else if (min.z > _center.z) {
					return _children[2]->Remove(obj); // + - +
				}
			}
			else if (min.y > _center.y) {
				if (max.z < _center.z) {
					return _children[1]->Remove(obj); // + + -
				}
				else if (min.z > _center.z) {
					return _children[0]->Remove(obj); // + + +
				}
			}
		}
	}

	//get index of our obj
	int index;
	for (int i = 0; i < _objects.size(); i++)
	{
		if (obj == _objects[i])
		{
			index = i;
		}
	}
	//erase obj
	_objects.erase(_objects.begin() + index);

	return true;
}

std::vector<Pair*>* OcTree::Collisions(std::vector<Pair*>* pairs)
{
	if (pairs == nullptr)
	{
		pairs = new std::vector<Pair*>;
	}

	OCObject* one = nullptr, * other = nullptr;

	for (int a = 0; a < _objects.size(); a++)
	{
		//OCObject * one = _objects[a];
		one = _objects[a];
		for (int b = a + 1; b < _objects.size(); b++)
		{
			//OCObject * other = _objects[b];
			other = _objects[b];

			//check if two objects collide, if so - save them as pair
			if (one->Collides(other))
			{
				Pair * newPair = new Pair();
				newPair->one = one;
				newPair->other = other;
				pairs->push_back(newPair);
			}
		}
		if (_layer > 0)
		{
			//check collisions in all child quads with objects from this quad
			for (OcTree * child : _children)
			{
				child->Collides(pairs, one);
			}
		}

	}
	if (_layer > 0)
	{
		//check collisions in all child quads
		for (OcTree * child : _children)
		{
			child->Collisions(pairs);
		}
	}
	return pairs;
}

void OcTree::Collides(std::vector<Pair*>* pairs, OCObject * one)
{
	for (int b = 0; b < _objects.size(); b++)
	{
		OCObject * other = _objects[b];
		if (one->Collides(other))
		{
			Pair * newPair = new Pair();
			newPair->one = one;
			newPair->other = other;
			pairs->push_back(newPair);
		}
	}
	if (_layer > 0) //very unoptimized
	{
		for (OcTree * child : _children)
		{
			child->Collides(pairs, one);
		}
	}
}

void OcTree::Update()
{
	//reinsert objects
	std::vector<OCObject*> objectsCopy;
	for (OCObject * obj : _objects)
	{
		objectsCopy.push_back(obj);
	}
	_objects.clear();
	for (OCObject * obj : objectsCopy)
	{
		_root->Add(obj);
	}

	if (id == 0)
	{
		ResetColliders();
		Collisions(nullptr)->size();
	}

	if (_layer > 0)
		for (OcTree * node : _children)
		{
			node->Update();
		}
}


void OcTree::Clear()
{
	if (_layer > 0) {
		for (OcTree* node : _children) {
			node->Clear();
		}
	}

	for (OCObject* object : _objects) {
		_world->remove(object);
		delete object;
	}

	_objects.clear();
}

void OcTree::ResetColliders()
{
	for (int i = 0; i < _objects.size(); i++)
	{
		_objects[i]->collider->isColliding = false;
	}

	if (_layer > 0)
	{
		for (int i = 0; i < 8; i++) {
			_children[i]->ResetColliders();
		}
	}
}

#pragma region

void OcTree::Render(World* pWorld)
{
	glUseProgram(0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::inverse(pWorld->getMainCamera()->getWorldTransform())));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(pWorld->getMainCamera()->getProjection()));
	_render();

}

void OcTree::_render()
{
	for (int i = 0; i < _objects.size(); i++)
	{
		_objects[i]->collider->renderWireFrame();
	}

	GLfloat const red[3] = { 1,0,0 };
	GLfloat const green[3] = { 0,1,0 };
	if (_objects.size() == 0) {
		glColor3fv(red);
		glLineWidth(1.0f);
	}
	else
	{
		glColor3fv(green);
		glLineWidth(1.5f);

	}

	glBegin(GL_LINES);
	glVertex3f(_center.x - _halfSize.x, _center.y + _halfSize.y, _center.z + _halfSize.z);
	glVertex3f(_center.x + _halfSize.x, _center.y + _halfSize.y, _center.z + _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x - _halfSize.x, _center.y - _halfSize.y, _center.z + _halfSize.z);
	glVertex3f(_center.x + _halfSize.x, _center.y - _halfSize.y, _center.z + _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x - _halfSize.x, _center.y + _halfSize.y, _center.z + _halfSize.z);
	glVertex3f(_center.x - _halfSize.x, _center.y - _halfSize.y, _center.z + _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x + _halfSize.x, _center.y + _halfSize.y, _center.z + _halfSize.z);
	glVertex3f(_center.x + _halfSize.x, _center.y - _halfSize.y, _center.z + _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x - _halfSize.x, _center.y + _halfSize.y, _center.z - _halfSize.z);
	glVertex3f(_center.x + _halfSize.x, _center.y + _halfSize.y, _center.z - _halfSize.z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(_center.x - _halfSize.x, _center.y - _halfSize.y, _center.z - _halfSize.z);
	glVertex3f(_center.x + _halfSize.x, _center.y - _halfSize.y, _center.z - _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x + _halfSize.x, _center.y + _halfSize.y, _center.z - _halfSize.z);
	glVertex3f(_center.x + _halfSize.x, _center.y - _halfSize.y, _center.z - _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x - _halfSize.x, _center.y + _halfSize.y, _center.z - _halfSize.z);
	glVertex3f(_center.x - _halfSize.x, _center.y - _halfSize.y, _center.z - _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x - _halfSize.x, _center.y + _halfSize.y, _center.z + _halfSize.z);
	glVertex3f(_center.x - _halfSize.x, _center.y + _halfSize.y, _center.z - _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x - _halfSize.x, _center.y - _halfSize.y, _center.z + _halfSize.z);
	glVertex3f(_center.x - _halfSize.x, _center.y - _halfSize.y, _center.z - _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x + _halfSize.x, _center.y + _halfSize.y, _center.z + _halfSize.z);
	glVertex3f(_center.x + _halfSize.x, _center.y + _halfSize.y, _center.z - _halfSize.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(_center.x + _halfSize.x, _center.y - _halfSize.y, _center.z + _halfSize.z);
	glVertex3f(_center.x + _halfSize.x, _center.y - _halfSize.y, _center.z - _halfSize.z);
	glEnd();

	if (_layer > 0)
	{
		for (int i = 0; i < 8; i++) {
			_children[i]->_render();
		}
	}
}

#pragma endregion Render OcTree lines
