#ifndef MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_

#include "saving/ItemSaver.h"
#include "saving/EntitySaver.h"
#include "saving/MapCoverSaver.h"

namespace GameSaver {

static void save() noexcept {
  ItemSaver::Save();
  NPCSaver::SaveNamedNPCs();
  MapCoverSaver::Save();
}
};      // namespace GameSaver
#endif  //MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_
