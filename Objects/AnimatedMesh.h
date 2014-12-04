#ifndef _FANIMATEDMESH_H_
#define _FANIMATEDMESH_H_
#include "Object.h"
#include "Misc/Logger.h"
#include "Irrlicht.h"
using namespace irr;
using namespace core;
namespace FCE
{
	class ANIMATEDMESH : public OBJECT
	{
		public:
			ANIMATEDMESH(scene::ISceneManager* manager, LOGGER* log);
			~ANIMATEDMESH();
			void onInit();
			void onUpdate();
			void onRender();

			int load(std::string filename);

			void setFrameLoop(int begin, int end);
			void setSpeed(float speed);
			
			scene::IAnimatedMesh* getMesh();
			scene::IAnimatedMeshSceneNode* getNode();

		private:
			LOGGER* log;
			int flbegin, flend;
			scene::IAnimatedMesh* mesh;
			scene::IAnimatedMeshSceneNode* anim;
			scene::ISceneManager* manager;
	};
}
#endif
