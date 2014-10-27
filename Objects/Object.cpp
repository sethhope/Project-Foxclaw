#include "Object.h"

using namespace FCE;

OBJECT::OBJECT()
{
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
	if(getParent())
	{
		
		globalPosition = ((OBJECT*)getParent())->globalPosition + localPosition;
		globalRotation = ((OBJECT*)getParent())->globalRotation + localRotation;
		globalScale = ((OBJECT*)getParent())->globalScale * localScale;
	}
	else
	{
		globalPosition = localPosition;
		globalRotation = localRotation;
		globalScale = localScale;
	}
	onUpdate();
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
	localPosition = pos;
}

void OBJECT::setRotation(core::vector3df rot)
{
	localRotation = rot;
}

void OBJECT::setScale(core::vector3df scale)
{
	localScale = scale;
}

void OBJECT::setName(std::string name)
{
	this->name = name;
}

std::string OBJECT::getName()
{
	return name;
}
