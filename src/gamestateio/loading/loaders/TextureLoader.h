#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TEXTURELOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TEXTURELOADER_H_
struct TextureLoader {

  static void load() {
    textures::STATUS_BAR = LoadTexture((ASSET_PATH + "ui/" + "player_ui3.png").c_str());
    textures::CROSS_HAIR_AIM =
        LoadTexture((ASSET_PATH + "ui/" + "crosshair_1.png").c_str());
    textures::CROSS_HAIR_SELECT =
        LoadTexture((ASSET_PATH + "ui/" + "crosshair_3.png").c_str());
    textures::ETHER_MARK = LoadTexture((ASSET_PATH + "ui/" + "ether_mark.png").c_str());
    textures::HEALTH_BAR = LoadTexture((ASSET_PATH + "ui/" + "healthbars.png").c_str());
    textures::KEY_MAP = LoadTexture((ASSET_PATH + "ui/" + "keymap.png").c_str());

    textures::TALENT_BACKGROUND_TILE =
        LoadTexture((ASSET_PATH + "ui/talents/" + "background.png").c_str());
    textures::CONNECTION_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/" + "connection_green.png").c_str());
    textures::CONNECTION_ORANGE =
        LoadTexture((ASSET_PATH + "ui/talents/" + "connection_orange.png").c_str());
    textures::CONNECTION_RED =
        LoadTexture((ASSET_PATH + "ui/talents/" + "connection_red.png").c_str());
    textures::NODE_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/" + "node_green.png").c_str());
    textures::NODE_ORANGE =
        LoadTexture((ASSET_PATH + "ui/talents/" + "node_orange.png").c_str());
    textures::NODE_PURPLE =
        LoadTexture((ASSET_PATH + "ui/talents/" + "node_purple.png").c_str());
    textures::TALENT_BIG =
        LoadTexture((ASSET_PATH + "ui/talents/" + "talent_big.png").c_str());
    textures::TALENT_BIG_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/" + "talent_big_green.png").c_str());
    textures::TALENT_BIG_PURPLE =
        LoadTexture((ASSET_PATH + "ui/talents/" + "talent_big_purple.png").c_str());
    textures::TALENT_MID =
        LoadTexture((ASSET_PATH + "ui/talents/" + "talent_mid.png").c_str());
    textures::TALENT_MID_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/" + "talent_mid_green.png").c_str());
    textures::TALENT_MID_PURPLE =
        LoadTexture((ASSET_PATH + "ui/talents/" + "talent_mid_purple.png").c_str());
    textures::TALENTNODE =
        LoadTexture((ASSET_PATH + "ui/talents/" + "talentnode.png").c_str());

    textures::amulet = LoadTexture((ASSET_PATH + "ui/inventory/" + "amulet.png").c_str());
    textures::bag = LoadTexture((ASSET_PATH + "ui/inventory/" + "bag.png").c_str());
    textures::boots = LoadTexture((ASSET_PATH + "ui/inventory/" + "boots.png").c_str());
    textures::chest = LoadTexture((ASSET_PATH + "ui/inventory/" + "chest.png").c_str());
    textures::helm = LoadTexture((ASSET_PATH + "ui/inventory/" + "helm.png").c_str());
    textures::offhand =
        LoadTexture((ASSET_PATH + "ui/inventory/" + "offhand.png").c_str());
    textures::pants = LoadTexture((ASSET_PATH + "ui/inventory/" + "pants.png").c_str());
    textures::relic = LoadTexture((ASSET_PATH + "ui/inventory/" + "relic.png").c_str());
    textures::ring = LoadTexture((ASSET_PATH + "ui/inventory/" + "ring.png").c_str());
    textures::weapon = LoadTexture((ASSET_PATH + "ui/inventory/" + "weapon.png").c_str());
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TEXTURELOADER_H_


