#include "Particle.h"

using namespace FCE;

PARTICLE::PARTICLE(IrrlichtDevice* device, LOGGER* log)
{
	ps = device->getSceneManager()->addParticleSystemSceneNode(true);
	this->device = device;
	this->log = log;
	size = core::aabbox3d<f32>(-1, 0, -1, 1, 1, 1);
	dir = core::vector3df(0, 0.1, 0);
	rateMin = 100;
	rateMax = 200;
	minAge = 500;
	maxAge = 1000;
	sizeMin = core::dimension2df(10, 10);
	sizeMax = core::dimension2df(20, 20);
	color1 = video::SColor(0,255,255,255);
	color2 = video::SColor(0,255,255,255);
	name = "LIGHT";
	log->debugData("Instantiated particle system");
}

PARTICLE::~PARTICLE()
{
	
}

void PARTICLE::onInit()
{
	log->logData("Initiating particle system", id);
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1,color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	ps->setPosition(this->globalPosition);
	ps->setScale(globalScale);
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	log->debugData("Created particle system");
}

void PARTICLE::onUpdate()
{
	ps->setPosition(globalPosition);
	ps->setRotation(globalRotation);
	ps->setScale(globalScale);
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
	log->debugData("Setting color of particle system", id);
	this->color1 = color1;
	this->color2 = color2;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData("Color set");
}
void PARTICLE::setPosition(core::vector3df position)
{
	log->debugData("Setting position of particle system", id);
	this->localPosition = position;
	log->debugData("Position set");
}
void PARTICLE::setScale(core::vector3df scale)
{
	log->debugData("Setting scale for particle system", id);
	this->localScale = scale;
	ps->setScale(scale);
	log->debugData("Scale set");
}
void PARTICLE::setDirection(core::vector3df dir)
{
	log->debugData("Setting direction of", id);
	this->dir = dir;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData("Direction set");
}
void PARTICLE::addAffector(std::string type)
{
	log->debugData("Adding affector to", id);
	log->debugData("Affector type", type);
	if(type == "fade")
	{
		scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
		ps->addAffector(paf);
		paf->drop();
	}
	else if(type == "gravity")
	{
		scene::IParticleAffector* paf = ps->createGravityAffector();
		ps->addAffector(paf);
		paf->drop();
	}
	else if(type == "rise")
	{
		scene::IParticleAffector* paf = ps->createGravityAffector(core::vector3df(0, 0.03, 0));
		ps->addAffector(paf);
		paf->drop();
	}else
	{
		log->debugData("Invalid affector type");
		return;
	}
	log->debugData("Affector added");
}
void PARTICLE::setAge(float minAge, float maxAge)
{
	log->debugData("Setting age of particle system", id);
	this->minAge = minAge;
	this->maxAge = maxAge;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData("Age set");
}
void PARTICLE::setRate(float rateMin, float rateMax)
{
	log->debugData("Setting rate of particle system", id);
	this->rateMin = rateMin;
	this->rateMax = rateMax;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData("Set rate");
}
void PARTICLE::setSize(core::dimension2df sizeMin, core::dimension2df sizeMax)
{
	log->debugData("Setting size of particle system", id);
	this->sizeMin = sizeMin;
	this->sizeMax = sizeMax;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData("Size set");
}
void PARTICLE::setID(int id)
{
	log->debugData("Setting ID of particle system to", id);
	this->id = id;
	log->debugData("ID set");
}
int PARTICLE::getID()
{
	return id;
}
void PARTICLE::setEmitterSize(core::aabbox3d<f32> size)
{
	log->debugData("Setting emitter size of particle system", id);
	this->size = size;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData("Emitter size set");
}
