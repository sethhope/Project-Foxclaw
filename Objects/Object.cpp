#include "Object.h"

using namespace FCE;

OBJECT::OBJECT()
{
}

OBJECT::~OBJECT()
{
	detach();
}

void OBJECT::init()
{
	onInit();
}

void OBJECT::update()
{
	onUpdate();
	if(getChild())
	{
		((OBJECT*)getChild())->update();
	}
}

void OBJECT::render()
{
	onRender();
	if(getChild())
	{
		((OBJECT*)getChild())->render();
	}
}
