#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H
#include "ShaderCallback.h"
#include "Misc/Logger.h"
#include "stdafx.h"
#define FCE_FRAG 1
#define FCE_VERT 2
namespace FCE
{
    class ShaderHandler
    {
        public:
            ShaderHandler(IrrlichtDevice* device, LOGGER* log);
            ~ShaderHandler();
            void addShader(std::string xmlFile, video::E_MATERIAL_TYPE curMaterial, u32 materialIndex);
            void addConstant(u32 shader, std::string key, f32 data);
            IrrlichtDevice* getDevice(){return device;};
            u32 m;
        protected:
        private:
            IrrlichtDevice* device;
            LOGGER* log;

            std::map<std::string, f32> fsConstants;
            std::map<std::string, f32> vsConstants;
    };
}
#endif // SHADERHANDLER_H
