#ifndef MAGE_QUEST_SRC_GAME_H_
#define MAGE_QUEST_SRC_GAME_H_

using namespace std::chrono_literals;

class Game {
  bool logic_thread_running = true;
  std::thread logic_thread;

#define UPDATE_AND_COLLISION()                                        \
                                                                      \
  SIMD_PRAGMA                                                         \
  for (const auto net_player : OTHER_PLAYERS) {                       \
    if (net_player) {                                                 \
      net_player->update();                                           \
    }                                                                 \
  }                                                                   \
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
  for (auto it = MONSTERS.begin(); it != MONSTERS.end();) {           \
    if ((*it)->dead) {                                                \
      it = MONSTERS.erase(it);                                        \
    } else {                                                          \
      (*it)->update();                                                \
      ++it;                                                           \
    }                                                                 \
  }                                                                   \
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
    Multiplayer::poll_events();
    switch (GAME_STATE) {
      case GameState::MainMenu: {
        break;
      }
      case GameState::Game:
        [[likely]] {
          UI_MANAGER.update();
          PLAYER_EFFECTS.update();
          PLAYER_STATS.update();
          PLAYER_HOTBAR.update();
          PLAYER.update();
          std::unique_lock<std::shared_mutex> lock(rwLock);
          UPDATE_AND_COLLISION()
          break;
        }
      case GameState::GameMenu: {
        UI_MANAGER.update();
        PLAYER_EFFECTS.update();
        PLAYER_STATS.update();
        PLAYER_HOTBAR.update();
        std::unique_lock<std::shared_mutex> lock(rwLock);
        UPDATE_AND_COLLISION()
        break;
      }
      case GameState::Loading: {
        break;
      }
      case GameState::GameOver: {
        break;
      }
    }
    Multiplayer::send_packets();
    GAME_TICK_TIME = cxstructs::getTime<std::chrono::nanoseconds>();
    PERF_TIME += GAME_TICK_TIME;
    PERF_FRAMES++;
  }

#define RESET_CAMERA()         \
  CAMERA_X = SCREEN_WIDTH / 2; \
  CAMERA_Y = SCREEN_HEIGHT / 2;

#define DRAW_ENTITIES()                             \
                                                    \
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

#define DRAW_GAME_UI()         \
  UI_MANAGER.player_ui.draw(); \
  UI_MANAGER.game_menu.draw();

#define DRAW_LOADING()                    \
  LoadingScreen::draw();                  \
  if (GameLoader::finished_cpu_loading) { \
    GameLoader::finish_loading();         \
  }

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
          break;
        }
      case GameState::GameMenu: {
        WorldRender::draw();
        DRAW_ENTITIES()
        WorldRender::draw_fore_ground();
        DRAW_GAME_UI()
        break;
      }
      case GameState::Loading: {
        DRAW_LOADING()
        break;
      }
      case GameState::GameOver:
        break;
    }
    if (UI_MANAGER.settings_menu.showFPS) {
      DrawFPS(25, 25);
    }
    FRAME_TIME = cxstructs::getTime<std::chrono::nanoseconds>(0);
    PERF_TIME += FRAME_TIME;
    PERF_FRAMES++;
  }

 public:
  Game() noexcept {
    NBN_UDP_Register();
    RNG_RANDOM.seed(std::random_device()());
    RAYLIB_LOGO = new GifDrawer(ASSET_PATH + "ui/titleScreen/raylib.gif");
    NBNET_LOGO = new LogoDrawer(ASSET_PATH + "ui/titleScreen/nbnet.png");
    Image icon = LoadImageR((ASSET_PATH + "Icons/icon2.png").c_str());
    SetWindowIcon(icon);
    UnloadImage(icon);
#ifdef MG2_DEBUG
    SetMasterVolume(0);
#endif
    PLAYER_HOTBAR.skills[1] = new FireStrike_Skill(true, 10, 6);
    PLAYER_HOTBAR.skills[4] = new FireBall_Skill(true, 5);
    for (uint_fast32_t i = 0; i < 1; i++) {
      MONSTERS.push_back(new SkeletonSpear({250.0F, 150}, 10));
    }
    //SettingsMenu::set_full_screen();
  }
  ~Game() noexcept {
    GameSaver::save();
    Multiplayer::close_mp();
    std::cout << PERF_TIME / PERF_FRAMES << std::endl;
    for (uint_fast32_t i = 0; i < 5589; i++) {
      UnloadTexture(TEXTURES[i]);
    }
    logic_thread_running = false;
    logic_thread.join();
    CloseAudioDevice();
    CloseWindowR();
  }
  void start() noexcept {
    GameLoader::load();
    logic_thread = std::thread(&Game::logic_loop, this);
    render_loop();
  }
};
#endif  //MAGE_QUEST_SRC_GAME_H_
