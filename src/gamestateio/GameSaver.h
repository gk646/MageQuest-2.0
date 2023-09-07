#ifndef MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_

#include "saving/ItemSaver.h"

struct GameSaver{

  static void save() noexcept{
    ItemSaver::save();
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_
