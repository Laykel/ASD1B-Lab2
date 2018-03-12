#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <ctime>
#include "Labo2_Types.h"
#include "Labo2_Puzzle.h"
#include "Labo2_Pieces.h"

using namespace std;
using puzzleVector = vector<Puzzle>;


int main() {
    time_t start = time(NULL);
    int bruteforce_progress_ctr = 1, unique_progress_ctr = 0, solution_ctr = 0;
    Puzzle p;
    puzzleVector solutions, uniqueSolutions;
    solutions.reserve(500000);
    uniqueSolutions.reserve(1000);
    // generate FastCubeVectors, and reserve their size (had crashes on my machine if i didn't reserve space before pushing contents)
    fastCubeVector allL,
               allS,
               allC,
               allT;
               
    allL.reserve(300);
    allS.reserve(300);
    allC.reserve(300);
    allT.reserve(300);
               
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
    bool valid;
    Cube tmp;
    for(FastCube l : allL)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    tmp = shift(FastCubeToCube(l), valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    if(valid && find(allL.begin(), allL.end(), CubeToFastCube(tmp)) == allL.end()){
                        allL.push_back(CubeToFastCube(tmp));
                    }
                }
                
    for(FastCube s : allS)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    tmp = shift(FastCubeToCube(s), valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    if(valid && find(allS.begin(), allS.end(), CubeToFastCube(tmp)) == allS.end()){
                        allS.push_back(CubeToFastCube(tmp));
                    }
                }
                
    for(FastCube t : allT)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    tmp = shift(FastCubeToCube(t), valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    if(valid && find(allT.begin(), allT.end(), CubeToFastCube(tmp)) == allT.end()){
                        allT.push_back(CubeToFastCube(tmp));
                    }
                }
    
    // since we don't want any rotations for C, we only add shifts that are not rotations of present elements.
    for(FastCube c : allC)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    tmp = shift(FastCubeToCube(c), valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    if(valid && none_of(allC.begin(), allC.end(), [&](FastCube fc){return areSimilar(CubeToFastCube(tmp), fc);})){
                        allC.push_back(CubeToFastCube(tmp));
                    }
                }
                    
    cout << "All shapes generated" << endl;
    cout << "diferent 'L' shape locations and orientations : " << allL.size() << endl
         << "diferent 'S' shape locations and orientations : " << allS.size() << endl
         << "diferent 'C' shape locations and orientations : " << allC.size() << endl
         << "diferent 'T' shape locations and orientations : " << allT.size() << endl;

    
    for (FastCube c : allC){
        if(p.addFastCube(c, Shape::C)){
            for(FastCube s : allS){
                if(p.addFastCube(s, Shape::S)){
                    for(FastCube t : allT){
                        if(p.addFastCube(t, Shape::T)){
                            for(FastCube l1 : allL){
                                if(p.addFastCube(l1, Shape::L)){
                                    for(FastCube l2 : allL){
                                        if(p.addFastCube(l2, Shape::L)){
                                            for(FastCube l3 : allL){
                                                if(p.addFastCube(l3, Shape::L)){       
                                                    for(FastCube l4 : allL){
                                                        p.addFastCube(l4, Shape::L);
                                                        if(p.isFilled()){
                                                            // stuff to do with solution
                                                            solution_ctr++;
                                                            solutions.push_back(Puzzle(p));
                                                            p.removeLastCube();
                                                        }
                                                    }
                                                p.removeLastCube();
                                                }
                                            }
                                        p.removeLastCube();
                                        }
                                    }
                                p.removeLastCube();
                                }
                            }
                        p.removeLastCube();
                        }
                    }
                p.removeLastCube();
                }
            }
        p.removeLastCube();
                cout << '\r' << "solution bruteforce progress : " 
                     << bruteforce_progress_ctr++ << "/" << allC.size()
                     << " (" << bruteforce_progress_ctr/(float)allC.size()*100.f
                     << "%), " << solution_ctr << " solutions already found.     " 
                     << flush;
        }
    }
    cout << endl << "copying unique solutions, could take a while : " << endl;
    for(Puzzle& p1 : solutions){
            unique_progress_ctr++;
        if(find(uniqueSolutions.begin(), uniqueSolutions.end(), p1) == uniqueSolutions.end()){
            uniqueSolutions.push_back(p1);
            cout << '\r' << uniqueSolutions.size() << " unique solutions found. "
                 << " (Progress : " << unique_progress_ctr << "/" << solutions.size()  
                 << ")" << flush;
        }
    }
    
    cout << endl << solution_ctr << " solutions, " << uniqueSolutions.size() 
         << " unique solutions, found in " << (time(NULL) - start) << " seconds";
    
    int i;
    cin >> i;
   return 0;
}