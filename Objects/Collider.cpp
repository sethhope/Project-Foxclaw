#include "Collider.h"
using namespace FCE;

COLLIDER::COLLIDER(NODE* parent, irrBulletWorld* world, LOGGER* log, int type)
{
	this->log = log;
	log->debugData(EXTRA, "Constructing collider");
	if(type >= MAX_TYPE)
	{
		this->type = type;
	}
	else
	{
		log->logData("Invalid type", type);
	}
	this->world = world;
	//TODO: FIX THIS THING THAT DOES A THING WHEN ATTACHING!
	log->debugData(EXTRA, "Attaching node");
 	parent->attach((NODE*)this);
	log->debugData(EXTRA, "Attached");
}

COLLIDER::~COLLIDER()
{
	
}

void COLLIDER::onInit()
{
	log->debugData(EXTRA, "Beginning collider init");
	if(type == COL_CUBE)
	{
		ICollisionShape* shape = new IBoxShape(thisNode, mass);
		body = world->addRigidBody(shape);
	}
	if(type == COL_SPHERE)
	{
		ICollisionShape* shape = new ISphereShape(thisNode, mass);
		body = world->addRigidBody(shape);
	}
	if(type == COL_MESH)
	{
		ICollisionShape* shape = new IConvexHullShape(thisNode, colMesh, mass);
		body = world->addRigidBody(shape);
	}
	log->debugData(EXTRA, "Finished collider init");
}

void COLLIDER::onUpdate()
{
	
}

void COLLIDER::onRender()
{
	
}

void COLLIDER::setType(int type)
{
	log->debugData(MAJOR, "Setting collider type to", type);
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
	log->debugData(MAJOR, "Setting collider lDamping to", lDamping);
	log->debugData(MAJOR, "Setting collider aDamping to", aDamping);
	body->setDamping(lDamping, aDamping);
}

void COLLIDER::setFriction(float friction)
{
	log->debugData(MAJOR, "Setting collider friction to", friction);
	body->setFriction(friction);
}

void COLLIDER::setMass(float mass)
{
	log->debugData(MAJOR, "Setting collider mass to", mass);
	body->setMassProps(mass, core::vector3df(0, 0, 0));
}

void COLLIDER::setVelocity(core::vector3df velocity)
{
	log->debugData(MAJOR, "Setting collider velocity");
	body->setAngularVelocity(velocity);
}
void COLLIDER::setID(int id)
{
	this->id = id;
}

int COLLIDER::getID()
{
	return id;
}

void COLLIDER::setMesh(scene::IMesh* colMesh)
{
	log->debugData(MAJOR, "Setting collider mesh");
	this->colMesh = colMesh;
}
