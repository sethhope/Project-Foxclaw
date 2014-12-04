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
#include "Objects/AnimatedMesh.h"

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
int Scene_addAnimatedMesh(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	int mesh = s->addAnimatedMesh(luaL_checkstring(L, 2), core::vector3df(luaL_checknumber(L,3),luaL_checknumber(L,4),luaL_checknumber(L,5)), core::vector3df(luaL_checknumber(L,6),luaL_checknumber(L,7),luaL_checknumber(L,8)),core::vector3df(luaL_checknumber(L,9),luaL_checknumber(L,10),luaL_checknumber(L,11)));
	lua_pushnumber(L,mesh);
	return 1;
}
int Scene_getAnimatedMesh(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	luaW_push<ANIMATEDMESH>(L, s->editAnimatedMesh(luaL_checknumber(L, 2)));
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
int Scene_addCamera(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	s->getCamera()->setType(luaL_checknumber(L, 2));
	s->getCamera()->init();
	return 0;
}
int Scene_getCamera(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	luaW_push<CAMERA>(L, s->getCamera());
	return 1;
}
int Scene_getKey(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	bool k = s->keyDown((irr::EKEY_CODE)luaL_checknumber(L, 2));
	if(k)
	{
		lua_pushnumber(L, 1);
	}else
	{
		lua_pushnumber(L, 0);
	}
	return 1;
}
int Scene_createCharacter(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	IKinematicCharacterController* c = new IKinematicCharacterController(s->getWorld());
	s->setCharacter(c);
	return 0;
}
int Scene_modifyCharacter(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	std::string f = luaL_checkstring(L, 2);
	if(f == "jumpforce")
	{
		s->getCharacter()->setJumpForce(luaL_checknumber(L, 3));
	}
	if(f == "jumpheight")
	{
		s->getCharacter()->setMaxJumpHeight(luaL_checknumber(L, 3));
	}
	if(f == "maxslope")
	{
		s->getCharacter()->setMaxSlope(luaL_checknumber(L, 3));
	}
	if(f == "jump")
	{
		s->getCharacter()->jump();
	}
	if(f == "warp")
	{
		s->getCharacter()->warp(core::vector3df(luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5)));
	}
	return 0;
}
int Scene_getCharacterData(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	std::string f = luaL_checkstring(L, 2);
	if(f == "jump")
	{
		if(s->getCharacter()->canJump())
		{
			lua_pushnumber(L, 1);
		}else
		{
			lua_pushnumber(L, 0);
		}
	}
	if(f == "onGround")
	{
		if(s->getCharacter()->isOnGround())
		{
			lua_pushnumber(L, 1);
		}else
		{
			lua_pushnumber(L, 0);
		}
	}
	if(f == "maxslope")
	{
		lua_pushnumber(L, s->getCharacter()->getMaxSlope());
	}
	if(f == "posX")
	{
		lua_pushnumber(L, s->getCharacter()->getWorldTransform().getTranslation().X);
	}
	if(f == "posY")
	{
		lua_pushnumber(L, s->getCharacter()->getWorldTransform().getTranslation().Y);
	}
	if(f == "posZ")
	{
		lua_pushnumber(L, s->getCharacter()->getWorldTransform().getTranslation().Z);
	}
	return 1;
}
int Scene_moveCharacter(lua_State* L)
{
	SCENE* s = luaW_check<SCENE>(L, 1);
	core::vector3df direction = core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
	core::vector3df view = core::vector3df(luaL_checknumber(L, 5), luaL_checknumber(L, 6), luaL_checknumber(L, 7));
	float multiplier = luaL_checknumber(L, 8);
	core::matrix4 m;
	m.setRotationDegrees(view);
	m.transformVect(direction);
	s->getCharacter()->setPositionIncrementPerSimulatorStep(direction*multiplier);
	return 0;
}

OBJECT* Object_new(lua_State* L)
{
	OBJECT* o = new OBJECT();
	return o;
}
int Object_getCollider(lua_State* L)
{
	OBJECT* o = luaW_check<OBJECT>(L, 1);
	luaW_push<COLLIDER>(L, o->getCollider());
	return 1;
}

COLLIDER* Collider_new(lua_State* L)
{
	return 0;
}

int Collider_setVelocity(lua_State* L)
{
	COLLIDER* c = luaW_check<COLLIDER>(L, 1);
	c->setVelocity(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L, 4)));
	return 0;
}
int Collider_setFriction(lua_State* L)
{
	COLLIDER* c = luaW_check<COLLIDER>(L, 1);
	c->setFriction(luaL_checknumber(L, 2));
	return 0;
}
int Collider_setDamping(lua_State* L)
{
	COLLIDER* c = luaW_check<COLLIDER>(L, 1);
	c->setDamping(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}
int Collider_setMass(lua_State* L)
{
	COLLIDER* c = luaW_check<COLLIDER>(L, 1);
	c->setMass(luaL_checknumber(L, 2));
	return 0;
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
	if(type == "MESH_CONVEXHULL")
	{
		m->addCollider(s->getLog(), COL_MESH_CONVEXHULL, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4), m->getMesh());
	}
	if(type == "MESH_TRIMESH")
	{
		m->addCollider(s->getLog(), COL_MESH_TRIMESH, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4), m->getMesh());
	}
	if(type == "MESH_GIMPACT")
	{
		m->addCollider(s->getLog(), COL_MESH_GIMPACT, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4), m->getMesh());
	}
	return 0;
}

