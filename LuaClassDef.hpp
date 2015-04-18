#ifndef _LUACLASSDEF_H_
#define _LUACLASSDEF_H_

#include "lua.hpp"
#include "luawrapper.hpp"
#include "Irrlicht.h"
#include "Game/ShaderCallback.h"

#include "Objects/Object.h"
#include "Objects/Node.h"
#include "Objects/Mesh.h"
#include "Objects/Light.h"
#include "Objects/Particle.h"
#include "Objects/EmptyObject.h"
#include "Objects/AnimatedMesh.h"
#include "Objects/Terrain.h"
#include "Game/GUI.h"
#include "Misc/Logger.h"
#include "Game/Scene.h"

#include "Misc/PostProcessing/PostProcessManager.h"
using namespace irr;
using namespace FCE;

int System_run(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 2);
    std::string filename = luaL_checkstring(L, 1);
    lua_State* tmp = L;
    if(luaL_loadfile(tmp, filename.c_str()))
    {
        s->getLog()->logData("Failed to load script", filename);
        s->getLog()->debugData(MINOR, "Error", lua_tostring(L, -1));
        return 0;
    }
    if(lua_pcall(tmp, 0, 0xd, 0))
    {
        s->getLog()->logData("Failed to run script", filename);
        s->getLog()->debugData(MINOR, "Error", lua_tostring(L, -1));
        return 0;
    }
    L = tmp;
    return 0;
}

SCENE* Scene_new(lua_State* L)
{
    SCENE* s = new SCENE();
    return s;
}
int Scene_Load(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->load(luaL_checkstring(L, 2));
    return 0;
}
int Scene_Save(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->save(luaL_checkstring(L, 2));
    return 0;
}

