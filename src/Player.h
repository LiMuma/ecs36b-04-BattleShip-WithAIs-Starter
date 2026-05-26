//
// Created by mfbut on 4/24/2026.
//

#ifndef ECS36BBATTLESHIPHUMANSONLY_PLAYER_H
#define ECS36BBATTLESHIPHUMANSONLY_PLAYER_H

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "Board.h"
#include "FiringResult.h"
#include "GameConfig.h"
#include "Orientation.h"
#include "ShipPlacement.h"

namespace BattleShip {
    /**
     * Represents a player in the game of battleship
     */
    class Player {
    public:

        Player(const std::string& name, const GameConfig& game_config);
        virtual ~Player() = default;

        virtual void notify_firing_result(int row, int col, const FiringResult& result);

        //getters

        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const Board& board() const;

        [[nodiscard]] Player& opponent();

        [[nodiscard]] const Player& opponent() const;

        //setters

        void set_opponent(Player& opponent);

        //player actions

        virtual void place_ships(std::istream& in, std::ostream& out) = 0;

        virtual std::pair<int, int> get_firing_location(std::istream& in, std::ostream& out) = 0;

        FiringResult receive_fire_at(int row, int col);

        //checks

        [[nodiscard]] bool are_all_ships_destroyed() const;

    protected:
        std::string name_;
        Board board_;
        std::map<char, int> ship_healths_;
        Player* opponent_;
    };
} // BattleShip

#endif //ECS36BBATTLESHIPHUMANSONLY_PLAYER_H
