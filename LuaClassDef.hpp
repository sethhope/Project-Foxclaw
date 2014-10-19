#ifndef _LUACLASSDEF_H_
#define _LUACLASSDEF_H_

#include "lua.hpp"
#include "luawrapper.hpp"
#include "Irrlicht.h"


#include "Objects/Object.h"
#include "Objects/Node.h"
#include "Objects/Mesh.h"
#include "Objects/Light.h"
#include "Objects/Particle.h"

#include "Misc/Logger.h"
#include "Game/Scene.h"
using namespace irr;
using namespace FCE;

SCENE* Scene_new(lua_State* L)
{
	SCENE* s = new SCENE();
	return s;
}
int Scene_addSound(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	int id = s->addSound(luaL_checkstring(L, 2), core::vector3df(luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5)), luaL_checknumber(L, 6));
	lua_pushnumber(L, id);
	return 1;
}

int Scene_stopSound(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	s->stopSound(luaL_checknumber(L, 2));
	return 0;
}

int Scene_isPlaying(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	bool i = s->isPlaying(luaL_checknumber(L, 2));
	if(i)
	{
		lua_pushnumber(L, 1);
	}else
	{
		lua_pushnumber(L, 0);
	}
	return 1;
}

int Scene_Slog(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	s->getLog()->logData(luaL_checkstring(L, 2));
	return 0;
}

int Scene_SNlog(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	s->getLog()->logData(luaL_checkstring(L, 2), luaL_checknumber(L, 3));
	return 0;
}
int Scene_SSlog(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	s->getLog()->logData(luaL_checkstring(L, 2), luaL_checkstring(L, 3));
	return 0;
}
int Scene_addMesh(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	int mesh = s->addMesh(luaL_checkstring(L, 2), core::vector3df(luaL_checknumber(L,3),luaL_checknumber(L,4),luaL_checknumber(L,5)), core::vector3df(luaL_checknumber(L,6),luaL_checknumber(L,7),luaL_checknumber(L,8)),core::vector3df(luaL_checknumber(L,9),luaL_checknumber(L,10),luaL_checknumber(L,11)));
	lua_pushnumber(L,mesh);
	return 1;
}
int Scene_getMesh(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	luaW_push<MESH>(L,s->editMesh(luaL_checknumber(L, 2)));
	return 1;
}

MESH* Mesh_new(lua_State* L)
{
	return 0;
}
int Mesh_setPos(lua_State* L)
{
	MESH* m = luaW_check<MESH>(L, 1);
	m->setCoords(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L, 4)));
	return 0;
}
int Mesh_setRot(lua_State* L)
{
	MESH* m = luaW_check<MESH>(L, 1);
	m->setRotation(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L,4)));
	return 0;
}
int Mesh_setScale(lua_State* L)
{
	MESH* m = luaW_check<MESH>(L, 1);
	m->setScale(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L,4)));
	return 0;
}
#endif
