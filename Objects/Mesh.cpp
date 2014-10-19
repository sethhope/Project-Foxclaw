#include "Mesh.h"
using namespace FCE;

MESH::MESH(scene::ISceneManager* manager, scene::ISceneNode* node, LOGGER* log)
{
	this->manager = manager;
	this->log = log;
	//this->node = node;
	id = -1;
	name = "MESH";
}

MESH::~MESH()
{
	
}

void MESH::setRotation(vector3df rotation)
{
	this->rotation = rotation;
}

void MESH::setRotation(float x, float y, float z)
{
	rotation = vector3df(x,y,z);
}

void MESH::setCoords(vector3df coords)
{
	this->position = coords;
}

void MESH::setCoords(float x, float y, float z)
{
	position = vector3df(x, y, z);
}

vector3df MESH::getRotation()
{
	return rotation;
}

vector3df MESH::getCoords()
{
	return position;
}

int MESH::getID()
{
	return id;
}

std::string MESH::getName()
{
	return name;
}
scene::IAnimatedMeshSceneNode* MESH::getNode()
{
	return node;
}

int MESH::load(std::string filename)
{
	log->logData("Loading mesh", filename);
	scene::IAnimatedMesh* mesh = manager->getMesh(filename.c_str());
	node = manager->addAnimatedMeshSceneNode(mesh);
	if(node)
	{
		log->debugData("Loaded mesh");
		return 0;
	}
	log->logData("Failed to load mesh", filename);
	return 1;
}
void MESH::setID(int id)
{
	this->id = id;
}

void MESH::setName(std::string name)
{
	this->name = name;
}
void MESH::onInit()
{
}

void MESH::onUpdate()
{
	node->setPosition(position);
	node->setRotation(rotation);
}
void MESH::onRender()
{
}
void MESH::setScale(core::vector3df scale)
{
	this->scale = scale;
}
