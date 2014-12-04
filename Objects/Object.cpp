#include "Object.h"

using namespace FCE;

OBJECT::OBJECT()
{
	name = "UNNAMED_OBJECT";
	position = core::vector3df(0, 0, 0);
	rotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
	thisNode=NULL;
	uDa = true;
	hasCollider = false;
	initialized = false;
}

OBJECT::~OBJECT()
{
	detach();
}

void OBJECT::init()
{
	onInit();
	initialized = true;
}

void OBJECT::update()
{
	onUpdate();
	if((this->getIrrNode() && !getParent() && !hasCollider) && uDa)
	{
		getIrrNode()->setPosition(position);
		getIrrNode()->setRotation(rotation);
		getIrrNode()->setScale(scale);
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
	if(initialized)
	{
		getIrrNode()->setPosition(pos);
	}
	uDa = true;
}

void OBJECT::setRotation(core::vector3df rot)
{
	rotation = rot;
	if(initialized)
	{
		getIrrNode()->setRotation(rot);
	}
	uDa = true;
}

void OBJECT::setScale(core::vector3df scale)
{
	this->scale = scale;
	 if(initialized)
	 {
	 	getIrrNode()->setScale(scale);
	 }
	uDa = true;
}

void OBJECT::setName(std::string name)
{
	this->name = name;
}

void OBJECT::addCollider(LOGGER* log, int type, scene::ISceneManager* manager, irrBulletWorld* world, float mass)
{
	collider = new COLLIDER(getIrrNode(), manager, world, type, mass, log);
	collider->init();
	hasCollider = true;
}

void OBJECT::addCollider(LOGGER* log, int type, scene::ISceneManager* manager, irrBulletWorld* world, float mass, scene::IMesh* colMesh)
{
	collider = new COLLIDER(getIrrNode(), manager, world, type, mass, log);
	collider->setMesh(colMesh);
	collider->init();
	hasCollider = true;
}

COLLIDER* OBJECT::getCollider()
{
	return collider;
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

void OBJECT::setID(int id)
{
	this->id = id;
}

int OBJECT::getID()
{
	return id;
}
