#include "Labo2_Types.h"

using namespace std;
ostream& operator<< (ostream& os, const Cube& c){
    cout << "     top       mid      bottom\n";
    for(int y = 0; y < length; y++){
        for(int x = 0; x < length; x++){
            cout << "[";
            for(int z = 0; z < length; z++){
               cout << c[x][y][z];
               if(z < length - 1)
                   cout << ", ";
            }
            cout << "] ";
        }
        cout << endl;
    }
        cout << endl;
    
    return os;
}

// allows to generate all 6 spacial rotations of a cube
cubeVector allSpaceRotations(const Cube& c){
    cubeVector cv {c, EMPTY_CUBE, EMPTY_CUBE, EMPTY_CUBE, EMPTY_CUBE, EMPTY_CUBE};
    for(int c = 1; c < 6;c++) 
        for(int x = 0; x < length; x++)
            for(int y = 0; y < length; y++)
                for(int z = 0; z < length; z++){
                    if(c < 4)
                        // generate rotations bases on previous rotated cube
                        cv.at(c)[x][y][z] = cv.at(c - 1)[length - 1 - y][x][z];
                    else{
                        // generate last rotations bases on rotations 0 and 2.
                        cv.at(c)[z][x][y] = cv.at((c - 4)*2)[length - 1 - y][x][z];
                    }
                }
    return cv;
    
}

// allows to generate all 4 facial rotations of a cube
cubeVector allFaceRotations(const Cube& c){
    cubeVector cv {c, EMPTY_CUBE, EMPTY_CUBE, EMPTY_CUBE};
    for(int c = 1; c < 4;c++)
        for(int x = 0; x < length; x++)
            for(int y = 0; y < length; y++)
                for(int z = 0; z < length ; z++)
                    // generate rotations bases on previous rotated cube
                    cv.at(c)[x][y][z] =  cv.at(c - 1)[x][length - 1 - z][y];

    return cv;
    
}
// generates a cubeVector containing all existing spacial duplicates of a given cube
cubeVector allDuplicates(const Cube& c){
    cubeVector cv;
    for(Cube c1 : allFaceRotations(c))
        for (Cube c2 : allSpaceRotations(c1))
            cv.push_back(c2);
    return cv;
}

bool operator== (const Cube& c1, const Cube& c2){
    // we check if c1 is a spacial duplication of c2
    for(Cube dc : allDuplicates(c2)){
        // we assume it's a duplicate, and we try to disprove it
        bool duplicate = true;
        for(int x = 0; x < length; x++)
            for(int y = 0; y < length; y++)
                for(int z = 0; z < length; z++)
                    duplicate &= c1[x][y][z] == dc[x][y][z];
         if(duplicate)
             return true;
    }
    return false;
}