#include "Object.h"

using namespace FCE;

OBJECT::OBJECT()
{
	name = "UNNAMED_OBJECT";
	position = core::vector3df(0, 0, 0);
	rotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
}

OBJECT::~OBJECT()
{
	detach();
}

void OBJECT::init()
{
	onInit();
}

void OBJECT::update()
{
	onUpdate();
	this->getIrrNode()->setPosition(position);
	this->getIrrNode()->setRotation(rotation);
	this->getIrrNode()->setScale(scale);
	if(getChild())
	{
		((OBJECT*)getChild())->update();
	}
}

void OBJECT::render()
{
	onRender();
	if(getChild())
	{
		((OBJECT*)getChild())->render();
	}
}

void OBJECT::setPosition(core::vector3df pos)
{
	position = pos;
}

void OBJECT::setRotation(core::vector3df rot)
{
	rotation = rot;
}

void OBJECT::setScale(core::vector3df scale)
{
	scale = scale;
}

void OBJECT::setName(std::string name)
{
	this->name = name;
}

std::string OBJECT::getName()
{
	return name;
}
