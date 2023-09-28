#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TEXTURELOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TEXTURELOADER_H_
struct TextureLoader {

  static void load() {
    textures::ui::STATUS_BAR =
        LoadTexture((ASSET_PATH + "ui/" + "player_ui3.png").c_str());
    textures::ui::CROSS_HAIR_AIM =
        LoadTexture((ASSET_PATH + "ui/" + "crosshair_1.png").c_str());
    textures::ui::CROSS_HAIR_SELECT =
        LoadTexture((ASSET_PATH + "ui/" + "crosshair_3.png").c_str());
    textures::ui::ETHER_MARK =
        LoadTexture((ASSET_PATH + "ui/" + "ether_mark.png").c_str());
    textures::ui::HEALTH_BAR =
        LoadTexture((ASSET_PATH + "ui/" + "healthbars.png").c_str());
    textures::ui::KEY_MAP = LoadTexture((ASSET_PATH + "ui/" + "keymap.png").c_str());

    textures::TALENT_BACKGROUND_TILE =
        LoadTexture((ASSET_PATH + "ui/talents/background.png").c_str());
    textures::CONNECTION_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/connection_green.png").c_str());
    textures::CONNECTION_ORANGE =
        LoadTexture((ASSET_PATH + "ui/talents/connection_orange.png").c_str());
    textures::CONNECTION_RED =
        LoadTexture((ASSET_PATH + "ui/talents/connection_red.png").c_str());
    textures::NODE_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/node_green.png").c_str());
    textures::NODE_ORANGE =
        LoadTexture((ASSET_PATH + "ui/talents/node_orange.png").c_str());
    textures::NODE_PURPLE =
        LoadTexture((ASSET_PATH + "ui/talents/node_purple.png").c_str());
    textures::TALENT_BIG =
        LoadTexture((ASSET_PATH + "ui/talents/talent_big.png").c_str());
    textures::TALENT_BIG_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/talent_big_green.png").c_str());
    textures::TALENT_BIG_PURPLE =
        LoadTexture((ASSET_PATH + "ui/talents/talent_big_purple.png").c_str());
    textures::TALENT_MID =
        LoadTexture((ASSET_PATH + "ui/talents/talent_mid.png").c_str());
    textures::TALENT_MID_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/talent_mid_green.png").c_str());
    textures::TALENT_MID_PURPLE =
        LoadTexture((ASSET_PATH + "ui/talents/talent_mid_purple.png").c_str());
    textures::TALENT_NODE =
        LoadTexture((ASSET_PATH + "ui/talents/talentnode.png").c_str());

    textures::amulet = LoadTexture((ASSET_PATH + "ui/inventory/amulet.png").c_str());
    textures::bag = LoadTexture((ASSET_PATH + "ui/inventory/bag.png").c_str());
    textures::boots = LoadTexture((ASSET_PATH + "ui/inventory/boots.png").c_str());
    textures::chest = LoadTexture((ASSET_PATH + "ui/inventory/chest.png").c_str());
    textures::helm = LoadTexture((ASSET_PATH + "ui/inventory/helm.png").c_str());
    textures::offhand =
        LoadTexture((ASSET_PATH + "ui/inventory/" + "offhand.png").c_str());
    textures::pants = LoadTexture((ASSET_PATH + "ui/inventory/pants.png").c_str());
    textures::relic = LoadTexture((ASSET_PATH + "ui/inventory/relic.png").c_str());
    textures::ring = LoadTexture((ASSET_PATH + "ui/inventory/ring.png").c_str());
    textures::weapon = LoadTexture((ASSET_PATH + "ui/inventory/weapon.png").c_str());

    textures::ui::bossbar =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/bossbar.png").c_str());
    textures::ui::skillbar::bag =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/bag.png").c_str());
    textures::ui::skillbar::abilities =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/abilities.png").c_str());
    textures::ui::skillbar::character =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/character.png").c_str());
    textures::ui::skillbar::map =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/map.png").c_str());
    textures::ui::skillbar::mouseleft =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/mouseleft.png").c_str());
    textures::ui::skillbar::mouseright =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/mouseright.png").c_str());
    textures::ui::skillbar::setting =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/settings.png").c_str());
    textures::ui::skillbar::skilltree =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/skilltree.png").c_str());
    textures::ui::skillbar::tooltip =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/tooltip_fancy.png").c_str());
    textures::ui::skillbar::skillbar =        LoadTexture((ASSET_PATH + "ui/skillbar/ui/skillbar.png").c_str());
    textures::ui::skillbar::xpbar =        LoadTexture((ASSET_PATH + "ui/skillbar/ui/xpbar.png").c_str());
    textures::ui::skillbar::tooltip =        LoadTexture((ASSET_PATH + "ui/skillbar/ui/xp_tooltip.png").c_str());

    textures::ui::skillbar::icons::infernoRay    = LoadTexture((ASSET_PATH+"ui/skillbar/icons/infernoRay.png").c_str());
    textures::ui::skillbar::icons::inspiration   = LoadTexture((ASSET_PATH+"ui/skillbar/icons/inspiration.png").c_str());
    textures::ui::skillbar::icons::lightning     = LoadTexture((ASSET_PATH+"ui/skillbar/icons/lightning.png").c_str());
    textures::ui::skillbar::icons::powerSurge    = LoadTexture((ASSET_PATH+"ui/skillbar/icons/powerSurge.png").c_str());
    textures::ui::skillbar::icons::pyroBlast     = LoadTexture((ASSET_PATH+"ui/skillbar/icons/pyroBlast.png").c_str());
    textures::ui::skillbar::icons::regenAura     = LoadTexture((ASSET_PATH+"ui/skillbar/icons/regenAura.png").c_str());
    textures::ui::skillbar::icons::ring_salvo    = LoadTexture((ASSET_PATH+"ui/skillbar/icons/ring_salvo.png").c_str());
    textures::ui::skillbar::icons::slimeBall     = LoadTexture((ASSET_PATH+"ui/skillbar/icons/slimeBall.png").c_str());
    textures::ui::skillbar::icons::solarFlare    = LoadTexture((ASSET_PATH+"ui/skillbar/icons/solarFlare.png").c_str());
    textures::ui::skillbar::icons::thunderSplash = LoadTexture((ASSET_PATH+"ui/skillbar/icons/thunderSplash.png").c_str());
    textures::ui::skillbar::icons::thunderStrike = LoadTexture((ASSET_PATH+"ui/skillbar/icons/thunderStrike.png").c_str());
    textures::ui::skillbar::icons::voidEruption  = LoadTexture((ASSET_PATH+"ui/skillbar/icons/voidEruption.png").c_str());
    textures::ui::skillbar::icons::voidField     = LoadTexture((ASSET_PATH+"ui/skillbar/icons/voidField.png").c_str());
    textures::ui::skillbar::icons::blastHammer   = LoadTexture((ASSET_PATH+"ui/skillbar/icons/blastHammer.png").c_str());
    textures::ui::skillbar::icons::energy_sphere = LoadTexture((ASSET_PATH+"ui/skillbar/icons/energy_sphere.png").c_str());
    textures::ui::skillbar::icons::fireBurst     = LoadTexture((ASSET_PATH+"ui/skillbar/icons/fireBurst.png").c_str());
    textures::ui::skillbar::icons::fireSword     = LoadTexture((ASSET_PATH+"ui/skillbar/icons/fireSword.png").c_str());


    textures::ui::toolTip = LoadTexture((ASSET_PATH+"ui/toolTip.png").c_str());
    textures::ui::buttonNormal = LoadTexture((ASSET_PATH+"ui/button_normal.png").c_str());
    textures::ui::buttonHovered = LoadTexture((ASSET_PATH+"ui/button_hovered.png").c_str());
    textures::ui::buttonPressed = LoadTexture((ASSET_PATH+"ui/button_pressed.png").c_str());


    textures::ui::buttonExpand = LoadTexture((ASSET_PATH+"ui/questpanel/button_expand.png").c_str());
    textures::ui::buttonExpandHovered = LoadTexture((ASSET_PATH+"ui/questpanel/button_expand_hovered.png").c_str());
    textures::ui::buttonCollapse = LoadTexture((ASSET_PATH+"ui/questpanel/button_collapse.png").c_str());
    textures::ui::buttonCollapseHovered = LoadTexture((ASSET_PATH+"ui/questpanel/button_collapse_hovered.png").c_str());

    textures::ui::dialogContinue = LoadTexture((ASSET_PATH+"ui/questpanel/dialog_continue.png").c_str());
    textures::ui::dialogContinue2 = LoadTexture((ASSET_PATH+"ui/questpanel/dialog_continue_2.png").c_str());
    textures::ui::dialogContinue3 = LoadTexture((ASSET_PATH+"ui/questpanel/dialog_continue_3.png").c_str());

    textures::ui::notificationWindow = LoadTexture((ASSET_PATH+"ui/prompts/notification_window.png").c_str());

    textures::ui::spendButtonNormal = LoadTexture((ASSET_PATH+"ui/inventory/characterPanel/plus_stat.png").c_str());
    textures::ui::spendButtonHovered = LoadTexture((ASSET_PATH+"ui/inventory/characterPanel/plus_stat_hovered.png").c_str());
    textures::ui::spendButtonPressed = LoadTexture((ASSET_PATH+"ui/inventory/characterPanel/plus_stat_pressed.png").c_str());


  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TEXTURELOADER_H_
