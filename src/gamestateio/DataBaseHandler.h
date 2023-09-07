#ifndef MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_


struct DataBaseHandler {
  static sqlite3* database;
  static sqlite3* gamesave;
  static void load() { load_items(); }
  static void save() {}
  static void init(){
    sqlite3_open("../DataBase.sqlite", &database);
    sqlite3_open("../GameSave.sqlite", &gamesave);
  }
 private:
  static void load_items() {
    ITEMS.reserve(80);
    std::array<std::string, 11> table_names = {
        "ARM_AMULET", "ARM_BAG",     "ARM_BOOTS",   "ARM_CHEST",
        "ARM_HEAD",   "ARM_OFFHAND", "ARM_ONEHAND", "ARM_PANTS",
        "ARM_RELICS", "ARM_RINGS",   "ARM_TWOHANDS"};
    for (const auto& name : table_names) {
      load_items_from_table(name);
    }
    CHAR_BAG[0].item = new Item(ITEMS[0]);
    CHAR_BAG[1].item = new Item(ITEMS[1]);
  }
  static void load_items_from_table(const std::string& table) {
    sqlite3_stmt* stmt;
    std::string sql = "SELECT * FROM " + table;

    int rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
      std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(database) << std::endl;
      sqlite3_finalize(stmt);
      return;
    }

    // Loop through each row in the result set
    while (sqlite3_step(stmt) == SQLITE_ROW) {

      auto ptr = sqlite3_column_text(stmt, 1);

      if (ptr != nullptr) {
        std::string name = reinterpret_cast<const char*>(ptr);
        int id = sqlite3_column_int(stmt, 0);
        int rarity = sqlite3_column_int(stmt, 2);
        int type = sqlite3_column_int(stmt, 3);
        std::string description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        std::string texturePath = ASSET_PATH + reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) + ".png";

        // Assuming LoadTexture is a function you've defined to load textures
        auto texture = LoadTexture(texturePath.c_str());

        // Assuming ITEMS is a std::vector of some kind of Item class/struct
        ITEMS.emplace_back(id, std::move(name), ItemRarity(rarity), ItemType(type), std::move(description), texture);
      }
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);
  }
};
sqlite3 * DataBaseHandler::database = nullptr;
sqlite3 * DataBaseHandler::gamesave= nullptr;

#endif  //MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_
