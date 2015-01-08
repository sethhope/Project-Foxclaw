#include "Light.h"
using namespace FCE;
LIGHT::LIGHT(scene::ISceneManager* manager, LOGGER* log)
{
	this->manager = manager;
	this->log = log;
	type = video::ELT_POINT;
	name = "LIGHT";
	position = core::vector3df(0, 0, 0);
	rotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
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

void LIGHT::setDropoff(f32 dropoff)
{
	this->dropoff = dropoff;
}
void LIGHT::onInit()
{
	log->debugData(MAJOR, "Creating light");
	scene::ILightSceneNode* tmp;
	tmp = manager->addLightSceneNode(NULL, position, color, dropoff);
	tmp->setLightType(type);
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
std::string LIGHT::getOType()
{
	return "LIGHT";
}
