#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MAPLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MAPLOADER_H_
struct MapLoader {

  static void load() {
    MAPS.reserve(11);
    MAPS.emplace_back("hermitCaveHillcrest", 70, Zone::Hillcrest_Hermit_Cave);
    MAPS.emplace_back("goblinCave", 120, Zone::Goblin_Cave);
    MAPS.emplace_back("TestRoom", 50, Zone::TestRoom);
    MAPS.emplace_back("Tutorial", 100, Zone::Tutorial);
    MAPS.emplace_back("FirstDungeon", 60, Zone::Ruin_Dungeon);
    MAPS.emplace_back("Hillcrest", 100, Zone::Hillcrest);
    MAPS.emplace_back("GrassLands", 500, Zone::GrassLands);
    MAPS.emplace_back("HillCrestPuzzleCellar", 50, Zone::Hillcrest_Treasure_Cave);
    MAPS.emplace_back("caveMarla", 60, Zone::Hillcrest_Mountain_Cave);
    MAPS.emplace_back("TheGrove", 200, Zone::The_Grove);
    MAPS.emplace_back("DeadPlains", 200, Zone::DeadPlains);
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MAPLOADER_H_
