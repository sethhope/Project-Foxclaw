#include "Script.h"
#include "LuaClassDef.hpp"
#include "Game/Scene.h"
using namespace FCE;

SCRIPT::SCRIPT(LOGGER* log)
{
	this->log = log;
	L = NULL;
}
SCRIPT::SCRIPT(lua_State* L, LOGGER* log)
{
	this->L = L;
	this->log = log;
}

SCRIPT::~SCRIPT()
{
	lua_close(L);
}

void SCRIPT::onInit()
{
	log->logData("Creating new luastate");
	L = luaL_newstate();
	if(L == NULL)
	{
		log->logData("Failed to create luastate");
		return;
	}
	log->debugData("Opening libraries");
	luaL_openlibs(L);
	log->debugData("Opened libraries");
	//TODO: add functions and classes

	static luaL_Reg Scene_metatable[] =
	{
		{"addSound", Scene_addSound},
		{"getSound", Scene_getSound},
		{"SLog", Scene_Slog},
		{"SNLog", Scene_SNlog},
		{"SSLog", Scene_SSlog},
		{"setDebug", Scene_setDebug},
		{"addMesh", Scene_addMesh},
		{"getMesh", Scene_getMesh},
		{"addParticle", Scene_addParticle},
		{"getParticle", Scene_getParticle},
		{NULL, NULL}
	};
	static luaL_Reg Mesh_metatable[] =
	{
		{"setPos", Mesh_setPos},
		{"setRot", Mesh_setRot},
		{"setScale", Mesh_setScale},
		{"setMaterial", Mesh_setMaterial},
		{NULL, NULL}
	};
	static luaL_Reg Particle_metatable[] =
	{
		{"setAge", Particle_setAge},
		{"setColors", Particle_setColors},
		{"setDirection", Particle_setDirection},
		{"setEmitterSize", Particle_setEmitterSize},
		{"setPosition", Particle_setPosition},
		{"setScale", Particle_setScale},
		{"setRate", Particle_setRate},
		{"setSize", Particle_setSize},
		{"addAffector", Particle_addAffector},
		{NULL, NULL}
	};
	static luaL_Reg Sound_metatable[] =
	{
		{"load", Sound_load},
		{"stop", Sound_stop},
		{"isPlaying", Sound_isPlaying},
		{"setVolume", Sound_setVolume},
		{"setPosition", Sound_setPosition},
		{"setDropoff", Sound_setDropoff},
		{"enableChorus", Sound_enableChorus},
		{"disableChorus", Sound_disableChorus},
		{"enableCompressor", Sound_enableCompressor},
		{"disableCompressor", Sound_disableCompressor},
		{"enableDistortion", Sound_enableDistortion},
		{"disableDistortion", Sound_disableDistortion},
		{"enableEcho", Sound_enableEcho},
		{"disableEcho", Sound_disableEcho},
		{"enableReverb", Sound_enableReverb},
		{"disableReverb", Sound_disableReverb},
		{NULL, NULL}
	};
	static luaL_Reg Empty_table[] =
	{
		{NULL, NULL}
	};
	
	luaW_register<SOUND>(L, "SOUND", Empty_table, Sound_metatable, Sound_new);
	luaW_register<PARTICLE>(L, "PARTICLE", Empty_table, Particle_metatable, Particle_new);
	luaW_register<MESH>(L, "MESH", Empty_table, Mesh_metatable, Mesh_new);
	luaW_register<SCENE>(L, "SCENE", Empty_table, Scene_metatable, Scene_new);

}

lua_State* SCRIPT::run(std::string filename)
{
	log->debugData("Attempting to run script", filename);
	lua_State* tmp = L;
	this->filename = filename;
	if(luaL_loadfile(tmp, filename.c_str()))
	{
		log->logData("Failed to load file", filename);
		log->logData(lua_tostring(L, -1));
		return NULL;
	}
	if(lua_pcall(tmp, 0, 0, 0))
	{
		log->logData("Failed to run script", filename);
		log->logData(lua_tostring(L, -1));
		return NULL;
	}
	L = tmp;
	log->debugData("Successfully ran", filename);
	return L;
}

lua_State* SCRIPT::runInit()
{
	log->debugData("Running init function");
	lua_getglobal(L, "init");
	int er = lua_pcall(L, 0, 0, 0);
	if(er != 0)
	{
		log->logData("Failed to run init function", lua_tostring(L, -1));
		return L;
	}
	log->debugData("Ran init function");
	return L;
}

void SCRIPT::onUpdate()
{
	lua_getglobal(L, "update");
	if(lua_pcall(L, 0, 0, 0)!=0)
	{
		log->logData("Failed to run update function", lua_tostring(L, -1));
		return;
	}
}
void SCRIPT::onRender()
{
	lua_getglobal(L, "render");
	if(lua_pcall(L, 0, 0, 0)!=0)
	{
		log->logData("Failed to run render function", lua_tostring(L, -1));
		return;
	}
}
