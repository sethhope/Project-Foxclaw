#ifndef _FPARTICLE_H_
#define _FPARTICLE_H_
#include "stdafx.h"
#include "Object.h"
#include "Irrlicht.h"
#include "Misc/Logger.h"
using namespace irr;
namespace FCE
{
	class PARTICLE : public OBJECT
	{
		public:
			PARTICLE(IrrlichtDevice* device, LOGGER* log);
			~PARTICLE();
			void onInit();
			void onUpdate();
			void onRender();
			void loadTexture(std::string filename);
			void setPosition(core::vector3df position);
			void setScale(core::vector3df scale);
			void setDirection(core::vector3df dir);
			void setColors(video::SColor color1, video::SColor color2);
			void addAffector(std::string type);
			void setRate(float rateMin, float rateMax);
			void setSize(core::dimension2df sizeMin, core::dimension2df sizeMax);
			void setAge(float minAge, float maxAge);
			void setID(int id);
			int getID();
		private:
			video::SColor color1, color2;
			core::vector3df dir;
			float rateMin, rateMax;
			float minAge, maxAge;
			core::dimension2df sizeMin, sizeMax;
			scene::IParticleSystemSceneNode* ps;
			scene::IParticleEmitter* em;
			int id;
			IrrlichtDevice* device;
			LOGGER* log;
	};
}
#endif
