#ifndef PIECE_H
#define PIECE_H
#include "Labo2_Types.h"

class Piece {
public:
    Piece(const cube& c);
    cube shift(bool& sucess, short dx = 0, short dy = 0, short dz = 0) const;
private:
    cube c;
    unsigned blockCount = 0;
};

#endif /* PIECE_H */

