#include "Light.h"
using namespace FCE;
LIGHT::LIGHT(scene::ISceneManager* manager, scene::ISceneNode* node, LOGGER* log)
{
	this->manager = manager;
	this->log = log;
	this->node = node;
	localPosition = core::vector3df(0, 0, 0);
	localRotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
	type = video::ELT_POINT;
}

LIGHT::~LIGHT()
{
	
}

void LIGHT::onUpdate()
{
	node->setPosition(globalPosition);
	node->setScale(globalScale);
	node->setRotation(globalRotation);
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
	return node;
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
	node = tmp;
	node->setScale(globalScale);
	if(!node)
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
