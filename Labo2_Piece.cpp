#include "Labo2_Piece.h"

using namespace std;
Piece::Piece(const Cube& c) {
    this->c = c;
    for(int x = 0; x < length; x++)
        for(int y = 0; y < length; y++)
            for(int z = 0; z < length; z++)
                blockCount += c[x][y][z];
}


Cube Piece::shift(bool& sucess, short dx, short dy, short dz) const{
    Cube toReturn = EMPTY_CUBE;
    unsigned ctr = 0;
    for(int x = 0; x < length - dx; x++)
        for(int y = 0; y < length - dy; y++)
            for(int z = 0; z < length - dz; z++){
                ctr += c[x][y][z];
                toReturn[x + dx][y + dy][z + dz] = c[x][y][z];
            }
    sucess = (ctr == blockCount);
    return toReturn;
}

Piece Piece::rotate(){
    Cube rc = EMPTY_CUBE;
    for(int x = 0; x < length; x++)
        for(int y = 0; y < length; y++)
            for(int z = 0; z < length; z++)
               rc[x][y][z] = c[z][x][y];
    
    return Piece(rc);
}

ostream& operator<< (ostream& os, const Piece& p){
    os << p.c;
    return os;
}