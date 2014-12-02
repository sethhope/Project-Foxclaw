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
#include "Objects/EmptyObject.h"

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

int Scene_getSound(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	luaW_push<SOUND>(L, s->editSound(luaL_checknumber(L, 2)));
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
int Scene_setDebug(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	s->getLog()->setDebug(luaL_checknumber(L, 2));
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
int Scene_addParticle(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	int id = s->addParticleSystem(core::vector3df(luaL_checknumber(L,2), luaL_checknumber(L,3), luaL_checknumber(L,4)), core::vector3df(luaL_checknumber(L,5), luaL_checknumber(L,6), luaL_checknumber(L,7)),core::vector3df(luaL_checknumber(L,8), luaL_checknumber(L,9), luaL_checknumber(L,10)), luaL_checkstring(L, 11));
	lua_pushnumber(L, id);
	return 1;
}
int Scene_getParticle(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	luaW_push<PARTICLE>(L, s->editParticleSystem(luaL_checknumber(L, 2)));
	return 1;
}
int Scene_addEmpty(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	int id = s->addEmptyObject(core::vector3df(luaL_checknumber(L,2),luaL_checknumber(L,3),luaL_checknumber(L,4)), core::vector3df(luaL_checknumber(L,5),luaL_checknumber(L,6),luaL_checknumber(L,7)),core::vector3df(luaL_checknumber(L,8),luaL_checknumber(L,9),luaL_checknumber(L,10)));
	lua_pushnumber(L, id);
	return 1;
}
int Scene_getEmpty(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	luaW_push<EMPTYOBJECT>(L, s->editEmpty(luaL_checknumber(L, 2)));
	return 1;
}

MESH* Mesh_new(lua_State* L)
{
	return 0;
}
int Mesh_setPos(lua_State* L)
{
	MESH* m = luaW_check<MESH>(L, 1);
	m->setPosition(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L, 4)));
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
int Mesh_setMaterial(lua_State* L)
{
	MESH* m = luaW_check<MESH>(L, 1);
	std::string type = luaL_checkstring(L, 2);
	if(type == "solid")
	{
		m->getIrrNode()->setMaterialType(video::EMT_SOLID);
	}
	if(type == "solid2")
	{
		m->getIrrNode()->setMaterialType(video::EMT_SOLID_2_LAYER);
	}
	if(type == "lightmap")
	{
		m->getIrrNode()->setMaterialType(video::EMT_LIGHTMAP);
	}
	if(type == "lightmaplight")
	{
		m->getIrrNode()->setMaterialType(video::EMT_LIGHTMAP_LIGHTING);
	}
	if(type == "normalmap_solid")
	{
		m->getIrrNode()->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
	}
	if(type == "parallax_solid")
	{
		m->getIrrNode()->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
	}
	return 0;
}
int Mesh_addCollider(lua_State* L)
{
	MESH* m = luaW_check<MESH>(L, 1);
	SCENE* s = luaW_check<SCENE>(L, 2);
	std::string type = luaL_checkstring(L, 3);
	if(type == "CUBE")
	{
		m->addCollider(s->getLog(), COL_CUBE, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4));
	}
	if(type == "SPHERE")
	{
		m->addCollider(s->getLog(), COL_SPHERE, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4));
	}
	if(type == "MESH")
	{
		m->addCollider(s->getLog(), COL_MESH, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4), m->getMesh());
	}
	return 0;
}

