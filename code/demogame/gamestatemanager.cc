﻿//------------------------------------------------------------------------------
//  gamestatemanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "gamestatemanager.h"
#include "models/modelcontext.h"
#include "graphics/graphicsentity.h"
#include "visibility/visibilitycontext.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "basegamefeature/components/transform.h"
#include "physicsfeature/components/physics.h"
#include "physicsfeature/managers/physicsmanager.h"
#include "physics/actorcontext.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "dynui/im3d/im3dcontext.h"
#include "imgui.h"
#include "util/random.h"
#include "characters/charactercontext.h"
#include "models/nodes/shaderstatenode.h"
#include "dynui/im3d/im3d.h"
#include "lighting/lightcontext.h"
#include "decals/decalcontext.h"
#include "resources/resourceserver.h"
#include "terrain/terraincontext.h"
#include "coregraphics/legacy/nvx2streamreader.h"
#include "coregraphics/primitivegroup.h"
#include "properties/movement.h"

#include "graphicsfeature/managers/graphicsmanager.h"
#include "game/gameserver.h"
#include "game/api.h"

#ifdef __WIN32__
#include <shellapi.h>
#elif __LINUX__

#endif

namespace Demo
{

__ImplementSingleton(GameStateManager)

//------------------------------------------------------------------------------
/**
*/
Game::ManagerAPI
GameStateManager::Create()
{
    n_assert(!GameStateManager::HasInstance());
    Singleton = new GameStateManager;

    Game::ManagerAPI api;
    api.OnActivate = &GameStateManager::OnActivate;
    api.OnBeginFrame = &GameStateManager::OnBeginFrame;
    api.OnFrame = &GameStateManager::OnFrame;
    return api;
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::Destroy()
{
    n_assert(GameStateManager::HasInstance());
    delete Singleton;
}

//------------------------------------------------------------------------------
/**
*/
GameStateManager::GameStateManager()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GameStateManager::~GameStateManager()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnActivate()
{
    float xOffset = -200;
    float zOffset = -200;
    float yOffset = 0;

    auto dood = Graphics::CreateEntity();
    Graphics::RegisterEntity<Models::ModelContext, Visibility::ObservableContext>(dood);
    Models::ModelContext::Setup(dood, "mdl:system/placeholder.n3", "ExampleScene", [dood]()
    {
        Visibility::ObservableContext::Setup(dood, Visibility::VisibilityEntityType::Model);
    });
    Models::ModelContext::SetTransform(dood, Math::translation(Math::vec3(0, 0, 5)));

    {
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("StaticGroundPlane/dev_ground_plane"_atm);
        Game::Entity entity = Game::CreateEntity(Game::GetWorld(WORLD_DEFAULT), info);
        Game::SetComponent(Game::GetWorld(WORLD_DEFAULT), entity, Game::GetComponentId("WorldTransform"_atm), Math::translation({ 2, yOffset, 0 }));

    }
    //{
    //    Game::EntityCreateInfo info;
    //    info.immediate = true;
    //    info.templateId = Game::GetTemplateId("StaticEnvironment/knob_metallic"_atm);
    //    Game::Entity entity = Game::CreateEntity(info);
    //    Game::SetComponent(entity, Game::GetComponentId("WorldTransform"_atm), Math::scaling(5, 5, 5) * Math::translation({ 5, 0, 5 }));
    //}
    //{
    //    Game::EntityCreateInfo info;
    //    info.immediate = true;
    //    info.templateId = Game::GetTemplateId("StaticEnvironment/knob_plastic"_atm);
    //    Game::Entity entity = Game::CreateEntity(info);
    //    Game::SetComponent(entity, Game::GetComponentId("WorldTransform"_atm), Math::scaling(5, 5, 5) * Math::translation({ 5, 0, 7 }));
    //}
    //{
    //    Game::EntityCreateInfo info;
    //    info.immediate = true;
    //    info.templateId = Game::GetTemplateId("StaticEnvironment/knob_reflective"_atm);
    //    Game::Entity entity = Game::CreateEntity(info);
    //    Game::SetComponent(entity, Game::GetComponentId("WorldTransform"_atm), Math::scaling(5, 5, 5) * Math::translation({ 5, 0, 9 }));
    //}
    //
    auto gameWorld = Game::GetWorld(WORLD_DEFAULT);
    for (int i = 0; i < 00; i++)
    {
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("PhysicsEntity/placeholder_box"_atm);
        Game::Entity entity = Game::CreateEntity(gameWorld, info);
        Game::SetComponent(gameWorld, entity, Game::GetComponentId("WorldTransform"_atm), Math::rotationyawpitchroll(0.01f, 0.01f, 0.01f) * Math::translation({ (Math::rand() - 0.5f) * 20.0f, yOffset + 5.0f + ((float)i * 1.01f), (Math::rand() - 0.5f) * 20.0f}));
    }
   
    //

    //Graphics::GraphicsEntityId terrain = Graphics::CreateEntity();
    //Terrain::TerrainContext::RegisterEntity(terrain);
    //
    //Terrain::TerrainContext::SetupTerrain(terrain,
    //    "tex:terrain/dirt_aerial_02_disp_8k_PNG_BC4_1.dds",
    //    "tex:system/black.dds",
    //    "tex:terrain/dirt_aerial_02_diff_2k.dds");
    //
    //Terrain::BiomeSetupSettings biomeSettings =
    //{
    //    0.5f, 900.0f, 64.0f
    //};
    //Terrain::TerrainContext::CreateBiome(biomeSettings,
    //    {
    //        "tex:terrain/brown_mud_leaves_01_diff_2k_PNG_BC7_1.dds",
    //        "tex:terrain/brown_mud_leaves_01_nor_2k_PNG_BC5_1.dds",
    //        "tex:terrain/brown_mud_leaves_01_material_2k_PNG_BC7_1.dds"
    //    },
    //    {
    //        "tex:terrain/dirt_aerial_02_diff_2k_PNG_BC7_1.dds",
    //        "tex:terrain/dirt_aerial_02_nor_2k_PNG_BC5_1.dds",
    //        "tex:terrain/dirt_aerial_02_material_2k_PNG_BC7_1.dds"
    //    },
    //    {
    //        "tex:terrain/snow_02_albedo_2k_PNG_BC7_1.dds",
    //        "tex:terrain/snow_02_nor_2k_PNG_BC5_1.dds",
    //        "tex:terrain/snow_02_material_2k_PNG_BC7_1.dds"
    //    },
    //    {
    //        "tex:terrain/rock_ground_02_albedo_2k_PNG_BC7_1.dds",
    //        "tex:terrain/rock_ground_02_nor_2k_PNG_BC5_1.dds",
    //        "tex:terrain/rock_ground_02_material_2k_PNG_BC7_1.dds"
    //    },
    //    "tex:system/white.dds"
    //);
    
    for (size_t i = 0; i < 120; i++)
    {
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("MovingEntity/cube"_atm);
        Game::Entity entity = Game::CreateEntity(gameWorld, info);
        Game::SetComponent(gameWorld, entity, Game::GetComponentId("WorldTransform"_atm), Math::translation({ (Math::rand() - 0.5f) * 30.0f, yOffset + 0.5f, (Math::rand() - 0.5f) * 30.0f }));
    }
    
    {
        auto files = IO::IoServer::Instance()->ListFiles("mdl:city", "*", true);
        
        int numFiles = (int)files.size();
        int numWidth = 32;

        auto gameWorld = Game::GetWorld(WORLD_DEFAULT);
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("StaticEnvironment/general"_atm);

        for (int x = 0; x < numWidth; x++)
        {
            for (int y = 0; y < numWidth; y++)
            {
                uint fileIndex = Util::FastRandom() % (numFiles);
                if (fileIndex >= numFiles)
                    continue;

                float rotation = (Util::FastRandom() % 4) * (N_PI / 2.0f);
                Game::Entity entity = Game::CreateEntity(gameWorld, info);
                Game::SetComponent<Math::mat4>(gameWorld, entity, Game::WorldTransform::ID(),
                    Math::scaling(10.0f) *
                    Math::rotationy(rotation) * 
                    Math::translation({ xOffset + (float)x * 13.0f, yOffset, zOffset + (float)y * 13.0f })
                );
                GraphicsFeature::Model model = Game::GetComponent<GraphicsFeature::Model>(gameWorld, entity);
                model.resource = files[fileIndex];
                Game::SetComponent<GraphicsFeature::Model>(gameWorld, entity, model);
            }
        }
    }

    Game::ProcessorBuilder("Demo.ShotSpawn"_atm)
        .On("OnEndFrame")
        .Func([](Game::World* world, Game::Owner const& entity, Demo::ShotSpawn& data) 
            {
                //PhysicsFeature::PhysicsActor Actor = Game::GetComponent<PhysicsFeature::PhysicsActor>(world, entity.value);
                //Physics::ActorContext::SetLinearVelocity(Actor.value, data.linearvelocity);
                //Game::Op::DeregisterComponent deregisterOp;
                //deregisterOp.entity = entity.value;
                //deregisterOp.component = Demo::ShotSpawn::ID();
                //Game::AddOp(Game::WorldGetScratchOpBuffer(Game::GetWorld(WORLD_DEFAULT)), deregisterOp);
                
            })
        .Build();

    GraphicsFeature::GraphicsFeatureUnit::Instance()->AddRenderUICallback([]()
    {
        
    });
}

Util::Queue<Game::Entity> entities;

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnBeginFrame()
{
    if (Input::InputServer::Instance()->GetDefaultKeyboard()->KeyPressed(Input::Key::Escape))
    {
        Core::SysFunc::Exit(0);
    }

    if (Input::InputServer::Instance()->GetDefaultMouse()->ButtonPressed(Input::MouseButton::Code::LeftButton))
    {
        Math::mat4 camTransform = GraphicsFeature::CameraManager::GetLocalTransform(GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultViewHandle());
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("PhysicsEntity/placeholder_box"_atm);
        Game::Entity entity = Game::CreateEntity(Game::GetWorld(WORLD_DEFAULT), info);
        entities.Enqueue(entity);
        Game::SetComponent<Math::mat4>(Game::GetWorld(WORLD_DEFAULT), entity, Game::WorldTransform::ID(), Math::translation((camTransform.position - (camTransform.z_axis * (3.0f + Math::rand(-1.0f, 2.0f)))).vec));

        Demo::ShotSpawn Shot;
        Shot.linearvelocity = (camTransform.z_axis * -(20.0f + Math::rand(-10.0f, 10.0f))).vec;
        Shot.angularvelocity = Math::vec3(Math::rand(-5.0f, 5.0f), Math::rand(-5.0f, 5.0f), Math::rand(-5.0f, 5.0f));
        Game::Op::RegisterComponent regOp;
        regOp.entity = entity;
        regOp.component = Demo::ShotSpawn::ID();
        regOp.value = &Shot;
        Game::AddOp(Game::WorldGetScratchOpBuffer(Game::GetWorld(WORLD_DEFAULT)), regOp);
    }

    static int frameIndex = 0;
    if (frameIndex % 10 == 0)
    {
        int limit = 100;
        if (Input::InputServer::Instance()->GetDefaultKeyboard()->KeyDown(Input::Key::F2))
        {
            limit = 0;
        }
        while (entities.Size() > limit)
        {
            Game::DeleteEntity(Game::GetWorld(WORLD_DEFAULT), entities.Dequeue());
        }
    }
    frameIndex++;
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnFrame()
{
#if __NEBULA_HTTP__
    if (Input::InputServer::Instance()->GetDefaultKeyboard()->KeyDown(Input::Key::F1))
    {
        // Open browser with debug page.
        Util::String url = "http://localhost:2100";
#ifdef __WIN32__
        ShellExecute(0, 0, url.AsCharPtr(), 0, 0, SW_SHOW);
#elif __LINUX__
        Util::String shellCommand = "open ";
        shellCommand.Append(url);
        system(shellCommand.AsCharPtr());
#else
        n_printf("Cannot open browser. URL is %s\n", url.AsCharPtr());
#endif
    }
#endif
}

} // namespace Game
