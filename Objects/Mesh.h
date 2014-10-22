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
			MESH(scene::ISceneManager* manager, scene::ISceneNode* node, LOGGER* log);
			~MESH();
			void onInit();
			void onUpdate();
			void onRender();
			
			int load(std::string filename);
			
			scene::IAnimatedMesh* getMesh();
			scene::IAnimatedMeshSceneNode* getNode();
			
			std::string getName();
			int getID();
			void setID(int id);
			void setName(std::string name);
			
		private:
			int id;
			std::string name;
			LOGGER* log;
			
			scene::IAnimatedMesh* mesh;
			scene::IAnimatedMeshSceneNode* node;
			scene::ISceneManager* manager;
	};
}
#endif
