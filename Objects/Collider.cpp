#include "Collider.h"
using namespace FCE;

COLLIDER::COLLIDER(NODE* parent, scene::ISceneManager* manager, irrBulletWorld* world, LOGGER* log, float mass, int type)
{
	this->log = log;
	log->debugData(EXTRA, "Constructing collider");
	if(type <= MAX_TYPE)
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
	thisNode = manager->;
	parent->getFirstParent()->attach(this);
	//attach(parent->getFirstParent());
	log->logData("Position Y", ((OBJECT*)parent)->getPosition().Y);
	setPosition(((OBJECT*)parent)->getPosition());
	setRotation(((OBJECT*)parent)->getRotation());
	setScale(((OBJECT*)parent)->getScale());
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
		log->debugData(EXTRA, "Creating Box collider");
		ICollisionShape* shape = new IBoxShape(getChild()->getIrrNode(), mass);
		log->logData("PosY Other", position.Y);
		body = world->addRigidBody(shape);
	}
	if(type == COL_SPHERE)
	{
		log->debugData(EXTRA, "Creating Sphere Collider");
		ICollisionShape* shape = new ISphereShape(getChild()->getIrrNode(), mass);
		body = world->addRigidBody(shape);
	}
	if(type == COL_MESH)
	{
		log->debugData(EXTRA, "Creating Mesh Collider");
		ICollisionShape* shape = new IGImpactMeshShape(getChild()->getIrrNode(), colMesh, mass);
		body = world->addRigidBody(shape);
	}
	//body->setActivationState(EAS_DISABLE_DEACTIVATION);
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
