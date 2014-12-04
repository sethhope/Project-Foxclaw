#include "Camera.h"
using namespace FCE;
CAMERA::CAMERA(scene::ISceneManager* manager, LOGGER* log)
{
	this->manager = manager;
	this->log = log;
	type = FCAM_FPS;
	initialized = false;
	objectTarget = false;
	offset = core::vector3df(0,0,0);
}
CAMERA::~CAMERA()
{
	
}
void CAMERA::onInit()
{
	log->debugData(MINOR, "Initializing Camera as type", type);
	if(type == FCAM_FPS)
	{
		log->debugData(MINOR, "Initialized Camera as type", type);
		camNode = manager->addCameraSceneNodeFPS();
	}
	if(type == FCAM_FREE)
	{
		log->debugData(MINOR, "Initialized Camera as type", type);
		camNode = manager->addCameraSceneNode();
	}
	if(type == FCAM_MAYA)
	{
		log->debugData(MINOR, "Initialized Camera as type", type);
		camNode = manager->addCameraSceneNodeMaya();
	}
	thisNode = camNode;
	initialized = true;
	log->debugData(MAJOR, "Camera initialized");
}

void CAMERA::onUpdate()
{
	uDa = false;
	if(objectTarget)
	{
		camNode->setTarget(target->getIrrNode()->getPosition()+offset);
	}
}

void CAMERA::onRender()
{
}

void CAMERA::setAspectRatio(float aspect)
{
	camNode->setAspectRatio(aspect);
}

void CAMERA::setType(int type)
{
	this->type = type;
}

void CAMERA::setTarget(OBJECT* target)
{
	this->target = target;
	camNode->bindTargetAndRotation(true);
	objectTarget = true;
}

void CAMERA::setTarget(core::vector3df pos)
{
	camNode->setTarget(pos);
}

void CAMERA::setOffset(core::vector3df offset)
{
	this->offset = offset;
}
void CAMERA::setUpVector(core::vector3df pos)
{
	camNode->setUpVector(pos);
}

void CAMERA::setNearClipping(float clip)
{
	camNode->setNearValue(clip);
}

void CAMERA::setFarClipping(float clip)
{
	camNode->setFarValue(clip);
}

void CAMERA::setFOV(float fov)
{
	camNode->setFOV(fov);
}

scene::ICameraSceneNode* CAMERA::getCamNode()
{
	return camNode;
}

float CAMERA::getFOV()
{
	return camNode->getFOV();
}

float CAMERA::getNearClipping()
{
	return camNode->getNearValue();
}

float CAMERA::getFarClipping()
{
	return camNode->getFarValue();
}

int CAMERA::getType()
{
	return type;
}

scene::ISceneManager* CAMERA::getManager()
{
	return manager;
}


