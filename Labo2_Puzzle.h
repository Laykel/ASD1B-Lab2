#ifndef PUZZLE_H
#define PUZZLE_H
#include "Labo2_Types.h"

class Puzzle {
public:
   Puzzle(const Cube& c = EMPTY_CUBE);
   bool addCube(const Cube& c);
   bool isFilled() const;
   bool isValid() const;
private:
    Cube c;
    cubeVector cc;
};

#endif /* PUZZLE_H */

