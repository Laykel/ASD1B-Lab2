#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include "Labo2_Types.h"
#include "Labo2_Puzzle.h"
#include "Labo2_Pieces.h"



using namespace std;
using puzzleVector = vector<Puzzle>;
using fastCubeVectorVector = std::vector<fastCubeVector>;

void appendStringToFile(const string& str, const string& filename){
    ofstream ofs(filename, ofstream::app);
    ofs << str;
    ofs.close();
}

void bruteforceSolutions(puzzleVector& solutions, const fastCubeVectorVector& pieces,
                         const shapeVector& sv, Puzzle& p, size_t index, 
                         size_t wantedSolutions = -1, bool verbose = false){
    // trivial error
    if (index <= 0)
        return;
    // other trivial error
    if(pieces.empty() || sv.empty())
        return;
    // if all solutions are found, we can just quit (since we add all the L shapes 
    // at the end, we need to multiply the wanted solutions by 24 to have the right
    // amount of solutions).
    if(wantedSolutions != size_t(-1) && solutions.size() >= wantedSolutions * 24)
        return;
    unsigned progress = 0;
    
    for(const FastCube& fc : pieces.at(index - 1)){
        if(p.addFastCube(fc, sv.at(index - 1))){
            if(p.isFilled()){
                // stuff to do with solutions
                solutions.push_back(Puzzle(p));
            }
            bruteforceSolutions(solutions, pieces, sv, p, index - 1, wantedSolutions);
            p.removeLastCube();
        }
        ++progress;
        if(verbose){
            cout << '\r' << "solution bruteforce progress : " 
                 << progress << "/" << pieces.at(index - 1).size() << " (" 
                 << (float)progress/(float)pieces.at(index - 1).size()*100.f << "%), " 
                 << solutions.size() << " solutions already found.       " << flush;
        }
    }
    
}

void copyUniqueSolutions(puzzleVector& solutions, puzzleVector& uniqueSolutions,
                         bool verbose = false, bool saveSolutions = false){
    
    unsigned ctr = 0;
    for(Puzzle& p : solutions){
            ctr++;
        if(find(uniqueSolutions.begin(), uniqueSolutions.end(), p) == uniqueSolutions.end()){
            uniqueSolutions.push_back(p);
            if(saveSolutions){
                string tmp = toString(p.getCodedCube(false)) + " : " + toString(p.getCodedCube(true)) + '\n';
                appendStringToFile(tmp, "solutions.txt");
            }
            if(verbose && ctr % 5 == 0)
                cout << '\r' << uniqueSolutions.size() << " unique solutions found."
                     << " (Progress : " << ctr << "/" << solutions.size() << ")" 
                     << flush;
        }
    }
}

void generateValidShifts(fastCubeVector& fcv){
    Cube tmp;
    bool valid = false;
    for(FastCube& l : fcv)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    tmp = shift(FastCubeToCube(l), valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    if(valid && find(fcv.begin(), fcv.end(), CubeToFastCube(tmp)) == fcv.end()){
                        fcv.push_back(CubeToFastCube(tmp));
                    }
                }
}

