#ifndef CONFIG_H
#define CONFIG_H
#include "stdafx.h"
#include "irrlicht.h"
#include <map>
using namespace irr;
namespace FCE
{
    class Config
    {
        public:
            Config();
            ~Config();
            void loadConfig(std::string filename);
            void saveConfig(std::string filename);
            std::map<std::string, f32> data;
    };
}
#endif // CONFIG_H
