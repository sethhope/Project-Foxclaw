#include "Particle.h"

using namespace FCE;

PARTICLE::PARTICLE(IrrlichtDevice* device, LOGGER* log)
{
	this->device = device;
	this->log = log;
	size = core::aabbox3d<f32>(-1, -1, -1, 1, 1, 1);
	dir = core::vector3df(0, 0.1, 0);
	rateMin = 100;
	rateMax = 200;
	minAge = 500;
	maxAge = 1000;
	sizeMin = core::dimension2df(10, 10);
	sizeMax = core::dimension2df(20, 20);
	color1 = video::SColor(0,255,255,255);
	color2 = video::SColor(0,255,255,255);
	name = "PARTICLE";
	position = core::vector3df(0, 0, 0);
	rotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
	log->debugData(MAJOR, "Instantiated particle system");
}

PARTICLE::~PARTICLE()
{
	
}

void PARTICLE::onInit()
{
	log->logData("Initiating particle system", id);
	ps = device->getSceneManager()->addParticleSystemSceneNode(true);
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1,color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	ps->setPosition(position);
	ps->setScale(scale);
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	thisNode = ps;
	log->debugData(MAJOR, "Created particle system");
}

void PARTICLE::onUpdate()
{
	thisNode = ps;
}

void PARTICLE::onRender()
{
	
}

void PARTICLE::loadTexture(std::string filename)
{
	log->debugData(MINOR, "Setting particle texture for", id);
	log->debugData(MAJOR, "Set to", filename);
	ps->setMaterialTexture(0, device->getVideoDriver()->getTexture(filename.c_str()));
	log->debugData(MAJOR, "Texture set");
}
void PARTICLE::setColors(video::SColor color1, video::SColor color2)
{
	log->debugData(MINOR, "Setting color of particle system", id);
	this->color1 = color1;
	this->color2 = color2;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData(MAJOR, "Color set");
}
void PARTICLE::setPosition(core::vector3df position)
{
	log->debugData(MINOR, "Setting position of particle system", id);
	this->position = position;
	log->debugData(MAJOR, "Position set");
}
void PARTICLE::setScale(core::vector3df scale)
{
	log->debugData(MINOR, "Setting scale for particle system", id);
	this->scale = scale;
	//ps->setScale(scale);
	log->debugData(MAJOR, "Scale set");
}
void PARTICLE::setDirection(core::vector3df dir)
{
	log->debugData(MINOR, "Setting direction of", id);
	this->dir = dir;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData(MAJOR, "Direction set");
}
void PARTICLE::addAffector(std::string type)
{
	log->debugData(MINOR, "Adding affector to", id);
	log->debugData(MAJOR, "Affector type", type);
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
		log->debugData(MINOR, "Invalid affector type");
		return;
	}
	log->debugData(MAJOR, "Affector added");
}
void PARTICLE::setAge(u32 minAge, u32 maxAge)
{
	log->debugData(MINOR, "Setting age of particle system", id);
	this->minAge = minAge;
	this->maxAge = maxAge;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData(MAJOR, "Age set");
}
void PARTICLE::setRate(u32 rateMin, u32 rateMax)
{
	log->debugData(MINOR, "Setting rate of particle system", id);
	this->rateMin = rateMin;
	this->rateMax = rateMax;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData(MAJOR, "Set rate");
}
void PARTICLE::setSize(core::dimension2df sizeMin, core::dimension2df sizeMax)
{
	log->debugData(MINOR, "Setting size of particle system", id);
	this->sizeMin = sizeMin;
	this->sizeMax = sizeMax;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData(MAJOR, "Size set");
}
void PARTICLE::setEmitterSize(core::aabbox3d<f32> size)
{
	log->debugData(MINOR, "Setting emitter size of particle system", id);
	this->size = size;
	scene::IParticleEmitter* em = ps->createBoxEmitter(size, dir, rateMin, rateMax, color1, color2, minAge, maxAge, 0, sizeMin, sizeMax);
	ps->setEmitter(em);
	em->drop();
	log->debugData(MAJOR, "Emitter size set");
}

std::string PARTICLE::getOType()
{
	return "PARTICLE";
}
