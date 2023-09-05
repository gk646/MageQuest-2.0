#ifndef MAGE_QUEST_SRC_GAME_H_
#define MAGE_QUEST_SRC_GAME_H_

using namespace std::chrono_literals;

class Game {
  bool logic_thread_running = true;
  std::thread logic_thread;

#define UPDATE_AND_COLLISION()                                      \
  for (auto monster : MONSTERS) {                                   \
    monster->update();                                              \
  }                                                                 \
  for (auto it = PROJECTILES.begin(); it != PROJECTILES.end();) {   \
    (*it)->update();                                                \
                                                                    \
    if ((*it)->dead) {                                              \
      delete *it;                                                   \
      it = PROJECTILES.erase(it);                                   \
    } else {                                                        \
      for (auto m_it = MONSTERS.begin(); m_it != MONSTERS.end();) { \
        if ((*m_it)->dead) {                                        \
          delete *m_it;                                             \
          m_it = MONSTERS.erase(m_it);                              \
        } else {                                                    \
          if ((*it)->intersects(**m_it)) {                          \
            (*m_it)->hit(**it);                                     \
          }                                                         \
          ++m_it;                                                   \
        }                                                           \
      }                                                             \
      if ((*it)->intersects(PLAYER)) {                              \
        PLAYER.hit(**it);                                           \
      }                                                             \
      ++it;                                                         \
    }                                                               \
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
    switch (GAME_STATE) {
      case GameState::MainMenu: {
        break;
      }
      case GameState::Game: {
        UI_MANAGER.update();
        PLAYER_EFFECTS.update();
        PLAYER_STATS.general.update();
        PLAYER_HOTBAR.update();
        PLAYER.movement();
        std::unique_lock<std::shared_mutex> lock(rwLock);
        UPDATE_AND_COLLISION()
        break;
      }
      case GameState::GameMenu: {
        UI_MANAGER.update();
        PLAYER_EFFECTS.update();
        PLAYER_STATS.general.update();
        PLAYER_HOTBAR.update();
        std::unique_lock<std::shared_mutex> lock(rwLock);
        UPDATE_AND_COLLISION()
        break;
      }
      case GameState::Loading: {
      }
    }
    GAME_TICK_TIME = cxstructs::getTime<std::chrono::nanoseconds>();
  }

#define RESET_CAMERA()         \
  CAMERA_X = SCREEN_WIDTH / 2; \
  CAMERA_Y = SCREEN_HEIGHT / 2;

#define DRAW_ENTITIES()                             \
  std::shared_lock<std::shared_mutex> lock(rwLock); \
  for (auto projectile : PROJECTILES) {             \
    projectile->draw();                             \
  }                                                 \
  for (auto monster : MONSTERS) {                   \
    monster->draw();                                \
  }                                                 \
  for (auto npc : NPCS) {                           \
    npc->draw();                                    \
  }                                                 \
  for (auto players : OTHER_PLAYERS) {              \
    players.draw();                                 \
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
      case GameState::Game: {
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
    }
    if (UI_MANAGER.settings_menu.showFPS) {
      DrawFPS(25, 25);
    }
    FRAME_TIME = cxstructs::getTime<std::chrono::nanoseconds>(0);
  }

 public:
  Game() noexcept{
    RAYLIB_LOGO = new GifDrawer(ASSET_PATH + "ui/titleScreen/raylib.gif");
    Image icon = LoadImage((ASSET_PATH + "Icons/icon2.png").c_str());
    SetWindowIcon(icon);
    UnloadImage(icon);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    PLAYER_HOTBAR.skills[1] = new FireStrike(true, 10, 6);
    PLAYER_HOTBAR.skills[4] = new FireBall(true, 5);
    for (uint_fast32_t i = 0; i < 100; i++) {
      MONSTERS.push_back(new SkeletonWarrior({250 + i * 50, 150}, 10));
    }
    //SettingsMenu::set_full_screen();
  }
  ~Game()noexcept {
    for (uint_fast32_t i = 0; i < 5589; i++) {
      UnloadTexture(TEXTURES[i]);
    }
    logic_thread_running = false;
    logic_thread.join();
    CloseAudioDevice();
    CloseWindow();
  }
  void start()noexcept {
    GameLoader::load();
    logic_thread = std::thread(&Game::logic_loop, this);
    render_loop();
  }
};
#endif  //MAGE_QUEST_SRC_GAME_H_
