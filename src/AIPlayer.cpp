#include "AIPlayer.h"

#include <random>

namespace BattleShip {
    std::mt19937 AIPlayer::rng;

    AIPlayer::AIPlayer(const std::string& name, const GameConfig& game_config)
        : Player(name, game_config) {
    }

    void AIPlayer::seed_rng(unsigned int seed) {
        rng.seed(seed);
    }

    void AIPlayer::place_ships(std::istream& in, std::ostream& out) {
        (void)in;

        for (const auto [ship_name, ship_length] : ship_healths_) {
            while (true) {
                ShipPlacement placement = make_random_placement(ship_name, ship_length);

                if (board_.can_ship_be_placed(placement)) {
                    board_.place_ship(placement);

                    out << name() << "'s Board\n";
                    out << board_.view_as_visible() << std::endl;

                    break;
                }
            }
        }
    }

    ShipPlacement AIPlayer::make_random_placement(char ship_name, int ship_length) {
        std::uniform_int_distribution<int> orientation_dist(0, 1);
        int orientation_value = orientation_dist(rng);

        Orientation orientation = orientation_value == 0
            ? Orientation::HORIZONTAL
            : Orientation::VERTICAL;

        int max_row;
        int max_col;

        if (orientation == Orientation::HORIZONTAL) {
            max_row = static_cast<int>(board_.num_rows()) - 1;
            max_col = static_cast<int>(board_.num_cols()) - ship_length;
        } else {
            max_row = static_cast<int>(board_.num_rows()) - ship_length;
            max_col = static_cast<int>(board_.num_cols()) - 1;
        }

        std::uniform_int_distribution<int> row_dist(0, max_row);
        std::uniform_int_distribution<int> col_dist(0, max_col);

        int row = row_dist(rng);
        int col = col_dist(rng);

        return ShipPlacement(ship_name, ship_length, row, col, orientation);
    }
}