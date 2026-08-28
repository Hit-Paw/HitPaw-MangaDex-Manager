#pragma once
// Pal — HitPaw palette extracted from main.cpp:98 for modularization.
// Single source of truth for colors; main.cpp should #include "pal.h" instead
// of duplicating the namespace. Keeps UI theming consistent across modules.

namespace Pal {
    constexpr auto BG       = "#0a0a0c";   // outer shell — near-black
    constexpr auto SURFACE  = "#0e0e10";   // nav rail / inputs
    constexpr auto CARD     = "#141416";   // card body
    constexpr auto ELEV     = "#1b1b1e";   // elevated card
    constexpr auto BORDER   = "#242429";   // default border
    constexpr auto BORDER_H = "#3c3c43";   // hover border
    constexpr auto ACCENT   = "#ff6a00";   // primary orange
    constexpr auto ACCENT_H = "#ff8c3a";   // hover orange
    constexpr auto ACCENT_D = "#cc5500";   // pressed
    constexpr auto ACCENT2  = "#BB9AF7";   // purple secondary
    constexpr auto ON_ACCENT= "#1a0800";   // dark text on orange
    constexpr auto TEXT     = "#D3DBE8";   // body text
    constexpr auto MUTED    = "#83838c";   // muted text
    constexpr auto GREEN    = "#9ECE6A";   // success
    constexpr auto RED      = "#F7768E";   // error
    constexpr auto ENTRY    = "#0a0a0c";   // input field bg
    constexpr auto CODE     = "#111113";   // code block bg
}
