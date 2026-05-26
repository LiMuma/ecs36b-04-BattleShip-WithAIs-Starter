//
// Created by mfbut on 4/24/2026.
//

#include "Player.h"

#include <algorithm>
#include <format>
#include <optional>
#include <ranges>
#include <sstream>
#include <unordered_map>

#include "ExtraStringMethods.h"

namespace BattleShip {

    Player::Player(const std::string& name, const GameConfig& game_config)
    : name_(name),
      board_(
          game_config.board_num_rows(),
          game_config.board_num_cols()
      ),
      ship_healths_(game_config.ship_healths()),
      opponent_(nullptr) {
    }

    void Player::notify_firing_result(int row, int col, const FiringResult& result) {
        (void)row;
        (void)col;
        (void)result;
    }

    /**
     * Get this player's name
     * @return : this player's name
     */
    const std::string& Player::name() const {
        return name_;
    }

    /**
     * Get this player's board
     * @return : this player's board
     */
    const Board& Player::board() const {
        return board_;
    }

    /**
     * Get this player's opponent
     * @return : this player's opponent
     */
    Player& Player::opponent() {
        return *opponent_;
    }

    /**
     * Get this player's opponent
     * @return : this player's opponent
     */
    const Player& Player::opponent() const {
        return *opponent_;
    }

    /**
     * Set this player's opponent to be the given player
     * @param opponent : this player's opponent
     */
    void Player::set_opponent(Player& opponent) {
        opponent_ = &opponent;
    }

    /**
     * Receive fire at row, column
     * @param row : the row that was shot at
     * @param col : the column that was shot at
     * @return : the results of shooting at the location
     */
    FiringResult Player::receive_fire_at(int row, int col) {
        FiringResult firing_result;
        board_.mark_as_fired_at(row, col);
        if (board_.has_ship_at(row, col)) {
            auto ship = board_.get_ship_at(row, col).value();
            firing_result.set_ship_hit(ship);
            int remaining_health = (ship_healths_.at(ship) -= 1);
            if (remaining_health == 0) {
                firing_result.set_ship_was_destroyed();
            }
        }
        return firing_result;
    }

    /**
     * Check if all of this player's ships are destroyed
     * @return : whether all of this player's ships are destroyed or not
     */
    bool Player::are_all_ships_destroyed() const {
        return ship_healths_.size() == 0 or
               std::ranges::all_of(ship_healths_ | std::ranges::views::values,
                                   [](const auto& health) { return health == 0; }
               );
    }
} // BattleShip
