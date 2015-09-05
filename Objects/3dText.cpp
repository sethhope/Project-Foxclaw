#include "3dText.h"
using namespace FCE;

TEXT3D::TEXT3D(scene::ISceneManager* manager, LOGGER* log)
{
    this->log = log;
    this->manager = manager;
    created = false;
}
TEXT3D::~TEXT3D()
{

}
void TEXT3D::onInit()
{
    thisNode = manager->addBillboardTextSceneNode(0, core::stringw(text.c_str()).c_str(), 0, core::dimension2d<f32>(sizeX, sizeY), position);
    created = true;
}
void TEXT3D::onUpdate()
{

}

void TEXT3D::onRender()
{

}

void TEXT3D::setText(std::string text)
{
    this->text = text;
    if(created == true)
    {
        ((scene::IBillboardTextSceneNode*)thisNode)->setText(core::stringw(text.c_str()).c_str());
    }
}

std::string TEXT3D::getText()
{
    return text;
}

void TEXT3D::setSize(f32 x, f32 y)
{
    sizeX = x;
    sizeY = y;
    if(created == true)
    {
        ((scene::IBillboardTextSceneNode*)thisNode)->setSize(core::dimension2d<f32>(sizeX, sizeY));
    }
}
std::string TEXT3D::getOType()
{
    return "TEXT3D";
}
