#ifndef _FSOUND_H_
#define _FSOUND_H_
#include "stdafx.h"
#include "Object.h"
#include "Irrlicht.h"
#include "irrKlang.h"
using namespace irr;
using namespace irrklang;
namespace FCE
{
	class SOUND : public OBJECT
	{
		public:
			SOUND();
			SOUND(ISoundEngine* engine);
			~SOUND();
			void onInit();
			void onUpdate();
			void onRender();
			void load(std::string filename, bool loop);
			void setVolume(float volume);
			ISound* getSound();
		private:
			ISoundEngine* engine;
			ISound* s;
			float volume;
	};
}
#endif
