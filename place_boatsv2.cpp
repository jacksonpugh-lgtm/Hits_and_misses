#include <iostream>
#include <vector>
#include <string>
#include <cctype>

#include "boats.hpp"

const int SIZE = 10;
const std::string SHIP = "??";

bool in_bounds(int r, int c)
{
    return r >= 0 && r < SIZE && c >= 0 && c < SIZE;
}

bool check_adjacent(const std::vector<std::vector<std::string>>& board, int r, int c)
{
    for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++)
        {
            int nr = r + dr;
            int nc = c + dc;
            if (in_bounds(nr, nc) && board[nr][nc] == SHIP)
                return false;
        }
    return true;
}

bool can_place(const std::vector<std::vector<std::string>>& board,
    int r, int c, int size, char dir)
{
    if (dir == 'H')
    {
        if (c + size > SIZE)
        {
            std::cout << "Not enough space horizontally for boat\n";
            return false;
        }
        for (int i = 0; i < size; i++)
            if (!check_adjacent(board, r, c + i))
            {
                std::cout << "Spot taken or too close to another ship, try again\n";
                return false;
            }
    }
    else
    {
        if (r + size > SIZE)
        {
            std::cout << "Not enough space vertically for boat\n";
            return false;
        }
        for (int i = 0; i < size; i++)
            if (!check_adjacent(board, r + i, c))
            {
                std::cout << "Spot taken or too close to another ship, try again\n";
                return false;
            }
    }
    return true;
}

void place_ship(std::vector<std::vector<std::string>>& board,
    int r, int c, int size, char dir)
{
    for (int i = 0; i < size; i++)
        if (dir == 'H') board[r][c + i] = SHIP;
        else board[r + i][c] = SHIP;
}

void place_boats(std::vector<std::vector<std::string>>& board)
{
    std::cout << "Start by placing your " << ship_types.size() << " boats on the board.\n";

    for (auto& ship : ship_types)
    {
        while (true)
        {
            std::cout << ship.name << "(" << ship.size << ") Pick a location to place a boat: ";

            char dir;
            std::string pos;

            if (!(std::cin >> dir >> pos)) return;

            dir = std::toupper(dir);

            if (dir != 'H' && dir != 'V' || pos.size() < 2)
            {
                std::cout << "Invalid input. Format: H A1\n";
                std::cin.ignore(10000, '\n');
                continue;
            }

            char row = std::toupper(pos[0]);
            if (row < 'A' || row > 'J')
            {
                std::cout << "Invalid input. Format: H A1\n";
                continue;
            }

            int col;
            try { col = std::stoi(pos.substr(1)) - 1; }
            catch (...) {
                std::cout << "Invalid input. Format: H A1\n";
                continue;
            }

            if (col < 0 || col >= SIZE)
            {
                std::cout << "Invalid input. Format: H A1\n";
                continue;
            }

            int r = row - 'A';

            if (!can_place(board, r, col, ship.size, dir))
                continue;

            place_ship(board, r, col, ship.size, dir);

            std::cout << "A boat of size " << ship.size
                << " will be placed at "
                << row << col + 1
                << (dir == 'H' ? " horizontally\n" : " vertically\n");

            break;
        }
    }
}