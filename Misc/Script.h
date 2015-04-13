#ifndef _FSCRIPT_H_
#define _FSCRIPT_H_
#include "lua.hpp"
#include "luawrapper.hpp"
#include "stdafx.h"
#include "Logger.h"
#include "Objects/Object.h"
#include "include/lua.hpp"

namespace FCE
{
class SCRIPT : public OBJECT
{
public:
    SCRIPT(LOGGER* log);
    SCRIPT(lua_State* L, LOGGER* log);
    ~SCRIPT();
    void onInit();
    void onUpdate();
    void onRender();
    std::string getOType();
    lua_State* runInit();
    lua_State* run(std::string filename);
    lua_State* L;
private:
    std::string filename;
    LOGGER* log;
};
}
#endif // _SCRIPT_H_
