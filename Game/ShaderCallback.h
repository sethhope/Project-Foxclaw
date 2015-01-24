#ifndef _FSHADERCALLBACK_H_
#define _FSHADERCALLBACK_H_
#include "stdafx.h"
#include "irrlicht.h"
using namespace irr;
namespace FCE
{
	class ShaderCallback : public video::IShaderConstantSetCallBack
	{
		public:
			ShaderCallback();
			~ShaderCallback();
			virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);
			IrrlichtDevice *device;
		private:
			
			bool useCG = false;
			bool useHighLevel = true;
	};
}
#endif
