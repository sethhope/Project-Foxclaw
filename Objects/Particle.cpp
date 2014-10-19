#include "Particle.h"

using namespace FCE;

PARTICLE::PARTICLE(IrrlichtDevice* device, LOGGER* log)
{
	ps = device->getSceneManager()->addParticleSystemSceneNode(true);
	this->device = device;
	this->log = log;
	log->debugData("Instantiated particle system");
}

PARTICLE::~PARTICLE()
{
	
}

void PARTICLE::onInit()
{
	log->logData("Initiating particle system", id);
	scene::IParticleEmitter* em = ps->createBoxEmitter(core::aabbox3d<f32>(-1, 0, -1, 1, 1, 1), dir, rateMin, rateMax, color1,color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	ps->setPosition(this->position);
	ps->setScale(scale);
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	log->debugData("Created particle system");
}

void PARTICLE::onUpdate()
{
	ps->setPosition(position);
}

void PARTICLE::onRender()
{
	
}

void PARTICLE::loadTexture(std::string filename)
{
	log->debugData("Setting particle texture for", id);
	log->debugData("Set to", filename);
	ps->setMaterialTexture(0, device->getVideoDriver()->getTexture(filename.c_str()));
	log->debugData("Texture set");
}
void PARTICLE::setColors(video::SColor color1, video::SColor color2)
{
	this->color1 = color1;
	this->color2 = color2;
}
void PARTICLE::setPosition(core::vector3df position)
{
	this->position = position;
}
void PARTICLE::setScale(core::vector3df scale)
{
	this->scale = scale;
}
void PARTICLE::setDirection(core::vector3df dir)
{
	this->dir = dir;
}
void PARTICLE::addAffector(std::string type)
{
	if(type == "fade")
	{
		scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
		ps->addAffector(paf);
		paf->drop();
	}
	if(type == "gravity")
	{
		scene::IParticleAffector* paf = ps->createGravityAffector();
		ps->addAffector(paf);
		paf->drop();
	}
	if(type == "rise")
	{
		scene::IParticleAffector* paf = ps->createGravityAffector(core::vector3df(0, 0.03, 0));
		ps->addAffector(paf);
		paf->drop();
	}
}
void PARTICLE::setAge(float minAge, float maxAge)
{
	this->minAge = minAge;
	this->maxAge = maxAge;
}
void PARTICLE::setRate(float rateMin, float rateMax)
{
	this->rateMin = rateMin;
	this->rateMax = rateMax;
}
void PARTICLE::setSize(core::dimension2df sizeMin, core::dimension2df sizeMax)
{
	this->sizeMin = sizeMin;
	this->sizeMax = sizeMax;
}
void PARTICLE::setID(int id)
{
	this->id = id;
}
int PARTICLE::getID()
{
	return id;
}
