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

void bruteforceSolutions(puzzleVector& solutions, fastCubeVectorVector& pieces,
                         shapeVector& sv, Puzzle& p, size_t index, 
                         bool verbose = false){
    
    // trivial error
    if (index >= sv.size() || index >= pieces.size())
        return;
    // other trivial error
    if(pieces.empty() || sv.empty())
        return;
    unsigned progress = 0;
    
    for(FastCube fc : pieces.at(index)){
        if(p.addFastCube(fc, sv.at(index))){
            if(p.isFilled()){
                // stuff to do with solutions
                solutions.push_back(Puzzle(p));
            }
            bruteforceSolutions(solutions, pieces, sv, p, index+1);
            p.removeLastCube();
        }
        
        if(verbose){
            cout << '\r' << "solution bruteforce progress : " 
                 << ++progress << "/" << pieces.at(index).size() << " (" 
                 << progress/(float)pieces.at(index).size()*100.f << "%), " 
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
    for(FastCube l : fcv)
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
                    if(valid && none_of(fcv.begin(), fcv.end(), [&](FastCube fc){return areSimilar(CubeToFastCube(tmp), fc);})){
                        fcv.push_back(CubeToFastCube(tmp));
                    }
                }
}

int main() {
    time_t start = time(NULL);
    Puzzle p;
    puzzleVector solutions, uniqueSolutions;
    solutions.reserve(500000);
    uniqueSolutions.reserve(1000);
    // generate FastCubeVectors, and reserve their size (had crashes on my machine if i didn't reserve space before pushing contents)
    fastCubeVector allL,
               allS,
               allC,
               allT;
               
    allL.reserve(150);
    allS.reserve(150);
    allC.reserve(150);
    allT.reserve(150);
               
    // add the rotations
    for(Cube& l : allRotations(L1))
        allL.push_back(CubeToFastCube(l));
    for(Cube& l : allRotations(L2))
        allL.push_back(CubeToFastCube(l));
    for(Cube& s : allRotations(S1))
        allS.push_back(CubeToFastCube(s));
    for(Cube& s : allRotations(S2))
        allS.push_back(CubeToFastCube(s));
    for(Cube& t : allRotations(T))
        allT.push_back(CubeToFastCube(t));
    // for the c cube, we don't want any rotations, as to remove any rotational duplicates
    allC.push_back(CubeToFastCube(C));
    
    // generate each known translation and add it if not present
    generateValidShifts(allL);
    generateValidShifts(allS);
    generateValidShifts(allT);
    generateUniqueShifts(allC);
    
                    
    cout << "All shapes generated" << endl;
    cout << "diferent 'L' shape locations and orientations : " << allL.size() << endl
         << "diferent 'S' shape locations and orientations : " << allS.size() << endl
         << "diferent 'T' shape locations and orientations : " << allT.size() << endl
         << "diferent 'C' shape locations and orientations : " << allC.size() << endl;

    fastCubeVectorVector fcvv{allC, allS, allT, allL, allL, allL, allL};
    shapeVector sv{Shape::C, Shape::S, Shape::T, Shape::L, Shape::L, Shape::L, Shape::L};
    // all shapes are generated
    bruteforceSolutions(solutions, fcvv,  sv, p, 0, true);
    cout << endl << solutions.size() << " solutions found in " << (time(NULL) - start) << " seconds";
    
    
    start = time(NULL);
    cout << endl << "copying unique solutions, could take a while : " << endl;
    copyUniqueSolutions(solutions, uniqueSolutions, true);
    
    cout << endl << uniqueSolutions.size() << " unique solutions, found in " 
         << (time(NULL) - start) << " seconds";
    
    int i;
    cin >> i;
   return 0;
}