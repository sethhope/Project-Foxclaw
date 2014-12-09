#include "Script.h"
#include "LuaClassDef.hpp"
#include "Game/Scene.h"

using namespace FCE;

SCRIPT::SCRIPT(LOGGER* log)
{
	this->log = log;
	L = NULL;
	position = core::vector3df(0, 0, 0);
	rotation = core::vector3df(0, 0, 0);
	scale = core::vector3df(1, 1, 1);
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
	log->debugData(MINOR, "Opening libraries");
	luaL_openlibs(L);
	log->debugData(MINOR, "Opened libraries");
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
		{"addAnimatedMesh", Scene_addAnimatedMesh},
		{"getAnimatedMesh", Scene_getAnimatedMesh},
		{"addParticle", Scene_addParticle},
		{"getParticle", Scene_getParticle},
		{"addEmpty", Scene_addEmpty},
		{"getEmpty", Scene_getEmpty},
		{"addCamera", Scene_addCamera},
		{"getCamera", Scene_getCamera},
		{"getKey", Scene_getKey},
		{"createCharacter", Scene_createCharacter},
		{"modifyCharacter", Scene_modifyCharacter},
		{"characterData", Scene_getCharacterData},
		{"moveCharacter", Scene_moveCharacter},
		{"getObject", Scene_getObject},
		{"setPhysicsDebug", Scene_setPhysicsDebug},
		{"setSkydome", Scene_setSkydome},
		{"removeObject", Scene_removeObject},
		{NULL, NULL}
	};
	static luaL_Reg Mesh_metatable[] =
	{
		{"setPos", Mesh_setPos},
		{"setRot", Mesh_setRot},
		{"setScale", Mesh_setScale},
		{"addCollider", Mesh_addCollider},
		{"toObject", Mesh_toObject},
		{NULL, NULL}
	};
	static luaL_Reg AnimatedMesh_metatable[] =
	{
		{"setPos", AnimatedMesh_setPos},
		{"setRot", AnimatedMesh_setRot},
		{"setScale", AnimatedMesh_setScale},
		{"addCollider", AnimatedMesh_addCollider},
		{"setFrameLoop", AnimatedMesh_setFrameLoop},
		{"setSpeed", AnimatedMesh_setSpeed},
		{"toObject", AnimatedMesh_toObject},
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
		{"addCollider", Particle_addCollider},
		{"toObject", Particle_toObject},
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
	static luaL_Reg Camera_metatable[] =
	{
		{"setFOV", Camera_setFOV},
		{"getFOV", Camera_getFOV},
		{"setClipping", Camera_setClipping},
		{"getClipping", Camera_getClipping},
		{"setTarget", Camera_setTarget},
		{"setAspect", Camera_setAspect},
		{"setOffset", Camera_setOffset},
		{"setPosition", Camera_setPosition},
		{NULL, NULL}
	};
	static luaL_Reg EmptyObject_metatable[] =
	{
		{"setPosition", Empty_setPosition},
		{"setRotation", Empty_setRotation},
		{"setScale", Empty_setScale},
		{"addCollider", Empty_addCollider},
		{"toObject", Empty_toObject},
		{NULL, NULL}
	};
	static luaL_Reg Empty_table[] =
	{
		{NULL, NULL}
	};
	static luaL_Reg Object_table[] =
	{
		{"getCollider", Object_getCollider},
		{"getPosition", Object_getPosition},
		{"getRotation", Object_getRotation},
		{"getScale", Object_getScale},
		{"setPosition", Object_setPosition},
		{"setRotation", Object_setRotation},
		{"setScale", Object_setScale},
		{"getName", Object_getName},
		{"setName", Object_setName},
		{"getID", Object_getID},
		{"addScript", Object_addScript},
		{"setMaterial", Object_setMaterial},
		{"setMaterialFlag", Object_setMaterialFlag},
		{"setMaterialTexture", Object_setTexture},
		{"attachTo", Object_attachTo},
		{NULL, NULL}
	};
	luaW_register<OBJECT>(L, "OBJECT", Empty_table, Object_table, Object_new);
	luaW_register<CAMERA>(L, "CAMERA", Empty_table, Camera_metatable, Camera_new);
	luaW_register<EMPTYOBJECT>(L, "EMPTY", Empty_table, EmptyObject_metatable, Empty_new);
	luaW_register<SOUND>(L, "SOUND", Empty_table, Sound_metatable, Sound_new);
	luaW_register<PARTICLE>(L, "PARTICLE", Empty_table, Particle_metatable, Particle_new);
	luaW_register<MESH>(L, "MESH", Empty_table, Mesh_metatable, Mesh_new);
	luaW_register<ANIMATEDMESH>(L, "ANIMATEDMESH", Empty_table, AnimatedMesh_metatable, AnimatedMesh_new);
	luaW_register<SCENE>(L, "SCENE", Empty_table, Scene_metatable, Scene_new);

}

lua_State* SCRIPT::run(std::string filename)
{
	log->debugData(MINOR, "Attempting to run script", filename);
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
	log->debugData(MINOR, "Successfully ran", filename);
	return L;
}

lua_State* SCRIPT::runInit()
{
	log->debugData(MAJOR, "Running init function");
	lua_getglobal(L, "init");
	int er = lua_pcall(L, 0, 0, 0);
	if(er != 0)
	{
		log->logData("Failed to run init function", lua_tostring(L, -1));
		return L;
	}
	log->debugData(MAJOR, "Ran init function");
	return L;
}

void SCRIPT::onUpdate()
{
	if(getParent())
	{
		luaW_push<OBJECT>(L, (OBJECT*)getParent());
		lua_setglobal(L, "ParentObject");
	}else
	{
		lua_pushnumber(L, 0);
		lua_setglobal(L, "ParentObject");
	}
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
