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
      for (auto& item : ITEMS) {
        if (item.id == itemSet.items[i].id && item.type == itemSet.items[i].type) {
          item.itemSet = num;
        }
      }
    }
    auto thresholds = Util::SplitString(vec[5], ',');
    auto desc = Util::SplitString(vec[7], ':');

    for (int i = 0; i < 3; i++) {
      itemSet.thresholds[i] = std::stoi(thresholds[i]);
      if (itemSet.thresholds[i] != -1) {
        itemSet.bonusCount++;
        int lineBreak = 0;
        auto waste = Util::WrapText(desc[i], Item::TOOL_TIP_WIDTH - 25, MINECRAFT_ITALIC,
                                    14, &lineBreak);
        itemSet.toolTipHeight += 14 * lineBreak;
      }
      itemSet.descriptions[i] = desc[i];
    }
    itemSet.toolTipHeight += 40;
    itemSet.title = vec[6];
    ITEM_SETS[(int)num] = itemSet;
  }
}
static void Load() noexcept {
  cxstructs::load_txt(ASSET_PATH + "Text/Info/BagControls.txt",
                      UI_MANAGER.playerUI.charBag.infoPanel.text);
  UI_MANAGER.playerUI.charBag.infoPanel.ReCalc();
}
}  // namespace MiscLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MISCLOADER_H_
