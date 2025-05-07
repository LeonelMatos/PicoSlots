#pragma once

#include "picosystem.hpp"
using namespace picosystem;
#include "sound.hpp"

#define DEBUG_GRID_SIZE 15

//Global Variables//
bool DEBUG_MODE = false;
bool SILENT_MODE = true;

//Configs
namespace Config {
    constexpr int START_CREDITS = 10;
    constexpr int JACKPOT_PAY = 10;
    constexpr int MATCH2_PAY = 2;
    constexpr uint32_t SPIN_DURATION_MS = 3000;
}

//Colors
struct Colors {
    color_t red = rgb(170,34,34);
    color_t yellow = rgb(238,238,51);
    color_t dark_green = rgb(68,102,34);
    color_t green = rgb(102,187,34);
    color_t grey = rgb(85,85,68);
    color_t white = rgb(255,255,255);
} color;