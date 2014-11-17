#include "Collider.h"
using namespace FCE;

COLLIDER::COLLIDER(NODE* parent, LOGGER* log)
{
	this->log = log;
	if(type >= MAX_TYPE)
	{
		this->type = type;
	}
	else
	{
		log->logData("Invalid type", type);
	}
	parent->attach((NODE)this);
}

COLLIDER::~COLLIDER()
{
	
}

void COLLIDER::onInit()
{
	//TODO: add init things
	//create rigid body
	//attach body to the node
}

void COLLIDER::onUpdate()
{
}

void COLLIDER::onRender()
{
	
}

void COLLIDER::setType(int type)
{
	if(type >= MAX_TYPE)
	{
		this->type = type;
	}
	else
	{
		log->logData("Invalid type", type);
	}
}

void COLLIDER::setDamping(float lDamping, float aDamping)
{
	body->setDamping(lDamping, aDamping);
}

void COLLIDER::setFriction(float friction)
{
	body->setFriction(friction);
}

void COLLIDER::setMass(float mass)
{
	body->setMassProps(mass, core::vector3df(0, 0, 0));
}

void COLLIDER::setVelocity(core::vector3df velocity)
{
	body->setAngularVelocity(velocity);
}
