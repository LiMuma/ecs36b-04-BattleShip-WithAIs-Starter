//
// Created by Max on 5/24/26.
//

#include "SearchAndDestroyAI.h"

namespace BattleShip {
    SearchAndDestroyAI::SearchAndDestroyAI(const std::string& name, const GameConfig& game_config)
        : RandomAI(name, game_config) {
    }

    std::pair<int, int> SearchAndDestroyAI::get_firing_location(std::istream& in, std::ostream& out) {
        if (!destroy_targets_.empty()) {
            auto target = destroy_targets_.front();
            destroy_targets_.pop_front();
            planned_targets_.erase(target);
            return target;
        }

        return get_random_location();
    }

    void SearchAndDestroyAI::notify_firing_result(int row, int col, const FiringResult& result) {
        if (result.is_hit()) {
            add_neighbors(row, col);
        }
    }

    void SearchAndDestroyAI::add_neighbors(int row, int col) {
        // Left
        try_add_target(row, col - 1);

        // Up
        try_add_target(row - 1, col);

        // Right
        try_add_target(row, col + 1);

        // Down
        try_add_target(row + 1, col);
    }

    void SearchAndDestroyAI::try_add_target(int row, int col) {
        const Board& opponent_board = opponent().board();

        if (!opponent_board.is_on(row, col)) {
            return;
        }

        if (opponent_board.has_been_fired_at(row, col)) {
            return;
        }

        std::pair<int, int> target{row, col};

        if (planned_targets_.contains(target)) {
            return;
        }

        destroy_targets_.push_back(target);
        planned_targets_.insert(target);

        remove_location_from_random_pool(target);
    }

    void SearchAndDestroyAI::place_ships(std::istream& in, std::ostream& out) {
        AIPlayer::place_ships(in, out);
    }
}