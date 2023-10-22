#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MAPLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MAPLOADER_H_
struct MapLoader {

  inline static void DeserializeBooleans(const unsigned char* data, bool** arr, int length) noexcept {
    int byteIndex = 0, bitIndex = 0;
    
    for (int i = 0; i < length; i++) {
      for (int j = 0; j < length; j++) {
        arr[i][j] = (data[byteIndex] & (1 << bitIndex)) != 0;
        bitIndex++;
        if (bitIndex == 8) {
          byteIndex++;
          bitIndex = 0;
        }
      }
    }
  }

  inline static void LoadMapCovers() noexcept {
    sqlite3_stmt* stmt;
    std::string sql = "SELECT COVER FROM MAP_COVERS WHERE ROWID = ?";

    for (auto& map : MAPS) {
      if (!DataBaseHandler::prepare_stmt(sql, DataBaseHandler::gamesave, &stmt)) return;

      sqlite3_bind_int(stmt, 1, (int)map.zone + 1);

      if (sqlite3_step(stmt) == SQLITE_ROW) {
        const auto* data =
            reinterpret_cast<const unsigned char*>(sqlite3_column_blob(stmt, 0));
        if (!data) break;
        DeserializeBooleans(data, map.map_cover, map.map_size);
      }

      sqlite3_finalize(stmt);
    }
  }

  static void load() {
    MAPS.reserve(11);
    MAPS.emplace_back("hermitCaveHillcrest", 70, Zone::Hillcrest_Hermit_Cave);
    MAPS.emplace_back("goblinCave", 120, Zone::Goblin_Cave);
    MAPS.emplace_back("TestRoom", 50, Zone::TestRoom);
    MAPS.emplace_back("Tutorial", 100, Zone::Tutorial);
    MAPS.emplace_back("FirstDungeon", 75, Zone::Ruin_Dungeon);
    MAPS.emplace_back("Hillcrest", 100, Zone::Hillcrest);
    MAPS.emplace_back("GrassLands", 500, Zone::GrassLands);
    MAPS.emplace_back("HillCrestPuzzleCellar", 50, Zone::Hillcrest_Treasure_Cave);
    MAPS.emplace_back("caveMarla", 60, Zone::Hillcrest_Mountain_Cave);
    MAPS.emplace_back("TheGrove", 200, Zone::The_Grove);
    MAPS.emplace_back("DeadPlains", 200, Zone::DeadPlains);
    LoadMapCovers();
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MAPLOADER_H_
