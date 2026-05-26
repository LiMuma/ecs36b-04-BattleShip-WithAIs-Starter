#include "Game.h"

#include <algorithm>
#include <format>
#include <ranges>
#include <string>
#include <limits>

#include "HumanPlayer.h"
#include "RandomAI.h"
#include "CheatingAI.h"
#include "SearchAndDestroyAI.h"

namespace BattleShip {

    Game::Game(const GameConfig& game_config,
               std::istream& in,
               std::ostream& out)
        : players_(),
          cur_player_index_(0),
          in_(in),
          out_(out) {

        create_players(game_config);

        for (int i = 0; i < static_cast<int>(players_.size()); ++i) {
            players_.at(i)->set_opponent(
                *players_.at((i + 1) % players_.size())
            );
        }
    }

    void Game::create_players(const GameConfig& game_config) {
        int mode;

        out_ << "What type of game do you want to play?\n";
        out_ << "1. Human vs Human\n";
        out_ << "2. Human vs AI\n";
        out_ << "3. AI vs AI\n";
        out_ << "Your choice: ";

        in_ >> mode;
        in_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (mode == 1) {

            players_.push_back(
                std::make_unique<HumanPlayer>(
                    game_config,
                    in_,
                    out_,
                    players_
                )
            );

            players_.back()->place_ships(in_, out_);

            players_.push_back(
                std::make_unique<HumanPlayer>(
                    game_config,
                    in_,
                    out_,
                    players_
                )
            );

            players_.back()->place_ships(in_, out_);

        }
        else if (mode == 2) {

            players_.push_back(
                std::make_unique<HumanPlayer>(
                    game_config,
                    in_,
                    out_,
                    players_
                )
            );

            players_.back()->place_ships(in_, out_);

            players_.push_back(
                create_ai_player(game_config, 1)
            );

            players_.back()->place_ships(in_, out_);

        }
        else {

            players_.push_back(
                create_ai_player(game_config, 1)
            );

            players_.back()->place_ships(in_, out_);

            players_.push_back(
                create_ai_player(game_config, 2)
            );

            players_.back()->place_ships(in_, out_);
        }
    }

    std::unique_ptr<Player>
    Game::create_ai_player(const GameConfig& game_config,
                           int ai_number) {

        int ai_type;

        out_ << "What AI do you want?\n";
        out_ << "1. Cheating AI\n";
        out_ << "2. Random AI\n";
        out_ << "3. Hunt Destroy AI\n";
        out_ << "Your choice: ";

        in_ >> ai_type;
        in_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string ai_name =
            "AI " + std::to_string(ai_number);

        if (ai_type == 1) {
            return std::make_unique<CheatingAI>(
                ai_name,
                game_config
            );
        }

        if (ai_type == 2) {
            return std::make_unique<RandomAI>(
                ai_name,
                game_config
            );
        }

        return std::make_unique<SearchAndDestroyAI>(
            ai_name,
            game_config
        );
    }

    void Game::play() {

        while (true) {

            auto firing_location =
                cur_player().get_firing_location(
                    in_,
                    out_
                );

            resolve_current_player_action(
                firing_location
            );

            if (is_game_over()) {
                announce_results();
                return;
            }

            advance_to_next_player();
        }
    }

    bool Game::is_game_over() const {

        constexpr auto is_loser =
            [](const auto& player) {
                return player->are_all_ships_destroyed();
            };

        return std::ranges::any_of(
            players_,
            is_loser
        );
    }

    void Game::resolve_current_player_action(
    const std::pair<int, int>& firing_location
) {

        const auto& [row, col] = firing_location;

        auto firing_result =
            cur_player()
                .opponent()
                .receive_fire_at(row, col);

        cur_player().notify_firing_result(
            row,
            col,
            firing_result
        );

        out_ << std::format(
            "\n{}'s Firing Board\n{}\n",
            cur_player().name(),
            cur_player()
                .opponent()
                .board()
                .view_as_hidden()
        );

        out_ << std::format(
            "\n{}'s Placement Board\n{}",
            cur_player().name(),
            cur_player()
                .board()
                .view_as_visible()
        );

        if (firing_result.is_hit()) {

            out_ << std::format(
                "{} hit {}'s {}!\n",
                cur_player().name(),
                cur_player().opponent().name(),
                firing_result.get_ship_hit().value()
            );

            if (firing_result.was_ship_destroyed()) {

                out_ << std::format(
                    "{} destroyed {}'s {}!\n\n",
                    cur_player().name(),
                    cur_player().opponent().name(),
                    firing_result.get_ship_hit().value()
                );
            }

        }
        else {
            out_ << "Missed.\n";
        }
    }

    void Game::advance_to_next_player() {
        cur_player_index_ =
            (cur_player_index_ + 1)
            % players_.size();
    }

    void Game::announce_results() const {
        out_ << cur_player().name()
             << " won the game!"
             << std::endl;
    }

    Player& Game::cur_player() {
        return *players_.at(cur_player_index_);
    }

    const Player& Game::cur_player() const {
        return *players_.at(cur_player_index_);
    }

}