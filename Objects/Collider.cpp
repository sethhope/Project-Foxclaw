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
	
	world->removeCollisionObject(body, true);
	body->remove();
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
	if(type == COL_MESH_GIMPACT)
	{
		log->debugData(MAJOR, "Creating Mesh GIMPACT Collider");
		ICollisionShape* shape = new IGImpactMeshShape(node, colMesh, mass);
		body = world->addRigidBody(shape);
	}
	if(type == COL_MESH_CONVEXHULL)
	{
		log->debugData(MAJOR, "Creating mesh CONVEXHULL collider");
		ICollisionShape* shape = new IConvexHullShape(node, colMesh, mass);
		body = world->addRigidBody(shape);
	}
	if(type == COL_MESH_TRIMESH)
	{
		log->debugData(MAJOR, "Creating mesh TRIMESH collider");
		ICollisionShape* shape = new IBvhTriangleMeshShape(node, colMesh, mass);
		body = world->addRigidBody(shape);
	}
	if(type == COL_LIQUID)
	{
		log->debugData(MAJOR, "Creating liquid collider");
		lbody = world->addLiquidBody(node->getPosition(), node->getBoundingBox());
	}
	if(type == COL_CAPSULE)
	{
		log->debugData(MAJOR, "Creating capsule collider");
		ICollisionShape* shape = new ICapsuleShape(node, mass);
		body = world->addRigidBody(shape);
	}
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
	if(type != COL_LIQUID && type != COL_SOFTBODY)
	{
		body->setDamping(lDamping, aDamping);
	}
}

void COLLIDER::setFriction(float friction)
{
	log->debugData(MAJOR, "Setting collider friction to", friction);
	if(type != COL_LIQUID && type != COL_SOFTBODY)
	{
		body->setFriction(friction);
	}
}

void COLLIDER::setMass(float mass)
{
	log->debugData(MAJOR, "Setting collider mass to", mass);
	if(type != COL_LIQUID && type != COL_SOFTBODY)
	{
		world->removeCollisionObject(body, false);
		core::vector3df inertia;
		body->getCollisionShape()->calculateLocalInertia(mass, inertia);
		body->setMassProps(mass, inertia);
	
		world->addRigidBody(body->getCollisionShape());
		body->setMassProps(mass, core::vector3df(0, 0, 0));
	}
}

void COLLIDER::setVelocity(core::vector3df velocity)
{
	log->debugData(MAJOR, "Setting collider velocity");
	if(type != COL_LIQUID && type != COL_SOFTBODY)
	{
		body->setAngularVelocity(velocity);
	}
}

void COLLIDER::setMesh(scene::IMesh* colMesh)
{
	log->debugData(MAJOR, "Setting collider mesh");
	this->colMesh = colMesh;
}

int COLLIDER::getType()
{
	return type;
}

float COLLIDER::getMass()
{
	return mass;
}
