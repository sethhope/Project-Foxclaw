#include "Collider.h"
using namespace FCE;

COLLIDER::COLLIDER(scene::ISceneNode* parent, scene::ISceneManager* manager, irrBulletWorld* world, int type, float mass, LOGGER* log)
{
	log->debugData(EXTRA, "Constructing collider");
	if(type <= MAX_TYPE)
	{
		this->type = type;
	}
	else
	{
		log->logData("Invalid collider type", type);
	}
	this->world = world;
	this->node = parent;
	this->mass = mass;
	this->log = log;
}

COLLIDER::~COLLIDER()
{
	
}

void COLLIDER::init()
{
	log->debugData(EXTRA, "Beginning collider init");
	if(type == COL_CUBE)
	{
		log->debugData(MAJOR, "Creating Box collider");
		ICollisionShape* shape = new IBoxShape(node, mass);
		body = world->addRigidBody(shape);
	}
	if(type == COL_SPHERE)
	{
		log->debugData(MAJOR, "Creating Sphere Collider");
		ICollisionShape* shape = new ISphereShape(node, mass);
		body = world->addRigidBody(shape);
	}
	if(type == COL_MESH)
	{
		log->debugData(MAJOR, "Creating Mesh Collider");
		ICollisionShape* shape = new IGImpactMeshShape(node, colMesh, mass);
		body = world->addRigidBody(shape);
	}
	//body->setActivationState(EAS_DISABLE_DEACTIVATION);
	log->debugData(EXTRA, "Finished collider init");
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
	world->removeCollisionObject(body, false);
	core::vector3df inertia;
	body->getCollisionShape()->calculateLocalInertia(mass, inertia);
	body->setMassProps(mass, inertia);
	
	world->addRigidBody(body->getCollisionShape());
	body->setMassProps(mass, core::vector3df(0, 0, 0));
}

void COLLIDER::setVelocity(core::vector3df velocity)
{
	log->debugData(MAJOR, "Setting collider velocity");
	body->setAngularVelocity(velocity);
}

void COLLIDER::setMesh(scene::IMesh* colMesh)
{
	log->debugData(MAJOR, "Setting collider mesh");
	this->colMesh = colMesh;
}
