/*Peter is doing a research on the Traveling Knight Problem (TKP) where you have to find the shortest closed tour of knight moves that visits each square of a given set of n squares on a chessboard exactly once. He thinks that the most difficult part of the problem is determining the smallest number of knight moves between two given squares and that, once you have accomplished this, finding the tour would be easy.

Of course you know that it is vice versa. So you must offer him a program that solves the "difficult" part.

Your job is to write a program that takes two squares a and b as input and then determines the number of knight moves on a shortest route from a to b.*/

#include <iostream>
#include <vector>
#include <string>
#include <queue>

class Tile {
public:
    Tile(int row, int column) : row(row), column(column), visited(false) {
        knightEnabledTiles.reserve(8);
    };

    void setKnightEnabledTiles(std::vector<std::vector<Tile>>& tiles, int size) {
        if (column + 2 < size) {
            if (row + 1 < size)
                knightEnabledTiles.push_back(&tiles[row + 1][column + 2]);
            if (row - 1 >= 0)
                knightEnabledTiles.push_back(&tiles[row - 1][column + 2]);
        }
        if (column - 2 >= 0) {
            if (row + 1 < size)
                knightEnabledTiles.push_back(&tiles[row + 1][column - 2]);
            if (row - 1 >= 0)
                knightEnabledTiles.push_back(&tiles[row - 1][column - 2]);
        }
        if (row + 2 < size) {
            if (column + 1 < size)
                knightEnabledTiles.push_back(&tiles[row + 2][column + 1]);
            if (column - 1 >= 0)
                knightEnabledTiles.push_back(&tiles[row + 2][column - 1]);
        }
        if (row - 2 >= 0) {
            if (column + 1 < size)
                knightEnabledTiles.push_back(&tiles[row - 2][column + 1]);
            if (column - 1 >= 0)
                knightEnabledTiles.push_back(&tiles[row - 2][column - 1]);
        }
    }

    std::vector<Tile*> getKnightEnabledTiles() { return knightEnabledTiles; }

    bool wasVisited() { return visited; };

    void switchState() { visited = !visited; };

    void resetState() { visited = false; }

private:
    int row;
    int column;
    bool visited;
    std::vector<Tile*> knightEnabledTiles;
};

class Chessboard {
public:
    Chessboard() : size(8) {
        tiles.resize(size);

        for (int i = 0; i < size; i++) {
            tiles[i].reserve(size);
            for (int j = 0; j < size; j++) {
                tiles[i].emplace_back(Tile(i, j));
            }
        }
    }

    Chessboard(int size) : size(size) {
        tiles.resize(size);

        for (int i = 0; i < size; i++) {
            tiles[i].reserve(size);
            for (int j = 0; j < size; j++) {
                tiles[i].emplace_back(Tile(i, j));
            }
        }
    }

    Tile* getTilePtr(std::pair<char, int> index) {
        return &tiles[index.second - 1][index.first - 'a'];
    }

    std::vector<Tile*> getKnightEnabledTiles(Tile* tile) {
        using namespace std;

        if (tile->getKnightEnabledTiles().empty())
            tile->setKnightEnabledTiles(tiles, size);

        return tile->getKnightEnabledTiles();
    }

    std::vector<Tile*> getKnightEnabledTiles(std::pair<char, int> index) {
        using namespace std;

        vector<Tile*> knightEnabledTiles = tiles[index.second - 1][index.first - 'a'].getKnightEnabledTiles();

        if (!knightEnabledTiles.empty()) 
            return knightEnabledTiles;
        else {
            tiles[index.second - 1][index.first - 'a'].setKnightEnabledTiles(tiles, size);
            return tiles[index.second - 1][index.first - 'a'].getKnightEnabledTiles();
        }
    }

    void resetTilesState() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++)
                tiles[i][j].resetState();
        }
    }

    int getSize() { return size; }

private:
    int size;
    std::vector<std::vector<Tile>> tiles;
};

int bfs(Chessboard& board, std::pair<char, int> currentTile, std::pair<char, int> targetTile);

int main() {
    using namespace std;

    Chessboard board;
    string tilePair;
    pair<char, int> startTile, targetTile;
    int moves;

    while(getline(cin, tilePair)) {
        startTile.first = tilePair[0];
        startTile.second = (board.getSize() + 1) - (tilePair[1] - '0');
        targetTile.first = tilePair[3];
        targetTile.second = (board.getSize() + 1) - (tilePair[4] - '0');
        
        moves = bfs(board, startTile, targetTile);
        cout << "To get from " << tilePair.substr(0, 2) << " to " << tilePair.substr(3, 2) << " takes " << moves << " knight moves.\n";

        board.resetTilesState();
    }

    return 0;
}

int bfs(Chessboard& board, std::pair<char, int> currentTileIndex, std::pair<char, int> targetTileIndex) {
    using namespace std;

    if (currentTileIndex == targetTileIndex) {
        return 0;
    }

    int moves = 0, leftOnLayer;
    bool targetFound = false;
    Tile* targetTile = board.getTilePtr(targetTileIndex);
    Tile* currentTile = board.getTilePtr(currentTileIndex);
    queue<Tile*> toVisit;

    toVisit.push(currentTile);
    leftOnLayer = toVisit.size();
    currentTile->switchState();

    while (!targetFound) {
        if (leftOnLayer == 0) {
            moves++;
            leftOnLayer = toVisit.size();
        }

        currentTile = toVisit.front();
        toVisit.pop();
        leftOnLayer--;

        for (Tile* tile : board.getKnightEnabledTiles(currentTile)) {
            if (tile == targetTile) {
                targetFound = true;
                moves++;
            }
            else if (!tile->wasVisited()) {
                tile->switchState();
                toVisit.push(tile);
            }
        }
    }

    return moves;
}