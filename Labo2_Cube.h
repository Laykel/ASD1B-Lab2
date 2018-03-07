#ifndef CUBE_H
#define CUBE_H
#include "Labo2_Types.h"
#include "Labo2_Piece.h"

class Cube {
public:
   Cube(const cube& c = EMPTY_CUBE);
   bool addCube(const cube& c);
   bool addPiece(const Piece& p, short dx = 0, short dy = 0, short dz = 0);
   bool isFilled() const;
   bool isValid() const;
private:
    cube c;
    currentCubes cc;
};

#endif /* CUBE_H */

