#ifndef MAGEQUEST_SRC_SYSTEM_BENCHMARK_H_
#define MAGEQUEST_SRC_SYSTEM_BENCHMARK_H_

struct BenchMark {
  static char buffer[25];

  static void draw_stats() {
    if (IsKeyDown(KEY_H)) {
      sprintf(buffer, "Frame Time: %lld", FRAME_TIME);
      DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.1, 20, GREEN);

      sprintf(buffer, "Game Tick: %lld", GAME_TICK_TIME);
      DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.15, 20, GREEN);
      PLAYER_STATS.mana = PLAYER_STATS.get_max_mana();
      PLAYER_STATS.effects[CDR_P] = 1;
      Multiplayer::draw_stats(buffer);
    }
  }
};
char BenchMark::buffer[25];
#endif  //MAGEQUEST_SRC_SYSTEM_BENCHMARK_H_
