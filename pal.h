#pragma once
// Pal — HitPaw "Ember" palette (v3.6 redesign).
// Single source of truth for colors; main.cpp should #include "pal.h" instead
// of duplicating the namespace. Keeps UI theming consistent across modules.
//
// Redesign notes (Ember):
//  - Surfaces shifted from cold blue-black greys to WARM brown-blacks so the
//    orange accent feels native instead of pasted-on.
//  - Contrast tiers widened: TEXT -> warm off-white, MUTED -> warm grey,
//    FAINT for placeholder/disabled. Borders are softer and sit closer to the
//    card body, so the grid reads as breathing instead of wireframed.
//  - ACCENT2 (secondary) moved from Tokyo-Night purple to warm amber. It is
//    used by the tab gradient, checkbox gradient and the card hover glow —
//    everything now glows in the same warm family.
//  - Same constant names as v3.5.0, so no call sites change; new tokens are
//    purely additive (HOVER, FAINT, SAND, GOLD, SKELETON, SEL_BG, SEL_BORDER).

namespace Pal {
    // ── Surfaces (warm dark) ────────────────────────────────────────────
    constexpr auto BG       = "#0d0b09";   // page — warm near-black
    constexpr auto SURFACE  = "#151210";   // nav rail / input chrome
    constexpr auto CARD     = "#1a1613";   // card body
    constexpr auto ELEV     = "#201b17";   // elevated card / popovers
    constexpr auto HOVER    = "#1c1714";   // subtle hover fill (nav, rows)
    constexpr auto ENTRY    = "#14100d";   // input field bg
    constexpr auto CODE     = "#0f0c0a";   // code block / terminal bg
    constexpr auto SKELETON = "#1e1915";   // loading placeholder fill

    // ── Borders (soft, warm) ───────────────────────────────────────────
    constexpr auto BORDER   = "#2a231c";   // default border
    constexpr auto BORDER_H = "#3d3227";   // hover border

    // ── Primary — HitPaw orange (identity kept) ────────────────────────
    constexpr auto ACCENT   = "#ff6a00";   // primary orange
    constexpr auto ACCENT_H = "#ff8a3d";   // hover orange
    constexpr auto ACCENT_D = "#d55400";   // pressed
    constexpr auto ON_ACCENT= "#201004";   // dark text on orange
    constexpr auto SEL_BG   = "#241505";   // selected-card tint (gradient end)
    constexpr auto SEL_BORDER = "#66350f"; // selected-card border

    // ── Secondary — warm amber (was purple) ────────────────────────────
    constexpr auto ACCENT2  = "#f5b87a";   // amber secondary
    constexpr auto SAND     = "#e8c68a";   // on-hold / info tone
    constexpr auto GOLD     = "#f5d491";   // re-reading tone

    // ── Text ───────────────────────────────────────────────────────────
    constexpr auto TEXT     = "#f5efe8";   // body text — warm off-white
    constexpr auto MUTED    = "#94867a";   // muted text — warm grey
    constexpr auto FAINT    = "#6a5f54";   // placeholders / disabled text

    // ── Semantic ───────────────────────────────────────────────────────
    constexpr auto GREEN    = "#86c86f";   // success (softer leaf)
    constexpr auto RED      = "#ef6e5e";   // error (warm coral)
}
