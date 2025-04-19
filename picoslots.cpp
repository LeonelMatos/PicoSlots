#include "picosystem.hpp"
#include "picoslots.hpp"

using namespace picosystem;

//Game State
struct SlotMachine {
    int reels[3] = {1, 1, 1};
    int target_reels[3];
    bool is_spinning = false;
    uint32_t spin_start = 0;
    uint32_t spin_duration = 2000; //2s
    int credits = 10;
} game;

void init() {
    srand(time_us());

    play(1000,500,100);
}

void update(uint32_t tick) {
    //Start spin
    if (!game.is_spinning && pressed(DOWN)) {
        if (game.credits <= 0) 
            return;
        
        game.is_spinning = true;
        game.spin_start = time_us();
        game.credits--;

        for (int i = 0; i < 3; i++) {
            game.target_reels[i] = 1 + (rand() % 6); //fruits 1-6
        }
    }

    //Spin animation
    if (game.is_spinning) {
        uint32_t spin_time = time_us() - game.spin_start;

        //Current reel position
        float progress = std::min(1.0f, (float)spin_time / (float)game.spin_duration);
        float ease_progress = progress < 0.9f ? progress * 1.1f : 0.99f + (progress - 0.9f) * 0.1f;

        for (int i = 0; i < 3; i++) {
            if (progress < 0.9f) {
                game.reels[i] = 1 + ((game.reels[i] + (rand() % 3) % 6));
            }
            else {
                float reel_progress = (progress - 0.9f) / 0.1f;
                if (reel_progress > (i * 0.3f + 0.1f)) {
                    game.reels[i] = game.target_reels[i];
                }
            }
        }

        //Stop when time is up
        if (spin_time >= game.spin_duration) {
            game.is_spinning = false;
            //Jackpot
            if (game.reels[0] == game.reels[1] && game.reels[1] == game.reels[2]) {
                game.credits += 10;
            }
            //Two matching
            else if (game.reels[0] == game.reels[1] ||
                    game.reels[0] == game.reels[2]  ||
                    game.reels[1] == game.reels[2]) {
                        game.credits += 2;
                    }
        }
    }
}

void draw(uint32_t tick) {
    pen(0,0,0);
    clear();

    //Background
    pen(58, 39, 27);
    frect(0, 0, 120, 120);

    //Title
    pen(255, 255, 0);
    text("PICO SLOTS", 30, 5);

    //Credits Background
    for (int i = 0; i < 8; i++) {
        pen((i % 2 == 0) ? color.dark_green : color.green);
        frect(i * 15, 110, 15, 15);
    }

    //Credits
    pen(255, 255, 255);
    char credit_text[20];
    sprintf(credit_text, "Credits: %d", game.credits);
    text(credit_text, 5, 110);

    //Instructions
    pen(100, 100, 100);
    text("Press DOWN to spin", 15, 125);


    //Slot Frames
    pen(150,100,50);
    frect(20, 30, 80, 60);
    pen(100, 70, 30);
    frect(24, 34, 74, 54);

    //Reels
    for (int i = 0; i < 3; i++) {
        pen(0,0,0);
        frect(30 + i * 25, 40, 20, 40);
        pen(50,50,80);
        rect(30 + i * 25, 40, 20, 40);
        

        sprite(game.reels[i], 32 + i * 25, 42);
    }

    if (!game.is_spinning && time_us() - game.spin_start < 3000000 && game.spin_start != 0) {
        if (game.reels[0] == game.reels[1] && game.reels[1] == game.reels[2]) {
            pen(255,255,0);
            text("JACKPOT!", 35, 85);
        }
        else if (game.reels[0] == game.reels[1] ||
                game.reels[0] == game.reels[2]  ||
                game.reels[1] == game.reels[2]) {
                    pen(255,255,0);
                    text("WINNER!", 40, 85);
                }
    }

    
}