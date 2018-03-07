#include "Labo2_Cube.h"

Cube::Cube(const cube& c) {
    this->c = c;
}


bool Cube::isFilled() const{
    for(square s : c)
        for(line l : s)
            for (int s : l)
                if (s != 1)
                    return false;
    return true;
}

bool Cube::addCube(const cube& c){
    this->c += c;
    cc.push_back(c);
    return isValid();
}

bool Cube::addPiece(const Piece& p, short dx, short dy, short dz){
    bool validShift;
    c = p.shift(validShift, dx, dy, dz);
    if(validShift)
        addCube(c);
    return validShift && isValid();
}

bool Cube::isValid() const{
    for(square s : c)
        for(line l : s)
            for (int s : l)
                if (s > 1)
                    return false;
    return true;
    
}
