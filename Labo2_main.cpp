#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <ctime>
#include "Labo2_Types.h"
#include "Labo2_Puzzle.h"
#include "Labo2_Pieces.h"

using namespace std;

int main() {
    time_t start = time(NULL);
    int ctr = 0;
    // generate cubeVectors, and reserve their size (had crashes on my machine if i didn't reserve space before pushing contents)
    cubeVector allL,
               allS,
               allC,
               allT;
               
    allL.reserve(300);
    allS.reserve(300);
    allC.reserve(300);
    allT.reserve(300);
               
    // add the rotations
    for(Cube& l : allRotations(L1))
        allL.push_back(l);
    for(Cube& l : allRotations(L2))
        allL.push_back(l);
    for(Cube& s : allRotations(S1))
        allS.push_back(s);
    for(Cube& s : allRotations(S2))
        allS.push_back(s);
    for(Cube& c : allRotations(C))
        allC.push_back(c);
    for(Cube& t : allRotations(T))
        allT.push_back(t);
    
    // generate each known translation and add it if not present
    bool valid;
    Cube tmp;
    for(Cube& l : allL)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    tmp = shift(l, valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    if(valid && none_of(allL.begin(), allL.end(),[&](const Cube& c){return c == tmp;})){
                        allL.push_back(tmp);
                    }
                }
                
    for(Cube& s : allS)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    Cube tmp = shift(s, valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    // dirty hack, but find didn't seem to work
                    if(valid && none_of(allS.begin(), allS.end(),[&](const Cube& c){return c == tmp;})){
                        allS.push_back(tmp);
                    }
                }
                
    for(Cube& c : allC)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    Cube tmp = shift(c, valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    // dirty hack, but find didn't seem to work
                    if(valid && none_of(allC.begin(), allC.end(),[&](const Cube& c1){return c1 == tmp;})){
                        allC.push_back(tmp);
                    }
                }
                
    for(Cube& t : allT)
        for(int x = -length; x < length; x++)
            for(int y = -length; y < length; y++)
                for(int z = -length; z < length; z++){
                    Cube tmp = shift(t, valid, x, y, z);
                    // if it's a valid shift, and is not present in the vector, we add it
                    // dirty hack, but find didn't seem to work
                    if(valid && none_of(allT.begin(), allT.end(),[&](const Cube& c){return c == tmp;})){
                        allT.push_back(tmp);
                    }
                }
                    
            
    cout << "All shapes generated" << endl;
// lol, don't even try to run this shit bellow, needs to have some stuff done before
 /*    for(Cube& l1 : allL) 
        for(Cube& l2 : allL)
            for(Cube& l3 : allL)
                for(Cube& l4 : allL)
                    for(Cube& s : allS)
                        for (Cube& c : allC)
                            for(Cube& t : allT)
                                ctr++; */
    cout << ctr << endl << (time(NULL) - start);
                            
    
   return 0;
}