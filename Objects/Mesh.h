#ifndef _FMESH_H_
#define _FMESH_H_
#include "Object.h"
#include "Misc/Logger.h"
#include "Irrlicht.h"
using namespace irr;
using namespace core;
namespace FCE
{
	class MESH : public OBJECT
	{
		public:
			MESH(scene::ISceneManager* manager, LOGGER* log);
			~MESH();
			void onInit();
			void onUpdate();
			void onRender();
			std::string getOType();
			
			u8 load(std::string filename);
			
			scene::IAnimatedMesh* getMesh();
			//scene::IAnimatedMeshSceneNode* getNode();
			
		private:
			u32 id;
			LOGGER* log;
			
			scene::IAnimatedMesh* mesh;
			scene::ISceneManager* manager;
	};
}
#endif
