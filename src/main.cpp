#include <ctime>
#include <fstream>
#include <iostream>
#include <print>

#include "AIPlayer.h"
#include "Game.h"
#include "GameConfig.h"

int main(int argc, char** argv) {
    if (argc != 2 && argc != 3) {
        std::println("Usage: ./BattleShip config_file [seed]");
        return 1;
    }

    std::ifstream config_file(argv[1]);

    if (!config_file.is_open()) {
        std::cerr << "Could not open config file: " << argv[1] << std::endl;
        return 1;
    }

    BattleShip::GameConfig game_config(config_file);

    unsigned int seed;

    if (argc == 3) {
        seed = static_cast<unsigned int>(std::stoul(argv[2]));
    } else {
        seed = static_cast<unsigned int>(std::time(nullptr));
    }

    BattleShip::AIPlayer::seed_rng(seed);
    BattleShip::Game game(game_config, std::cin, std::cout);
    game.play();

    return 0;
}