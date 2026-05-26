#include "RandomAI.h"

#include <algorithm>
#include <ranges>

namespace BattleShip {
    RandomAI::RandomAI(const std::string& name, const GameConfig& game_config)
        : AIPlayer(name, game_config), has_shuffled_(false) {
        create_firing_locations();
    }

    void RandomAI::create_firing_locations() {
        firing_locations_.clear();

        for (int row = 0; row < static_cast<int>(board_.num_rows()); ++row) {
            for (int col = 0; col < static_cast<int>(board_.num_cols()); ++col) {
                firing_locations_.push_back({row, col});
            }
        }
    }

    std::pair<int, int> RandomAI::get_random_location() {

        if (!has_shuffled_) {

            std::shuffle(
                firing_locations_.begin(),
                firing_locations_.end(),
                rng
            );

            has_shuffled_ = true;
        }

        auto location = firing_locations_.back();

        firing_locations_.pop_back();

        return location;
    }

    void RandomAI::remove_location_from_random_pool(const std::pair<int, int>& location) {
        auto iter = std::ranges::find(firing_locations_, location);

        if (iter != firing_locations_.end()) {
            firing_locations_.erase(iter);
        }
    }

    std::pair<int, int> RandomAI::get_firing_location(std::istream& in, std::ostream& out) {
        (void)in;
        (void)out;

        return get_random_location();
    }
}