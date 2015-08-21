
#include "Bone.h"
using namespace FCE;

BONEOBJECT::BONEOBJECT(scene::ISceneNode* bone)
{
    thisNode = bone;
}

BONEOBJECT::~BONEOBJECT()
{

}

void BONEOBJECT::onInit()
{

}

void BONEOBJECT::onUpdate()
{
    position=getIrrNode()->getPosition();
    rotation=getIrrNode()->getRotation();
    if(!children.empty())
	{
		for(std::vector<NODE*>::iterator it = children.begin(); it < children.end(); it++)
		{
			//((OBJECT*)(*it))->position = false;
		}
	}
	uDa = false;
}

void BONEOBJECT::onRender()
{

}

std::string BONEOBJECT::getOType()
{
    return "BONEOBJECT";
}