PARTICLE* Particle_new(lua_State* L)
{
	return 0;
}
int Particle_addAffector(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	p->addAffector(luaL_checkstring(L, 2));
	return 0;
}
int Particle_setAge(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	p->setAge(luaL_checknumber(L, 2), luaL_checknumber(L,3));
	return 0;
}
int Particle_setColors(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	p->setColors(video::SColor(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5)), video::SColor(luaL_checknumber(L, 6), luaL_checknumber(L, 7), luaL_checknumber(L, 8), luaL_checknumber(L, 9)));
	return 0;
}
int Particle_setDirection(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	p->setDirection(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
	return 0;
}
int Particle_setEmitterSize(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	core::aabbox3d<f32> size = core::aabbox3d<f32>(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5), luaL_checknumber(L, 6), luaL_checknumber(L, 7));
	p->setEmitterSize(size);
	return 0;
}
int Particle_setPosition(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	p->setPosition(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
	return 0;
}
int Particle_setScale(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	p->setScale(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
	return 0;
}
int Particle_setRate(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	p->setRate(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}
int Particle_setSize(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	p->setSize(core::dimension2d<f32>(luaL_checknumber(L, 2), luaL_checknumber(L, 2)), core::dimension2d<f32>(luaL_checknumber(L, 3), luaL_checknumber(L, 3)));
	return 0;
}
int Particle_addCollider(lua_State* L)
{
	PARTICLE* m = luaW_check<PARTICLE>(L, 1);
	SCENE* s = luaW_check<SCENE>(L, 2);
	std::string type = luaL_checkstring(L, 3);
	if(type == "CUBE")
	{
		m->addCollider(s->getLog(), COL_CUBE, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4));
	}
	if(type == "SPHERE")
	{
		m->addCollider(s->getLog(), COL_SPHERE, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4));
	}
	return 0;
}

SOUND* Sound_new(lua_State* L)
{
	SOUND* s = new SOUND();
	//SHOULD NEVER BE EXECUTED FROM LUA!
	return s;
}

int Sound_load(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	bool loop;
	if(luaL_checknumber(L, 3)==1)
	{
		loop = true;
	}else
	{
		loop = false;
	}
	s->load(luaL_checkstring(L, 2), loop);
	return 0;
}
int Sound_stop(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	s->getSound()->stop();
	return 0;
}
int Sound_isPlaying(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	if(s->getSound()->isFinished() == true)
	{
		lua_pushnumber(L, 1);
	}else
	{
		lua_pushnumber(L, 0);
	}
	return 1;
}
int Sound_setVolume(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	s->setVolume(luaL_checknumber(L, 2));
	return 0;
}
int Sound_setPosition(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	s->setPosition(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
	return 0;
}
int Sound_setDropoff(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	s->getSound()->setMinDistance(luaL_checknumber(L, 2));
	s->getSound()->setMaxDistance(luaL_checknumber(L, 3));
	return 0;
}
int Sound_enableChorus(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	ISoundEffectControl* fx = s->getSound()->getSoundEffectControl();
	fx->enableChorusSoundEffect(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}
int Sound_disableChorus(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	ISoundEffectControl* fx = s->getSound()->getSoundEffectControl();
	fx->disableChorusSoundEffect();
	return 0;
}
int Sound_enableCompressor(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	ISoundEffectControl* fx = s->getSound()->getSoundEffectControl();
	fx->enableCompressorSoundEffect(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
	return 0;
}
int Sound_disableCompressor(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	s->getSound()->getSoundEffectControl()->disableCompressorSoundEffect();
	return 0;
}
int Sound_enableDistortion(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	ISoundEffectControl* fx = s->getSound()->getSoundEffectControl();
	fx->enableDistortionSoundEffect(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5), luaL_checknumber(L, 6));
	return 0;
}
int Sound_disableDistortion(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	s->getSound()->getSoundEffectControl()->disableDistortionSoundEffect();
	return 0;
}
int Sound_enableEcho(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	ISoundEffectControl* fx = s->getSound()->getSoundEffectControl();
	fx->enableEchoSoundEffect(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5), luaL_checknumber(L, 6));
	return 0;
}
int Sound_disableEcho(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	s->getSound()->getSoundEffectControl()->disableEchoSoundEffect();
	return 0;
}
int Sound_enableReverb(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	ISoundEffectControl* fx = s->getSound()->getSoundEffectControl();
	fx->enableWavesReverbSoundEffect(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}
int Sound_disableReverb(lua_State* L)
{
	SOUND* s = luaW_check<SOUND>(L, 1);
	s->getSound()->getSoundEffectControl()->disableWavesReverbSoundEffect();
	return 0;
}

EMPTYOBJECT* Empty_new(lua_State* L)
{
	//EMPTYOBJECT* e = new EMPTYOBJECT();
	//return e;
}

int Empty_setPosition(lua_State* L)
{
	EMPTYOBJECT* e = luaW_check<EMPTYOBJECT>(L, 1);
	e->setPosition(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
	return 0;
}

int Empty_setRotation(lua_State* L)
{
	EMPTYOBJECT* e = luaW_check<EMPTYOBJECT>(L, 1);
	e->setRotation(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
	return 0;
}

int Empty_setScale(lua_State* L)
{
	EMPTYOBJECT* e = luaW_check<EMPTYOBJECT>(L, 1);
	e->setScale(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
	return 0;
}
int Empty_addCollider(lua_State* L)
{
	EMPTYOBJECT* m = luaW_check<EMPTYOBJECT>(L, 1);
	SCENE* s = luaW_check<SCENE>(L, 2);
	std::string type = luaL_checkstring(L, 3);
	if(type == "CUBE")
	{
		m->addCollider(s->getLog(), COL_CUBE, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4));
	}
	if(type == "SPHERE")
	{
		m->addCollider(s->getLog(), COL_SPHERE, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4));
	}
	return 0;
}

#endif
