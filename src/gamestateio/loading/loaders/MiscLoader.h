#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MISCLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MISCLOADER_H_
namespace MiscLoader {
static void Load() noexcept {
  cxstructs::load_txt(ASSET_PATH + "Text/Info/BagControls.txt",
                      UI_MANAGER.playerUI.charBag.infoPanel.text);


  UI_MANAGER.playerUI.charBag.infoPanel.ReCalc();
}
}  // namespace MiscLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MISCLOADER_H_
