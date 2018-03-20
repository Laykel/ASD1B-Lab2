#ifndef TYPES_H
#define TYPES_H
#include <valarray>
#include <vector>
#include <iostream>
#include <string>

using Line = std::valarray<int>;
using Square = std::valarray<Line>;
using Cube = std::valarray<Square>;
using cubeVector = std::vector<Cube>;
// why use an uint_fast32_t ? because if we want fast, let's go all the way...
using FastCube = uint_fast32_t;
using fastCubeVector = std::vector<FastCube>;
// prime numbers to be able to sign a solution
enum class Shape{L, C, T, S};

const int SHAPE_TO_CUBES_COUNT[] = {4, 3, 4, 4};
const int SHAPE_TO_SIGNATURE[] = {2, 3, 5, 7};

using shapeVector = std::vector<Shape>;

const int length = 3;
// full, all 1 3x3x3 Cube
const Cube FILLED_CUBE = Cube(Square(Line(1,length),length),length);
// default, all 0 3x3x3 Cube
const Cube EMPTY_CUBE = Cube(Square(Line(0,length),length),length);

// full, all 1 3x3x3 Fast Cube
const FastCube FILLED_FAST_CUBE = (FastCube)0x7FFFFFF;

// default, all 0 3x3x3 Fast Cube
const FastCube EMPTY_FAST_CUBE  = (FastCube)0;

// operateurs de flux
std::ostream& operator<< (std::ostream& os, const Cube& c);
std::ostream& operator<< (std::ostream& os, const Shape& s);

// allows to generate all 6 spacial rotations of a a given cube / fastCube
cubeVector allSpaceRotations(const Cube& c);

// allows to generate all 4 facial rotations of a a given cube / fastCube
cubeVector allFaceRotations(const Cube& c);


// allows to generate all 24 rotations in space of a given cube / fastCube
cubeVector allRotations(const Cube& c);

// compares direct equality
bool operator== (const Cube& c1, const Cube& c2);
std::string toString(const Cube& c);
// compares spacial equality
bool areSimilar(const Cube& c1, const Cube& c2);

Cube shift(const Cube& c, bool& sucess, int dx, int dy, int dz);

FastCube CubeToFastCube(const Cube& c);
Cube FastCubeToCube(FastCube fc);

#endif /* TYPES_H */

