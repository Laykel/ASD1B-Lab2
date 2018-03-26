/**
 \file   Labo2_Puzzle.cpp
 \author Filipe Fortunato
 \date   07.03.2018

 Definition of the elements of the Puzzle class.
*/

#include "Labo2_Puzzle.h"
#include <algorithm>

using namespace std;

Puzzle::Puzzle(const Cube& c) : Puzzle(CubeToFastCube(c)){}

Puzzle::Puzzle(FastCube fc) : fc(fc){}

Puzzle::Puzzle(const Puzzle& p) {
    fc = p.fc;
    fcv = fastCubeVector(p.fcv);
    sv = shapeVector(p.sv);
    sortedfcv = fastCubeVector(p.fcv); 
    sorted = true; 
    sort(sortedfcv.begin(), sortedfcv.end()); 
}

bool Puzzle::isFilled() const {
    return fc == FILLED_FAST_CUBE;
}

bool Puzzle::addCube(const Cube& c, Shape s) {
    return addFastCube(CubeToFastCube(c), s);
}

bool Puzzle::addFastCube(FastCube fc, Shape s){
    // if we can add the fast cube without breaking the puzzle, we do
    if ((this->fc & fc) == 0){
        this->fc += fc;
        fcv.push_back(fc);
        sv.push_back(s);
        return true;
    }
    return false;
}

void Puzzle::removeLastCube() {
    this->fc -= fcv.back();
    fcv.resize(fcv.size() - 1);
    sv.resize(sv.size() - 1);
}

Cube Puzzle::getCodedCube(bool ShapeEncoding) const{
    Cube cc = EMPTY_CUBE;
    for (size_t i = 0; i < fcv.size(); i++){
        size_t encoding = (ShapeEncoding)? (size_t)sv.at(i) : i;
        for(size_t j = 0; j <= encoding; j++)
            cc += FastCubeToCube(fcv.at(i));
    }
    
    return cc;
}

// two puzzles are the same if they have "similar" solutions
bool operator== (const Puzzle& p1, const Puzzle& p2) {
    for(size_t i = 0; i < p1.sortedfcv.size() && i < p2.sortedfcv.size(); i++)
        if(p1.sortedfcv.at(i) != p2.sortedfcv.at(i)) 
            return false;
    return true;
}