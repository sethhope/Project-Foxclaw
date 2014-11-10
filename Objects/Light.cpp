#include "Light.h"
using namespace FCE;
LIGHT::LIGHT(scene::ISceneManager* manager, LOGGER* log)
{
	this->manager = manager;
	this->log = log;
	localPosition = core::vector3df(0, 0, 0);
	localRotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
	type = video::ELT_POINT;
	name = "LIGHT";
}

LIGHT::~LIGHT()
{
	
}

void LIGHT::onUpdate()
{
	thisNode->setPosition(globalPosition);
	thisNode->setScale(globalScale);
	thisNode->setRotation(globalRotation);
}

void LIGHT::onRender()
{
	
}

void LIGHT::setColor(video::SColorf color)
{
	this->color = color;
}

scene::ISceneNode* LIGHT::getNode()
{
	return thisNode;
}
void LIGHT::setDropoff(float dropoff)
{
	this->dropoff = dropoff;
}
void LIGHT::onInit()
{
	log->debugData("Creating light");
	scene::ILightSceneNode* tmp;
	tmp = manager->addLightSceneNode(NULL, globalPosition, color, dropoff);
	tmp->getLightData().Type = type;
	thisNode = tmp;
	thisNode->setScale(globalScale);
	if(!thisNode)
	{
		log->logData("Light creation failed");
		return;
	}
	log->debugData("Light created");
}
void LIGHT::setType(video::E_LIGHT_TYPE type)
{
	this->type = type;
}
void LIGHT::setID(int id)
{
	this->id = id;
}
int LIGHT::getID()
{
	return id;
}
