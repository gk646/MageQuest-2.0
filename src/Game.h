#ifndef MAGE_QUEST_SRC_GAME_H_
#define MAGE_QUEST_SRC_GAME_H_

class Game {
  inline static bool isLogicThreadRunning = true;
  std::thread logicThread;

  static void UPDATE_AND_COLLISION() {
    SIMD_PRAGMA
    for (auto it = WORLD_OBJECTS.begin(); it != WORLD_OBJECTS.end();) {
      if ((*it)->isDead) {
        delete *it;
        it = WORLD_OBJECTS.erase(it);
      } else {
        (*it)->Update();
        ++it;
      }
    }

    SIMD_PRAGMA
    for (const auto net_player : OTHER_PLAYERS) {
      if (net_player) {
        net_player->Update();
      }
    }

    SIMD_PRAGMA
    for (auto it = NPCS.begin(); it != NPCS.end();) {
      if ((*it)->isDead) {
        it = NPCS.erase(it);
      } else {
        (*it)->Update();
        ++it;
      }
    }

    SIMD_PRAGMA
    for (auto it = MONSTERS.begin(); it != MONSTERS.end();) {
      if ((*it)->isDead) [[unlikely]] {
        delete *it;
        it = MONSTERS.erase(it);
      } else {
        (*it)->Update();
        ++it;
      }
    }

    for (auto it = PROJECTILES.begin(); it != PROJECTILES.end();) {
      (*it)->Update();

      if ((*it)->isDead) [[unlikely]] {
        delete *it;
        it = PROJECTILES.erase(it);
      } else if (MP_TYPE == MultiplayerType::CLIENT) {
        ++it;
      } else {
        for (auto m_it = MONSTERS.begin(); m_it != MONSTERS.end();) {
          if ((*m_it)->isDead) [[unlikely]] {
            delete *m_it;
            m_it = MONSTERS.erase(m_it);
          } else {
            if ((*m_it)->isUpdated && (*it)->Intersects(**m_it)) [[unlikely]] {
              (*m_it)->Hit(**it);
            }
            ++m_it;
          }
        }
        for (auto np : OTHER_PLAYERS) {
          if (np) {
            if ((*it)->Intersects(*np)) {
              np->Hit(**it);
            }
          }
        }
        if ((*it)->Intersects(PLAYER)) [[unlikely]] {
          PLAYER.Hit(**it);
        }
        ++it;
      }
    }
    for (auto it = DAMAGE_NUMBERS.begin(); it != DAMAGE_NUMBERS.end();) {
      if (it->Update()) {
        it = DAMAGE_NUMBERS.erase(it);
      } else {
        it++;
      }
    }
  }

  inline static void LogicLoop() noexcept {
    auto nextUpdate = std::chrono::high_resolution_clock::now();
    auto targetDuration = std::chrono::milliseconds(16);

    while (isLogicThreadRunning) {
      auto currentTime = std::chrono::high_resolution_clock::now();

      if (currentTime >= nextUpdate) {
        cxstructs::now();
        GameTick();
        GAME_TICK_TIME = cxstructs::getTime<std::chrono::microseconds>();
        PERF_TIME += GAME_TICK_TIME;
        PERF_FRAMES++;

        nextUpdate = nextUpdate + targetDuration;

        while (nextUpdate < std::chrono::high_resolution_clock::now()) {
          nextUpdate += targetDuration;
        }
      } else {
        std::this_thread::sleep_until(nextUpdate);
      }
    }
  }
  static inline void GameTick() noexcept {
    GAME_STATISTICS.Update();
    Lighting::UpdateScreenEffects();
    //SteamAPI_RunCallbacks();
    Multiplayer::PollPackets();
    std::unique_lock<std::shared_mutex> lock(rwLock);
    switch (GAME_STATE) {
      case GameState::MainMenu: {
        SoundManager::Update();
        break;
      }
      case GameState::Game:
      [[likely]] case GameState::GameMenu:
        SoundManager::Update();
        TileEvents::Update();
        WorldManager::Update();
        PLAYER_STATS.Update();
        PLAYER_EFFECTS.Update();
        UI_MANAGER.Update();
        PLAYER.Update();
        UPDATE_AND_COLLISION();
        break;
      case GameState::Loading: {
      } break;
      case GameState::GameOver: {
        SoundManager::Update();
      } break;
    }
    lock.unlock();
    Util::Update();
    Multiplayer::BroadCastGameState();
  }

