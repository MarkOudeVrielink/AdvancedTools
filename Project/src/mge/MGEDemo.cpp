#include <glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"

#include "mge/core/Camera.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/OrbitBehaviour.h"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

#include <SFML/Window/Keyboard.hpp>

bool teststarted = false;

ofstream outfile("..\\_vs2015\\release_static_exe\\Data.txt", ios::out);

MGEDemo::testSet _testData[] = {
	//objects, %static, %AABB
	//AABB vs. AABB 0% static
	{ 10, 0, 100},
	{ 25, 0, 100},
	{ 100, 0, 100},
	{ 250, 0, 100},
	{ 1000, 0, 100},
	{ 2500, 0, 100},
	{ 10000, 0, 100},
	//AABB vs. AABB 25% static
	{ 10, 25, 100 },
	{ 25, 25, 100 },
	{ 100, 25, 100 },
	{ 250, 25, 100 },
	{ 1000, 25, 100 },
	{ 2500, 25, 100 },
	{ 10000, 25, 100 },
	//AABB vs. AABB 50% static
	{ 10, 50, 100 },
	{ 25, 50, 100 },
	{ 100, 50, 100 },
	{ 250, 50, 100 },
	{ 1000, 50, 100 },
	{ 2500, 50, 100 },
	{ 10000, 50, 100 },
	//AABB vs. AABB 75% static
	{ 10, 75, 100 },
	{ 25, 75, 100 },
	{ 100, 75, 100 },
	{ 250, 75, 100 },
	{ 1000, 75, 100 },
	{ 2500, 75, 100 },
	{ 10000, 75, 100 },
	//AABB vs. AABB 100% static
	{ 10, 100, 100 },
	{ 25, 100, 100 },
	{ 100, 100, 100 },
	{ 250, 100, 100 },
	{ 1000, 100, 100 },
	{ 2500, 100, 100 },
	{ 10000, 100, 100 },
	//AABB vs.OOBB 0% static
	{ 10, 0, 50 },
	{ 25, 0, 50 },
	{ 100, 0, 50 },
	{ 250, 0, 50 },
	{ 1000, 0, 50 },
	{ 2500, 0, 50 },
	{ 10000, 0, 50 },
	//AABB vs.OOBB 25% static
	{ 10, 25, 50 },
	{ 25, 25, 50 },
	{ 100, 25, 50 },
	{ 250, 25, 50 },
	{ 1000, 25, 50 },
	{ 2500, 25, 50 },
	{ 10000, 25, 50 },
	//AABB vs.OOBB 50% static
	{ 10, 50, 50 },
	{ 25, 50, 50 },
	{ 100, 50, 50 },
	{ 250, 50, 50 },
	{ 1000, 50, 50 },
	{ 2500, 50, 50 },
	{ 10000, 50, 50 },
	//AABB vs.OOBB 75% static
	{ 10, 75, 50 },
	{ 25, 75, 50 },
	{ 100, 75, 50 },
	{ 250, 75, 50 },
	{ 1000, 75, 50 },
	{ 2500, 75, 50 },
	{ 10000, 75, 50 },
	//AABB vs.OOBB 100% static
	{ 10, 100, 50 },
	{ 25, 100, 50 },
	{ 100, 100, 50 },
	{ 250, 100, 50 },
	{ 1000, 100, 50 },
	{ 2500, 100, 50 },
	{ 10000, 100, 50 },
	//OOBB vs.OOBB 0% static
	{ 10, 0, 0 },
	{ 25, 0, 0 },
	{ 100, 0, 0 },
	{ 250, 0, 0 },
	{ 1000, 0, 0 },
	{ 2500, 0, 0 },
	{ 10000, 0, 0 },
	//OOBB vs.OOBB 25% static
	{ 10, 25, 0 },
	{ 25, 25, 0 },
	{ 100, 25, 0 },
	{ 250, 25, 0 },
	{ 1000, 25, 0 },
	{ 2500, 25, 0 },
	{ 10000, 25, 0 },
	//OOBB vs.OOBB 50% static
	{ 10, 50, 0 },
	{ 25, 50, 0 },
	{ 100, 50, 0 },
	{ 250, 50, 0 },
	{ 1000, 50, 0 },
	{ 2500, 50, 0 },
	{ 10000, 50, 0 },
	//OOBB vs.OOBB 75% static
	{ 10, 75, 0 },
	{ 25, 75, 0 },
	{ 100, 75, 0 },
	{ 250, 75, 0 },
	{ 1000, 75, 0 },
	{ 2500, 75, 0 },
	{ 10000, 75, 0 },
	//OOBB vs.OOBB 100% static
	{ 10, 100, 0 },
	{ 25, 100, 0 },
	{ 100, 100, 0 },
	{ 250, 100, 0 },
	{ 1000, 100, 0 },
	{ 2500, 100, 0 },
	{ 10000, 100, 0 },
};
int currenttestSet = 45;
int octreeDepth = 1;

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame (),_hud(0)
{
}

void MGEDemo::initialize() {
	//Read octree depth from file.
	ifstream depthFile("..\\_vs2015\\release_static_exe\\OctreeDepth.txt");
	depthFile >> octreeDepth;

	ifstream testFile("..\\_vs2015\\release_static_exe\\TestSet.txt");
	testFile >> currenttestSet;
	
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part
	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;

	// To make sure the first write to the file won't be overriden.
	outfile.open("..\\_vs2015\\release_static_exe\\Data.txt", std::ios_base::app);
	outfile << "Test data:" << endl;
	outfile.close();
}

