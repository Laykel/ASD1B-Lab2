#ifndef TYPES_H
#define TYPES_H
#include <valarray>
#include <vector>
#include <iostream>

using Line = std::valarray<int>;
using Square = std::valarray<Line>;
using Cube = std::valarray<Square>;
using cubeVector = std::vector<Cube>;

const int length = 3;
// full, all 1 3x3x3 Cube
const Cube FILLED_CUBE = Cube(Square(Line(1,length),length),length);
// default, all 0 3x3x3 Cube
const Cube EMPTY_CUBE = Cube(Square(Line(0,length),length),length);


// inlined function because of lack of cpp file.
std::ostream& operator<< (std::ostream& os, const Cube& c);

// allows to generate all 6 spacial rotations of a cube
cubeVector allSpaceRotations(const Cube& c);

// allows to generate all 4 facial rotations of a cube
cubeVector allFaceRotations(const Cube& c);

cubeVector allRotations(const Cube& c);

bool operator== (const Cube& c1, const Cube& c2);
bool areSimilar(const Cube& c1, const Cube& c2);

Cube shift(const Cube& c, bool& sucess, int dx, int dy, int dz);

#endif /* TYPES_H */

