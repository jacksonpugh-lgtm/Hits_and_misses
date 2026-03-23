#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cctype>

#include "boats.hpp"
#include "battleship.hpp"

const int SIZE = 10;

const std::string SHIP = "??";
const std::string HIT = "??";
const std::string MISS = "??";
const std::string SUNK = "??";

bool in_bounds(int r, int c)
{
    return r >= 0 && r < SIZE && c >= 0 && c < SIZE;
}

void bfs_mark_sunk(std::vector<std::vector<std::string>>& board, int r, int c)
{
    std::queue<std::pair<int, int>> q;
    q.push({ r,c });

    while (!q.empty())
    {
        auto [cr, cc] = q.front(); q.pop();

        if (board[cr][cc] != HIT) continue;

        board[cr][cc] = SUNK;

        int dr[4] = { -1,1,0,0 };
        int dc[4] = { 0,0,-1,1 };

        for (int i = 0; i < 4; i++)
        {
            int nr = cr + dr[i];
            int nc = cc + dc[i];
            if (in_bounds(nr, nc) && board[nr][nc] == HIT)
                q.push({ nr,nc });
        }
    }
}

int total_ship_cells()
{
    int total = 0;
    for (auto& s : ship_types)
        total += s.size;
    return total;
}

int count_enemy_hits(const std::vector<std::vector<std::string>>& board)
{
    int count = 0;
    for (auto& row : board)
        for (auto& c : row)
            if (c == HIT || c == SUNK)
                count++;
    return count;
}

int count_player_ships(const std::vector<std::vector<std::string>>& board)
{
    int count = 0;
    for (auto& row : board)
        for (auto& c : row)
            if (c == SHIP)
                count++;
    return count;
}

void start_game(const std::string& name)
{
    std::cout << "Hello " << name << ". Do you want to play a game?\n";

    std::string response;
    std::cin >> response;

    if (response == "no" || response == "No")
    {
        std::cout << "Run me when you are ready!\n";
        return;
    }
    else if (!(response == "yes" || response == "Yes"))
    {
        std::cout << "Invalid input\n";
        exit(2);
    }

    std::vector<std::vector<std::string>> your_board(SIZE, std::vector<std::string>(SIZE, " "));
    std::vector<std::vector<std::string>> enemy_board(SIZE, std::vector<std::string>(SIZE, " "));

    place_boats(your_board);

    std::cout << "Done with board setup\n";
    std::cout << "Let the Games Begin!\n";

    int total = total_ship_cells();

    while (true)
    {
        // PLAYER TURN
        while (true)
        {
            std::cout << "Your turn! Enter your shot (expected: A1 H/M/S): ";

            std::string coord, result;
            if (!(std::cin >> coord >> result)) return;

            char row = std::toupper(coord[0]);
            int col;

            try { col = std::stoi(coord.substr(1)) - 1; }
            catch (...) {
                std::cout << "Invalid input. Format: A1 H\n";
                continue;
            }

            if (row < 'A' || row > 'J' || col < 0 || col >= SIZE ||
                (result != "H" && result != "M" && result != "S"))
            {
                std::cout << "Invalid input. Format: A1 H\n";
                continue;
            }

            int r = row - 'A';

            if (enemy_board[r][col] != " ")
            {
                std::cout << "You already fired at this location\n";
                continue;
            }

            if (result == "H") enemy_board[r][col] = HIT;
            else if (result == "M") enemy_board[r][col] = MISS;
            else
            {
                enemy_board[r][col] = HIT;
                bfs_mark_sunk(enemy_board, r, col);
            }

            std::cout << "You shot at " << row << col + 1 << ": "
                << (result == "H" ? "HIT!" : result == "M" ? "MISS!" : "SUNK!") << "\n";

            break;
        }

        if (count_enemy_hits(enemy_board) == total)
        {
            std::cout << "You sunk all enemy ships. YOU WIN!\n";
            return;
        }

        // ENEMY TURN
        while (true)
        {
            std::cout << "Enemy's turn! Enter enemy shot (expected: A1): ";

            std::string coord;
            if (!(std::cin >> coord)) return;

            char row = std::toupper(coord[0]);
            int col;

            try { col = std::stoi(coord.substr(1)) - 1; }
            catch (...) {
                std::cout << "Invalid input. Expected format: A1\n";
                continue;
            }

            if (row < 'A' || row > 'J' || col < 0 || col >= SIZE)
            {
                std::cout << "Invalid input. Expected format: A1\n";
                continue;
            }

            int r = row - 'A';

            if (your_board[r][col] == HIT || your_board[r][col] == MISS || your_board[r][col] == SUNK)
            {
                std::cout << "Enemy already fired at this location\n";
                continue;
            }

            if (your_board[r][col] == SHIP)
            {
                your_board[r][col] = HIT;
                std::cout << "Enemy shot at " << row << col + 1 << ": HIT!\n";
            }
            else
            {
                your_board[r][col] = MISS;
                std::cout << "Enemy shot at " << row << col + 1 << ": MISS!\n";
            }

            break;
        }

        if (count_player_ships(your_board) == 0)
        {
            std::cout << "All your ships have been sunk. YOU LOST!\n";
            return;
        }
    }
}