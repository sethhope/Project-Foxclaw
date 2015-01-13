#include "Object.h"

using namespace FCE;

OBJECT::OBJECT()
{
	name = "UNNAMED_OBJECT";
	position = core::vector3df(0, 0, 0);
	rotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
	thisNode = NULL;
	uDa = true;
	hasCollider = false;
	initialized = false;
}

OBJECT::~OBJECT()
{
	detach();
	if(hasCollider)
	{
		delete collider;
	}
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
	core::vector3df lastPos = position;
	position = pos;
	if(initialized)
	{
		getIrrNode()->setPosition(pos);
	}
	if(hasCollider)
	{
		collider->body->getPointer()->translate(btVector3(pos.X-lastPos.X, pos.Y-lastPos.Y, pos.Z-lastPos.Z));
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
	if(hasCollider)
	{
		core::matrix4 trans;
		trans = collider->body->getWorldTransform();
		trans.setRotationDegrees(rot);
		collider->body->setWorldTransform(trans);
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
	 if(hasCollider)
	 {
	 	collider->world->removeCollisionObject(collider->body, false);
		collider->init();
	 }
	uDa = true;
}

void OBJECT::setName(std::string name)
{
	this->name = name;
}

void OBJECT::addCollider(LOGGER* log, u16 type, scene::ISceneManager* manager, irrBulletWorld* world, f32 mass)
{
	collider = new COLLIDER(getIrrNode(), manager, world, type, mass, log);
	collider->init();
	hasCollider = true;
}

void OBJECT::addCollider(LOGGER* log, u16 type, scene::ISceneManager* manager, irrBulletWorld* world, f32 mass, scene::IMesh* colMesh)
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

void OBJECT::setID(u32 id)
{
	this->id = id;
}

u32 OBJECT::getID()
{
	return id;
}
void OBJECT::setMetaData(std::string key, f32 data)
{
	metadata[key] = data;
}
f32 OBJECT::getMetaData(std::string key)
{
	return metadata[key];
}
