#include "HumanPlayer.h"
#include <format>
#include <ranges>
#include <sstream>
#include <unordered_map>

#include "ExtraStringMethods.h"

namespace BattleShip {
    HumanPlayer::HumanPlayer(
        const GameConfig& game_config,
        std::istream& in,
        std::ostream& out,
        const std::vector<std::unique_ptr<Player>>& other_players)
        : Player("", game_config) {

        bool duplicate;

        do {
            duplicate = false;

            out << "Enter your name: ";
            std::getline(in, name_);
            name_ = strip(name_, " \t\n");

            for (const auto& player : other_players) {
                if (player->name() == name_) {
                    duplicate = true;
                    break;
                }
            }

        } while (duplicate);
    }

    void HumanPlayer::place_ships(std::istream& in, std::ostream& out) {
        for (const auto [ship_name, ship_health] : ship_healths_) {
            out << board_.view_as_visible() << std::endl;
            place_ship(ship_name, ship_health, in, out);
            out << board_.view_as_visible() << std::endl;
        }
    }

    void HumanPlayer::place_ship(char ship_name, int ship_length,
                                 std::istream& in, std::ostream& out) {
        while (true) {
            auto orientation = get_orientation(ship_name, in, out);

            if (!orientation.has_value()) {
                continue;
            }

            auto placement = get_placement(
                ship_name,
                ship_length,
                orientation.value(),
                in,
                out
            );

            if (placement.has_value()) {
                board_.place_ship(placement.value());
                return;
            }
        }
    }

    std::optional<Orientation> HumanPlayer::get_orientation(
        char ship_name,
        std::istream& in,
        std::ostream& out) const {

        static const std::unordered_map<std::string, Orientation> valid_input{
            {"horizontal", Orientation::HORIZONTAL},
            {"h", Orientation::HORIZONTAL},
            {"vertical", Orientation::VERTICAL},
            {"v", Orientation::VERTICAL}
        };

        out << std::format(
            "{}, do you want to place {}, horizontally or vertically?\n"
            "Enter h for horizontal or v for vertical\n"
            "Your choice: ",
            name(),
            ship_name
        );

        std::string user_input;
        std::getline(in, user_input);
        user_input = lower(strip(user_input, " \t\n"));

        if (!valid_input.contains(user_input)) {
            return {};
        }

        return valid_input.at(user_input);
    }

    std::optional<ShipPlacement> HumanPlayer::get_placement(
        char ship_name,
        int ship_length,
        Orientation orientation,
        std::istream& in,
        std::ostream& out) const {

        std::string prompt = std::format(
            "{}, enter the row and column you want to place {}, "
            "which is {} long, at with a space in between row and col: ",
            name(),
            ship_name,
            ship_length
        );

        auto possible_row_and_col = get_row_and_column(prompt, in, out);

        if (!possible_row_and_col.has_value()) {
            return {};
        }

        auto [start_row, start_col] = possible_row_and_col.value();

        ShipPlacement placement(
            ship_name,
            ship_length,
            start_row,
            start_col,
            orientation
        );

        if (!board_.can_ship_be_placed(placement)) {
            return {};
        }

        return placement;
    }

    std::optional<std::pair<int, int>> HumanPlayer::get_row_and_column(
        const std::string& prompt,
        std::istream& in,
        std::ostream& out) const {

        out << prompt;

        std::string user_input;
        std::getline(in, user_input);

        std::stringstream line_parser(user_input);

        int row;
        int col;

        if (!(line_parser >> row >> col)) {
            return {};
        }

        std::string extra;

        if (line_parser >> extra) {
            return {};
        }

        return std::make_pair(row, col);
    }

    std::pair<int, int> HumanPlayer::get_firing_location(
        std::istream& in,
        std::ostream& out) {

        const std::string prompt = std::format(
            "{}, where would you like to fire?\n"
            "Enter your attack coordinate in the form row col: ",
            name()
        );

        out << std::format(
            "{}'s Firing Board\n{}\n\n",
            name(),
            opponent().board().view_as_hidden()
        );

        out << std::format(
            "{}'s Placement Board\n{}\n",
            name(),
            board_.view_as_visible()
        );

        while (true) {
            auto possible_row_and_col = get_row_and_column(prompt, in, out);

            if (!possible_row_and_col.has_value()) {
                continue;
            }

            auto [row, col] = possible_row_and_col.value();

            if (!opponent().board().is_on(row, col)) {
                continue;
            }

            if (opponent().board().has_been_fired_at(row, col)) {
                continue;
            }

            return std::make_pair(row, col);
        }
    }
}