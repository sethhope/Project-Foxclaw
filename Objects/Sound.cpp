#include "Sound.h"
using namespace FCE;

SOUND::SOUND()
{
	
}
SOUND::SOUND(ISoundEngine* engine, LOGGER* log)
{
	this->engine = engine;
	this->log = log;
	volume = 1.0f;
}
SOUND::~SOUND()
{
	
}

void SOUND::onInit()
{
	
}

void SOUND::onUpdate()
{
	s->setPosition(globalPosition);
	s->setVolume(volume);
}

void SOUND::onRender()
{
	
}

void SOUND::load(std::string filename, bool loop)
{
	log->debugData("Loading sound", filename);
	s = engine->play3D(filename.c_str(), globalPosition, loop, false, true, ESM_AUTO_DETECT, true);
	if(!s)
	{
		log->logData("Failed to load sound", filename);
	}else
	{
		log->debugData("Sound loaded");
	}
}

void SOUND::setVolume(float volume)
{
	log->debugData("Setting volume of", this->id);
	log->debugData("Setting to", volume);
	this->volume = volume;
}
ISound* SOUND::getSound()
{
	return s;
}
void SOUND::setID(int id)
{
	this->id = id;
}

int SOUND::getID()
{
	return id;
}
