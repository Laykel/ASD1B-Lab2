#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include "Labo2_Types.h"
#include "Labo2_Puzzle.h"
#include "Labo2_Piece.h"
#include "Labo2_Pieces.h"

using namespace std;

int main() {
    Piece p(L1);    
    cout << "Piece 1, Rot 0 :\n" << p;
    
    p = p.rotate();
    cout << "Piece 1, Rot 2 :\n" << p;
    
    p = p.rotate();
    cout << "Piece 1, Rot 3 :\n" << p;
    
    p = p.rotate();
    cout << "Piece 1, Rot 4(0) :\n" << p;
    
   return 0;
}