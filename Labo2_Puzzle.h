/**
 \file   Labo2_Puzzle.h
 \author Filipe Fortunato
 \date   07.03.2018

 Declaration of the Puzzle class.
 A puzzle contains a collection of pieces, and provides us with member functions
 to manipulate those pieces.
 We now use only "fast cubes" to enhance the speed significantly, although we still
 provide functions to convert to a normal cube and the other way (see Labo2_Types.h).
*/

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
    fastCubeVector sortedfcv; 
    bool sorted = false; 
};

#endif /* PUZZLE_H */

