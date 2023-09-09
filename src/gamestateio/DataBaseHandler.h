#ifndef MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_

struct DataBaseHandler {
  static sqlite3* database;
  static sqlite3* gamesave;
  static void load() noexcept {
    init();
    cxstructs::now();
    std::array<std::string, 11> table_names = {
        "ARM_AMULET", "ARM_BAG",     "ARM_BOOTS",   "ARM_CHEST",
        "ARM_HEAD",   "ARM_OFFHAND", "ARM_ONEHAND", "ARM_PANTS",
        "ARM_RELICS", "ARM_RINGS",   "ARM_TWOHANDS"};
    for (const auto& name : table_names) {
      create_items_from_table(name);
    }

    load_items_from_table(PLAYER_EQUIPPED, "PLAYER_INV", 10);
    CharacterBag::add_slots(8);
    load_items_from_table(PLAYER_BAG, "PLAYER_BAG", PLAYER_BAG_SIZE);
    cxstructs::printTime<std::chrono::nanoseconds>();
  }
  static bool prepare_stmt(const std::string& sql, sqlite3* db,
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
  static void init() noexcept {
    ITEMS.reserve(80);
    sqlite3_open("../DataBase.sqlite", &database);
    sqlite3_open("../GameSave.sqlite", &gamesave);
  }
  static void create_items_from_table(const std::string& table) noexcept {
    sqlite3_stmt* stmt;
    if (!prepare_stmt("SELECT * FROM " + table, database, &stmt))
      return;

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
        parse_effect_text(s.effects, sqlite3_column_text(stmt, 7));
        parse_attributes(s.effects,
                         reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
      }
    }
    sqlite3_finalize(stmt);
  }
  static void load_items_from_table(InventorySlot* slots, const std::string& table,
                                    int length = 100) noexcept {
    sqlite3_stmt* stmt;
    if (!prepare_stmt("SELECT * FROM " + table, gamesave, &stmt))
      return;

    for (int i = 0; i < length && sqlite3_step(stmt) == SQLITE_ROW; ++i) {
      slots[i].item = get_matching_item(
          sqlite3_column_int(stmt, 0), ItemType(sqlite3_column_int(stmt, 1)),
          sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3));
      parse_effect_text(slots[i].item->effects, sqlite3_column_text(stmt, 4));
    }
    sqlite3_finalize(stmt);
  }
  inline static Item* get_matching_item(int id, ItemType type, int quality, int level) {
    for (const auto& item : ITEMS) {
      if (item.id == id && item.type == type) {
        return new Item(item.id,item.name,item.rarity,item.type,item.description,item.texture,quality,level);
      }
    }
    return nullptr;
  }
  inline static void parse_effect_text(float* arr, const unsigned char* ptr) {
    if (!ptr || !arr) {
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
  static void parse_attributes(float* arr, const std::string& input) {
    std::regex pattern(R"((\w+)([-+]?\d+))");
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
  inline static void apply_item_effects() {}
};
sqlite3* DataBaseHandler::database = nullptr;
sqlite3* DataBaseHandler::gamesave = nullptr;

#endif  //MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_
