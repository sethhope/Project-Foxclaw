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
			virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);
			IrrlichtDevice *device;

			std::map<std::string, f32> vsConstants;
			std::map<std::string, f32> fsConstants;

		private:

			bool useCG = false;
			bool useHighLevel = true;
	};
}
#endif
