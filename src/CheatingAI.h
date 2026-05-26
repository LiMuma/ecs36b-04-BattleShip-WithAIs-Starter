//
// Created by Max on 5/24/26.
//

#ifndef ECS36B_04_BATTLESHIP_WITHAIS_STARTER_CHEATINGAI_H
#define ECS36B_04_BATTLESHIP_WITHAIS_STARTER_CHEATINGAI_H

#include "AIPlayer.h"

namespace BattleShip {
    class CheatingAI : public AIPlayer {
    public:
        CheatingAI(const std::string& name, const GameConfig& game_config);

        std::pair<int, int> get_firing_location(std::istream& in, std::ostream& out) override;
    };
}



#endif //ECS36B_04_BATTLESHIP_WITHAIS_STARTER_CHEATINGAI_H
