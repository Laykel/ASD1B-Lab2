#ifndef TYPES_H
#define TYPES_H
#include <valarray>
#include <vector>
#include <iostream>

using Line = std::valarray<int>;
using Square = std::valarray<Line>;
using Cube = std::valarray<Square>;
using currentCubes = std::vector<Cube>;

const int length = 3;
// full, all 1 3x3x3 Cube
const Cube FILLED_CUBE = Cube(Square(Line(1,length),length),length);
// default, all 0 3x3x3 Cube
const Cube EMPTY_CUBE = Cube(Square(Line(0,length),length),length);

// inlined function because of lack of cpp file.
inline std::ostream& operator<< (std::ostream& os, const Cube& c){
    std::cout << "     top       mid      bottom\n";
    for(int y = 0; y < length; y++){
        for(int x = 0; x < length; x++){
            std::cout << "[";
            for(int z = 0; z < length; z++){
               std::cout << c[x][y][z];
               if(z < length - 1)
                   std::cout << ", ";
            }
            std::cout << "] ";
        }
        std::cout << std::endl;
    }
        std::cout << std::endl;
    
    return os;
}

#endif /* TYPES_H */

