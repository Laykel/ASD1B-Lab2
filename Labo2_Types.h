#ifndef TYPES_H
#define TYPES_H
#include <valarray>
#include <vector>

using line = std::valarray<int>;
using square = std::valarray<line>;
using cube = std::valarray<square>;
using currentCubes = std::vector<cube>;

const int DIM = 3;
// full, all 1 3x3x3 cube
const cube FILLED_CUBE = cube(square(line(1,DIM),DIM),DIM);
// default, all 0 3x3x3 cube
const cube EMPTY_CUBE = cube(square(line(0,DIM),DIM),DIM);

#endif /* TYPES_H */

