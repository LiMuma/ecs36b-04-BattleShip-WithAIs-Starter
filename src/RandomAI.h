//
// Created by Max on 5/24/26.
//

#ifndef ECS36B_04_BATTLESHIP_WITHAIS_STARTER_RANDOMAI_H
#define ECS36B_04_BATTLESHIP_WITHAIS_STARTER_RANDOMAI_H

#include "AIPlayer.h"

#include <vector>
#include <utility>

namespace BattleShip {
    class RandomAI : public AIPlayer {
    public:
        RandomAI(const std::string& name, const GameConfig& game_config);


        std::pair<int, int> get_firing_location(std::istream& in, std::ostream& out) override;

    protected:
        std::vector<std::pair<int, int>> firing_locations_;
        bool has_shuffled_;

        void create_firing_locations();
        std::pair<int, int> get_random_location();
        void remove_location_from_random_pool(const std::pair<int, int>& location);
    };
}
#endif //ECS36B_04_BATTLESHIP_WITHAIS_STARTER_RANDOMAI_H
