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
	boneAttached = false;
}

OBJECT::~OBJECT()
{
	detach();
	if(hasCollider)
	{
		collider->~COLLIDER();
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
	if((this->getIrrNode() && !getParent() && !hasCollider && !boneAttached) && uDa)
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
	if(hasCollider || boneAttached)
	{
		position=getIrrNode()->getPosition();
		rotation=getIrrNode()->getRotation();
	}
}
void OBJECT::render()
{
	onRender();
	/*if(hasShader)
    {
        for(std::vector<u32>::iterator it = shader->materials.begin(); it < shader->materials.end(); it++)
        {
            thisNode->setMaterialType((video::E_MATERIAL_TYPE)(*it));
            thisNode->render();
        }
    }*/
	if(!children.empty())
	{
		for(std::vector<NODE*>::iterator it = children.begin(); it < children.end(); it++)
		{
			((OBJECT*)(*it))->render();
		}
	}
}
void OBJECT::deconstruct()
{
    onDeconstruct();
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
void OBJECT::useShader(IrrlichtDevice* device, LOGGER* log, std::string shaderName, u32 materialIndex)
{
    if(device->getVideoDriver()->getDriverType() == video::EDT_OPENGL)
    {
        hasShader = true;
        this->shaderName = shaderName;
        shader = new ShaderHandler(device, log);
        for(int i = 0; i < 8; i++)
        {
            std::stringstream key;
            key<<"mTexture";
            key<<i;
            shader->addConstant(FCE_FRAG, key.str(), i);
        }
        for(std::map<std::string, f32>::iterator it = vsshaderConstants.begin(); it != vsshaderConstants.end(); it++)
        {
            f32 pt = it->second;
            shader->addConstant(FCE_VERT, it->first.c_str(), pt);
        }
        for(std::map<std::string, f32>::iterator it = fsshaderConstants.begin(); it != fsshaderConstants.end(); it++)
        {
            f32 pt = it->second;
            shader->addConstant(FCE_FRAG, it->first.c_str(), pt);
        }
        shader->addShader(shaderName, thisNode->getMaterial(0).MaterialType, materialIndex);

        thisNode->getMaterial(materialIndex).setFlag(video::EMF_LIGHTING, false);
        thisNode->getMaterial(materialIndex).MaterialType = (video::E_MATERIAL_TYPE)shader->materials[0];
        //thisNode->setMaterialFlag(video::EMF_LIGHTING, false);
        //thisNode->setMaterialType((video::E_MATERIAL_TYPE)shader->materials[0]);
    }else
    {
        log->logData("Running DirectX");
    }
}
void OBJECT::setShaderConstant(u32 shader, std::string key, f32 data)
{
    if(shader == FCE_FRAG)
    {
        fsshaderConstants[key] = data;
    }else if(shader == FCE_VERT)
    {
        vsshaderConstants[key] = data;
    }
}
