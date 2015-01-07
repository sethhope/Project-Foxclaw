#include "AnimatedMesh.h"
using namespace FCE;

ANIMATEDMESH::ANIMATEDMESH(scene::ISceneManager* manager,  LOGGER* log)
{
	this->manager = manager;
	this->log = log;
	id = -1;
	name = "MESH";
	flbegin = 1;
	flend = 1;
	position = core::vector3df(0, 0, 0);
	rotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
}

ANIMATEDMESH::~ANIMATEDMESH()
{

}

int ANIMATEDMESH::load(std::string filename)
{
	log->logData("Loading animated mesh", filename);
	mesh = manager->getMesh(filename.c_str());
	anim = manager->addAnimatedMeshSceneNode(mesh);
	if(anim)
	{
		log->debugData(MAJOR, "Loaded Animated mesh");
		thisNode = anim;
		return 0;
	}
	log->logData("Failed to load mesh", filename);
	return 1;
}

void ANIMATEDMESH::onInit()
{
}

void ANIMATEDMESH::onUpdate()
{
}

void ANIMATEDMESH::onRender()
{
}

scene::IAnimatedMesh* ANIMATEDMESH::getMesh()
{
	return mesh;
}
scene::IAnimatedMeshSceneNode* ANIMATEDMESH::getNode()
{
	return anim;
}
void ANIMATEDMESH::setFrameLoop(u32 begin, u32 end)
{
	if(flbegin != begin || flend != end)
	{
		anim->setFrameLoop(begin, end);
		flbegin = begin;
		flend = end;
	}
}

void ANIMATEDMESH::setSpeed(f32 speed)
{
	anim->setAnimationSpeed(speed);
}

std::string ANIMATEDMESH::getOType()
{
	return "ANIMATEDMESH";
}
