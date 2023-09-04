#ifndef MAGEQUEST_SRC_SYSTEM_BENCHMARK_H_
#define MAGEQUEST_SRC_SYSTEM_BENCHMARK_H_

struct BenchMark {
  static char buffer[50];

  static void draw_stats() {
    if (IsKeyDown(KEY_H)) {
      sprintf(buffer, "Frame Time: %lld", FRAME_TIME);
      DrawText(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.1, 20, GREEN);

      sprintf(buffer, "Game Tick: %lld", GAME_TICK_TIME);
      DrawText(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.15, 20, GREEN);
      PLAYER_STATS.general.mana = PLAYER_STATS.general.max_mana;
      PLAYER_STATS.combat_stats.cooldown_reduction = 100;
    }
  }
};
char BenchMark::buffer[50];
#endif  //MAGEQUEST_SRC_SYSTEM_BENCHMARK_H_
