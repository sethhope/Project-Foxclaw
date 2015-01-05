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
			std::string getOType();
			
			void loadTexture(std::string filename);
			void setPosition(core::vector3df position);
			void setScale(core::vector3df scale);
			void setDirection(core::vector3df dir);
			void setColors(video::SColor color1, video::SColor color2);
			void addAffector(std::string type);
			void setRate(int rateMin, int rateMax);
			void setSize(core::dimension2df sizeMin, core::dimension2df sizeMax);
			void setAge(int minAge, int maxAge);
			void setEmitterSize(core::aabbox3d<f32> size);
		private:
			video::SColor color1, color2;
			core::vector3df dir;
			int rateMin, rateMax;
			int minAge, maxAge;
			core::aabbox3d<f32> size;
			core::dimension2df sizeMin, sizeMax;
			scene::IParticleSystemSceneNode* ps;
			scene::IParticleEmitter* em;
			IrrlichtDevice* device;
			LOGGER* log;
	};
}
#endif
