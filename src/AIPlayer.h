#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"

#include <random>

namespace BattleShip {
    class AIPlayer : public Player {
    public:
        AIPlayer(const std::string& name, const GameConfig& game_config);

        static void seed_rng(unsigned int seed);

        void place_ships(std::istream& in, std::ostream& out) override;

    protected:
        static std::mt19937 rng;

        ShipPlacement make_random_placement(char ship_name, int ship_length);
    };
}

#endif