#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_FONTLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_FONTLOADER_H_
struct FontLoader{

  static void load(){
    ANT_PARTY =  LoadFont((ASSET_PATH+"Fonts/Ant Party.ttf").c_str());
    VARNISHED =  LoadFont((ASSET_PATH+"Fonts/Varnished.ttf").c_str());
    MINECRAFT_BOLD =  LoadFont((ASSET_PATH+"Fonts/MinecraftBold-nMK1.otf").c_str());
    EDIT_UNDO =  LoadFont((ASSET_PATH+"Fonts/EditUndoBrk-RwaV.ttf").c_str());
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_FONTLOADER_H_
