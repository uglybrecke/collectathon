#include <bn_backdrop.h>
#include <bn_core.h>
#include <bn_display.h>
#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_random.h>
#include <bn_rect.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_size.h>
#include <bn_string.h>

#include "bn_sprite_items_dot.h"
#include "bn_sprite_items_square.h"
#include "common_fixed_8x16_font.h"

#include "bn_sprite_items_hot.h"

// Pixels / Frame player moves at
static constexpr bn::fixed SPEED = 2;
static constexpr bn::fixed BOOST_SPEED = 4;

// Width and height of the the player and treasure bounding boxes
static constexpr bn::size PLAYER_SIZE = {8, 8};
static constexpr bn::size TREASURE_SIZE = {8, 8};

// Full bounds of the screen
static constexpr int MIN_Y = -bn::display::height() / 2;
static constexpr int MAX_Y = bn::display::height() / 2;
static constexpr int MIN_X = -bn::display::width() / 2;
static constexpr int MAX_X = bn::display::width() / 2;

// Number of characters required to show the longest numer possible in an int (-2147483647)
static constexpr int MAX_SCORE_CHARS = 11;

// Score location
static constexpr int SCORE_X = 70;
static constexpr int SCORE_Y = -70;

// Player start location
static constexpr int PLAYER_INITIAL_X = 0;
static constexpr int PLAYER_INITIAL_Y = 0;

// Treasure start location
static constexpr int TREASURE_INITIAL_X = 0;
static constexpr int TREASURE_INITIAL_Y = -25;

int main()
{
    bn::core::init();

    bn::random rng = bn::random();

    bn::backdrop::set_color(bn::color(5, 0, 8));

    // Will hold the sprites for the score
    bn::vector<bn::sprite_ptr, MAX_SCORE_CHARS> score_sprites = {};
    bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);

    int score = 0;
    int boosts = 3; //int of 3 boosts

    int boost_timer = 0; // create a counter variable

    // pressing _A will start a "counter" if boosts is greater than 0;
    // pressing _A will subtract 1 from the boosts if greater than 0;
    // plan to make two seperate paths for movement
    // make a new speed variable to hold boost    

    bn::sprite_ptr player = bn::sprite_items::square.create_sprite(PLAYER_INITIAL_X, PLAYER_INITIAL_Y);
    bn::sprite_ptr treasure = bn::sprite_items::dot.create_sprite(TREASURE_INITIAL_X, TREASURE_INITIAL_Y);

    //make 3 boost squares
    bn::sprite_ptr boost_1 = bn::sprite_items::hot.create_sprite(-110, -70);
    bn::sprite_ptr boost_2 = bn::sprite_items::hot.create_sprite(-100, -70);
    bn::sprite_ptr boost_3 = bn::sprite_items::hot.create_sprite(-90, -70);

    while (true)
    {
        //figure out how to place a "boost square on top of the screen"


        if (bn::keypad::a_pressed()) {
            if (boosts > 0) {
                boosts--;
                boost_timer += 360; // (add 6 seconds of boost)
            }
        }

        //with boost
        if (boost_timer > 0) {
            // Move player with d-pad
            if (bn::keypad::left_held())
            {
                player.set_x(player.x() - BOOST_SPEED);
            }
            if (bn::keypad::right_held())
            {
                player.set_x(player.x() + BOOST_SPEED);
            }
            if (bn::keypad::up_held())
            {
                player.set_y(player.y() - BOOST_SPEED);
            }
            if (bn::keypad::down_held())
            {
                player.set_y(player.y() + BOOST_SPEED);
            }
        //without boost
        } else {
            // Move player with d-pad
            if (bn::keypad::left_held())
            {
                player.set_x(player.x() - SPEED);
            }
            if (bn::keypad::right_held())
            {
                player.set_x(player.x() + SPEED);
            }
            if (bn::keypad::up_held())
            {
                player.set_y(player.y() - SPEED);
            }
            if (bn::keypad::down_held())
            {
                player.set_y(player.y() + SPEED);
            }
        }

        //restart game button
        if (bn::keypad::start_pressed())
        {
            player.set_position(PLAYER_INITIAL_X, PLAYER_INITIAL_Y);
            treasure.set_position(TREASURE_INITIAL_X, TREASURE_INITIAL_Y);
            score = 0;
            boost_timer = 0;
            boosts = 3;
        }

        // logic for crossing outside of screen bounds
        //if greater set to min
        if (player.x() > MAX_X) {
            player.set_x(MIN_X);
        }
        //if smaller set to max
        if (player.x() < MIN_X) {
            player.set_x(MAX_X);
        }
        //if greater set to min
        if (player.y() > MAX_Y) {
            player.set_y(MIN_Y);
        }
        //if smaller set to max
        if (player.y() < MIN_Y) {
            player.set_y(MAX_Y);
        }

        // The bounding boxes of the player and treasure, snapped to integer pixels
        bn::rect player_rect = bn::rect(player.x().round_integer(),
                                        player.y().round_integer(),
                                        PLAYER_SIZE.width(),
                                        PLAYER_SIZE.height());
        bn::rect treasure_rect = bn::rect(treasure.x().round_integer(),
                                          treasure.y().round_integer(),
                                          TREASURE_SIZE.width(),
                                          TREASURE_SIZE.height());

        // If the bounding boxes overlap, set the treasure to a new location an increase score
        if (player_rect.intersects(treasure_rect))
        {
            // Jump to any random point in the screen
            int new_x = rng.get_int(MIN_X, MAX_X);
            int new_y = rng.get_int(MIN_Y, MAX_Y);
            treasure.set_position(new_x, new_y);

            score++;
        }

        // Update score display
        bn::string<MAX_SCORE_CHARS> score_string = bn::to_string<MAX_SCORE_CHARS>(score);
        score_sprites.clear();
        text_generator.generate(SCORE_X, SCORE_Y,
                                score_string,
                                score_sprites);

        //reduce boost timer
        if (boost_timer > 0) {
            boost_timer--;
        }

        // Update RNG seed every frame so we don't get the same sequence of positions every time
        rng.update();

        bn::core::update();
    }
}