#ifndef MAGEQUEST_SRC_MULTIPLAYER_MENUS_HOSTMENU_H_
#define MAGEQUEST_SRC_MULTIPLAYER_MENUS_HOSTMENU_H_

struct HostMenu {
  float PLAYER_WIDTH = 150.0f;
  float PLAYER_HEIGHT = 250.0f;
  float PLAYER_GAP = 170.0f;
  const float ROUNDNESS = 0.3f;
  const int FONT_SIZE_SELF = 16;
  const int FONT_SIZE_OTHER = 16;

  void update_values() {
    PLAYER_WIDTH = 150.0f * UI_SCALE;
    PLAYER_HEIGHT = 250.0f * UI_SCALE;
    PLAYER_GAP = 30.0f * UI_SCALE;
  }

  void drawPlayerBox(float x, float y) {
    DrawRectangleRounded({x, y, PLAYER_WIDTH, PLAYER_HEIGHT}, ROUNDNESS, ROUND_SEGMENTS,
                         Colors::mediumLightGrey);
    DrawRectangleRoundedLines({x, y, PLAYER_WIDTH, PLAYER_HEIGHT}, ROUNDNESS,
                              ROUND_SEGMENTS, 2, Colors::darkBackground);
  }

  void drawPlayerName(const std::string& name, float x, float y) const {
    DrawTextR(
        name.c_str(),
        x + PLAYER_WIDTH / 2 - MeasureTextEx(MINECRAFT_BOLD, name.c_str(), 15, 1).x / 2,
        y + PLAYER_HEIGHT / 2, FONT_SIZE_OTHER, BLACK);
  }

  void drawSelfName(float x, float y) const {
    auto name = SteamFriends()->GetPersonaName();
    DrawTextR(name,
              x + PLAYER_WIDTH / 2 - MeasureTextEx(MINECRAFT_BOLD, name, 15, 1).x / 2,
              y + PLAYER_HEIGHT / 2, FONT_SIZE_OTHER, BLACK);
  }

  void draw() {
    update_values();
    float totalWidth = 4 * PLAYER_WIDTH + 3 * PLAYER_GAP;
    float startX = (GetScreenWidth() - totalWidth) / 2;
    float startY = (GetScreenHeight() - PLAYER_HEIGHT) / 2;

    for (uint_fast32_t i = 0; i < 4; i++) {
      drawPlayerBox(startX, startY);

      if (i == 0) {
        drawSelfName(startX, startY);
      } else if (OTHER_PLAYERS[i - 1]) {
        drawPlayerName(OTHER_PLAYERS[i - 1]->name, startX, startY);
      } else if (GuiButton({startX, startY, PLAYER_WIDTH, PLAYER_HEIGHT}, "Add player")) {
        SteamFriends()->ActivateGameOverlayInviteDialog(LOBBY_ID);
      }

      startX += PLAYER_WIDTH + PLAYER_GAP;
    }
  }

  void oldstart() {
    GAME_STATE = GameState::Game;
    StopSound(sound::intro);
    Server::init();
  }
};
#endif  //MAGEQUEST_SRC_MULTIPLAYER_MENUS_HOSTMENU_H_
