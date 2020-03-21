#ifndef MGEDEMO_H
#define MGEDEMO_H

#include <mge/core/AbstractGame.hpp>
#include "OcTree.h"
#include "OCObject.h"

class DebugHud;

class MGEDemo: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		MGEDemo();
		virtual ~MGEDemo();

        virtual void initialize();

		struct testSet {
			int objects;
			int percentageStatic;
			int percentageAABB;
		};
			   
	protected:
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		DebugHud* _hud;                   //hud display

        void _updateHud();
		virtual void _update(float pStep);

        MGEDemo(const MGEDemo&);
        MGEDemo& operator=(const MGEDemo&);

		OcTree* _ocTree;
		void StartTest(int objCount, int staticChance, int percentAABB);
		int frameIndex;
		float fpsSumm = 0;
		std::vector<OCObject*> population;
				
	
	
};

#endif // MGEDEMO_H
