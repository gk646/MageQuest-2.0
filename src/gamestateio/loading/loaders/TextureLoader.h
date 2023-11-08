#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TEXTURELOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TEXTURELOADER_H_
struct TextureLoader {
  static void LoadEffectInfo() noexcept {
    auto table = Util::ReadMGI("mgi/Effects.mgi");
    for (const auto& vec : table) {
      effectToInfo.insert(
          {effectMap[vec[0]],
           {vec[1], vec[2], LoadTexture((ASSET_PATH + "ui/effects/" + vec[3]).c_str())}});
    }
  }
  static void Load() {
    LoadEffectInfo();
    shadowToTexture[ShadowType::TREE_GREEN_BUSH] =
        LoadTexture((ASSET_PATH + "textures/Shadows/0.png").c_str());
    shadowToTexture[ShadowType::TREE_GREEN_POINTY] =
        LoadTexture((ASSET_PATH + "textures/Shadows/9.png").c_str());
    shadowToTexture[ShadowType::TREE_GREEN_SWAMPY] =
        LoadTexture((ASSET_PATH + "textures/Shadows/10.png").c_str());
    shadowToTexture[ShadowType::ROCK_SMALL_1] =
        LoadTexture((ASSET_PATH + "textures/Shadows/1.png").c_str());
    shadowToTexture[ShadowType::ROCK_SMALL_2] =
        LoadTexture((ASSET_PATH + "textures/Shadows/6.png").c_str());
    shadowToTexture[ShadowType::ROCK_SMALL_3] =
        LoadTexture((ASSET_PATH + "textures/Shadows/7.png").c_str());
    shadowToTexture[ShadowType::ROCK_SMALL_4] =
        LoadTexture((ASSET_PATH + "textures/Shadows/8.png").c_str());

    textures::ui::questpanel::choiceBox =
        LoadTexture((ASSET_PATH + "ui/questpanel/choice_normal.png").c_str());
    textures::ui::questpanel::choiceBoxHovered =
        LoadTexture((ASSET_PATH + "ui/questpanel/choice_hovered.png").c_str());

    textures::ui::questpanel::questBox =
        LoadTexture((ASSET_PATH + "ui/questpanel/questBox.png").c_str());
    textures::ui::questpanel::questBoxHovered =
        LoadTexture((ASSET_PATH + "ui/questpanel/questBox_hovered.png").c_str());
    textures::ui::questpanel::questBoxPressed =
        LoadTexture((ASSET_PATH + "ui/questpanel/questBox_pressed.png").c_str());

    Lighting::Shaders::spotLight =
        LoadShader(nullptr, (ASSET_PATH + "Shaders/spot_light.fs").c_str());
    Lighting::Shaders::nightShader =
        LoadShader(nullptr, (ASSET_PATH + "Shaders/day_night.fs").c_str());
    Lighting::Shaders::postProcessing =
        LoadShader(nullptr, (ASSET_PATH + "Shaders/post_processing.fs").c_str());
    Lighting::Shaders::SPOT_LIGHT_COLOR =
        GetShaderLocation(Lighting::Shaders::spotLight, "lightColors");
    Lighting::Shaders::SPOT_LIGHT_POSITION =
        GetShaderLocation(Lighting::Shaders::spotLight, "lightPositions");
    Lighting::Shaders::SPOT_LIGHT_RADIUS =
        GetShaderLocation(Lighting::Shaders::spotLight, "radii");
    Lighting::Shaders::SPOT_LIGHT_NUM =
        GetShaderLocation(Lighting::Shaders::spotLight, "numLights");
    Lighting::Shaders::SPOT_LIGHT_TIME =
        GetShaderLocation(Lighting::Shaders::spotLight, "time");
    Lighting::Shaders::SPOT_LIGHT_ALPHA =
        GetShaderLocation(Lighting::Shaders::spotLight, "nightAlpha");
    Lighting::Shaders::SPOT_LIGHT_PLAYER =
        GetShaderLocation(Lighting::Shaders::spotLight, "playerPos");

    Lighting::Shaders::NIGHT_SHADER_ALPHA =
        GetShaderLocation(Lighting::Shaders::nightShader, "nightAlpha");
    Lighting::Shaders::NIGHT_SHADER_PLAYER =
        GetShaderLocation(Lighting::Shaders::nightShader, "playerPos");

    textures::ui::STATUS_BAR = LoadTexture((ASSET_PATH + "ui/player_ui3.png").c_str());
    textures::ui::CROSS_HAIR_AIM =
        LoadTexture((ASSET_PATH + "ui/crosshair_1.png").c_str());
    textures::ui::CROSS_HAIR_SELECT =
        LoadTexture((ASSET_PATH + "ui/crosshair_3.png").c_str());
    textures::ui::ETHER_MARK = LoadTexture((ASSET_PATH + "ui/ether_mark.png").c_str());
    textures::ui::HEALTH_BAR = LoadTexture((ASSET_PATH + "ui/healthbars.png").c_str());
    textures::ui::KEY_MAP = LoadTexture((ASSET_PATH + "ui/keymap.png").c_str());

    textures::ui::talentpanel::TALENT_BACKGROUND_TILE =
        LoadTexture((ASSET_PATH + "ui/talents/background.png").c_str());
    textures::ui::talentpanel::CONNECTION_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/connection_green.png").c_str());
    textures::ui::talentpanel::CONNECTION_ORANGE =
        LoadTexture((ASSET_PATH + "ui/talents/connection_orange.png").c_str());
    textures::ui::talentpanel::CONNECTION_RED =
        LoadTexture((ASSET_PATH + "ui/talents/connection_red.png").c_str());
    textures::ui::talentpanel::NODE_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/node_green.png").c_str());
    textures::ui::talentpanel::NODE_ORANGE =
        LoadTexture((ASSET_PATH + "ui/talents/node_orange.png").c_str());
    textures::ui::talentpanel::NODE_PURPLE =
        LoadTexture((ASSET_PATH + "ui/talents/node_purple.png").c_str());
    textures::ui::talentpanel::TALENT_BIG =
        LoadTexture((ASSET_PATH + "ui/talents/talent_big.png").c_str());
    textures::ui::talentpanel::TALENT_BIG_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/talent_big_green.png").c_str());
    textures::ui::talentpanel::TALENT_BIG_PURPLE =
        LoadTexture((ASSET_PATH + "ui/talents/talent_big_purple.png").c_str());
    textures::ui::talentpanel::TALENT_MID =
        LoadTexture((ASSET_PATH + "ui/talents/talent_mid.png").c_str());
    textures::ui::talentpanel::TALENT_MID_GREEN =
        LoadTexture((ASSET_PATH + "ui/talents/talent_mid_green.png").c_str());
    textures::ui::talentpanel::TALENT_MID_PURPLE =
        LoadTexture((ASSET_PATH + "ui/talents/talent_mid_purple.png").c_str());
    textures::ui::talentpanel::TALENT_NODE =
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

    textures::ui::skillbar::bag =        LoadTexture((ASSET_PATH + "ui/skillbar/ui/bag.png").c_str());
    textures::ui::skillbar::castbar =        LoadTexture((ASSET_PATH + "ui/skillbar/ui/castbar.png").c_str());

    textures::ui::skillbar::abilities =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/abilities.png").c_str());

    textures::ui::skillbar::character =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/character.png").c_str());

    textures::ui::skillbar::map =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/map.png").c_str());

    textures::ui::skillbar::map_pressed =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/map_pressed.png").c_str());

    textures::ui::skillbar::setting =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/settings.png").c_str());
    textures::ui::skillbar::setting_pressed =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/settings_pressed.png").c_str());

    textures::ui::skillbar::skilltree =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/skilltree.png").c_str());

    textures::ui::skillbar::skilltree_pressed =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/skilltree_pressed.png").c_str());

    textures::ui::skillbar::tooltip =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/tooltip_fancy.png").c_str());
    textures::ui::skillbar::mouseleft =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/mouseleft.png").c_str());
    textures::ui::skillbar::mouseright =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/mouseright.png").c_str());
    textures::ui::skillbar::skillbar =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/skillbar.png").c_str());
    textures::ui::skillbar::xpbar =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/xpbar.png").c_str());
    textures::ui::skillbar::tooltip =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/xp_tooltip.png").c_str());
    textures::ui::skillbar::skillRange =
        LoadTexture((ASSET_PATH + "ui/skillbar/ui/skill_range.png").c_str());

    textures::ui::skillbar::icons::infernoRay =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/infernoRay.png").c_str());
    textures::ui::skillbar::icons::frostNova =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/frostNova.png").c_str());
    textures::ui::skillbar::icons::arcaneBolt =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/arcaneBolt.png").c_str());
    textures::ui::skillbar::icons::locked =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/locked.png").c_str());
    textures::ui::skillbar::icons::inspiration =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/inspiration.png").c_str());
    textures::ui::skillbar::icons::lightning =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/lightning.png").c_str());
    textures::ui::skillbar::icons::powerSurge =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/powerSurge.png").c_str());
    textures::ui::skillbar::icons::pyroBlast =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/pyroBlast.png").c_str());
    textures::ui::skillbar::icons::regenAura =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/regenAura.png").c_str());
    textures::ui::skillbar::icons::ring_salvo =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/ring_salvo.png").c_str());
    textures::ui::skillbar::icons::slimeBall =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/slimeBall.png").c_str());
    textures::ui::skillbar::icons::solarFlare =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/solarFlare.png").c_str());
    textures::ui::skillbar::icons::thunderSplash =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/thunderSplash.png").c_str());
    textures::ui::skillbar::icons::thunderStrike =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/thunderStrike.png").c_str());
    textures::ui::skillbar::icons::voidEruption =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/voidEruption.png").c_str());
    textures::ui::skillbar::icons::voidField =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/voidField.png").c_str());
    textures::ui::skillbar::icons::blastHammer =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/blastHammer.png").c_str());
    textures::ui::skillbar::icons::energy_sphere =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/energy_sphere.png").c_str());
    textures::ui::skillbar::icons::fireBurst =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/fireBurst.png").c_str());
    textures::ui::skillbar::icons::fireSword =
        LoadTexture((ASSET_PATH + "ui/skillbar/icons/fireSword.png").c_str());

    textures::ui::toolTip = LoadTexture((ASSET_PATH + "ui/toolTip.png").c_str());
    textures::ui::buttonNormal =
        LoadTexture((ASSET_PATH + "ui/button_normal.png").c_str());
    textures::ui::buttonHovered =
        LoadTexture((ASSET_PATH + "ui/button_hovered.png").c_str());
    textures::ui::buttonPressed =
        LoadTexture((ASSET_PATH + "ui/button_pressed.png").c_str());

    textures::ui::buttonExpand =
        LoadTexture((ASSET_PATH + "ui/questpanel/button_expand.png").c_str());
    textures::ui::buttonExpandHovered =
        LoadTexture((ASSET_PATH + "ui/questpanel/button_expand_hovered.png").c_str());
    textures::ui::buttonCollapse =
        LoadTexture((ASSET_PATH + "ui/questpanel/button_collapse.png").c_str());
    textures::ui::buttonCollapseHovered =
        LoadTexture((ASSET_PATH + "ui/questpanel/button_collapse_hovered.png").c_str());

    textures::ui::dialogContinue =
        LoadTexture((ASSET_PATH + "ui/questpanel/dialog_continue.png").c_str());
    textures::ui::dialogContinue2 =
        LoadTexture((ASSET_PATH + "ui/questpanel/dialog_continue_2.png").c_str());
    textures::ui::dialogContinue3 =
        LoadTexture((ASSET_PATH + "ui/questpanel/dialog_continue_3.png").c_str());

    textures::ui::notificationWindow =
        LoadTexture((ASSET_PATH + "ui/prompts/notification_window.png").c_str());

    textures::ui::spendButtonNormal =
        LoadTexture((ASSET_PATH + "ui/inventory/characterPanel/plus_stat.png").c_str());
    textures::ui::spendButtonHovered = LoadTexture(
        (ASSET_PATH + "ui/inventory/characterPanel/plus_stat_hovered.png").c_str());
    textures::ui::spendButtonPressed = LoadTexture(
        (ASSET_PATH + "ui/inventory/characterPanel/plus_stat_pressed.png").c_str());
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_TEXTURELOADER_H_