//build the game _world
void MGEDemo::_initializeScene()
{
	
    _renderer->setClearColor(0,0,0);
	
    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,6,7));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);
	
	_ocTree = new OcTree(glm::vec3(0,0,0), glm::vec3(20,20,20), octreeDepth, nullptr, _world);
	
	GameObject* cameraTarget = new GameObject("obj", glm::vec3(0,0,0));
    camera->setBehaviour(new OrbitBehaviour (cameraTarget, _window, 30, 3, 65)); 		
}

void MGEDemo::_render() {
	_ocTree->Update();

    AbstractGame::_render();
    _updateHud();

	_ocTree->Render(_world);	
}

int loops = 0;
int runCount = 0;
string debugInfo = "Press R to start the tests.";

int fpsCount;
int collisioncount = 0;

void MGEDemo::_updateHud() {
	_hud->setDebugInfo(debugInfo);
    _hud->draw();
}

void MGEDemo::_update(float pStep)
{
	_world->update(pStep);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !teststarted) {
		teststarted = true;
		StartTest(_testData[currenttestSet].objects, _testData[currenttestSet].percentageStatic, _testData[currenttestSet].percentageAABB);	
		cout << "Start tests " << octreeDepth << " depth" << endl;
	}

	debugInfo = "fps: " + std::to_string(_fps) + "\n collsions: " + std::to_string(_ocTree->Collisions(nullptr)->size());
	/*
	if (teststarted) {
		if (currenttestSet < 104) {
			if (runCount <= 10) {
				if (loops >= 100) {
					fpsCount += fpsSumm / loops;
					_ocTree->Clear();
					
					loops = 0;
					fpsSumm = 0;
					runCount++;
				}
				else {					
					fpsSumm += _fps;					
					loops++;
				}
			}
			else {				
				string filetext = "Avg. fps:" + std::to_string((int)fpsCount / 10) + "\n" +					
					"Objects: " + std::to_string(_testData[currenttestSet].objects) + "\n" +
					"AABB vs. OOBB: " + std::to_string(_testData[currenttestSet].percentageAABB) + " - " + std::to_string(100 - _testData[currenttestSet].percentageAABB) + "\n" +
					"Static: " + std::to_string(_testData[currenttestSet].percentageStatic) + " %\n" +
					"Octree depth: " + std::to_string(octreeDepth) + "\n";				

				outfile.open("..\\_vs2015\\release_static_exe\\Data.txt", std::ios_base::app);
				outfile << filetext << endl;
				outfile.close();
				
				cout << currenttestSet << " Done" << endl;

				currenttestSet++;
				fpsCount = 0;
				loops = 0;
				runCount = 0;				
				StartTest(_testData[currenttestSet].objects, _testData[currenttestSet].percentageStatic, _testData[currenttestSet].percentageAABB);
			}			
		}
		else {
			teststarted = false;
			cout << "Tests done" << endl;
		}
		
	}*/
}

MGEDemo::~MGEDemo()
{
	//dtor
}


void MGEDemo::StartTest(int objCount, int staticChance, int percentAABB)
{
	//std::cout << "Starting test - Object count: " << objCount << " Static rate:  " << staticChance << " Amount of AABB-objects: " << percentAABB << std::endl;
	frameIndex = 0;
	fpsSumm = 0;
	glm::vec3 startVelocity = glm::vec3(1, 1, 1);

	///////////////////////////////////
	int count = objCount;
	float statChance = (float)staticChance;
	///////////////////////////////////

	int numbreOfStaticItems = (int)(((float)(count)) / 100 * statChance);
	int staticCount = 0;
	bool isStatic = false;

	int AABBObjects = count / 100 * percentAABB;
	int AABBCount = 0;

	//pos
	glm::vec3 ocPos = glm::vec3(-20, -20, -20);
	float objectsPerLine = cbrtf((float)count);
	float posStep = 40 / objectsPerLine;

	for (int i = 0; i < count; i++)
	{
		//get position
		ocPos.x += posStep;
		if (ocPos.x > 20)
		{
			ocPos.x = -20;
			ocPos.y += posStep;
			if (ocPos.y > 20)
			{
				ocPos.y = -20;
				ocPos.z += posStep;
				if (ocPos.z > 20)
				{
					ocPos.z = -20;
				}
			}
		}
		//define if object is static
		isStatic = false;
		if (staticCount < numbreOfStaticItems)
		{
			isStatic = true;
			staticCount++;
		}

		//spawn object
		OCObject * ocObj;
		if (AABBCount < AABBObjects) //create AABB objects until we've reached the percentage.
		{
			ocObj = new OCObject(ocPos + glm::vec3(1, 1, 1), new AABB(glm::vec3(0, 0, 0), glm::vec3(0.3f, 0.5f, 0.7f)), isStatic);
			_world->add(ocObj);
			_ocTree->Add(ocObj);
			
			AABBCount++;
		}
		else //The rest will be OOBB.
		{
			ocObj = new OCObject(ocPos + glm::vec3(1, 1, 1), new OBB(glm::vec3(0, 0, 0), glm::vec3(0.3f, 0.5f, 0.7f)), isStatic);
			_world->add(ocObj);
			_ocTree->Add(ocObj);

			//rotate
			ocObj->rotate((float)(i * 5), glm::vec3(1, 0, 0));
			ocObj->rotate((float)(i * 2), glm::vec3(0, 1, 0));
			ocObj->rotate((float)(i * 3), glm::vec3(0, 0, 1));
		}

		//random velocity
		float minSpeed = 0.1f;
		float maxSpeed = 0.5f;
		float currentSpeed = (maxSpeed - minSpeed) / count * i + minSpeed;
		ocObj->velocity = glm::normalize(glm::rotate(startVelocity, i * 5.0f, glm::vec3(0, 1, 0))) * currentSpeed / 10;

		population.push_back(ocObj);
	}
}