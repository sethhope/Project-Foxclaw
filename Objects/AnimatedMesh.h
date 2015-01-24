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
			std::string getOType();
			
			int load(std::string filename);

			void setFrameLoop(u32 begin, u32 end);
			void setSpeed(f32 speed);
			u32 getCurrentFrame();
			std::string getFilename();
			
			scene::IAnimatedMesh* getMesh();
			scene::IAnimatedMeshSceneNode* getNode();

		private:
			LOGGER* log;
			u32 flbegin, flend;
			std::string filename;
			scene::IAnimatedMesh* mesh;
			scene::IAnimatedMeshSceneNode* anim;
			scene::ISceneManager* manager;
	};
}
#endif
