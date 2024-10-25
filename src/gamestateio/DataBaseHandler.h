#ifndef MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_

namespace DataBaseHandler {
static sqlite3* dataBase = nullptr;
static sqlite3* gameSave = nullptr;
inline static void Init() noexcept {
  sqlite3_open("../DataBase.sqlite", &dataBase);
  sqlite3_open("../GameSave.sqlite", &gameSave);
}
//Takes a created UPDATE (with "columns" amount of "=?") stmt and fills it with null values the given row
inline static void AddNULLRow(sqlite3_stmt* stmt, int columns, int row) noexcept {
  for (int i = 1; i <= columns + 1; ++i) {
    sqlite3_bind_null(stmt, i);
  }
  sqlite3_bind_int(stmt, columns + 2, row);
  sqlite3_step(stmt);
}
//Prepares a statement for the given database
inline static bool PrepareStmt(const std::string& sql, sqlite3* db,
                               sqlite3_stmt** stmt) noexcept {
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, nullptr);
  if (rc != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(*stmt);
    return false;
  }
  return true;
}
//Assumes table has column with name: NUM // only GameSave.sqlite
inline static void SaveNumToTable(int value, const std::string& name, int y) noexcept {
  sqlite3_stmt* stmt;
  std::string sql = "UPDATE " + name + " SET NUM = ? WHERE ROWID = ?";

  if (!DataBaseHandler::PrepareStmt(sql, DataBaseHandler::gameSave, &stmt)) return;

  sqlite3_bind_int(stmt, 1, value);
  sqlite3_bind_int(stmt, 2, y);

  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}
//Loads a value from a table in the given row // only GameSave.sqlite
inline static float GetNumFromTable(const std::string& name, int y) noexcept {
  sqlite3_stmt* stmt;
  std::string sql = "SELECT NUM FROM " + name + " WHERE ROWID=?";

  if (!DataBaseHandler::PrepareStmt(sql, DataBaseHandler::gameSave, &stmt)) return 0.0f;

  sqlite3_bind_int(stmt, 1, y);
  if (sqlite3_step(stmt) != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return 0.0f;
  }
  auto val = static_cast<float>(sqlite3_column_double(stmt, 0));

  sqlite3_finalize(stmt);
  return val;
}

//Works for all *_STATE tables // returns true if an entry with the given values exists
inline static bool StateExistsInTable(const std::string& tableName, int enumVal, int xPos,
                                      int yPos, int zone) noexcept {
  sqlite3_stmt* stmt;
  int exists = 0;
  std::string sql = "SELECT EXISTS(SELECT 1 FROM " + tableName +
                    " WHERE TYPE=? AND X_POS=? AND Y_POS=? AND ZONE=? LIMIT 1)";
  if (!PrepareStmt(sql, gameSave, &stmt)) return false;

  sqlite3_bind_int(stmt, 1, enumVal);
  sqlite3_bind_int(stmt, 2, xPos);
  sqlite3_bind_int(stmt, 3, yPos);
  sqlite3_bind_int(stmt, 4, zone);

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    exists = sqlite3_column_int(stmt, 0);
  }
  sqlite3_finalize(stmt);

  return exists;
}
//Appends a state with the given value to the table
inline static void AddStateToTable(const std::string& tableName, int enumVal, int xPos,
                                   int yPos, int zone) noexcept {
  sqlite3_stmt* stmt;
  std::string sql =
      "INSERT INTO " + tableName + " (TYPE, X_POS, Y_POS, ZONE) VALUES (?, ?, ?, ?)";
  if (!PrepareStmt(sql, gameSave, &stmt)) return;

  sqlite3_bind_int(stmt, 1, enumVal);
  sqlite3_bind_int(stmt, 2, xPos);
  sqlite3_bind_int(stmt, 3, yPos);
  sqlite3_bind_int(stmt, 4, zone);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
  }
  sqlite3_finalize(stmt);
}
//Adds the talentID to the table indicating its activation // only GameSave.sqlite
inline static void AddActivatedTalent(int16_t talentID) noexcept {
  sqlite3_stmt* stmt;
  std::string sql = "INSERT OR REPLACE INTO PLAYER_TALENTS (TALENT_ID) VALUES (?);";

  if (!DataBaseHandler::PrepareStmt(sql, DataBaseHandler::gameSave, &stmt)) return;

  sqlite3_bind_int(stmt, 1, talentID);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    // Handle error
  }

  sqlite3_finalize(stmt);
}
//Deletes all rows with ID greater than 4 (first 4 are the paths -> always activated)// only GameSave.sqlite
inline static void ClearTalents() noexcept {
  sqlite3_stmt* stmt;
  std::string sql = "DELETE FROM PLAYER_TALENTS WHERE PrimaryKeyColumn > 4;";

  if (!DataBaseHandler::PrepareStmt(sql, DataBaseHandler::gameSave, &stmt)) return;

  if (sqlite3_step(stmt) != SQLITE_DONE) {}

  sqlite3_finalize(stmt);
}

}  // namespace DataBaseHandler

#endif  //MAGEQUEST_SRC_GAMESTATEIO_DATABASEHANDLER_H_
