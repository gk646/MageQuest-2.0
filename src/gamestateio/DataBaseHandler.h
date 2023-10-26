#ifndef MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_

struct DataBaseHandler {
  static sqlite3* dataBase;
  static sqlite3* gameSave;
  inline static void init() noexcept {
    sqlite3_open("../DataBase.sqlite", &dataBase);
    sqlite3_open("../GameSave.sqlite", &gameSave);
  }
  static void load() noexcept {
    ITEMS.reserve(80);
    std::array<std::string, 11> table_names = {
        "ARM_AMULET", "ARM_BAG",     "ARM_BOOTS",   "ARM_CHEST",
        "ARM_HEAD",   "ARM_OFFHAND", "ARM_ONEHAND", "ARM_PANTS",
        "ARM_RELICS", "ARM_RINGS",   "ARM_TWOHANDS"};
    for (const auto& name : table_names) {
      create_items_from_table(name);
    }
    RANGE_EXISTING_ITEMS = std::uniform_int_distribution<int>(0, ITEMS.size() - 1);

    std::cout << "ITEMS LOADED: " << ITEMS.size() << std::endl;
    LoadItemsFromTable(PLAYER_EQUIPPED, "PLAYER_INV", 10, true);
    LoadItemsFromTable(UI_MANAGER.playerUI.charBag.bagPanel.bagSlots.data(), "PLAYER_INV",
                       4, true, 12);
    LoadItemsFromTable(PLAYER_BAG, "PLAYER_BAG", PLAYER_STATS.GetBagSlots());
  }
  static bool PrepareStmt(const std::string& sql, sqlite3* db,
                          sqlite3_stmt** stmt) noexcept {
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, nullptr);
    if (rc != SQLITE_OK) {
      std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_finalize(*stmt);
      return false;
    }
    return true;
  }

 private:
  static void create_items_from_table(const std::string& table) noexcept {
    sqlite3_stmt* stmt;
    if (!PrepareStmt("SELECT * FROM " + table, dataBase, &stmt)) return;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const char* name_ptr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      if (name_ptr) {
        const char* description_ptr =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        const char* texturePath_ptr =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        std::string texturePath = ASSET_PATH + texturePath_ptr + ".png";

        auto& s = ITEMS.emplace_back(sqlite3_column_int(stmt, 0), name_ptr,
                                     ItemRarity(sqlite3_column_int(stmt, 2)),
                                     ItemType(sqlite3_column_int(stmt, 3)),
                                     description_ptr, LoadTexture(texturePath.c_str()));
        ParseEffectText(s.effects, sqlite3_column_text(stmt, 7));
        ParseAttributeStats(s.effects,
                            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
      }
    }
    sqlite3_finalize(stmt);
  }
  static void LoadItemsFromTable(InventorySlot* slots, const std::string& table,
                                 int length, bool equip = false,
                                 int offsetY = 0) noexcept {
    sqlite3_stmt* stmt;
    if (!PrepareStmt("SELECT * FROM " + table, gameSave, &stmt)) return;

    for (int i = 0; i < length + offsetY && sqlite3_step(stmt) == SQLITE_ROW; ++i) {
      if (i < offsetY) continue;

      slots[i - offsetY].item = ItemDropHandler::GetShallowCloneItem(
          sqlite3_column_int(stmt, 0), ItemType(sqlite3_column_int(stmt, 1)),
          sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3));

      if (slots[i - offsetY].item) {
        ParseEffectText(slots[i - offsetY].item->effects, sqlite3_column_text(stmt, 4));
        if (equip) {
          PLAYER_STATS.EquipItem(slots[i - offsetY].item->effects);
        }
      }
    }
    sqlite3_finalize(stmt);
  }

  inline static void ParseEffectText(float* arr, const unsigned char* ptr) {
    if (!ptr) {
      return;
    }
    std::stringstream ss(reinterpret_cast<const char*>(ptr));
    std::string pair;
    while (std::getline(ss, pair, ';')) {
      size_t sep = pair.find(':');
      if (sep != std::string::npos) {
        uint_fast32_t index = std::stoi(pair.substr(0, sep));
        float value = std::stof(pair.substr(sep + 1));
        arr[index] += value;
      }
    }
  }
  inline static void ParseAttributeStats(float* arr, const std::string& input) {
    std::regex pattern(R"(([a-zA-Z]+)([-+]?\d+))");
    std::smatch matches;

    auto it = input.cbegin();
    while (std::regex_search(it, input.cend(), matches, pattern)) {
      std::string attribute = matches[1].str();
      int value = std::stoi(matches[2].str());
      if (attrToStat.find(attribute) != attrToStat.end()) {
        Stat stat = attrToStat[attribute];
        arr[stat] += value;
      }
      it = matches.suffix().first;
    }
  }
};
sqlite3* DataBaseHandler::dataBase = nullptr;
sqlite3* DataBaseHandler::gameSave = nullptr;

#endif  //MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_
