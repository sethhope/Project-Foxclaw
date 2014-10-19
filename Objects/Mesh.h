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
			
			vector3df getCoords();
			void setCoords(float x, float y, float z);
			void setCoords(vector3df coords);
			
			vector3df getRotation();
			void setRotation(float x, float y, float z);
			void setRotation(vector3df rotation);
			
			void setScale(vector3df scale);
			
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
