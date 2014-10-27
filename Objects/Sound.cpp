#include "Sound.h"
using namespace FCE;

SOUND::SOUND()
{
	
}
SOUND::SOUND(ISoundEngine* engine)
{
	this->engine = engine;
	volume = 1;
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
	s = engine->play3D(filename.c_str(), globalPosition, loop, false, true, ESM_AUTO_DETECT, true);
}

void SOUND::setVolume(float volume)
{
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
