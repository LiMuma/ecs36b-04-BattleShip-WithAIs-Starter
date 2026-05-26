//
// Created by mfbut on 4/24/2026.
//
#include <algorithm>
#include <ranges>
#include <sstream>

#include "Board.h"

namespace BattleShip {
    /**
     * Create a num_rows X num_cols board filled with blank_char and using
     * the specified hit and miss markers
     * @param num_rows : The number of rows in the board
     * @param num_cols : The number of columns in the board
     * @param blank_char : The character used to represent an unfilled, unfired at location
     * @param hit_marker : The character used to indicate a hit
     * @param miss_marker : The character used to indicate a miss
     */
    Board::Board(const int num_rows, const int num_cols,
                 const char blank_char, const char hit_marker,
                 const char miss_marker) : contents(num_rows, std::vector<BoardCell>(num_cols)), blank_(blank_char),
                                           hit_marker_(hit_marker), miss_marker_(miss_marker) {
    }

    /**
     * Get the cell at row col
     * @param row : the row we want to get the cell from
     * @param col : the column we want to get the cell from
     * @return : the cell at row, col
     */
    const BoardCell& Board::at(int row, int col) const {
        return contents.at(row).at(col);
    }

    /**
     * Get the cell at row col
     * @param row : the row we want to get the cell from
     * @param col : the column we want to get the cell from
     * @return : the cell at row, col
     */
    BoardCell& Board::at(int row, int col) {
        return contents.at(row).at(col);
    }

    /**
     * Check if the board at row, col has been fired at already
     * @param row : the row of the board to check
     * @param col : the column of the board to check
     * @return : whether the board at row, col has been fired at or not
     */
    bool Board::has_been_fired_at(int row, int col) const {
        return at(row, col).has_been_fired_at();
    }

    /**
     * Mard the board at row, col as fired at
     * @param row : the row of the board to mark
     * @param col : the column of the board to mark
     */
    void Board::mark_as_fired_at(int row, int col) {
        at(row, col).mark_as_fired_at();
    }


    /**
     * Check if the specified location is on the board or not
     * @param row : the row of the board to check
     * @param col : the column of the board to check
     * @return : whether row, col is on the board or not
     */
    bool Board::is_on(const int row, const int col) const {
        return row >= 0 and row < static_cast<int>(num_rows())
               and col >= 0 and col < static_cast<int>(num_cols());
    }

    /**
     * Check if there is a ship on this board at row, col
    * @param row : the row of the board to check
     * @param col : the column of the board to check
     * @return : whether there is a ship at row, col or not
     */
    bool Board::has_ship_at(int row, int col) const {
        return at(row, col).has_ship();
    }

    /**
     * Get the ship at row, col if there is one
     * @param row : the row to retrieve the ship from
     * @param col : the column to retrieve the ship from
     * @return : an optional containing the ship at the location if there is one
     * otherwise an empty optional
     */
    std::optional<char> Board::get_ship_at(const int row, const int col) const {
        return at(row, col).ship();
    }

    /**
     * Add a ship to row, col and row, col only
     * @param ship : the character used to represent the ship
     * @param row : the row to add the ship to
     * @param col : the column used to add the ship to
     */
    void Board::add_ship_at(const char ship, const int row, const int col) {
        at(row, col).add_ship(ship);
    }

    /**
     * Check that there are now ships between [start, stop]
     * @param start : a pair specifying the row, col the ship starts at
     * @param stop : a pair specifying the row, col the ship ends at (inclusive)
     * @return : whether there are any ships between [start, stop] or not
     */
    bool Board::are_no_ships_between(const std::pair<int, int> start, const std::pair<int, int> stop) const {
        // if on the same row, we don't want to move to a different one
        int row_inc = start.first == stop.first ? 0 : 1;
        // if on the same column, we don't want to move to a different one
        int col_inc = start.second == stop.second ? 0 : 1;

        //check to see if there are any ships
        for (int i = start.first, j = start.second; i <= stop.first and j <= stop.second; i += row_inc, j += col_inc) {
            if (has_ship_at(i, j)) {
                return false;
            }
        }
        return true;
    }

    /**
     * Check if the ship can be at the specified placement
     * @param placement : the ship, and where it would like to be placed
     * @return : whether the board can be placed at this location or not
     */
    bool Board::can_ship_be_placed(const ShipPlacement& placement) const {
        return is_on(placement.start.first, placement.stop.second)
               and is_on(placement.stop.first, placement.stop.second)
               and are_no_ships_between(placement.start, placement.stop);
    }

    /**
     * Place the ship at the specified location
     * @param placement : the ship and where to place it
     */
    void Board::place_ship(const ShipPlacement& placement) {
        const auto& start = placement.start;
        const auto& stop = placement.stop;

        // if on the same row, we don't want to move to a different one
        int row_inc = start.first == stop.first ? 0 : 1;
        //if one the same column, we don't want to move to a different one
        int col_inc = start.second == stop.second ? 0 : 1;

        //place the ship at all locations it occupies
        for (int i = start.first, j = start.second; i <= stop.first and j <= stop.second; i += row_inc, j += col_inc) {
            add_ship_at(placement.ship_name, i, j);
        }
    }


    /**
     * Get the number of rows in this board
     * @return : the number of rows in this board
     */
    size_t Board::num_rows() const {
        return contents.size();
    }

    /**
     * Get the number of columns in this board
     * @return : the numebr of columns in this board
     */
    size_t Board::num_cols() const {
        return contents.front().size();
    }

    /**
     * Get a string representing the board with only the locations that
     * have been fired at revealed
     * @return : a string representing the board with only the locations that
     * have been fired at revealed
     */
    std::string Board::view_as_hidden() const {
        std::stringstream out;

        //figure out how wide the separator needs to be
        const std::string sep(std::to_string(num_rows()).size(), ' ');

        // print column headers
        out << sep << ' ';
        for (const auto i: std::ranges::views::iota(0ul, num_cols())) {
            out << i << sep;
        }
        out << std::endl;

        for (const auto& [i, row]: std::ranges::views::enumerate(contents)) {
            out << i << sep;
            for (const auto& cell: row) {
                if (cell.has_been_fired_at()) {
                    if (cell.has_ship()) {
                        out << hit_marker_;
                    } else {
                        out << miss_marker_;
                    }
                } else {
                    out << blank_;
                }
                out << sep;
            }
            out << std::endl;
        }
        return out.str();
    }

    /**
     * Get a string representing the board with all spaces visible
     * @return : a string representing the board with all spaces visible
     */
    std::string Board::view_as_visible() const {
        std::stringstream out;

        //figure out how wide the separator needs to be
        const std::string sep(std::to_string(num_rows()).size(), ' ');

        // print column headers
        out << sep << ' ';
        for (const auto i: std::ranges::views::iota(0ul, num_cols())) {
            out << i << sep;
        }
        out << std::endl;

        for (const auto& [i, row]: std::ranges::views::enumerate(contents)) {
            out << i << sep;
            for (const auto& cell: row) {
                if (cell.has_been_fired_at()) {
                    if (cell.has_ship()) {
                        out << hit_marker_;
                    } else {
                        out << miss_marker_;
                    }
                } else {
                    out << cell.ship().value_or(blank_);
                }
                out << sep;
            }
            out << std::endl;
        }
        return out.str();
    }
} // BattleShip
