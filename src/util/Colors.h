#ifndef DUNGEONM_SRC_UTIL_COLORS_H_
#define DUNGEONM_SRC_UTIL_COLORS_H_

namespace Colors {
// QUALITIES
inline const Color setItem = {13, 210, 15, 255};
inline const Color mediumQuality = {27, 178, 1, 255};
inline const Color highQuality = {178, 14, 23, 255};
inline const Color rareColor = {26, 111, 175, 255};
inline const Color epicColor = {168, 93, 218, 255};
inline const Color legendaryColor = {239, 103, 3, 255};


// INTERFACE
inline const Color LightGreyAlpha = {192, 203, 220, 217};
inline const Color LightGreyTransparent = {150, 150, 150, 76};
inline const Color LightGreyLessTransp = {150, 150, 150, 128};
inline const Color darkBackground = {90, 105, 136, 255};
inline const Color xpbarOrange = {254, 132, 23, 229};
inline const Color LightGrey = {192, 203, 220, 255};
inline const Color mediumVeryLight = {172, 180, 205, 255};
inline const Color mediumLightGrey = {155, 169, 194, 255};
inline const Color Red = {255, 0, 68, 255};
inline const Color gray = {123, 123, 120, 255};
inline const Color button = {77, 19, 2, 229};
inline const Color lightGreyMiddleAlpha = {192, 203, 220, 153};
inline const Color questNameBeige = {228, 166, 114, 255};
inline const Color bossNamePurple = {114, 69, 144, 255};
inline const Color questMarkerYellow = {254, 231, 97, 255};
inline const Color journalBrown = {165, 120, 85, 255};

// MAP
inline const Color map_green = {99, 199, 77, 255};
inline const Color mediumLightGreyTransparent = {155, 169, 194, 229};
inline const Color blue_npc = {18, 78, 137, 255};
inline const Color black = {24, 20, 37, 255};

// Talents
inline const Color black_transparent = {24, 24, 24, 217};
inline const Color purple_dark = {31, 1, 73, 255};
inline const Color white = {252, 252, 252, 255};

// DAMAGE TYPES
inline const Color fire_red = {230, 34, 34, 255};
inline const Color arcane_blue = {106, 190, 250, 255};
inline const Color dark_magic_purple = {128, 0, 128, 255};
inline const Color physical_grey = {90, 105, 136, 255};
inline const Color poison_green = {105, 215, 44, 255};

// MISC
inline const Color transparent = {0, 0, 0, 76};  // Alpha 0.3 * 255 = 76
}  // namespace Colors
#endif  //DUNGEONM_SRC_UTIL_COLORS_H_
