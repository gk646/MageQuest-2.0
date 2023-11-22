#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MISCLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MISCLOADER_H_
namespace MiscLoader {

static void LoadItemSets() noexcept {
  auto table = Util::ReadMGI("mgi/ItemSets.mgi");
  for (const auto& vec : table) {
    auto num = stringToItemSet[vec[0]];
    ItemSet itemSet{};
    for (int i = 0; i < 4; i++) {
      auto idString = Util::SplitString(vec[i + 1], ',');
      itemSet.items[i].id = (int8_t)std::stoi(idString[0]);
      itemSet.items[i].type = ItemType(std::stoi(idString[1]));
      auto threshholds = Util::SplitString(vec[5], ',');
      itemSet.firstBonus = std::stoi(threshholds[0]);
      itemSet.secondBonus = std::stoi(threshholds[1]);
      itemSet.thirdBonus = std::stoi(threshholds[2]);
    }
    ITEM_SETS[(int)num] = itemSet;
  }
}
static void Load() noexcept {
  cxstructs::load_txt(ASSET_PATH + "Text/Info/BagControls.txt",
                      UI_MANAGER.playerUI.charBag.infoPanel.text);
  UI_MANAGER.playerUI.charBag.infoPanel.ReCalc();
  LoadItemSets();
}
}  // namespace MiscLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MISCLOADER_H_
