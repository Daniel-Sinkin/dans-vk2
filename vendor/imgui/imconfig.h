// vendor/imgui/imconfig.h
// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#pragma once














//---- Use FreeType + plutosvg or lunasvg to render OpenType SVG fonts (SVGinOT)
// Only works in combination with IMGUI_ENABLE_FREETYPE.
// - plutosvg is currently easier to install, as e.g. it is part of vcpkg. It will support more fonts and may load them faster. See misc/freetype/README for instructions.
// - Both require headers to be available in the include path + program to be linked with the library code (not provided).
// - (note: lunasvg implementation is based on Freetype's rsvg-port.c which is licensed under CeCILL-C Free Software License Agreement)
//#define IMGUI_ENABLE_FREETYPE_PLUTOSVG
//#define IMGUI_ENABLE_FREETYPE_LUNASVG









