#ifndef MAGEQUEST_SRC_SYSTEM_BENCHMARK_H_
#define MAGEQUEST_SRC_SYSTEM_BENCHMARK_H_

struct BenchMark {

  static void draw_stats() {
    if (IsKeyDown(KEY_H)) {
      sprintf(TEXT_BUFFER, "Frame Time: %lld", FRAME_TIME);
      DrawTextR(TEXT_BUFFER, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.1, 20, GREEN);

      sprintf(TEXT_BUFFER, "Game Tick: %lld", GAME_TICK_TIME);
      DrawTextR(TEXT_BUFFER, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.15, 20, GREEN);
      PLAYER_STATS.mana = PLAYER_STATS.GetMaxMana();
      PLAYER_STATS.effects[CDR_P] = 1;
      Multiplayer::DrawStats(TEXT_BUFFER);
    }
  }
};
#endif  //MAGEQUEST_SRC_SYSTEM_BENCHMARK_H_
