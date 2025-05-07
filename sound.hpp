//Sound all in one
#pragma once 

#include "picosystem.hpp"
using namespace picosystem;
#include <unordered_map>
#include <functional>


extern bool DEBUG_MODE;
extern bool SILENT_MODE;

const voice_t STARTUP_VOICE = voice(50, 100, 70, 200, 5, 50, 20);
const voice_t SPIN_VOICE = voice(10, 50, 0, 100, -20, 100, 0, 10, 5);
const voice_t WIN_VOICE = voice(10, 100, 80, 300, 10, 100, 50, 5, 10);
const voice_t REEL_STOP_VOICE = voice(5, 30, 50, 100, 0, 0, 10);

// Musical Notes (Hz)
namespace Notes {
    constexpr float A4 = 440.0f;
    constexpr float C5 = 523.25f;
    constexpr float E5 = 659.25f;
    constexpr float A5 = 880.0f;
    constexpr float C6 = 1046.50f;
}

//Sound functions//
inline void startup_jingle() {
    play(STARTUP_VOICE, Notes::A4, 200, 100);
    play(STARTUP_VOICE, Notes::C5, 200, 100);
    play(STARTUP_VOICE, Notes::C6, 200, 100);
    play(STARTUP_VOICE, Notes::E5, 200, 100);
}

inline void reel1_stop() {
    play(REEL_STOP_VOICE, Notes::C5, 300, 100);
}
inline void reel2_stop() {
    play(REEL_STOP_VOICE, Notes::E5, 300, 100);
}
inline void reel3_stop() {
    play(REEL_STOP_VOICE, Notes::A5, 300, 100);
}

inline void win_jackpot() {
    play(WIN_VOICE, Notes::C6, 500, 90);
}

inline void win_double() {
    play(WIN_VOICE, Notes::E5, 300, 80);
}

//Lookup table//
inline const std::unordered_map<std::string, std::function<void()>> sound_lookup = {
    {"startup_jingle", startup_jingle},
    {"reel1_stop", reel1_stop},
    {"reel2_stop", reel2_stop},
    {"reel3_stop", reel3_stop},
    {"win_jackpot", win_jackpot},
    {"win_double", win_double},
};

//Sound wrapper//
inline void play_sound(const std::string &name) {
    if (SILENT_MODE) return;
    if (auto it = sound_lookup.find(name); it != sound_lookup.end()) {
        it->second();
    }
}