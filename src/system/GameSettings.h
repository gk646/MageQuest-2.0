
#ifndef MAGEQUEST_SRC_SYSTEM_SETTINGS_H_
#define MAGEQUEST_SRC_SYSTEM_SETTINGS_H_

struct GameSettings {
  float uiScale = 1;
  float masterVolume = 1;
  float musicVolume = 1;
  int16_t roundSegments = 35;
  int16_t targetFPS = 120;

  int16_t playerKeybinds[KB_END] = {0};

  bool fastUI = false;
  bool showFPS = false;
  bool showHealthNumbers = false;
  bool disableDynamicLighting = false;
  bool isFullScreen = true;

  void LoadDefaultSettings() {
    uiScale = 1;
    masterVolume = 1;
    musicVolume = 1;
    roundSegments = 35;
    targetFPS = 120;

    playerKeybinds[KB_PLAYER_LIGHT] = KEY_R;
    playerKeybinds[KB_ABILITY_1] = KEY_ONE;
    playerKeybinds[KB_ABILITY_2] = KEY_TWO;
    playerKeybinds[KB_ABILITY_3] = KEY_THREE;
    playerKeybinds[KB_ABILITY_4] = KEY_FOUR;
    playerKeybinds[KB_ABILITY_5] = -1;
    playerKeybinds[KB_ABILITY_6] = -2;

    fastUI = false;
    showFPS = false;
    showHealthNumbers = false;
    disableDynamicLighting = false;
  }
  static void Load(GameSettings&) noexcept;
  static void Save(GameSettings&) noexcept;
};
inline static GameSettings GAME_SETTINGS;
void GameSettings::Load(GameSettings& gm) noexcept {
  gm.LoadDefaultSettings();
  int i = 1;
  gm.uiScale = DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++) / 100;
  gm.masterVolume = DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++) / 100;
  gm.musicVolume = DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++) / 100;
  gm.roundSegments = (int16_t)DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++);
  gm.targetFPS = DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++);

  for (short& playerKeybind : gm.playerKeybinds) {
    playerKeybind = (int16_t)DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++);
  }

  gm.fastUI = DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++) == 1;
  gm.showFPS = DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++) == 1;
  gm.showHealthNumbers = DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++) == 1;
  gm.disableDynamicLighting = DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++) == 1;
  gm.isFullScreen = DataBaseHandler::GetNumFromTable("SETTINGS_SAVE", i++) == 1;
}
void GameSettings::Save(GameSettings& gm) noexcept {
  int i = 1;
  DataBaseHandler::SaveNumToTable(gm.uiScale * 100, "SETTINGS_SAVE", i++);
  DataBaseHandler::SaveNumToTable(gm.masterVolume * 100, "SETTINGS_SAVE", i++);
  DataBaseHandler::SaveNumToTable(gm.musicVolume * 100, "SETTINGS_SAVE", i++);
  DataBaseHandler::SaveNumToTable(gm.roundSegments, "SETTINGS_SAVE", i++);
  DataBaseHandler::SaveNumToTable(gm.targetFPS, "SETTINGS_SAVE", i++);

  for (short playerKeybind : gm.playerKeybinds) {
    DataBaseHandler::SaveNumToTable(playerKeybind, "SETTINGS_SAVE", i++);
  }

  DataBaseHandler::SaveNumToTable(gm.fastUI ? 1 : 0, "SETTINGS_SAVE", i++);
  DataBaseHandler::SaveNumToTable(gm.showFPS ? 1 : 0, "SETTINGS_SAVE", i++);
  DataBaseHandler::SaveNumToTable(gm.showHealthNumbers ? 1 : 0, "SETTINGS_SAVE", i++);
  DataBaseHandler::SaveNumToTable(gm.disableDynamicLighting ? 1 : 0, "SETTINGS_SAVE",
                                  i++);
  DataBaseHandler::SaveNumToTable(gm.isFullScreen ? 1 : 0, "SETTINGS_SAVE", i++);
}

#endif  //MAGEQUEST_SRC_SYSTEM_SETTINGS_H_
