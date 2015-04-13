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
        {"load", Scene_Load},
        {"save", Scene_Save},
        {"Objects", Scene_Objects},
        {"RenderEffect", Scene_RenderEffect},
        {"deltaTime", Scene_deltaTime},
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
        {"addLight", Scene_addLight},
        {"getLight", Scene_getLight},
        {"addTerrain", Scene_addTerrain},
        {"getTerrain", Scene_getTerrain},
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
        {"setTimeScale", Scene_setTimeScale},
        {"getTimeScale", Scene_getTimeScale},
        {"getMouseButton", Scene_getMouseButton},
        {"getMousePosition", Scene_getMousePos},
        {"addWindow", Scene_addWindow},
        {"addSlider", Scene_addSlider},
        {"addButton", Scene_addButton},
        {"addEditBox", Scene_addEditBox},
        {"addText", Scene_addText},
        {"addListBox", Scene_addListBox},
        {"addListItem", Scene_addListItem},
        {"addImage", Scene_addImage},
        {"addFileDiag", Scene_addFileDiag},
        {"removeListItem", Scene_removeListItem},
        {"setMetaData", Scene_setMetaData},
        {"getMetaData", Scene_getMetaData},
        {"clearGUI", Scene_clearGUI},
        {"setGUIColor", Scene_setGUIColor},
        {"getGUIColor", Scene_getGUIColor},
        {"setGUIFont", Scene_setGUIFont},
        {"getMouseControl", Scene_getMouseControl},
        {"giveMouseControl", Scene_giveMouseControl},
        {"setGravity", Scene_setGravity},
        {"getGUIObject", Scene_getGUIObject},
        {"setFog", Scene_setFog},
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
        {"getFrame", AnimatedMesh_getFrame},
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
        {"getType", Camera_getType},
        {"setFOV", Camera_setFOV},
        {"getFOV", Camera_getFOV},
        {"setClipping", Camera_setClipping},
        {"getClipping", Camera_getClipping},
        {"setTarget", Camera_setTarget},
        {"setAspect", Camera_setAspect},
        {"setOffset", Camera_setOffset},
        {"setPosition", Camera_setPosition},
        {"getPosition", Camera_getPosition},
        {"setRotation", Camera_setRotation},
        {"getRotation", Camera_getRotation},
        {"attachTo", Camera_attachTo},
        {"setUpVector", Camera_setUpVector},
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
    static luaL_Reg Light_metatable[] =
    {
        {"setColor", Light_setColor},
        {"setDropoff", Light_setDropoff},
        {"setType", Light_setType},
        {"getColor", Light_getColor},
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
        {"getType", Object_getType},
        {"getID", Object_getID},
        {"addScript", Object_addScript},
        {"setMaterial", Object_setMaterial},
        {"setMaterialFlag", Object_setMaterialFlag},
        {"setMaterialTexture", Object_setTexture},
        {"useShader", Object_useShader},
        {"attachTo", Object_attachTo},
        {"setMetaData", Object_setMetaData},
        {"getMetaData", Object_getMetaData},
        {"getUpVector", Object_getUpVector},
        {NULL, NULL}
    };
    static luaL_Reg Collider_table[] =
    {
        {"setVelocity", Collider_setVelocity},
        {"setAVelocity", Collider_setAVelocity},
        {"setFriction", Collider_setFriction},
        {"setDamping", Collider_setDamping},
        {"setMass", Collider_setMass},
        {"checkCollisionWith", Collider_checkCollisionWith},
        {"setLocal", Collider_setLocal},
        {"addCentralForce", Collider_addCentralForce},
        {"addForce", Collider_addForce},
        {"addTorque", Collider_addTorque},
        {"setState", Collider_setState},
        {NULL, NULL}
    };
    static luaL_Reg Terrain_table[] =
    {
        {"empty", Terrain_newEmpty},
        {"load", Terrain_load},
        {"getHeight", Terrain_getHeight},
        {"setHeight", Terrain_setHeight},
        {"getSizeX", Terrain_getSizeX},
        {"getSizeY", Terrain_getSizeY},
        {"addCollider", Terrain_addCollider},
        {NULL, NULL}
    };
    static luaL_Reg GUI_table[] =
    {
        {"setText", GUI_setText},
        {"setTitle", GUI_setTitle},
        {"removeElement", GUI_remove},
        {NULL, NULL}
    };
    lua_pushcfunction(L, System_run);
    lua_setglobal(L, "System_run");
    luaW_register<GUI>(L, "GUI", Empty_table, GUI_table, GUI_new);
    luaW_register<TERRAIN>(L, "TERRAIN", Empty_table, Terrain_table, Terrain_new);
    luaW_register<LIGHT>(L, "LIGHT", Empty_table, Light_metatable, Light_new);
    luaW_register<COLLIDER>(L, "COLLIDER", Empty_table, Collider_table, Collider_new);
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
std::string SCRIPT::getOType()
{
    return "SCRIPT";
}
