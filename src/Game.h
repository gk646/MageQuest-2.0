#ifndef MAGE_QUEST_SRC_GAME_H_
#define MAGE_QUEST_SRC_GAME_H_

class Game {
  static bool logic_thread_running;
  std::thread logic_thread;

#define UPDATE_AND_COLLISION()                                        \
  SIMD_PRAGMA                                                         \
  for (auto it = WORLD_OBJECTS.begin(); it != WORLD_OBJECTS.end();) { \
    if ((*it)->dead) {                                                \
      delete *it;                                                     \
      it = WORLD_OBJECTS.erase(it);                                   \
    } else {                                                          \
      (*it)->update();                                                \
      ++it;                                                           \
    }                                                                 \
  }                                                                   \
                                                                      \
  SIMD_PRAGMA                                                         \
  for (const auto net_player : OTHER_PLAYERS) {                       \
    if (net_player) {                                                 \
      net_player->update();                                           \
    }                                                                 \
  }                                                                   \
                                                                      \
  SIMD_PRAGMA                                                         \
  for (auto it = NPCS.begin(); it != NPCS.end();) {                   \
    if ((*it)->dead) {                                                \
      it = NPCS.erase(it);                                            \
    } else {                                                          \
      (*it)->update();                                                \
      ++it;                                                           \
    }                                                                 \
  }                                                                   \
                                                                      \
  SIMD_PRAGMA                                                         \
  for (auto it = MONSTERS.begin(); it != MONSTERS.end();) {           \
    if ((*it)->dead) [[unlikely]] {                                   \
      delete *it;                                                     \
      it = MONSTERS.erase(it);                                        \
    } else {                                                          \
      (*it)->update();                                                \
      ++it;                                                           \
    }                                                                 \
  }                                                                   \
                                                                      \
  for (auto it = PROJECTILES.begin(); it != PROJECTILES.end();) {     \
    (*it)->update();                                                  \
                                                                      \
    if ((*it)->dead) [[unlikely]] {                                   \
      delete *it;                                                     \
      it = PROJECTILES.erase(it);                                     \
    } else if (MP_TYPE == MultiplayerType::CLIENT) {                  \
      ++it;                                                           \
    } else {                                                          \
      for (auto m_it = MONSTERS.begin(); m_it != MONSTERS.end();) {   \
        if ((*m_it)->dead) [[unlikely]] {                             \
          delete *m_it;                                               \
          m_it = MONSTERS.erase(m_it);                                \
        } else {                                                      \
          if ((*it)->intersects(**m_it)) [[unlikely]] {               \
            (*m_it)->hit(**it);                                       \
          }                                                           \
          ++m_it;                                                     \
        }                                                             \
      }                                                               \
      for (auto np : OTHER_PLAYERS) {                                 \
        if (np) {                                                     \
          if ((*it)->intersects(*np)) {                               \
            np->hit(**it);                                            \
          }                                                           \
        }                                                             \
      }                                                               \
      if ((*it)->intersects(PLAYER)) [[unlikely]] {                   \
        PLAYER.hit(**it);                                             \
      }                                                               \
      ++it;                                                           \
    }                                                                 \
  }

