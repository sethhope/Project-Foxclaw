#ifndef _FLIGHT_H_
#define _FLIGHT_H_
#include "Irrlicht.h"
#include "Object.h"
#include "Misc/Logger.h"
using namespace irr;
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
			void setColor(video::SColorf color);
			void setDropoff(float dropoff);
			void setType(video::E_LIGHT_TYPE type);
			int getID();
			void setID(int id);
		private:
			int id;
			core::vector3df position;
			core::vector3df scale;
			video::SColorf color;
			//scene::ISceneNode* node;
			scene::ISceneManager* manager;
			video::E_LIGHT_TYPE type;
			float dropoff;
			LOGGER* log;
			
	};
}
#endif
