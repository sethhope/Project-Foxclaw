#include "Object.h"

using namespace FCE;

OBJECT::OBJECT()
{
	name = "UNNAMED_OBJECT";
	position = core::vector3df(0, 0, 0);
	rotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
	thisNode=NULL;
	uDa = false;
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
	if( this->getIrrNode() && !getParent())
	{
		getIrrNode()->setPosition(position);
		getIrrNode()->setRotation(rotation);
		getIrrNode()->setScale(scale);
	}else
	{
		/*if(uDa == true && this->getIrrNode())
		{
			getIrrNode()->setPosition(position);
			getIrrNode()->setRotation(rotation);
			getIrrNode()->setScale(scale);
			uDa = false;
		}*/
	}
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
	uDa = true;
}

void OBJECT::setRotation(core::vector3df rot)
{
	rotation = rot;
	uDa = true;
}

void OBJECT::setScale(core::vector3df scale)
{
	scale = scale;
	uDa = true;
}

void OBJECT::setName(std::string name)
{
	this->name = name;
}

std::string OBJECT::getName()
{
	return name;
}

core::vector3df OBJECT::getPosition()
{
	return position;
}

core::vector3df  OBJECT::getRotation()
{
	return rotation;
}

core::vector3df  OBJECT::getScale()
{
	return scale;
}
