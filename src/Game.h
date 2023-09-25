#ifndef MAGE_QUEST_SRC_GAME_H_
#define MAGE_QUEST_SRC_GAME_H_

using namespace std::chrono_literals;

class Game {
  bool logic_thread_running = true;
  std::thread logic_thread;

#define UPDATE_AND_COLLISION()                                        \
  std::unique_lock<std::shared_mutex> lock(rwLock);                   \
  SIMD_PRAGMA                                                         \
  for (auto it = WORLD_OBJECTS.begin(); it != WORLD_OBJECTS.end();) { \
    if ((*it)->dead) {                                                \
      delete *it;                                                     \
      it = WORLD_OBJECTS.erase(it);                                   \
    } else {                                                          \
      if ((*it)->intersects(PLAYER)) {                                \
        (*it)->collision();                                           \
      }                                                               \
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

  void logic_loop() const noexcept {
    auto nextUpdate = std::chrono::high_resolution_clock::now();
    auto targetDuration = std::chrono::milliseconds(16);

    while (logic_thread_running) {
      auto currentTime = std::chrono::high_resolution_clock::now();

      if (currentTime >= nextUpdate) {
        game_tick();
        nextUpdate = nextUpdate + targetDuration;

        while (nextUpdate < std::chrono::high_resolution_clock::now()) {
          nextUpdate += targetDuration;
        }
      } else {
        std::this_thread::sleep_until(nextUpdate);
      }
    }
  }
  static inline void game_tick() noexcept {
    cxstructs::now();
    Multiplayer::PollPackets();
    SteamAPI_RunCallbacks();
    UI_MANAGER.update();
    switch (GAME_STATE) {
      case GameState::MainMenu: {
        break;
      }
      case GameState::Game:
        [[likely]] {
          PLAYER_EFFECTS.update();
          PLAYER_STATS.update();
          PLAYER_HOTBAR.update();
          PLAYER.update();
          UPDATE_AND_COLLISION()
        }
        break;
      case GameState::GameMenu: {
        PLAYER_EFFECTS.update();
        PLAYER_STATS.update();
        PLAYER_HOTBAR.update();
        UPDATE_AND_COLLISION()
      } break;
      case GameState::Loading: {
      } break;
      case GameState::GameOver: {
      } break;
    }
    Multiplayer::BroadCastGameState();
    GAME_TICK_TIME = cxstructs::getTime<std::chrono::nanoseconds>();
    PERF_TIME += GAME_TICK_TIME;
    PERF_FRAMES++;
  }

#define RESET_CAMERA()         \
  CAMERA_X = SCREEN_WIDTH / 2; \
  CAMERA_Y = SCREEN_HEIGHT / 2;

#define DRAW_ENTITIES()                             \
  std::shared_lock<std::shared_mutex> lock(rwLock); \
  for (auto object : WORLD_OBJECTS) {               \
    object->draw();                                 \
  }                                                 \
  for (auto projectile : PROJECTILES) {             \
    projectile->draw();                             \
  }                                                 \
  for (auto monster : MONSTERS) {                   \
    monster->draw();                                \
  }                                                 \
  for (auto npc : NPCS) {                           \
    npc->draw();                                    \
  }                                                 \
  for (auto net_player : OTHER_PLAYERS) {           \
    if (net_player) {                               \
      net_player->draw();                           \
    }                                               \
  }                                                 \
  lock.unlock();                                    \
  PLAYER.draw();

  static void render_loop() noexcept {
    while (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(WHITE);

      draw_frame();

      BenchMark::draw_stats();
      EndDrawing();
    }
  }
  static inline void draw_frame() noexcept {
    cxstructs::now(0);
    RESET_CAMERA()
    UI_MANAGER.ui_update();
    switch (GAME_STATE) {
      case GameState::MainMenu: {
        UI_MANAGER.main_menu.draw();
        break;
      }
      case GameState::Game:
        [[likely]] {
          WorldRender::draw();
          DRAW_ENTITIES()
          WorldRender::draw_fore_ground();
          UI_MANAGER.player_ui.draw();
        }
        break;
      case GameState::GameMenu: {
        WorldRender::draw();
        DRAW_ENTITIES()
        WorldRender::draw_fore_ground();
        UI_MANAGER.player_ui.draw();
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
    FRAME_TIME = cxstructs::getTime<std::chrono::nanoseconds>(0);
    PERF_TIME += FRAME_TIME;
    PERF_FRAMES++;
  }

 public:
  Game() noexcept {
    SetMouseCursorImage((ASSET_PATH+"ui/cursor.png").c_str(),0,0);
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
    PLAYER_HOTBAR.skills[1] = new FireStrike_Skill(true, 6);
    PLAYER_HOTBAR.skills[4] = new EnergySphere_Skill(true);
    for (uint_fast32_t i = 0; i < 1; i++) {
      MONSTERS.push_back(new Ghost({250.0F+i*5, 150}, 10));
    }
    NPCS.push_back(new Deckard(11, 4));
    //SettingsMenu::set_full_screen();
  }
  ~Game() noexcept {
    GameSaver::save();
    Multiplayer::CloseMultiplayer();
    std::cout << PERF_TIME / PERF_FRAMES << std::endl;
    for (uint_fast32_t i = 0; i < 5589; i++) {
      UnloadTexture(TEXTURES[i]);
    }
    logic_thread_running = false;
    logic_thread.join();
    CloseAudioDevice();
    CloseWindowR();
    SteamAPI_Shutdown();
  }
  void start() noexcept {

    GameLoader::load();
    logic_thread = std::thread(&Game::logic_loop, this);
    render_loop();
  }
};
#endif  //MAGE_QUEST_SRC_GAME_H_
