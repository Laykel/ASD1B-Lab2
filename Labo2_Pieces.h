/**
 \file   Labo2_Pieces.h
 \author Filipe Fortunato
 \date   07.03.2018

 Definition of the shapes used in the simulation.
 The shapes' rotations are computed, not stored.
*/

#ifndef PIECES_H
#define PIECES_H
#include "Labo2_Types.h"

// define empty lines and empty squares to simplify the rest of the header
#define EL {0,0,0}
#define ES {EL,EL,EL}

Cube L1({{{0,0,1},
        {0,0,1},
        {0,1,1}},
        ES,
        ES}),
   
L2({{{0,1,1},
     {0,0,1},
     {0,0,1}},
     ES,
     ES});
      
Cube C ({{{1,1,0},
          {1,0,0},
          EL},
          ES,
          ES});
      
Cube T ({{{1,1,1},
          {0,1,0},
          EL},
          ES,
          ES});
      
Cube S1({{{1,0,0},
          {1,1,0},
          {0,1,0}},
          ES,
          ES}),
      
S2({{{0,1,0},
     {1,1,0},
     {1,0,0}},
     ES,
     ES});

FastCube FCL1 = CubeToFastCube(L1), 
         FCL2 = CubeToFastCube(L2),
         FCS1 = CubeToFastCube(S1),
         FCS2 = CubeToFastCube(S2),
         FCC = CubeToFastCube(C),
         FCT = CubeToFastCube(T);
      
Cube Test({{{1,2,3},
            {4,5,6},
            {7,8,9}},
            {{11,12,13},
            {14,15,16},
            {17,18,19}},
            {{21,22,23},
            {24,25,26},
            {27,28,29}}});
#endif
