#ifndef _FSHADERCALLBACK_H_
#define _FSHADERCALLBACK_H_
#include "stdafx.h"
#include <map>
#include "irrlicht.h"
using namespace irr;
namespace FCE
{
	class ShaderCallback : public video::IShaderConstantSetCallBack
	{
		public:
			ShaderCallback();
			~ShaderCallback();
			virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData){
                video::IVideoDriver*  driver = services->getVideoDriver();
                core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
                invWorld.makeInverse();

                if(useHighLevel)
                {
                    services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
                }
                else
                {
                    services->setVertexShaderConstant(invWorld.pointer(), 0, 4);
                }

                core::matrix4 worldView;
                worldView = driver->getTransform(video::ETS_PROJECTION);
                worldView *= driver->getTransform(video::ETS_VIEW);
                worldView *= driver->getTransform(video::ETS_WORLD);

                if(useHighLevel)
                {
                    services->setVertexShaderConstant("mWorldViewProj", worldView.pointer(), 16);
                }
                else
                {
                    services->setVertexShaderConstant(worldView.pointer(), 4, 4);
                }

                f32 timept = device->getTimer()->getTime();
                if(useHighLevel)
                {
                    services->setVertexShaderConstant("mWorldTime", &timept, 1);
                }
                irr::s32 t[] = {0,1,2,3};

                services->setVertexShaderConstant("Texture0", reinterpret_cast<f32*>(&t[0]), 1);
                services->setVertexShaderConstant("Texture1", reinterpret_cast<f32*>(&t[1]), 1);
                services->setVertexShaderConstant("Texture2", reinterpret_cast<f32*>(&t[2]), 1);
                services->setVertexShaderConstant("Texture3", reinterpret_cast<f32*>(&t[3]), 1);
                for(std::map<std::string, f32>::iterator it = vsConstants.begin(); it != vsConstants.end(); it++)
                {
                    f32 pt = it->second;
                    services->setVertexShaderConstant(it->first.c_str(), reinterpret_cast<f32*>(&pt), 1);
                }
                for(std::map<std::string, f32>::iterator it = fsConstants.begin(); it != fsConstants.end(); it++)
                {
                    f32 pt = it->second;
                    services->setPixelShaderConstant(it->first.c_str(),reinterpret_cast<f32*>(&pt), 1);
                }
			}
			IrrlichtDevice *device;

			std::map<std::string, f32> vsConstants;
			std::map<std::string, f32> fsConstants;

		private:

			bool useCG = false;
			bool useHighLevel = true;
	};
}
#endif
