#include "EventReceiver.h"

using namespace irr;
using namespace FCE;

FEventReceiver::FEventReceiver()
{
	for(int i = 0; i < KEY_KEY_CODES_COUNT; i++)
	{
		keys[i] = false;
	}
}

bool FEventReceiver::KeyDown(EKEY_CODE keyCode)
{
	return keys[keyCode];
}

bool FEventReceiver::OnEvent(const SEvent& event)
{
	if(event.EventType == EET_KEY_INPUT_EVENT)
	{
		keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}
	return false;
}
