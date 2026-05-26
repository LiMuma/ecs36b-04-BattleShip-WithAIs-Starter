//
// Created by Max on 5/24/26.
//

#include "CheatingAI.h"

namespace BattleShip {
    CheatingAI::CheatingAI(const std::string& name, const GameConfig& game_config)
        : AIPlayer(name, game_config) {
    }

    std::pair<int, int> CheatingAI::get_firing_location(std::istream& in, std::ostream& out) {
        const Board& opponent_board = opponent().board();

        for (int row = 0; row < static_cast<int>(opponent_board.num_rows()); ++row) {
            for (int col = 0; col < static_cast<int>(opponent_board.num_cols()); ++col) {
                if (opponent_board.has_ship_at(row, col) &&
                    !opponent_board.has_been_fired_at(row, col)) {
                    return {row, col};
                    }
            }
        }

        return {0, 0};
    }
}