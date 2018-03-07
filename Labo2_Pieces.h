#ifndef PIECES_H
#define PIECES_H
#include "Labo2_Piece.h"

// defines pour reduire la taille du header, Empty line & Empty Square
#define EL {0,0,0}
#define ES {EL,EL,EL}

    cube L1({{{0,0,1},
              {0,0,1},
              {0,1,1}},
              ES,
              ES}),
          
         L2({{{0,1,1},
              {0,0,1},
              {0,0,1}},
              ES,
              ES}),
          
         L3({{{0,1,0},
              {0,1,0},
              {0,1,1}},
              ES,
              ES}),
           
         L4({{{0,1,1},
              {0,1,0},
              {0,1,0}},
              ES,
              ES}),
       
         L5({{{1,1,1},
              {1,0,0},
              EL},
              ES,
              ES}),
           
         L6({{{1,1,1},
              {0,0,1},
              EL},
              ES,
              ES}),
           
         L7({{{0,0,1},
              {1,1,1},
              EL},
              ES,
              ES}),
           
         L8({{{1,0,0},
              {1,1,1},
              EL},
              ES,
              ES});
              
#endif