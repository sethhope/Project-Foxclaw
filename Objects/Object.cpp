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
	hasShader = false;
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
	if(!children.empty())
	{
		for(std::vector<NODE*>::iterator it = children.begin(); it < children.end(); it++)
		{
			((OBJECT*)(*it))->update();
		}
	}
	if(hasCollider)
	{
		position=getIrrNode()->getPosition();
		rotation=getIrrNode()->getRotation();
	}
}

void OBJECT::render()
{
	onRender();
	if(!children.empty())
	{
		for(std::vector<NODE*>::iterator it = children.begin(); it < children.end(); it++)
		{
			((OBJECT*)(*it))->render();
		}
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
core::vector3df OBJECT::getUpVector()
{
	core::matrix4 m;
	m.setRotationDegrees(getIrrNode()->getRotation());
	core::vector3df upv = core::vector3df(0, 1, 0);
	m.transformVect(upv);
	return upv;
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
void OBJECT::useShader(IrrlichtDevice* device, std::string vsName, std::string fsName)
{
    this->vsName = vsName;
    this->fsName = fsName;
    hasShader = true;
    io::path vsFile = vsName.c_str();
    io::path psFile = fsName.c_str();

    ShaderCallback* scb = new ShaderCallback();
    scb->device = device;
    video::IGPUProgrammingServices* gpu = device->getVideoDriver()->getGPUProgrammingServices();
    const video::E_GPU_SHADING_LANGUAGE sLang = video::EGSL_DEFAULT;
    video::E_MATERIAL_TYPE currM = thisNode->getMaterial(0).MaterialType;
    s32 mat1 = gpu->addHighLevelShaderMaterialFromFiles(vsFile, "vertexMain", video::EVST_VS_1_1, psFile, "pixelMain", video::EPST_PS_1_1, scb, currM, 0, sLang);

    scb->drop();

    thisNode->setMaterialFlag(video::EMF_LIGHTING, false);
    thisNode->setMaterialType((video::E_MATERIAL_TYPE)mat1);
}
