#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"
#include "Orientation.h"
#include "ShipPlacement.h"

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace BattleShip {
    class HumanPlayer : public Player {
    public:
        HumanPlayer(const GameConfig& game_config,
                    std::istream& in,
                    std::ostream& out,
                    const std::vector<std::unique_ptr<Player>>& other_players);

        void place_ships(std::istream& in, std::ostream& out) override;

        std::pair<int, int> get_firing_location(std::istream& in, std::ostream& out) override;

    private:
        void place_ship(char ship_name, int ship_length,
                        std::istream& in, std::ostream& out);

        [[nodiscard]] std::optional<Orientation> get_orientation(
            char ship_name,
            std::istream& in,
            std::ostream& out) const;

        [[nodiscard]] std::optional<ShipPlacement> get_placement(
            char ship_name,
            int ship_length,
            Orientation orientation,
            std::istream& in,
            std::ostream& out) const;

        [[nodiscard]] std::optional<std::pair<int, int>> get_row_and_column(
            const std::string& prompt,
            std::istream& in,
            std::ostream& out) const;
    };
}

#endif