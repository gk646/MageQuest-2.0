#ifndef MAGEQUEST_SRC_GAMESTATEIO_SAVING_MAPCOVERSAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_SAVING_MAPCOVERSAVER_H_

namespace MapCoverSaver {
inline static unsigned char* SerializeBooleans(bool** arr, int length) noexcept {
  int totalBytes = ceil((double)(length * length) / 8);
  auto* data = new unsigned char[totalBytes];
  memset(data, true, totalBytes);

  int byteIndex = 0, bitIndex = 0;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length; j++) {
      if (arr[i][j]) {
        data[byteIndex] |= (1 << bitIndex);
      }
      bitIndex++;
      if (bitIndex == 8) {
        byteIndex++;
        bitIndex = 0;
      }
    }
  }
  return data;
}
inline static void InsertBooleanBLOB(sqlite3_stmt* stmt, unsigned char* data,
                                     int length) {
  if (sqlite3_bind_blob(stmt, 1, data, length, SQLITE_STATIC) != SQLITE_OK) {
    std::cerr << "error bind" << std::endl;
  }

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "error step" << std::endl;
  }

  sqlite3_finalize(stmt);
}
inline static void Save() noexcept {
  sqlite3_stmt* stmt;
  std::string sql = "UPDATE MAP_COVERS SET COVER = ? WHERE ROWID = ?";

  for (const auto& map : MAPS) {
    if (!DataBaseHandler::prepare_stmt(sql, DataBaseHandler::gamesave, &stmt)) return;

    auto data = SerializeBooleans(map.map_cover, map.map_size);
    sqlite3_bind_int(stmt, 2, (int)map.zone + 1);
    InsertBooleanBLOB(stmt, data, (map.map_size*map.map_size)/8);

    delete[] data;
  }
}
}  // namespace MapCoverSaver
#endif  //MAGEQUEST_SRC_GAMESTATEIO_SAVING_MAPCOVERSAVER_H_
