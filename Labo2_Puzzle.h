#ifndef PUZZLE_H
#define PUZZLE_H
#include "Labo2_Types.h"

class Puzzle {
public:
   Puzzle(const Cube& c = EMPTY_CUBE);
   Puzzle(FastCube fc);
   Puzzle(const Puzzle& p);
   
   // deprecated, prefer adding fast cubes for speed purposes
   bool addCube(const Cube& c, Shape s);
   bool addFastCube(FastCube fc, Shape s);
   void removeLastCube();
   bool isFilled() const;
   bool isValid() const;
   Cube getCodedCube(bool ShapeEncoding = false) const;
   friend bool operator== (const Puzzle& p1, const Puzzle& p2);
   
private:
    FastCube fc;
    fastCubeVector fcv;
    shapeVector sv;
};

#endif /* PUZZLE_H */

