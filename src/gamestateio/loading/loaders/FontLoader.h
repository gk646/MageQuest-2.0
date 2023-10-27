#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_FONTLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_FONTLOADER_H_
struct FontLoader{
  static void Load(){
    ANT_PARTY =  LoadFont((ASSET_PATH+"Fonts/Ant Party.ttf").c_str());
    VARNISHED =  LoadFont((ASSET_PATH+"Fonts/Varnished.ttf").c_str());
    MINECRAFT_BOLD =  LoadFont((ASSET_PATH+"Fonts/MinecraftBold-nMK1.otf").c_str());
    MINECRAFT_ITALIC =  LoadFont((ASSET_PATH+"Fonts/MinecraftItalic-R8Mo.otf").c_str());
    MINECRAFT_REGULAR=  LoadFont((ASSET_PATH+"Fonts/MinecraftRegular-Bmg3.otf").c_str());
    EDIT_UNDO =  LoadFont((ASSET_PATH+"Fonts/EditUndoBrk-RwaV.ttf").c_str());
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_FONTLOADER_H_
