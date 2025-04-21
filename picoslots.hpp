#pragma once

#include "picosystem.hpp"

using namespace picosystem;

#define DEBUG_GRID_SIZE 15

//Configs
namespace Config {
    constexpr int START_CREDITS = 10;
    constexpr int JACKPOT_PAY = 10;
    constexpr int MATCH2_PAY = 2;
    constexpr uint32_t SPIN_DURATION_MS = 3000;
}

//State (not currently used)
enum WinState {
    NONE,
    DOUBLE,
    JACKPOT
} win_state;

//Colors
struct Colors {
    color_t red = rgb(170,34,34);
    color_t yellow = rgb(238,238,51);
    color_t dark_green = rgb(68,102,34);
    color_t green = rgb(102,187,34);
    color_t grey = rgb(85,85,68);
    color_t white = rgb(255,255,255);
} color;

// Audio Const

namespace Audio {
    extern const voice_t STARTUP_VOICE;
    extern const voice_t SPIN_VOICE;
    extern const voice_t WIN_VOICE;
    extern const voice_t REEL_STOP_VOICE;
}

// Musical Notes (Hz)
namespace Notes {
    constexpr float A4 = 440.0f;
    constexpr float C5 = 523.25f;
    constexpr float E5 = 659.25f;
    constexpr float A5 = 880.0f;
    constexpr float C6 = 1046.50f;
}