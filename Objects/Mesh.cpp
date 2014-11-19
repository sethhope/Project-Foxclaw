#include "Mesh.h"
using namespace FCE;

MESH::MESH(scene::ISceneManager* manager,  LOGGER* log)
{
	this->manager = manager;
	this->log = log;
	id = -1;
	name = "MESH";
}

MESH::~MESH()
{
	
}

int MESH::getID()
{
	return id;
}
/*scene::IAnimatedMeshSceneNode* MESH::getNode()
{
	return node;
}*/

int MESH::load(std::string filename)
{
	log->logData("Loading mesh", filename);
	scene::IAnimatedMesh* mesh = manager->getMesh(filename.c_str());
	scene::IAnimatedMeshSceneNode* node = manager->addAnimatedMeshSceneNode(mesh);
	if(node)
	{
		log->debugData(MAJOR, "Loaded mesh");
		thisNode = node;
		return 0;
	}
	log->logData("Failed to load mesh", filename);
	return 1;
}
void MESH::setID(int id)
{
	this->id = id;
}

void MESH::onInit()
{
}

void MESH::onUpdate()
{
}
void MESH::onRender()
{
}

scene::IAnimatedMesh* MESH::getMesh()
{
	return mesh;
}
