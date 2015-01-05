#ifndef _FSOUND_H_
#define _FSOUND_H_
#include "stdafx.h"
#include "Object.h"
#include "Misc/Logger.h"
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
			SOUND(ISoundEngine* engine, LOGGER* log);
			~SOUND();
			void onInit();
			void onUpdate();
			void onRender();
			std::string getOType();
			void load(std::string filename, bool loop);
			void setVolume(float volume);
			ISound* getSound();
		private:
			ISoundEngine* engine;
			ISound* s;
			LOGGER* log;
			float volume;
	};
}
#endif
