#ifndef PUZZLE_H
#define PUZZLE_H
#include "Labo2_Types.h"
#include "Labo2_Piece.h"

class Puzzle {
public:
   Puzzle(const Cube& c = EMPTY_CUBE);
   bool addCube(const Cube& c);
   bool addPiece(const Piece& p, short dx = 0, short dy = 0, short dz = 0);
   bool isFilled() const;
   bool isValid() const;
private:
    Cube c;
    currentCubes cc;
};

#endif /* PUZZLE_H */