int Scene_deltaTime(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    lua_pushnumber(L, s->deltaTime);
    return 1;
}
int Scene_Objects(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    if(s->lastID > 0)
    {
        lua_pushnumber(L, ((s->lastID)-1));
    }
    else
    {
        lua_pushnumber(L, 0);
    }
    return 1;
}
int Scene_addSound(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    int id = s->addSound(luaL_checkstring(L, 2), core::vector3df(luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5)), luaL_checknumber(L, 6));
    lua_pushnumber(L, id);
    return 1;
}
int Scene_RenderEffect(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->postManager->render((E_POSTPROCESS_EFFECT)luaL_checknumber(L, 2));
    return 0;
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
int Scene_getBacklog(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    lua_newtable(L);
    for(int i = 0; i < 10; i++)
    {
        lua_pushnumber(L, i+1);
        lua_pushstring(L, s->getLog()->backlog[i].c_str());
        lua_settable(L, -3);
    }
    lua_pushnumber(L, 11);
    return 2;
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
int Scene_addTerrain(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    int id = s->addTerrain(core::vector3df(luaL_checknumber(L,2),luaL_checknumber(L,3),luaL_checknumber(L,4)), core::vector3df(luaL_checknumber(L,5),luaL_checknumber(L,6),luaL_checknumber(L,7)),core::vector3df(luaL_checknumber(L,8),luaL_checknumber(L,9),luaL_checknumber(L,10)));
    lua_pushnumber(L, id);
    return 1;
}
int Scene_getTerrain(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    luaW_push<TERRAIN>(L, s->getTerrain(luaL_checknumber(L, 2)));
    return 1;
}
int Scene_addLight(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    video::E_LIGHT_TYPE light;
    if(luaL_checknumber(L, 12) == 0)
    {
        light = video::ELT_POINT;
    }
    if(luaL_checknumber(L, 12) == 1)
    {
        light = video::ELT_SPOT;
    }
    if(luaL_checknumber(L, 12) == 2)
    {
        light = video::ELT_DIRECTIONAL;
    }
    int id = s->addLight(core::vector3df(luaL_checknumber(L,2),luaL_checknumber(L,3),luaL_checknumber(L,4)), core::vector3df(luaL_checknumber(L,5),luaL_checknumber(L,6),luaL_checknumber(L,7)),core::vector3df(luaL_checknumber(L,8),luaL_checknumber(L,9),luaL_checknumber(L,10)), luaL_checknumber(L, 11), light);
    lua_pushnumber(L, id);
    return 1;
}
int Scene_getLight(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    luaW_push<LIGHT>(L, s->editLight(luaL_checknumber(L, 2)));
    return 1;
}
int Scene_addCamera(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getCamera()->setType(luaL_checknumber(L, 2));
    s->getCamera()->init();
    s->getCamera()->update();
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
    }
    else
    {
        lua_pushnumber(L, 0);
    }
    return 1;
}
int Scene_getMouseButton(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    bool b = s->MouseDown(luaL_checknumber(L, 2));
    if(b)
    {
        lua_pushnumber(L, 1);
    }
    else
    {
        lua_pushnumber(L, 0);
    }
    return 1;
}
int Scene_getMousePos(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    int x = s->MousePos().X;
    int y = s->MousePos().Y;
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}
int Scene_createCharacter(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    IKinematicCharacterController* c = new IKinematicCharacterController(s->getWorld(), (f32)luaL_checknumber(L, 2), (f32)luaL_checknumber(L, 3), (f32)luaL_checknumber(L, 4));
    c->setUseGhostSweepTest(true);
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
        }
        else
        {
            lua_pushnumber(L, 0);
        }
    }
    if(f == "onGround")
    {
        if(s->getCharacter()->isOnGround())
        {
            lua_pushnumber(L, 1);
        }
        else
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
int Scene_setPhysicsDebug(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    bool d = false;
    if(luaL_checknumber(L, 2) != 0)
    {
        d = true;
    }
    s->setDebug(d);
    return 0;
}
int Scene_getObject(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    luaW_push<OBJECT>(L,s->getObject(luaL_checknumber(L, 2)));
    return 1;
}
int Scene_setSkydome(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->setSkydome(luaL_checkstring(L, 2));
    return 0;
}
int Scene_removeObject(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->removeObject(luaL_checknumber(L, 2));
    return 0;
}
int Scene_setTimeScale(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->setTimeScale(luaL_checknumber(L, 2));
    return 0;
}
int Scene_getTimeScale(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    lua_pushnumber(L, s->getTimeScale());
    return 1;
}

int Scene_addButton(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Creating button");
    GUI* g = new GUI(s->getDevice()->getGUIEnvironment(), s->getLog());
    core::stringw t1 = luaL_checkstring(L, 2);
    core::stringw t2 = luaL_checkstring(L, 3);
    u32 x1 = luaL_checknumber(L, 4);
    u32 y1 = luaL_checknumber(L, 5);
    u32 x2 = luaL_checknumber(L, 6);
    u32 y2 = luaL_checknumber(L, 7);
    u32 parent = luaL_checknumber(L, 8);
    std::string script = luaL_checkstring(L, 9);
    g->initButton(0, t1, t2, rect<s32>(x1, y1, x2, y2));
    SCRIPT* guiScript = new SCRIPT(s->getLog());
    guiScript->init();
    luaW_push<SCENE>(guiScript->L, s);
    lua_setglobal(guiScript->L, "MainScene");
    guiScript->run((std::string)(s->getDevice()->getFileSystem()->getAbsolutePath(script.c_str()).c_str()));
    g->addScript(guiScript);
    u32 but = s->addGui(g, parent);
    lua_pushnumber(L, but);
    return 1;
}
int Scene_addSlider(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Creating slider");
    GUI* g = new GUI(s->getDevice()->getGUIEnvironment(), s->getLog());
    bool h;
    u8 hor = luaL_checknumber(L, 2);
    if(hor == 1)
    {
        h = true;
    }
    else
    {
        h = false;
    }
    u32 x1 = luaL_checknumber(L, 3);
    u32 y1 = luaL_checknumber(L, 4);
    u32 x2 = luaL_checknumber(L, 5);
    u32 y2 = luaL_checknumber(L, 6);
    u32 parent = luaL_checknumber(L, 7);
    std::string script = luaL_checkstring(L, 8);
    g->initScrollBar(0, h, rect<s32>(x1, y1, x2, y2));
    SCRIPT* guiScript = new SCRIPT(s->getLog());
    guiScript->init();
    luaW_push<SCENE>(guiScript->L, s);
    lua_setglobal(guiScript->L, "MainScene");
    guiScript->run((std::string)(s->getDevice()->getFileSystem()->getAbsolutePath(script.c_str()).c_str()));
    g->addScript(guiScript);
    u32 but = s->addGui(g, parent);
    lua_pushnumber(L, but);
    return 1;
}
int Scene_addEditBox(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Creating editbox");
    GUI* g = new GUI(s->getDevice()->getGUIEnvironment(), s->getLog());
    core::stringw text = luaL_checkstring(L, 2);
    u32 x1 = luaL_checknumber(L, 3);
    u32 y1 = luaL_checknumber(L, 4);
    u32 x2 = luaL_checknumber(L, 5);
    u32 y2 = luaL_checknumber(L, 6);
    u8 border = luaL_checknumber(L, 7);
    std::string script = luaL_checkstring(L, 9);
    bool useBorder;
    if(border == 1)
    {
        useBorder = true;
    }
    else
    {
        useBorder = false;
    }
    u32 parent = luaL_checknumber(L, 8);
    g->initEditBox(0, text, rect<s32>(x1, y1, x2, y2), useBorder);
    SCRIPT* guiScript = new SCRIPT(s->getLog());
    guiScript->init();
    luaW_push<SCENE>(guiScript->L, s);
    lua_setglobal(guiScript->L, "MainScene");
    guiScript->run((std::string)(s->getDevice()->getFileSystem()->getAbsolutePath(script.c_str()).c_str()));
    g->addScript(guiScript);
    u32 ed = s->addGui(g, parent);
    lua_pushnumber(L, ed);
    return 1;
}

int Scene_addListBox(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Creating Listbox");
    GUI* g = new GUI(s->getDevice()->getGUIEnvironment(), s->getLog());
    u32 x1 = luaL_checknumber(L, 2);
    u32 y1 = luaL_checknumber(L, 3);
    u32 x2 = luaL_checknumber(L, 4);
    u32 y2 = luaL_checknumber(L, 5);
    u32 parent = luaL_checknumber(L, 6);
    std::string script = luaL_checkstring(L, 7);
    g->initListBox(0, rect<s32>(x1, y1, x2, y2));
    SCRIPT* guiScript = new SCRIPT(s->getLog());
    guiScript->init();
    luaW_push<SCENE>(guiScript->L, s);
    lua_setglobal(guiScript->L, "MainScene");
    guiScript->run((std::string)(s->getDevice()->getFileSystem()->getAbsolutePath(script.c_str()).c_str()));
    g->addScript(guiScript);
    u32 list = s->addGui(g, parent);
    lua_pushnumber(L, list);
    return 1;
}

int Scene_addImage(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Creating image");
    GUI* g = new GUI(s->getDevice()->getGUIEnvironment(), s->getLog());
    u32 x1 = luaL_checknumber(L, 2);
    u32 y1 = luaL_checknumber(L, 3);
    u32 x2 = luaL_checknumber(L, 4);
    u32 y2 = luaL_checknumber(L, 5);
    std::string filename = luaL_checkstring(L, 6);
    u32 parent = luaL_checknumber(L, 7);
    g->initImage(0, s->getDevice()->getVideoDriver()->getTexture(filename.c_str()), core::rect<s32>(x1, y1, x2, y2));
    u32 list = s->addGui(g, parent);
    return 0;
}
int Scene_addFileDiag(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Creating File Dialog");
    GUI* g = new GUI(s->getDevice()->getGUIEnvironment(), s->getLog());
    std::string title = luaL_checkstring(L, 2);
    u32 parent = luaL_checknumber(L, 3);
    std::string script = luaL_checkstring(L, 4);
    core::stringw wtitle = core::stringw(title.c_str());
    g->initFileDiag(0, wtitle);
    SCRIPT* guiScript = new SCRIPT(s->getLog());
    guiScript->init();
    luaW_push<SCENE>(guiScript->L, s);
    lua_setglobal(guiScript->L, "MainScene");
    guiScript->run((std::string)(s->getDevice()->getFileSystem()->getAbsolutePath(script.c_str()).c_str()));
    g->addScript(guiScript);
    u32 list = s->addGui(g, parent);
    return 0;
}
int Scene_addListItem(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Adding list item");
    core::stringw itemName = luaL_checkstring(L, 2);
    u32 itemID = luaL_checknumber(L, 3);
    u32 retID = 0;
    bool found = false;
    for(std::vector<GUI*>::iterator it = s->callers.begin(); it < s->callers.end(); it++)
    {
        if((*it)->guiCaller == itemID)
        {
            found = true;
            retID = (*it)->addItemToList(itemName);
        }
    }
    if(!found)
    {
        for(std::vector<GUI*>::iterator it = s->guiObjects.begin(); it < s->guiObjects.end(); it++)
        {
            if((*it)->guiCaller == itemID)
            {
                found = true;
                retID = (*it)->addItemToList(itemName);
            }
        }
    }
    if(!found)
    {
        s->getLog()->debugData(MAJOR, "Item not found");
        return 0;
    }
    lua_pushnumber(L, retID);
    return 1;
}

int Scene_clearGUI(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Clearing GUI");
    for(std::vector<GUI*>::iterator it = s->callers.begin(); it < s->callers.end(); it++)
    {
        (*it)->element->setVisible(false);
    }
    return 0;
}

int Scene_setGUIColor(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    for(u32 i = 0; i < gui::EGDC_COUNT; i++)
    {
        s->getDevice()->getGUIEnvironment()->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, video::SColor(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5)));
    }
    return 0;
}
int Scene_getGUIColor(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    video::SColorf c = s->getDevice()->getGUIEnvironment()->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)0);
    lua_pushnumber(L, c.r);
    lua_pushnumber(L, c.g);
    lua_pushnumber(L, c.b);
    lua_pushnumber(L, c.a);
    return 4;
}
int Scene_setGUIFont(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getDevice()->getGUIEnvironment()->getSkin()->setFont(s->getDevice()->getGUIEnvironment()->getFont(s->getDevice()->getFileSystem()->getAbsolutePath(luaL_checkstring(L, 2)).c_str()));
    return 0;
}
int Scene_removeListItem(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Removing list item");
    u32 itemID = luaL_checknumber(L, 2);
    u32 listID = luaL_checknumber(L, 3);
    bool found = false;
    for(std::vector<GUI*>::iterator it = s->callers.begin(); it < s->callers.end(); it++)
    {
        if((*it)->guiCaller == itemID)
        {
            found = true;
            (*it)->removeItemFromList(listID);
        }
    }
    if(!found)
    {
        for(std::vector<GUI*>::iterator it = s->guiObjects.begin(); it < s->guiObjects.end(); it++)
        {
            if((*it)->guiCaller == itemID)
            {
                found = true;
                (*it)->removeItemFromList(listID);
            }
        }
    }
    if(!found)
    {
        s->getLog()->debugData(MAJOR, "Item not found");
        return 0;
    }
    return 0;
}

