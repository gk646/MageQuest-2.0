#ifndef MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_

#include "saving/ItemSaver.h"
#include "saving/EntitySaver.h"
#include "saving/MapCoverSaver.h"
#include "saving/QuestSaver.h"

namespace GameSaver {

static void Save() noexcept {
  ItemSaver::Save();
  NPCSaver::SaveNamedNPCs();
  MapCoverSaver::Save();
  QuestSaver::Save();
}
}  // namespace GameSaver
#endif  //MAGEQUEST_SRC_GAMESTATEIO_GAMESAVER_H_