  inline static void DrawEntities() noexcept {
    for (auto object : WORLD_OBJECTS) {
      if (object->isUpdated) {
        object->Draw();
      }
    }
    for (auto projectile : PROJECTILES) {
      projectile->Draw();
    }
    for (auto monster : MONSTERS) {
      if (monster->isUpdated) {
        monster->Draw();
      }
    }
    for (auto npc : NPCS) {
      if (npc->isUpdated) {
        npc->Draw();
      }
    }
    for (auto net_player : OTHER_PLAYERS) {
      if (net_player) {
        net_player->Draw();
      }
    }
    PLAYER.Draw();
    for (const auto& dn : DAMAGE_NUMBERS) {
      dn.Draw();
    }
  }
  inline static void DrawGame() noexcept {
    WorldRender::PreRenderTasks();
    if (!GAME_SETTINGS.disableDynamicLighting) [[likely]] {
      BeginTextureMode(FIRST_LAYER_BUFFER);
      ClearBackground(BLANK);
      Lighting::Shaders::StartDynamicLights();
      WorldRender::DrawBackGround();
      DrawEntities();
      Lighting::AmbientOcclusion::DrawAmbientOcclusion();
      EndShaderMode();
      EndTextureMode();

      Lighting::Shaders::StartPostProcessing();
      DrawTextureFlipped(FIRST_LAYER_BUFFER.texture, 0, 0, true);
      EndShaderMode();

      BeginTextureMode(FIRST_LAYER_BUFFER);
      ClearBackground(BLANK);
      Lighting::Shaders::StartNightShader();
      WorldRender::DrawForeGround();
      EndShaderMode();
      EndTextureMode();

      Lighting::Shaders::StartPostProcessing();
      DrawTextureFlipped(FIRST_LAYER_BUFFER.texture, 0, 0, true);
      EndShaderMode();

      UI_MANAGER.playerUI.Draw();
      Lighting::DrawScreenEffects();
    } else {
      WorldRender::DrawBackGround();
      Lighting::AmbientOcclusion::DrawAmbientOcclusion();
      DrawEntities();
      WorldRender::DrawForeGround();
      UI_MANAGER.playerUI.Draw();
      Lighting::DrawScreenEffects();
    }
  }
  static inline void DrawFrame() noexcept {
    UI_MANAGER.UIUpdate();
    std::unique_lock<std::shared_mutex> lock(rwLock);
    switch (GAME_STATE) {
      case GameState::MainMenu: {
        UI_MANAGER.mainMenu.Draw();
        break;
      }
      case GameState::Game:
        [[likely]] {
          DrawGame();
        }
        break;
      case GameState::GameMenu: {
        DrawGame();
        UI_MANAGER.gameMenu.Draw();
      } break;
      case GameState::Loading: {
        LoadingScreen::draw();
        if (GameLoader::finished_cpu_loading) {
          GameLoader::LoadWithGPU();
        }
        break;
      }
      case GameState::GameOver:
        break;
    }
    if (GAME_SETTINGS.showFPS) {
      DrawFPS(25, 25);
    }
  }
  static void RenderLoop() noexcept {
    while (!WindowShouldClose()) {
      cxstructs::now(0);
      BeginDrawing();
      ClearBackground(BLANK);

      DrawFrame();

      BenchMark::draw_stats();
      EndDrawing();
      FRAME_TIME = cxstructs::getTime<std::chrono::microseconds>(0);
      PERF_TIME += FRAME_TIME;
      PERF_FRAMES++;
    }
  }

 public:
  Game() noexcept {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
    SetConfigFlags(FLAG_WINDOW_TOPMOST);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(GAME_SETTINGS.targetFPS);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 21);
    InitWindow(1280, 960, "Mage Quest II");
    SCREEN_WIDTH = GetScreenWidth();
    SCREEN_HEIGHT = GetScreenHeight();
    InitAudioDevice();
    SetExitKey(0);
    FIRST_LAYER_BUFFER = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetMouseCursorImage((ASSET_PATH + "ui/cursor.png").c_str(), 0, 0);
    //PLAYER_ID = SteamUser()->GetSteamID();
    //PLAYER_NAME = SteamFriends()->GetPersonaName();
    UI_MANAGER.playerUI.charPanel.header_text = PLAYER_NAME.data();
    RNG_RANDOM.seed(std::random_device()());
    RAYLIB_LOGO = new GifDrawer(ASSET_PATH + "ui/titleScreen/raylib.gif");
    Image icon = LoadImageR((ASSET_PATH + "Icons/icon2.png").c_str());
    FontLoader::Load();
    SetWindowIcon(icon);
    UnloadImage(icon);
    //PLAYER_QUESTS.push_back()
#ifdef MG2_DEBUG
    //SetMasterVolume(0);
#endif
    SettingsMenu::SetFullScreen();
  }
  ~Game() noexcept {
    GameSaver::Save();
    Multiplayer::CloseMultiplayer();
    std::cout << PERF_TIME / PERF_FRAMES << std::endl;
    for (uint16_t i = 0; i < TEXTURE_COUNT + 1500; i++) {
      UnloadTextureI(i);
    }
    isLogicThreadRunning = false;
    logicThread.join();
    CloseAudioDevice();
    CloseWindowR();
    SteamAPI_Shutdown();
  }
  void Start() noexcept {
    GameLoader::Load();
    logicThread = std::thread(Game::LogicLoop);
    RenderLoop();
  }
};
#endif  //MAGE_QUEST_SRC_GAME_H_
