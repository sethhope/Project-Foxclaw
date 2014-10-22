#ifndef _FSOUND_H_
#define _FSOUND_H_
#include "Object.h"
#include "Irrlicht.h"
#include "irrKlang.h"
namespace FCE
{
	class SOUND : public OBJECT
	{
		public:
			SOUND();
			~SOUND();
			void onInit();
			void onUpdate();
			void onRender();
			
		
	};
}
#endif
