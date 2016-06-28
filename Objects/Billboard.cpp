#include "Billboard.h"
using namespace FCE;
BILLBOARD::BILLBOARD(scene::ISceneManager* manager, LOGGER* log)
{
    this->log = log;
    this->manager = manager;
    setup = false;
    id = -1;
    name = "BILLBOARD";
    position = core::vector3df(0, 0, 0);
    rotation = core::vector3df(0, 0, 0);
    scale = core::vector3df(1, 1, 1);
}

BILLBOARD::~BILLBOARD()
{

}

void BILLBOARD::onInit()
{
    setup = true;
    thisNode = manager->addBillboardSceneNode(thisNode, core::dimension2d<f32>(50, 50));
    thisNode->setMaterialFlag(video::EMF_LIGHTING, false);
    thisNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
}

void BILLBOARD::onUpdate()
{

}

void BILLBOARD::onRender()
{

}

void BILLBOARD::setTexture(IrrlichtDevice* device, std::string file)
{
    filename = file;
    log->debugData(MAJOR, "Setting texture to", filename);
    if(setup==true)
    {
        thisNode->setMaterialTexture(0, manager->getVideoDriver()->getTexture(device->getFileSystem()->getAbsolutePath(filename.c_str()).c_str()));
    }
}

void BILLBOARD::setSize(f32 x, f32 y)
{
    ((scene::IBillboardSceneNode*)thisNode)->setSize(core::dimension2d<f32>(x, y));
}
