#ifndef _FEVENT_RECEIVER_H_
#define _FEVENT_RECEIVER_H_
#include "stdafx.h"
#include "irrlicht.h"

using namespace irr;
namespace FCE
{
	class FEventReceiver : public IEventReceiver
	{
  public:
			FEventReceiver();
			virtual bool OnEvent(const SEvent& event);
			virtual bool KeyDown(EKEY_CODE keyCode);
		private:
			bool keys[KEY_KEY_CODES_COUNT];
	};
}
#endif
