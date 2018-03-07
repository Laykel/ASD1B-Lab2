#ifndef CUBE_H
#define CUBE_H
#include <valarray>

using Line = std::valarray<int>;
using Square = std::valarray<Line>;
using cube = std::valarray<Square>;

class Cube {
public:
   Cube();
private:

};

#endif /* CUBE_H */

