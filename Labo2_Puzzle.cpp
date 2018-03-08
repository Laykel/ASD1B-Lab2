#include "Labo2_Puzzle.h"

Puzzle::Puzzle(const Cube& c) {
    this->c = c;
}


bool Puzzle::isFilled() const {
    for(Square s : c)
        for(Line l : s)
            for (int s : l)
                if (s != 1)
                    return false;
    return true;
}

bool Puzzle::addCube(const Cube& c) {
    this->c += c;
    cc.push_back(c);
    return isValid();
}

bool Puzzle::isValid() const {
    for(Square s : c)
        for(Line l : s)
            for (int s : l)
                if (s > 1)
                    return false;
    return true;
}
