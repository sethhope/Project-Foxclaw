#include "EmptyObject.h"
using namespace FCE;

EMPTYOBJECT::EMPTYOBJECT(scene::ISceneManager* manager)
{
    thisNode = manager->addEmptySceneNode();
}

EMPTYOBJECT::~EMPTYOBJECT()
{

}

void EMPTYOBJECT::onInit()
{

}

void EMPTYOBJECT::onUpdate()
{

}

void EMPTYOBJECT::onRender()
{

}

std::string EMPTYOBJECT::getOType()
{
    return "EMPTYOBJECT";
}