  inline static void LogicLoop() noexcept {
    auto nextUpdate = std::chrono::high_resolution_clock::now();
    auto targetDuration = std::chrono::milliseconds(16);

    while (logic_thread_running) {
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
    WorldManager::Update();
    Lighting::UpdateScreenEffects();
    SteamAPI_RunCallbacks();
    Multiplayer::PollPackets();
    std::unique_lock<std::shared_mutex> lock(rwLock);
    switch (GAME_STATE) {
      case GameState::MainMenu: {
        break;
      }
      case GameState::Game:
        [[likely]] {
          PLAYER_STATS.update();
          PLAYER_EFFECTS.Update();
          PLAYER.update();
          UPDATE_AND_COLLISION()
          UI_MANAGER.Update();
        }
        break;
      case GameState::GameMenu: {
        PLAYER_STATS.update();
        PLAYER_EFFECTS.Update();
        UPDATE_AND_COLLISION()
        UI_MANAGER.Update();
      } break;
      case GameState::Loading: {
      } break;
      case GameState::GameOver: {
      } break;
    }
    lock.unlock();
    Multiplayer::BroadCastGameState();
  }

#define RESET_CAMERA()         \
  CAMERA_X = SCREEN_WIDTH / 2; \
  CAMERA_Y = SCREEN_HEIGHT / 2;

#define DRAW_ENTITIES()                   \
  for (auto object : WORLD_OBJECTS) {     \
    if (object->active) {                 \
      object->draw();                     \
    }                                     \
  }                                       \
  for (auto projectile : PROJECTILES) {   \
    projectile->draw();                   \
  }                                       \
  for (auto monster : MONSTERS) {         \
    if (monster->active) {                \
      monster->draw();                    \
    }                                     \
  }                                       \
  for (auto npc : NPCS) {                 \
    if (npc->active) {                    \
      npc->draw();                        \
    }                                     \
  }                                       \
  for (auto net_player : OTHER_PLAYERS) { \
    if (net_player) {                     \
      net_player->draw();                 \
    }                                     \
  }                                       \
  PLAYER.draw();

  inline static void DrawGame() noexcept {
    WorldRender::PreRenderTasks();
    if (!DISABLE_DYNAMIC_LIGHTING) [[likely]] {

      BeginTextureMode(FIRST_LAYER_BUFFER);
      ClearBackground(BLANK);
      Lighting::Shaders::StartDynamicLights();
      WorldRender::DrawBackGround();
      Lighting::AmbientOcclusion::DrawAmbientOcclusion();
      DRAW_ENTITIES();
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

      UI_MANAGER.player_ui.Draw();
      Lighting::DrawScreenEffects();
    } else {
      WorldRender::DrawBackGround();
      DRAW_ENTITIES()
      WorldRender::DrawForeGround();
      UI_MANAGER.player_ui.Draw();
    }
  }
  static inline void DrawFrame() noexcept {
    RESET_CAMERA()
    UI_MANAGER.UIUpdate();
    std::unique_lock<std::shared_mutex> lock(rwLock);
    switch (GAME_STATE) {
      case GameState::MainMenu: {
        UI_MANAGER.main_menu.draw();
        break;
      }
      case GameState::Game:
        [[likely]] {
          DrawGame();
        }
        break;
      case GameState::GameMenu: {
        DrawGame();
        UI_MANAGER.game_menu.draw();
      } break;
      case GameState::Loading: {
        LoadingScreen::draw();
        if (GameLoader::finished_cpu_loading) {
          GameLoader::finish_loading();
        }
        break;
      }
      case GameState::GameOver:
        break;
    }
    if (SHOW_FPS) {
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
    SetConfigFlags(FLAG_WINDOW_TOPMOST);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(TARGET_FPS);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 21);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mage Quest II");
    InitAudioDevice();
    SetExitKey(0);
    FIRST_LAYER_BUFFER = LoadRenderTexture(1280, 960);
    SetMouseCursorImage((ASSET_PATH + "ui/cursor.png").c_str(), 0, 0);
    PLAYER_ID = SteamUser()->GetSteamID();
    PLAYER_NAME = SteamFriends()->GetPersonaName();
    UI_MANAGER.player_ui.char_panel.header_text = PLAYER_NAME.data();
    RNG_RANDOM.seed(std::random_device()());
    RAYLIB_LOGO = new GifDrawer(ASSET_PATH + "ui/titleScreen/raylib.gif");
    Image icon = LoadImageR((ASSET_PATH + "Icons/icon2.png").c_str());
    SetWindowIcon(icon);
    UnloadImage(icon);
    //PLAYER_QUESTS.push_back()
#ifdef MG2_DEBUG
    //SetMasterVolume(0);
#endif
    UI_MANAGER.player_ui.playerHotbar.skills[1] = new FireStrike_Skill(true, 6);
    UI_MANAGER.player_ui.playerHotbar.skills[4] = new EnergySphere_Skill(true);
    for (uint_fast32_t i = 0; i < 3; i++) {
      MONSTERS.push_back(new Ghost({250.0F + i * 5, 150}, 10));
    }
    SettingsMenu::set_full_screen();
  }
  ~Game() noexcept {
    GameSaver::save();
    Multiplayer::CloseMultiplayer();
    std::cout << PERF_TIME / PERF_FRAMES << std::endl;
    for (uint16_t i = 0; i < TileLoader::TEXTURE_COUNT + 1500; i++) {
      UnloadTextureI(i);
    }
    logic_thread_running = false;
    logic_thread.join();
    CloseAudioDevice();
    CloseWindowR();
    SteamAPI_Shutdown();
  }
  void start() noexcept {
    GameLoader::load();
    logic_thread = std::thread(Game::LogicLoop);
    RenderLoop();
  }
};
bool Game::logic_thread_running = true;
#endif  //MAGE_QUEST_SRC_GAME_H_
