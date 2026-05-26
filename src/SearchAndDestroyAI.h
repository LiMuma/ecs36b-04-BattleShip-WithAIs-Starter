//
// Created by Max on 5/24/26.
//

#ifndef ECS36B_04_BATTLESHIP_WITHAIS_STARTER_SEARCHANDDESTROYAI_H
#define ECS36B_04_BATTLESHIP_WITHAIS_STARTER_SEARCHANDDESTROYAI_H

#include "RandomAI.h"

#include <deque>
#include <set>

namespace BattleShip {
    class SearchAndDestroyAI : public RandomAI {
    public:
        SearchAndDestroyAI(const std::string& name, const GameConfig& game_config);

        std::pair<int, int> get_firing_location(std::istream& in, std::ostream& out) override;

        void notify_firing_result(int row, int col, const FiringResult& result) override;

        void place_ships(std::istream& in, std::ostream& out) override;

    private:
        std::deque<std::pair<int, int>> destroy_targets_;
        std::set<std::pair<int, int>> planned_targets_;

        void add_neighbors(int row, int col);
        void try_add_target(int row, int col);
    };
}

#endif //ECS36B_04_BATTLESHIP_WITHAIS_STARTER_SEARCHANDDESTROYAI_H
