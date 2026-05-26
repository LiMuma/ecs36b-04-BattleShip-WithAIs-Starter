#ifndef ECS36BBATTLESHIPHUMANSONLY_GAME_H
#define ECS36BBATTLESHIPHUMANSONLY_GAME_H

#include <iostream>
#include <memory>
#include <vector>

#include "GameConfig.h"
#include "Player.h"

namespace BattleShip {
    class Game {
    public:
        explicit Game(const GameConfig& game_config, std::istream& in, std::ostream& out);

        void play();

        [[nodiscard]] bool is_game_over() const;

        void resolve_current_player_action(const std::pair<int, int>& firing_location);

        void advance_to_next_player();

        void announce_results() const;

        [[nodiscard]] Player& cur_player();

        [[nodiscard]] const Player& cur_player() const;

    private:
        std::vector<std::unique_ptr<Player>> players_;
        size_t cur_player_index_;
        std::istream& in_;
        std::ostream& out_;

        void create_players(const GameConfig& game_config);

        std::unique_ptr<Player> create_ai_player(const GameConfig& game_config, int ai_number);
    };
}

#endif