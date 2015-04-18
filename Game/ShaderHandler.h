#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H
#include "ShaderCallback.h"
#include "Misc/Logger.h"
#include "stdafx.h"
namespace FCE
{
    class ShaderHandler
    {
        public:
            ShaderHandler(IrrlichtDevice* device, LOGGER* log);
            ~ShaderHandler();
            void addShader(std::string xmlFile);
            u32 getMaterial();
        protected:
        private:
            IrrlichtDevice* device;
            LOGGER* log;
            u32 material;
    };
}
#endif // SHADERHANDLER_H
