#include "Camera.h"
using namespace FCE;
CAMERA::CAMERA(scene::ISceneManager* manager, LOGGER* log)
{
    this->manager = manager;
    this->log = log;
    type = FCAM_FPS;
    initialized = false;
    objectTarget = false;
    offset = core::vector3df(0,0,0);
}
CAMERA::~CAMERA()
{

}
void CAMERA::onInit()
{
    log->debugData(MINOR, "Initializing Camera as type", type);
    if(type == FCAM_FPS)
    {
        log->debugData(MINOR, "Initialized Camera as type", type);
        camNode = manager->addCameraSceneNodeFPS(0, 100, 0);
    }
    if(type == FCAM_FREE)
    {
        log->debugData(MINOR, "Initialized Camera as type", type);
        camNode = manager->addCameraSceneNode();
    }
    if(type == FCAM_MAYA)
    {
        log->debugData(MINOR, "Initialized Camera as type", type);
        camNode = manager->addCameraSceneNodeMaya();
    }
    thisNode = camNode;
    initialized = true;
    log->debugData(MAJOR, "Camera initialized");
}

void CAMERA::onUpdate()
{
    uDa = false;
    if(objectTarget)
    {
        camNode->setTarget(target->getIrrNode()->getPosition()+offset);
    }
    if(getParent())
    {
        camNode->setUpVector(((OBJECT*)getParent())->getUpVector());
    }
}

void CAMERA::onRender()
{
}

void CAMERA::setAspectRatio(f32 aspect)
{
    camNode->setAspectRatio(aspect);
}

void CAMERA::setType(u8 type)
{
    this->type = type;
}

void CAMERA::setTarget(OBJECT* target)
{
    this->target = target;
    camNode->bindTargetAndRotation(true);
    objectTarget = true;
}

void CAMERA::setTarget(core::vector3df pos)
{
    camNode->setTarget(pos);
    objectTarget = false;
}

void CAMERA::setOffset(core::vector3df offset)
{
    this->offset = offset;
}
void CAMERA::setUpVector(core::vector3df pos)
{
    camNode->setUpVector(pos);
}

void CAMERA::setNearClipping(f32 clip)
{
    camNode->setNearValue(clip);
}

void CAMERA::setFarClipping(f32 clip)
{
    camNode->setFarValue(clip);
}

void CAMERA::setFOV(f32 fov)
{
    camNode->setFOV(fov);
}

scene::ICameraSceneNode* CAMERA::getCamNode()
{
    return camNode;
}

f32 CAMERA::getFOV()
{
    return camNode->getFOV();
}

f32 CAMERA::getNearClipping()
{
    return camNode->getNearValue();
}

f32 CAMERA::getFarClipping()
{
    return camNode->getFarValue();
}

u8 CAMERA::getType()
{
    return type;
}

scene::ISceneManager* CAMERA::getManager()
{
    return manager;
}
std::string CAMERA::getOType()
{
    return "CAMERA";
}

OBJECT* CAMERA::getTarget()
{
    return target;
}
