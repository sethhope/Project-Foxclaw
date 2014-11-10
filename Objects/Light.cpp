#include "Light.h"
using namespace FCE;
LIGHT::LIGHT(scene::ISceneManager* manager, LOGGER* log)
{
	this->manager = manager;
	this->log = log;
	type = video::ELT_POINT;
	name = "LIGHT";
}

LIGHT::~LIGHT()
{
	
}

void LIGHT::onUpdate()
{
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
	log->debugData(MAJOR, "Creating light");
	scene::ILightSceneNode* tmp;
	tmp = manager->addLightSceneNode(NULL, position, color, dropoff);
	tmp->getLightData().Type = type;
	thisNode = tmp;
	thisNode->setScale(scale);
	if(!thisNode)
	{
		log->logData("Light creation failed");
		return;
	}
	log->debugData(MAJOR, "Light created");
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