void generateUniqueShifts(fastCubeVector& fcv){
    Cube tmp;
    bool valid = false;
    for(FastCube c : fcv)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    tmp = shift(FastCubeToCube(c), valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    if(valid && none_of(fcv.begin(), fcv.end(), [&](FastCube fc){return areSimilar(tmp, FastCubeToCube(fc));})){
                        fcv.push_back(CubeToFastCube(tmp));
                    }
                }
}
// verbose lvl 1 : cout only progress in brutefoce
// verbose lvl 2 : cout only progress in piece gen
// verbose lvl 3 : cout both progress
void findSolutions(puzzleVector& solutions, const fastCubeVectorVector& uniquePieces,
                   const shapeVector& sv, size_t wantedSolutions = -1, 
                   bool genAllRotations = false, unsigned verboseLevel = 0){
    time_t start = time(NULL);
    fastCubeVectorVector pieces;
    Puzzle p;
    // if we don't want all rotations, we don't initialize the last piece automaticly
    for(size_t i = 0; i < uniquePieces.size() - (genAllRotations? 0 : 1); i++){
        fastCubeVector fcv;
        for(const FastCube& fc : uniquePieces.at(i))
            for(const Cube& cr : allRotations(FastCubeToCube(fc)))
                fcv.push_back(CubeToFastCube(cr));
        generateValidShifts(fcv);
        pieces.push_back(fcv);
    }
    // if we want all rotations, we don't initialize the last pieces manually
    if(!genAllRotations){
        // initial, non-rotating piece gen
        pieces.push_back(fastCubeVector());
        for(const FastCube& fc : uniquePieces.back())
                pieces.back().push_back(fc);
        generateUniqueShifts(pieces.back());
    }
    if(verboseLevel == 1 || verboseLevel == 3){
        cout << "All shapes generated" << endl;
        for(size_t i = 0; i < pieces.size(); i++)
            cout << "shape " << sv.at(i) << " locations and orientations : " 
                 << pieces.at(i).size() << endl;
    }
             
    bruteforceSolutions(solutions, pieces, sv, p, pieces.size(), wantedSolutions,
                        verboseLevel > 1);
    
    // if we are not generating solutions with all rotations, we divide the final result
    // by 2, because of a 180° rotation in the axis 0,0,0 -> 1,1,1 , which is a pain to detect.
    if(verboseLevel > 1)
    cout << endl << solutions.size() / (1 + !genAllRotations) << " solutions found in " 
         << (time(NULL) - start) << " seconds";
}


int main() {
    puzzleVector solutions, uniqueSolutions;
    solutions.reserve(500000);
    uniqueSolutions.reserve(1000);
    
    fastCubeVector fcvC{FCC}, fcvT{FCT}, fcvS{FCS1,FCS2}, fcvL{FCL1,FCL2};
    fastCubeVectorVector InitLists {fcvL,fcvC,fcvT,fcvS};
    vector<unsigned> signatures;

    time_t start = time(NULL);
    for(size_t i = 0; i < 4; ++i)
        for(size_t j = 0; j < 4; ++j)
            for(size_t k = 0; k < 4; ++k)
                for(size_t l = 0; l < 4; ++l)
                    for(size_t m = 0; m < 4; ++m)
                        for(size_t n = 0; n < 4; ++n)
                            for(size_t o = 0; o < 4; ++o){
                                unsigned cubeCount = 0;
                                unsigned signature = 1;
                                for(int val : {i,j,k,l,m,n,o}){
                                    cubeCount += SHAPE_TO_CUBES_COUNT[(int)Shape(val)];
                                    signature *= SHAPE_TO_SIGNATURE[(int)Shape(val)];
                                }
                                if(cubeCount != length*length*length ||
                                   find(signatures.begin(), signatures.end(), signature)
                                   != signatures.end())
                                   continue;
                                signatures.push_back(signature);
                                
                                cout << endl << "Testing combination : "; 
                                cout << Shape(i) << Shape(j) << Shape(k) << Shape(l)
                                     << Shape(m) << Shape(n) << Shape(o) << endl;
                                     
                                findSolutions(solutions, 
                                    {InitLists.at(i), InitLists.at(j), InitLists.at(k),
                                     InitLists.at(l), InitLists.at(m), InitLists.at(n),
                                     InitLists.at(o)},
                                    {Shape(i), Shape(j), Shape(k), Shape(l), 
                                     Shape(m), Shape(n), Shape(o)}, -1, false, 2);
                                     
                                cout << endl;
                                if(solutions.size())
                                    cout << "Solutions exist for combination : ";
                                else 
                                    cout << "No solutions for combination : "; 
                                cout << Shape(i) << Shape(j) << Shape(k) << Shape(l)
                                     << Shape(m) << Shape(n) << Shape(o) << endl;
                                copyUniqueSolutions(solutions, uniqueSolutions, false);
                                cout << "unique solutions : " 
                                     << uniqueSolutions.size() / 2 << endl;
                                solutions.resize(0);
                                uniqueSolutions.resize(0);
                            }
    cout << (time(NULL) - start) << " seconds to compute all solutions and unique solutions for all possible combinations of pieces that fit in a 3x3x3 cube.";
    
    int i;
    cin >> i;
   return 0;
}