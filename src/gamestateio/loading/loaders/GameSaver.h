#ifndef MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_

#include "../../saving/ItemSaver.h"
#include "../../saving/EntitySaver.h"

struct GameSaver {

  static void save() noexcept {
    ItemSaver::save();
    NPCSaver::SaveNamedNPCs();
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_
