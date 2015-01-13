#ifndef _FLIGHT_H_
#define _FLIGHT_H_
#include "Irrlicht.h"
#include "Object.h"
#include "Misc/Logger.h"
using namespace irr;
using namespace core;
namespace FCE
{
	class LIGHT : public OBJECT
	{
		public:
			LIGHT(scene::ISceneManager* manager, LOGGER* log);
			~LIGHT();
			void onInit();
			void onUpdate();
			void onRender();
			std::string getOType();
			
			void setColor(video::SColorf color);
			void setDropoff(f32 dropoff);
			void setType(video::E_LIGHT_TYPE type);
			video::E_LIGHT_TYPE getType();
			f32 getDropoff();
			video::SColorf getColor();
		private:
			core::vector3df position;
			core::vector3df scale;
			video::SColorf color;
			scene::ISceneManager* manager;
			video::E_LIGHT_TYPE type;
			f32 dropoff;
			LOGGER* log;
			
	};
}
#endif
