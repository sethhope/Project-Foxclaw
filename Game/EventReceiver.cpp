#include "EventReceiver.h"

using namespace irr;
using namespace FCE;



FEventReceiver::FEventReceiver()
{
    //Sets all keys and mouse buttons to FALSE
	for(u16 i = 0; i < KEY_KEY_CODES_COUNT; i++)
	{
		keys[i] = false;
	}
	mouseButtons[0] = false;
	mouseButtons[1] = false;
	mouseButtons[2] = false;
}
FEventReceiver::~FEventReceiver()
{
    //Clears the GUICallers
	callers.clear();
}
bool FEventReceiver::KeyDown(EKEY_CODE keyCode)
{
    //Checks for keydown
	return keys[keyCode];
}

core::position2di FEventReceiver::mousePos()
{
    //Returns the position of the mouse on screen
	return Position;
}

bool FEventReceiver::MouseButton(u8 button)
{
    //Returns if the mouse button (button) is down.
	return mouseButtons[button];
}

bool FEventReceiver::OnEvent(const SEvent& event)
{
    //Begin event
	bool override = false;
	//iterate through GUI callers
	for(std::vector<GUI*>::iterator it = callers.begin(); it < callers.end(); it++)
	{
		if((*it)->guiCaller != 0)
		{
			bool tmp = (*it)->OnEvent(event);
			if(tmp == true)
			{
			    //If GUI event, override the event receiver
				override = true;
			}
		}
	}
	if(override)
	{
	    //Cancels all keypress overrides for the event receiver so that the GUI can receive them
		for(u16 i = 0; i < KEY_KEY_CODES_COUNT; i++)
		{
			keys[i] = false;
		}
		return false;
	}
 	if(event.EventType == EET_KEY_INPUT_EVENT)
	{
	    //If key input, set the key in the array to down
		keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
		return true;
	}
	if(event.EventType == EET_MOUSE_INPUT_EVENT)
	{
	    //on mouse event, handle corresponding buttons and movements.
		switch(event.MouseInput.Event)
		{
			case EMIE_LMOUSE_PRESSED_DOWN:
				mouseButtons[0] = true;
			break;
			case EMIE_LMOUSE_LEFT_UP:
				mouseButtons[0] = false;
			break;
			case EMIE_MMOUSE_PRESSED_DOWN:
				mouseButtons[1] = true;
			break;
			case EMIE_MMOUSE_LEFT_UP:
				mouseButtons[1] = false;
			break;
			case EMIE_RMOUSE_PRESSED_DOWN:
				mouseButtons[2] = true;
			break;
			case EMIE_RMOUSE_LEFT_UP:
				mouseButtons[2] = false;
			break;
			case EMIE_MOUSE_MOVED:
				Position.X = event.MouseInput.X;
				Position.Y = event.MouseInput.Y;
			break;
			default:
			break;
		}
	}
	return false;
}
void FEventReceiver::addGUI(GUI* gui)
{
    //Add a GUI element
	callers.push_back(gui);
}
//=================================================================
//To those who enter the light code... This will be fixed, I swear!
//=================================================================
void FEventReceiver::OnPreRender(core::array<scene::ISceneNode*> & lightList)
{
	lights = &lightList;
}
void FEventReceiver::OnPostRender()
{
	for(u32 i = 0; i < lights->size(); i++)
	{
		(*lights)[i]->setVisible(true);
	}
}
void FEventReceiver::OnRenderPassPreRender(scene::E_SCENE_NODE_RENDER_PASS renderPass)
{
	CurrentRenderPass = renderPass;
}
void FEventReceiver::OnRenderPassPostRender(scene::E_SCENE_NODE_RENDER_PASS renderPass)
{
	if(scene::ESNRP_SOLID == renderPass)
	{
		for(u32 i = 0; i < lights->size(); i++)
		{
			(*lights)[i]->setVisible(false);
		}
	}
}
void FEventReceiver::OnNodePreRender(scene::ISceneNode* node)
{
	CurrentSceneNode = node;
	if(scene::ESNRP_SOLID != CurrentRenderPass)
	{
		return;
	}
	const core::vector3df nodePos = node->getAbsolutePosition();
	core::array<LightDistanceElement> sorting;
	sorting.reallocate(lights->size());
	for(u32 i = 0; i < lights->size(); i++)
	{
		scene::ISceneNode* lightNode = (*lights)[i];
		const f64 distance = lightNode->getAbsolutePosition().getDistanceFromSQ(nodePos);
		sorting.push_back(LightDistanceElement(lightNode, distance));
	}
	sorting.sort();
	for(u32 i = 0; i < sorting.size(); i++)
	{
		sorting[i].node->setVisible(i < 3);
	}
}
void FEventReceiver::OnNodePostRender(scene::ISceneNode* node)
{

}
