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
    name = "SOUND_EMITTER";
    position = core::vector3df(0, 0, 0);
    rotation = core::vector3df(0, 0, 0);
    scale = core::vector3df(1, 1, 1);
}
SOUND::~SOUND()
{

}

void SOUND::onInit()
{

}

void SOUND::onUpdate()
{
    s->setPosition(position);
    s->setVolume(volume);
}

void SOUND::onRender()
{

}

void SOUND::load(std::string filename, bool loop)
{
    log->debugData(MINOR, "Loading sound", filename);
    s = engine->play3D(filename.c_str(), position, loop, false, true, ESM_AUTO_DETECT, true);
    if(!s)
    {
        log->logData("Failed to load sound", filename);
    }
    else
    {
        log->debugData(MAJOR, "Sound loaded");
    }
}

void SOUND::setVolume(f32 volume)
{
    log->debugData(EXTRA, "Setting volume of", this->id);
    log->debugData(EXTRA, "Setting to", volume);
    this->volume = volume;
}
ISound* SOUND::getSound()
{
    return s;
}
std::string SOUND::getOType()
{
    return "SOUND";
}