int Scene_addWindow(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Creating window");
    GUI* g = new GUI(s->getDevice()->getGUIEnvironment(), s->getLog());
    core::stringw title = luaL_checkstring(L, 2);
    u32 x1 = luaL_checknumber(L, 3);
    u32 y1 = luaL_checknumber(L, 4);
    u32 x2 = luaL_checknumber(L, 5);
    u32 y2 = luaL_checknumber(L, 6);
    u32 parent = luaL_checknumber(L, 7);
    std::string script = luaL_checkstring(L, 8);
    g->initWindow(0, title, rect<s32>(x1, y1, x2, y2), true);
    SCRIPT* guiScript = new SCRIPT(s->getLog());
    guiScript->init();
    luaW_push<SCENE>(guiScript->L, s);
    lua_setglobal(guiScript->L, "MainScene");
    guiScript->run((std::string)(s->getDevice()->getFileSystem()->getAbsolutePath(script.c_str()).c_str()));
    g->addScript(guiScript);
    u32 wind = s->addGui(g, parent);
    lua_pushnumber(L, wind);
    return 1;
}
int Scene_addText(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getLog()->debugData(MAJOR, "Creating text");
    GUI* g = new GUI(s->getDevice()->getGUIEnvironment(), s->getLog());
    core::stringw text = luaL_checkstring(L, 2);
    u32 x1 = luaL_checknumber(L, 3);
    u32 y1 = luaL_checknumber(L, 4);
    u32 x2 = luaL_checknumber(L, 5);
    u32 y2 = luaL_checknumber(L, 6);
    u32 parent = luaL_checknumber(L, 7);
    g->initText(0, text, false, true, rect<s32>(x1, y1, x2, y2));
    u32 wind = s->addGui(g, parent);
    lua_pushnumber(L, wind);
    return 1;
}
int Scene_setMetaData(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->setMetaData(luaL_checkstring(L, 2), luaL_checknumber(L, 3));
    return 0;
}

