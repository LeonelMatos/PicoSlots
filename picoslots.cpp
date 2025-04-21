#include "picosystem.hpp"
#include "picoslots.hpp"

using namespace picosystem;

bool DEBUG_MODE = false;

//Game State
struct SlotMachine {
    int reels[3] = {1, 1, 1};
    int target_reels[3];
    uint32_t win_display_start = 0;
    bool has_win = false;
    bool is_spinning = false;
    int reels_stopped = 0;
    uint32_t spin_start = 0;
    uint32_t spin_duration = Config::SPIN_DURATION_MS;
    uint32_t reel_stop_delay = 1000;
    int credits = Config::START_CREDITS;
    float lever_position = 0.0f;
    bool lever_pressed = false;
    
} game;

const voice_t Audio::STARTUP_VOICE = voice(50, 100, 70, 200, 5, 50, 20);
const voice_t Audio::SPIN_VOICE = voice(10, 50, 0, 100, -20, 100, 0, 10, 5);
const voice_t Audio::WIN_VOICE = voice(10, 100, 80, 300, 10, 100, 50, 5, 10);
const voice_t Audio::REEL_STOP_VOICE = voice(5, 30, 50, 100, 0, 0, 10);

void startup_jingle() {
    play(Audio::STARTUP_VOICE, Notes::A4, 200, 100);
    play(Audio::STARTUP_VOICE, Notes::C5, 200, 100);
    play(Audio::STARTUP_VOICE, Notes::C6, 200, 100);
    play(Audio::STARTUP_VOICE, Notes::E5, 200, 100);
}

void draw_debug_grid() {
    pen(30,30,30);
    for(int x = 0; x < 120; x += DEBUG_GRID_SIZE) vline(x, 0, 120);
    for(int y = 0; y < 120; y += DEBUG_GRID_SIZE) hline(0, y, 120);
}

void init() {
    srand(time_us());
    startup_jingle();
}

void start_spin() {
    game.is_spinning = true;
    game.spin_start = time_us();
    game.reels_stopped = 0;
    game.credits--;
    
    for (int i = 0; i < 3; i++) {
        game.target_reels[i] = 1 + (rand() % 6);
    }
}

void update_lever() {
    const float lever_speed = 0.1f;
    
    // Lever control using spin state and button press
    if (game.is_spinning) {
        // Lever stays down while spinning
        game.lever_position = 1.0f;
    } 
    else if (button(DOWN)) {
        // Pressing down moves lever
        game.lever_position = std::min(1.0f, game.lever_position + lever_speed);
        
        // Start spin when fully pressed
        if (game.lever_position >= 1.0f && game.credits > 0) {
            start_spin();
        }
    }
    else {
        // Return lever up when not pressed
        game.lever_position = std::max(0.0f, game.lever_position - lever_speed);
    }
}

void update(uint32_t tick) {

    if (pressed(A)) DEBUG_MODE = !DEBUG_MODE;

    update_lever();

    //Spin animation
    if (game.is_spinning) {
        uint32_t spin_time = (time_us() - game.spin_start) / 1000;

        //Calc reels stop timing
        uint32_t reel1_stop = game.spin_duration / 2;
        uint32_t reel2_stop = reel1_stop + game.reel_stop_delay;
        uint32_t reel3_stop = reel2_stop + game.reel_stop_delay;

        for (int i = 0; i < 3; i++) {
            if ((i == 0 && spin_time < reel1_stop) ||
                (i == 1 && spin_time < reel2_stop) ||
                (i == 2 && spin_time < reel3_stop)) {
                game.reels[i] = 1 + (time_us() / 10000 + i) % 6;
            }
        }

        if (spin_time >= reel1_stop && game.reels_stopped == 0) {
            game.reels[0] = game.target_reels[0];
            game.reels_stopped = 1;
            play(Audio::REEL_STOP_VOICE, Notes::C5, 300, 100);
        }
        if (spin_time >= reel2_stop && game.reels_stopped == 1) {
            game.reels[1] = game.target_reels[1];
            game.reels_stopped = 2;
            play(Audio::REEL_STOP_VOICE, Notes::E5, 300, 100);
        }
        if (spin_time >= reel3_stop && game.reels_stopped == 2) {
            game.reels[2] = game.target_reels[2];
            game.reels_stopped = 3;
            play(Audio::REEL_STOP_VOICE, Notes::A5, 300, 100);
        }

        if (game.reels_stopped == 3) {
            game.is_spinning = false;
            game.has_win = false;
            
            // Exact win condition checks
            bool jackpot = (game.reels[0] == game.reels[1]) && 
                          (game.reels[1] == game.reels[2]);
            bool double_win = (game.reels[0] == game.reels[1]) || 
                            (game.reels[1] == game.reels[2]) || 
                            (game.reels[0] == game.reels[2]);

            if (jackpot) {
                game.has_win = true;
                game.credits += Config::JACKPOT_PAY;
                play(Audio::WIN_VOICE, Notes::C6, 500, 90);
            } 
            else if (double_win) {
                game.has_win = true;
                game.credits += Config::MATCH2_PAY;
                play(Audio::WIN_VOICE, Notes::E5, 300, 80);
            }
            
            game.win_display_start = time_us();
            game.reels_stopped = 0; // Reset for next spin
        } 
    }
}

void draw(uint32_t tick) {
    //Background
    pen(0,0,0);
    frect(0, 0, 120, 120);

    //Title Bar
    pen(color.white);
    frect(28,0,69,18);
    pen(color.red);
    frect(30, 0, 65, 16);

    //Title
    pen(255, 255, 0);
    text("PICO SLOTS", 31, 5);

    //Credits Background
    for (int i = 0; i < 8; i++) {
        pen((i % 2 == 0) ? color.dark_green : color.green);
        frect(i * 15, 110, 15, 15);
    }

    //Credits
    pen(255, 255, 255);
    char credit_text[20];
    sprintf(credit_text, "Credits: %d", game.credits);
    text(credit_text, 5, 112);

    //Main Background
    pen(255,255,255);
    frect(15, 30, 90, 60);


    //Reels
    for(int i = 0; i < 3; i++) {
        int slot_x = 30 + i * 21;
        
        // Draw slot frame
        pen(50, 50, 80);
        frect(slot_x - 1, 42, 17, 35);
        
        // Highlight stopping reel
        if (game.is_spinning && game.reels_stopped == i) {
            pen(255, 255, 100);
            rect(slot_x - 2, 41, 19, 37);
        }
        
        // Draw three visible symbols (top, middle, bottom)
        sprite(game.reels[i], slot_x, 36); // Top
        sprite(game.reels[i], slot_x, 52); // Middle
        sprite(game.reels[i], slot_x, 68); // Bottom
    }

    //Winning texts
    pen(color.yellow);
    if (game.has_win && (time_us() - game.win_display_start < 3000000)) {
        if (game.reels[0] == game.reels[1] && game.reels[1] == game.reels[2]) {
            text("JACKPOT!", 35, 85);
        }
        else {
            text("WINNER!", 40, 85);
        }
    }

    //Lever
    float lever_y = 37.0f + (43.0f * game.lever_position);
    pen(color.yellow);
    fcircle(112, lever_y, 10);

    if (DEBUG_MODE)
        draw_debug_grid();

    
}