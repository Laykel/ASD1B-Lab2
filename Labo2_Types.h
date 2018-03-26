/**
 \file   Labo2_Types.h
 \author Filipe Fortunato
 \date   07.03.2018

 Definition of useful types and constants, and declaration of functions
 to manipulate those types, convert between them.
*/

#ifndef TYPES_H
#define TYPES_H
#include <valarray>
#include <vector>
#include <iostream>
#include <string>

// ------------------------- Types definitions ----------------------------

using Line = std::valarray<int>;
using Square = std::valarray<Line>;
using Cube = std::valarray<Square>;
using cubeVector = std::vector<Cube>;

// faster to create, faster to compute cubes
using FastCube = uint_fast32_t;
using fastCubeVector = std::vector<FastCube>;

// prime numbers to be able to sign a solution
enum class Shape{L, C, T, S};
using shapeVector = std::vector<Shape>;

// ---------------------- Constants definitions --------------------------

const int SHAPE_TO_CUBES_COUNT[] = {4, 3, 4, 4};
const int SHAPE_TO_SIGNATURE[] = {2, 3, 5, 7};
const std::string SHAPE_TO_STRING[] = {"L", "C", "T", "S"};

const int length = 3;
// full, all 1 3x3x3 Cube
const Cube FILLED_CUBE = Cube(Square(Line(1,length),length),length);
// default, all 0 3x3x3 Cube
const Cube EMPTY_CUBE = Cube(Square(Line(0,length),length),length);

// full, all 1 3x3x3 Fast Cube
const FastCube FILLED_FAST_CUBE = (FastCube)0x7FFFFFF;

// default, all 0 3x3x3 Fast Cube
const FastCube EMPTY_FAST_CUBE  = (FastCube)0;

// ---------------- Functions and overloads definitions -----------------

// stream operators overloads
std::ostream& operator<< (std::ostream& os, const Cube& c);
std::ostream& operator<< (std::ostream& os, const Shape& s);

// compares direct equality
bool operator== (const Cube& c1, const Cube& c2);
// compares spacial equality
bool areSimilar(const Cube& c1, const Cube& c2);

// allows to generate all 6 spacial rotations of a a given cube / fastCube
cubeVector allSpaceRotations(const Cube& c);

// allows to generate all 4 facial rotations of a a given cube / fastCube
cubeVector allFaceRotations(const Cube& c);

// allows to generate all 24 rotations in space of a given cube / fastCube
cubeVector allRotations(const Cube& c);

Cube shift(const Cube& c, bool& sucess, int dx, int dy, int dz);

// conversion functions
std::string toString(const Cube& c);
FastCube CubeToFastCube(const Cube& c);
Cube FastCubeToCube(FastCube fc);

#endif /* TYPES_H */