int Mesh_toObject(lua_State* L)
{
	MESH* m = luaW_check<MESH>(L, 1);
	luaW_push<OBJECT>(L, (OBJECT*)m);
	return 1;
}

ANIMATEDMESH* AnimatedMesh_new(lua_State* L)
{
	return 0;
}
int AnimatedMesh_setPos(lua_State* L)
{
	ANIMATEDMESH* m = luaW_check<ANIMATEDMESH>(L, 1);
	m->setPosition(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L, 4)));
	return 0;
}
int AnimatedMesh_setRot(lua_State* L)
{
	ANIMATEDMESH* m = luaW_check<ANIMATEDMESH>(L, 1);
	m->setRotation(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L,4)));
	return 0;
}
int AnimatedMesh_setScale(lua_State* L)
{
	ANIMATEDMESH* m = luaW_check<ANIMATEDMESH>(L, 1);
	m->setScale(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L,4)));
	return 0;
}
int AnimatedMesh_setMaterial(lua_State* L)
{
	ANIMATEDMESH* m = luaW_check<ANIMATEDMESH>(L, 1);
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
int AnimatedMesh_addCollider(lua_State* L)
{
	ANIMATEDMESH* m = luaW_check<ANIMATEDMESH>(L, 1);
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
	if(type == "MESH_CONVEXHULL")
	{
		m->addCollider(s->getLog(), COL_MESH_CONVEXHULL, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4), m->getMesh());
	}
	if(type == "MESH_TRIMESH")
	{
		m->addCollider(s->getLog(), COL_MESH_TRIMESH, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4), m->getMesh());
	}
	if(type == "MESH_GIMPACT")
	{
		m->addCollider(s->getLog(), COL_MESH_GIMPACT, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4), m->getMesh());
	}
	return 0;
}
int AnimatedMesh_setFrameLoop(lua_State* L)
{
	ANIMATEDMESH* m = luaW_check<ANIMATEDMESH>(L, 1);
	m->setFrameLoop(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}
int AnimatedMesh_setSpeed(lua_State* L)
{
	ANIMATEDMESH* m = luaW_check<ANIMATEDMESH>(L, 1);
	m->setSpeed(luaL_checknumber(L, 2));
	return 0;
}
int AnimatedMesh_toObject(lua_State* L)
{
	ANIMATEDMESH* m = luaW_check<ANIMATEDMESH>(L, 1);
	luaW_push<OBJECT>(L, (OBJECT*)m);
	return 1;
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
int Particle_toObject(lua_State* L)
{
	PARTICLE* p = luaW_check<PARTICLE>(L, 1);
	luaW_push<OBJECT>(L, (OBJECT*)p);
	return 1;
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
int Empty_toObject(lua_State* L)
{
	EMPTYOBJECT* e = luaW_check<EMPTYOBJECT>(L, 1);
	luaW_push<OBJECT>(L, (OBJECT*)e);
	return 1;
}
CAMERA* Camera_new(lua_State* L)
{
	return NULL;
}

int Camera_setFOV(lua_State* L)
{
	CAMERA* c = luaW_check<CAMERA>(L, 1);
	c->setFOV(luaL_checknumber(L, 2));
	return 0;
}
int Camera_getFOV(lua_State* L)
{
	CAMERA* c = luaW_check<CAMERA>(L, 1);
	lua_pushnumber(L, c->getFOV());
	return 1;
}
int Camera_setClipping(lua_State* L)
{
	CAMERA* c = luaW_check<CAMERA>(L, 1);
	c->setNearClipping(luaL_checknumber(L, 2));
	c->setFarClipping(luaL_checknumber(L, 3));
	return 0;
}
int Camera_getClipping(lua_State* L)
{
	CAMERA* c = luaW_check<CAMERA>(L, 1);
	lua_pushnumber(L, c->getNearClipping());
	lua_pushnumber(L, c->getFarClipping());
	return 2;
}
int Camera_setTarget(lua_State* L)
{
	CAMERA* c = luaW_check<CAMERA>(L, 1);
	c->setTarget(luaW_check<OBJECT>(L, 2));
	return 0;
}
int Camera_setAspect(lua_State* L)
{
	CAMERA* c = luaW_check<CAMERA>(L, 1);
	c->setAspectRatio(luaL_checknumber(L, 2));
	return 0;
}
int Camera_setOffset(lua_State* L)
{
	CAMERA* c = luaW_check<CAMERA>(L, 1);
	c->setOffset(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
	return 0;
}
#endif
