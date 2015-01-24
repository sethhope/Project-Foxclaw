#ifndef _FPOSTSHADER_H_
#define _FPOSTSHADER_H_
#include "stdafx.h"
#include "irrlicht.h"
#include "Misc/Logger.h"
#include "ShaderCallback.h"

using namespace irr;

namespace FCE
{
	class POSTSHADER
	{
		public:
			POSTSHADER(IrrlichtDevice* device, LOGGER* log);
			~POSTSHADER();
			
			void load(std::string vert, std::string frag);
			void init();
			void update();
			void render(video::ITexture* planeImage);
			video::ITexture* target;
		private:
			LOGGER* log;
			IrrlichtDevice* device;
			scene::ICameraSceneNode* shaderCam;
			
			scene::IBillboardSceneNode* plane;
	};
}
#endif
