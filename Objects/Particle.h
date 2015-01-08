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
			void setRate(u32 rateMin, u32 rateMax);
			void setSize(core::dimension2df sizeMin, core::dimension2df sizeMax);
			void setAge(u32 minAge, u32 maxAge);
			void setEmitterSize(core::aabbox3d<f32> size);
		private:
			video::SColor color1, color2;
			core::vector3df dir;
			u32 rateMin, rateMax;
			u32 minAge, maxAge;
			core::aabbox3d<f32> size;
			core::dimension2df sizeMin, sizeMax;
			scene::IParticleSystemSceneNode* ps;
			scene::IParticleEmitter* em;
			IrrlichtDevice* device;
			LOGGER* log;
	};
}
#endif