int Scene_getMetaData(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    lua_pushnumber(L, s->getMetaData(luaL_checkstring(L, 2)));
    return 1;
}

int Scene_getMouseControl(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getCamera()->getCamNode()->setInputReceiverEnabled(false);
    return 0;
}

int Scene_giveMouseControl(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getCamera()->getCamNode()->setInputReceiverEnabled(true);
    return 0;
}

int Scene_setGravity(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getWorld()->setGravity(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L, 4)));
    return 0;
}
int Scene_getGUIObject(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    std::vector<GUI*> c = s->callers;
    for(std::vector<GUI*>::iterator it = c.begin(); it < c.end(); it++)
    {
        if((*it)->guiCaller == luaL_checknumber(L, 2))
        {
            luaW_push<GUI>(L, (*it));
            return 1;
        }
    }
    s->getLog()->debugData(MINOR, "Failed to get GUI Object", luaL_checknumber(L, 2));
    return 0;
}
int Scene_setFog(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    video::SColor c = video::SColor(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
    s->getDevice()->getVideoDriver()->setFog(c, video::EFT_FOG_LINEAR, luaL_checknumber(L, 6), luaL_checknumber(L, 7));
    return 0;
}
int Scene_addZip(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getDevice()->getFileSystem()->addFileArchive(s->getDevice()->getFileSystem()->getAbsolutePath(luaL_checkstring(L, 2)).c_str(), true, true, io::EFAT_ZIP);
    return 0;
}
int Scene_removeZip(lua_State* L)
{
    SCENE* s = luaW_check<SCENE>(L, 1);
    s->getDevice()->getFileSystem()->removeFileArchive(s->getDevice()->getFileSystem()->getAbsolutePath(luaL_checkstring(L, 2)).c_str());
    return 0;
}
OBJECT* Object_new(lua_State* L)
{
    OBJECT* o = new OBJECT();
    return o;
}
int Object_setMetaData(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    o->setMetaData(luaL_checkstring(L, 2), luaL_checknumber(L, 3));
    return 0;
}
int Object_getMetaData(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    lua_pushnumber(L, o->getMetaData(luaL_checkstring(L, 2)));
    return 1;
}
int Object_getCollider(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    luaW_push<COLLIDER>(L, o->getCollider());
    return 1;
}
int Object_getPosition(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    lua_pushnumber(L, o->getPosition().X);
    lua_pushnumber(L, o->getPosition().Y);
    lua_pushnumber(L, o->getPosition().Z);
    return 3;
}
int Object_getRotation(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    lua_pushnumber(L, o->getRotation().X);
    lua_pushnumber(L, o->getRotation().Y);
    lua_pushnumber(L, o->getRotation().Z);
    return 3;
}
int Object_getScale(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    lua_pushnumber(L, o->getScale().X);
    lua_pushnumber(L, o->getScale().Y);
    lua_pushnumber(L, o->getScale().Z);
    return 3;
}
int Object_setPosition(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    o->setPosition(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
    return 0;
}

int Object_setRotation(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    o->setRotation(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
    return 0;
}
int Object_setScale(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    o->setScale(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
    return 0;
}
int Object_getName(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    lua_pushstring(L, o->getName().c_str());
    return 1;
}
int Object_setName(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    o->setName(luaL_checkstring(L, 2));
    return 0;
}
int Object_getType(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    lua_pushstring(L, o->getOType().c_str());
    return 1;
}
int Object_getID(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    lua_pushnumber(L, o->getID());
    return 1;
}
int Object_addScript(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    SCRIPT* s = new SCRIPT(luaW_check<SCENE>(L, 2)->getLog());
    s->init();
    s->attachTo((NODE*)o);
    s->run((std::string)(luaW_check<SCENE>(L, 2)->getDevice()->getFileSystem()->getAbsolutePath(luaL_checkstring(L, 3)).c_str()));
    luaW_push<SCENE>(s->L, luaW_check<SCENE>(L, 2));
    lua_setglobal(s->L, "MainScene");
    luaW_push<OBJECT>(s->L, o);
    lua_setglobal(s->L, "ParentObject");
    s->runInit();
    return 0;
}
int Object_setMaterial(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    std::string type = luaL_checkstring(L, 2);
    if(o->getIrrNode()->getType() == scene::ESNT_CUBE || o->getIrrNode()->getType() == scene::ESNT_MESH || o->getIrrNode()->getType() == scene::ESNT_ANIMATED_MESH || o->getIrrNode()->getType() == scene::ESNT_BILLBOARD || o->getIrrNode()->getType() == scene::ESNT_WATER_SURFACE || o->getIrrNode()->getType() == scene::ESNT_SPHERE)
    {
        o->hasShader=false;
        if(type == "solid")
        {
            o->getIrrNode()->setMaterialType(video::EMT_SOLID);
        }
        if(type == "solid_2_layer")
        {
            o->getIrrNode()->setMaterialType(video::EMT_SOLID_2_LAYER);
        }
        if(type == "lightmap")
        {
            o->getIrrNode()->setMaterialType(video::EMT_LIGHTMAP);
        }
        if(type == "lit_lightmap")
        {
            o->getIrrNode()->setMaterialType(video::EMT_LIGHTMAP_LIGHTING);
        }
        if(type == "normalmap_solid")
        {
            o->getIrrNode()->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
        }
        if(type == "parallax_solid")
        {
            o->getIrrNode()->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
        }
        if(type == "sphere_map")
        {
            o->getIrrNode()->setMaterialType(video::EMT_SPHERE_MAP);
        }
        if(type == "detail_map")
        {
            o->getIrrNode()->setMaterialType(video::EMT_DETAIL_MAP);
        }
        if(type == "solid_spheremap")
        {
            o->getIrrNode()->setMaterialType(video::EMT_REFLECTION_2_LAYER);
        }
        if(type == "transparent_alpha")
        {
            o->getIrrNode()->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        }
        if(type == "transparent_alpha_fast")
        {
            o->getIrrNode()->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
        }
        if(type == "transparent_add")
        {
            o->getIrrNode()->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        }
    }
    return 0;
}
int Object_useShader(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    SCENE* s = luaW_check<SCENE>(L, 2);
    o->useShader(s->getDevice(), s->getLog(), s->getDevice()->getFileSystem()->getAbsolutePath(luaL_checkstring(L, 3)).c_str());
    return 0;
}
int Object_setMaterialFlag(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    std::string type = luaL_checkstring(L, 2);
    if(o->getIrrNode()->getType() == scene::ESNT_CUBE || o->getIrrNode()->getType() == scene::ESNT_MESH || o->getIrrNode()->getType() == scene::ESNT_ANIMATED_MESH || o->getIrrNode()->getType() == scene::ESNT_BILLBOARD || o->getIrrNode()->getType() == scene::ESNT_WATER_SURFACE || o->getIrrNode()->getType() == scene::ESNT_SPHERE)
    {
        bool value;
        if(luaL_checknumber(L, 3)==0)
        {
            value = false;
        }
        else
        {
            value = true;
        }
        if(type == "wireframe")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_WIREFRAME, value);
        }
        if(type == "pointcloud")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_POINTCLOUD, value);
        }
        if(type == "gouraud_shading")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_GOURAUD_SHADING, value);
        }
        if(type == "lighting")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_LIGHTING, value);
        }
        if(type == "zbuffer")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_ZBUFFER, value);
        }
        if(type == "zwrite")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_ZWRITE_ENABLE, value);
        }
        if(type == "backface_culling")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_BACK_FACE_CULLING, value);
        }
        if(type == "frontface_culling")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, value);
        }
        if(type == "bilinear")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_BILINEAR_FILTER, value);
        }
        if(type == "trilinear")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_TRILINEAR_FILTER, value);
        }
        if(type == "anisotropic")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, value);
        }
        if(type == "fog")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_FOG_ENABLE, value);
        }
        if(type == "normalize_normals")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, value);
        }
        if(type == "texturewrap")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_TEXTURE_WRAP, value);
        }
        if(type == "antialias")
        {
            o->getIrrNode()->setMaterialFlag(video::EMF_ANTI_ALIASING, value);
        }
        if(type == "Material_parameter")
        {
            o->getIrrNode()->getMaterial(0).MaterialTypeParam = value;
        }
        if(type == "castshadow")
        {
            if(o->getOType() == "ANIMATEDMESH" || o->getOType() == "MESH")
                ((scene::IAnimatedMeshSceneNode*)o->getIrrNode())->addShadowVolumeSceneNode();
        }
    }
    return 0;
}
int Object_setTexture(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    SCENE* s = luaW_check<SCENE>(L, 2);
    if(o->getIrrNode()->getType() == scene::ESNT_CUBE || o->getIrrNode()->getType() == scene::ESNT_MESH || o->getIrrNode()->getType() == scene::ESNT_ANIMATED_MESH || o->getIrrNode()->getType() == scene::ESNT_BILLBOARD || o->getIrrNode()->getType() == scene::ESNT_WATER_SURFACE || o->getIrrNode()->getType() == scene::ESNT_SPHERE)
    {
        o->getIrrNode()->setMaterialTexture(luaL_checknumber(L, 3), s->getDevice()->getSceneManager()->getVideoDriver()->getTexture(luaL_checkstring(L, 4)));
    }
    return 0;
}
int Object_attachTo(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    OBJECT* o_ = luaW_check<OBJECT>(L, 2);
    o->attachTo((NODE*)o_);
    return 0;
}
int Object_getUpVector(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    core::vector3df uv = o->getUpVector();
    lua_pushnumber(L, uv.X);
    lua_pushnumber(L, uv.Y);
    lua_pushnumber(L, uv.Z);
    return 3;
}
int Object_getChildren(lua_State* L)
{
    OBJECT* o = luaW_check<OBJECT>(L, 1);
    std::vector<NODE*> c = o->children;
    u32 key = 1;
    lua_newtable(L);
    for(std::vector<NODE*>::iterator it = c.begin(); it < c.end(); it++)
    {
        lua_pushnumber(L, key);
        luaW_push<OBJECT>(L, (OBJECT*)(*it));
        lua_settable(L, -3);
        key++;
    }
    lua_pushnumber(L, key-1);
    return 2;
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
int Collider_setAVelocity(lua_State* L)
{
    COLLIDER* c = luaW_check<COLLIDER>(L, 1);
    c->setAVelocity(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L, 4)));
    return 0;
}
int Collider_addCentralForce(lua_State* L)
{
    COLLIDER* c = luaW_check<COLLIDER>(L, 1);
    c->addCentralForce(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L, 4)));
    return 0;
}
int Collider_addForce(lua_State* L)
{
    COLLIDER* c = luaW_check<COLLIDER>(L, 1);
    c->addForce(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L, 4)), core::vector3df(luaL_checknumber(L, 5), luaL_checknumber(L,6), luaL_checknumber(L, 7)));
    return 0;
}
int Collider_addTorque(lua_State* L)
{
    COLLIDER* c = luaW_check<COLLIDER>(L, 1);
    c->addTorque(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L,3), luaL_checknumber(L, 4)));
    return 0;
}
int Collider_setState(lua_State* L)
{
    COLLIDER* c = luaW_check<COLLIDER>(L, 1);
    std::string state = luaL_checkstring(L, 2);
    if(state == "ACTIVE")
    {
        c->body->setActivationState(EAS_ACTIVE);
    }
    if(state == "NO_DEACTIVATION")
    {
        c->body->setActivationState(EAS_DISABLE_DEACTIVATION);
    }
    if(state == "SLEEPING")
    {
        c->body->setActivationState(EAS_SLEEPING);
    }
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
int Collider_checkCollisionWith(lua_State* L)
{
    COLLIDER* c = luaW_check<COLLIDER>(L, 1);
    OBJECT* o = luaW_check<OBJECT>(L, 2);
    bool col = c->body->checkCollideWith(o->getCollider()->body);
    if(col == true)
    {
        lua_pushnumber(L, 1);
    }
    else
    {
        lua_pushnumber(L, 0);
    }
    return 1;
}
int Collider_setLocal(lua_State* L)
{
    COLLIDER* c = luaW_check<COLLIDER>(L, 1);
    int l = luaL_checknumber(L, 2);
    if(l == 0)
    {
        c->setLocal(false);
    }
    else
    {
        c->setLocal(true);
    }
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
int AnimatedMesh_getFrame(lua_State* L)
{
    ANIMATEDMESH* m = luaW_check<ANIMATEDMESH>(L, 1);
    u32 frame = m->getNode()->getFrameNr();
    lua_pushnumber(L, frame);
    return 1;
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
    }
    else
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
    }
    else
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
    if(type == "CAPSULE")
    {
        m->addCollider(s->getLog(), COL_CAPSULE, s->getDevice()->getSceneManager(), s->getWorld(), luaL_checknumber(L, 4));
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

int Camera_getType(lua_State* L)
{
    CAMERA* c = luaW_check<CAMERA>(L, 1);
    lua_pushnumber(L, c->getType());
    return 1;
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
int Camera_setPosition(lua_State* L)
{
    CAMERA* c = luaW_check<CAMERA>(L, 1);
    c->setPosition(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
    return 0;
}
int Camera_getPosition(lua_State* L)
{
    CAMERA* c = luaW_check<CAMERA>(L, 1);
    core::vector3df pos = c->getCamNode()->getAbsolutePosition();
    lua_pushnumber(L, pos.X);
    lua_pushnumber(L, pos.Y);
    lua_pushnumber(L, pos.Z);
    return 3;
}

int Camera_setRotation(lua_State* L)
{
    CAMERA* c = luaW_check<CAMERA>(L, 1);
    c->getCamNode()->setRotation(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
    return 0;
}
int Camera_getRotation(lua_State* L)
{
    CAMERA* c = luaW_check<CAMERA>(L, 1);
    core::vector3df rot = c->getCamNode()->getRotation();
    lua_pushnumber(L, rot.X);
    lua_pushnumber(L, rot.Y);
    lua_pushnumber(L, rot.Z);
    return 3;
}

int Camera_attachTo(lua_State* L)
{
    CAMERA* c = luaW_check<CAMERA>(L, 1);
    OBJECT* o = luaW_check<OBJECT>(L, 2);

    c->attachTo((NODE*)o);
    return 0;
}

int Camera_setUpVector(lua_State* L)
{
    CAMERA* c = luaW_check<CAMERA>(L, 1);
    c->setUpVector(core::vector3df(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
    return 0;
}
LIGHT* Light_new(lua_State* L)
{
    LIGHT* l;
    return l;
    //NEVER USE
}

int Light_setColor(lua_State* L)
{
    LIGHT* l = luaW_check<LIGHT>(L, 1);
    l->setColor(video::SColorf(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
    return 0;
}
int Light_getColor(lua_State* L)
{
    LIGHT* l = luaW_check<LIGHT>(L, 1);
    video::SColorf c = l->getColor();
    lua_pushnumber(L, c.r);
    lua_pushnumber(L, c.g);
    lua_pushnumber(L, c.b);
    return 3;
}
int Light_setDropoff(lua_State* L)
{
    LIGHT* l = luaW_check<LIGHT>(L, 1);
    l->setDropoff(luaL_checknumber(L, 2));
    return 0;
}
int Light_setType(lua_State* L)
{
    LIGHT* l = luaW_check<LIGHT>(L, 1);
    video::E_LIGHT_TYPE light;
    if(luaL_checknumber(L, 2) == 0)
    {
        light = video::ELT_POINT;
    }
    if(luaL_checknumber(L, 2) == 1)
    {
        light = video::ELT_SPOT;
    }
    if(luaL_checknumber(L, 2) == 2)
    {
        light = video::ELT_DIRECTIONAL;
    }
    l->setType(light);
    return 0;
}

TERRAIN* Terrain_new(lua_State* L)
{
    return 0;
}
int Terrain_newEmpty(lua_State* L)
{
    TERRAIN* t = luaW_check<TERRAIN>(L, 1);
    t->empty(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
    return 0;
}
int Terrain_load(lua_State* L)
{
    TERRAIN* t = luaW_check<TERRAIN>(L, 1);
    t->load(luaL_checkstring(L, 2));
    return 0;
}
int Terrain_getHeight(lua_State* L)
{
    TERRAIN* t = luaW_check<TERRAIN>(L, 1);
    lua_pushnumber(L, t->getHeightmap()->getData(luaL_checknumber(L,2), luaL_checknumber(L,3)));
    return 1;
}
int Terrain_setHeight(lua_State* L)
{
    TERRAIN* t = luaW_check<TERRAIN>(L, 1);
    t->setHeight(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
    return 0;
}
int Terrain_getSizeX(lua_State* L)
{
    TERRAIN* t = luaW_check<TERRAIN>(L, 1);
    lua_pushnumber(L, t->getHeightmap()->getWidth());
    return 1;
}
int Terrain_getSizeY(lua_State* L)
{
    TERRAIN* t = luaW_check<TERRAIN>(L, 1);
    lua_pushnumber(L, t->getHeightmap()->getHeight());
    return 1;
}

int Terrain_addCollider(lua_State* L)
{
    TERRAIN* m = luaW_check<TERRAIN>(L, 1);
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

GUI* GUI_new(lua_State* L)
{
    return 0;
}
int GUI_setText(lua_State* L)
{
    GUI* g = luaW_check<GUI>(L, 1);
    g->element->setText(core::stringw(luaL_checkstring(L, 2)).c_str());
    return 0;
}
int GUI_remove(lua_State* L)
{
    GUI* g = luaW_check<GUI>(L, 1);
    g->element->setVisible(false);
    return 0;
}
int GUI_getCaller(lua_State* L)
{
    GUI* g = luaW_check<GUI>(L, 1);
    lua_pushnumber(L, g->guiCaller);
    return 1;
}
int GUI_setTitle(lua_State* L)
{
    GUI* g = luaW_check<GUI>(L, 1);
    g->element->setName(core::stringw(luaL_checkstring(L, 2)).c_str());
    return 0;
}
#endif
