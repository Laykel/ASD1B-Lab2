#include "Labo2_Piece.h"

Piece::Piece(const cube& c) {
    this->c = c;
    for(int x = 0; x < DIM; x++)
        for(int y = 0; y < DIM; y++)
            for(int z = 0; z < DIM; z++)
                blockCount += c[x][y][z];
}


cube Piece::shift(bool& sucess, short dx, short dy, short dz) const{
    cube toReturn = EMPTY_CUBE;
    unsigned ctr = 0;
    for(int x = 0; x < DIM - dx; x++)
        for(int y = 0; y < DIM - dy; y++)
            for(int z = 0; z < DIM - dz; z++){
                ctr += c[x][y][z];
                toReturn[x + dx][y + dy][z + dz] = c[x][y][z];
            }
    sucess = (ctr == blockCount);
    return toReturn;
}