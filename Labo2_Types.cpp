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
    for(int r = 1; r < 6;r++) 
        for(int x = 0; x < length; x++)
            for(int y = 0; y < length; y++)
                for(int z = 0; z < length; z++){
                    if(r < 4)
                        // generate rotations bases on previous rotated cube
                        cv.at(r)[x][y][z] = cv.at(r - 1)[length - 1 - y][x][z];
                    else{
                        // generate last rotations bases on rotations 0 and 2.
                        cv.at(r)[z][x][y] = cv.at((r - 4)*2)[length - 1 - y][x][z];
                    }
                }
    return cv;
}

// allows to generate all 6 spacial rotations of a fast cube
fastCubeVector allSpaceRotations(FastCube c){
    fastCubeVector fcv {c, EMPTY_FAST_CUBE, EMPTY_FAST_CUBE, EMPTY_FAST_CUBE, 
                        EMPTY_FAST_CUBE, EMPTY_FAST_CUBE};
                        
    for(int r = 1; r < 6;r++) 
        for(int x = 0; x < length; x++)
            for(int y = 0; y < length; y++)
                for(int z = 0; z < length; z++){
                    if(r < 4){
                        // generate rotations bases on previous rotated cube
                        int rhs = (fcv.at(r - 1) >> (length * length * (length - 1 - y) + length * x + z)) & 1;
                        fcv.at(r) |= (rhs << (length * length * x + length * y + z));
                    }
                    else{
                        // generate last rotations bases on rotations 0 and 2.
                        int rhs = (fcv.at((r - 4)*2) >> (length * length * (length - 1 - y) + length * x + z)) & 1;
                        fcv.at(r) |= (rhs << (length * length * x + length * y + z));
                    }
                }
    return fcv;
    
}

// allows to generate all 4 facial rotations of a cube
cubeVector allFaceRotations(const Cube& c){
    cubeVector cv {c, EMPTY_CUBE, EMPTY_CUBE, EMPTY_CUBE};
    for(int r = 1; r < 4;r++)
        for(int x = 0; x < length; x++)
            for(int y = 0; y < length; y++)
                for(int z = 0; z < length ; z++)
                    // generate rotations bases on previous rotated cube
                    cv.at(r)[x][y][z] =  cv.at(r - 1)[x][length - 1 - z][y];

    return cv;
}

// allows to generate all 4 facial rotations of a fast cube
fastCubeVector allFaceRotations(FastCube fc){
    fastCubeVector fcv {fc, EMPTY_FAST_CUBE, EMPTY_FAST_CUBE, EMPTY_FAST_CUBE};
    for(int r = 1; r < 4;r++)
        for(int x = 0; x < length; x++)
            for(int y = 0; y < length; y++)
                for(int z = 0; z < length ; z++){
                    // generate rotations bases on previous rotated cube
                    int rhs = (fcv.at(r - 1) >> (length * length * x + length * (length - 1 - z) + y)) & 1;
                    fcv.at(r) |= (rhs << (length * length * x + length * y + z));
                }
    return fcv;
}

// generates a cubeVector containing all existing spacial rotations of a given cube
cubeVector allRotations(const Cube& c){
    cubeVector cv;
    for(Cube& c1 : allFaceRotations(c))
        for (Cube& c2 : allSpaceRotations(c1))
            cv.push_back(c2);
    return cv;
}

// generates a fastCubeVector containing all existing spacial rotations of a given fast cube
fastCubeVector allRotations(FastCube fc){
    fastCubeVector fcv;
    for(FastCube fc1 : allFaceRotations(fc))
        for (FastCube fc2 : allSpaceRotations(fc1))
            fcv.push_back(fc2);
    return fcv;
}

Cube shift(const Cube& c, bool& sucess, int dx, int dy, int dz){
    Cube shifted = EMPTY_CUBE;
    unsigned cubeSize = 0, shiftedSize = 0;
    for(int x = 0; x < length; x++)
        for(int y = 0; y < length; y++)
            for(int z = 0; z < length; z++){
                cubeSize += c[x][y][z];
                if(x + dx >= 0 && x + dx < length &&
                   y + dy >= 0 && y + dy < length &&
                   z + dz >= 0 && z + dz < length){
                        shifted[x + dx][y + dy][z + dz] = c[x][y][z];
                        shiftedSize += c[x][y][z];
                   }
            }
    sucess = (cubeSize == shiftedSize);
    return shifted;
}

bool operator== (const Cube& c1, const Cube& c2){
    for(int x = 0; x < length; x++)
        for(int y = 0; y < length; y++)
            for(int z = 0; z < length; z++)
                if (c1[x][y][z] != c2[x][y][z])
                    return false;
    return true;
}

string toString(const Cube& c){
    string str;
    for(int x = 0; x < length; x++)
        for(int y = 0; y < length; y++)
            for(int z = 0; z < length; z++)
                str += to_string(c[x][y][z]);
    return str;
}

bool areSimilar(const Cube& c1, const Cube& c2){
    for(Cube& fr : allFaceRotations(c1))
            if (c2 == fr)
                return true;
    return false;
}

bool areSimilar(FastCube fc1, FastCube fc2){
    for(FastCube fr : allFaceRotations(fc1))
            if (fc2 == fr)
                return true;
       
    return false;
}

FastCube CubeToFastCube(const Cube& c){
    FastCube fc = 0;
    int ctr = 0;
    for(int x = 0; x < length; x++)
        for(int y = 0; y < length; y++)
            for(int z = 0; z < length; z++)
                fc |= (c[x][y][z] << ctr++);
    return fc;
}

Cube FastCubeToCube(FastCube fc){
    Cube c = EMPTY_CUBE;
    int ctr = 0;
    for(int x = 0; x < length; x++)
        for(int y = 0; y < length; y++)
            for(int z = 0; z < length; z++)
                c[x][y][z] = (fc >> ctr++) & 1;
    return c;
    
}