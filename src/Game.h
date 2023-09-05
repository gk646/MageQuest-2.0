#ifndef MAGE_QUEST_SRC_GAME_H_
#define MAGE_QUEST_SRC_GAME_H_

using namespace std::chrono_literals;

class Game {
  int targetFPS = 120;
  bool logic_thread_running = true;
  bool multiplayer = false;

  std::thread logic_thread;
  UIManager ui_manager;

  void render() noexcept {
    while (!(WindowShouldClose() && !IsKeyDown(KEY_ESCAPE))) {
      cxstructs::now(0);
      BeginDrawing();
      ClearBackground(WHITE);
      if (GAME_STATE == GameState::Game || GAME_STATE == GameState::GameMenu) {
        drawGame();
      }
      ui_manager.draw();
      BenchMark::draw_stats();
      EndDrawing();
      FRAME_TIME = cxstructs::getTime<std::chrono::nanoseconds>(0);
    }
  }
  void game_logic() noexcept {
    auto nextUpdate = std::chrono::high_resolution_clock::now();
    auto targetDuration = std::chrono::milliseconds(16);

    while (logic_thread_running) {
      auto currentTime = std::chrono::high_resolution_clock::now();

      if (currentTime >= nextUpdate) {
        game_tick();
        nextUpdate = nextUpdate + targetDuration;

        while (nextUpdate < std::chrono::high_resolution_clock::now()) {
          nextUpdate = nextUpdate + targetDuration;
        }
      } else {
        std::this_thread::sleep_until(nextUpdate);
      }
    }
  }

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

  inline void game_tick() noexcept {
    cxstructs::now();
    switch (GAME_STATE) {
      case GameState::MainMenu: {
        break;
      }
      case GameState::Game: {
        ui_manager.update();
        PLAYER_EFFECTS.update();
        PLAYER_STATS.general.update();
        PLAYER_HOTBAR.update();
        PLAYER.movement();
        std::unique_lock<std::shared_mutex> lock(rwLock);
        UPDATE_AND_COLLISION()
        break;
      }
      case GameState::GameMenu: {
        ui_manager.update();
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
  static inline void drawGame() noexcept {
    CAMERA_X = SCREEN_WIDTH / 2;
    CAMERA_Y = SCREEN_HEIGHT / 2;
    WorldRender::draw();
    std::shared_lock<std::shared_mutex> lock(rwLock);
    for (auto& projectile : PROJECTILES) {
      projectile->draw();
    }
    for (auto& npc : NPCS) {
      npc->draw();
    }
    for (auto& monster : MONSTERS) {
      monster->draw();
    }
    for (auto players : OTHER_PLAYERS) {
      players.draw();
    }
    PLAYER.draw();
    WorldRender::draw_fore_ground();
  }

 public:
  Game() {
    RAYLIB_LOGO = new GifDrawer(ASSET_PATH + "ui/titleScreen/raylib.gif");
    PLAYER_HOTBAR.skills[1] = new FireStrike(true, 10, 6);
    PLAYER_HOTBAR.skills[4] = new FireBall(true, 5);
    for (uint_fast32_t i = 0; i < 100; i++) {
      MONSTERS.push_back(new SkeletonWarrior({250+i*50,150},10));
    }
    //SettingsMenu::set_full_screen();
  }
  ~Game() {
    for (uint_fast32_t i = 0; i < 5589; i++) {
      UnloadTexture(TEXTURES[i]);
    }
    logic_thread_running = false;
    logic_thread.join();
    CloseAudioDevice();
    CloseWindow();
  }
  void start() {
    GameLoader::load();
    logic_thread = std::thread(&Game::game_logic, this);
    render();
  }
};
#endif  //MAGE_QUEST_SRC_GAME_H_
