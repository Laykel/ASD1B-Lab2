#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include "Labo2_Types.h"
#include "Labo2_Puzzle.h"
#include "Labo2_Pieces.h"

#define CLEAR_BUFFER cin.ignore(numeric_limits<streamsize>::max(), '\n');

using namespace std;
using puzzleVector = vector<Puzzle>;
using fastCubeVectorVector = std::vector<fastCubeVector>;

// function to open a file with "filename" name, and append the contents of string
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
    // if all solutions are found, we can just quit.
    if(wantedSolutions != size_t(-1) && solutions.size() >= wantedSolutions)
        return;
    
    unsigned progress = 0;
    
    for(const FastCube& fc : pieces.at(index - 1)){
        if(p.addFastCube(fc, sv.at(index - 1))){
            if(p.isFilled()){
                // stuff to do with solutions
                solutions.push_back(Puzzle(p));
            }
            // recursive call
            bruteforceSolutions(solutions, pieces, sv, p, index - 1, wantedSolutions);
            
            p.removeLastCube();
        }
        ++progress;
        if(verbose){
            cout << '\r' << "solution bruteforce progress : " 
                 << progress << "/" << pieces.at(index - 1).size() << " (" 
                 << (float)progress/(float)pieces.at(index - 1).size()*100.f 
                 << "%)        " << flush;
        }
    }
    
}

// function that copies all of the unique solutions, from a piece swap duplication
// point of view. can also save the found solutions to a "solutions.txt" file
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
                cout << "\rcopying unique solutions (Progress : " << ctr << "/" 
                     << solutions.size() << ")" << flush;
        }
    }
}

// generate all valid shifts and add them to the vector if not present.
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

// generate all unique shifts, from a rotational point of view 
// (if a shift can be found with a rotation of another shift, don't add it)
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

// recursively computes a factorial of a positive integer.
unsigned factorial(unsigned n){
    if(n == 0)
        return 1;
    return n * factorial(n - 1);
}

// verbose lvl 1 : no cout
// verbose lvl 1 : cout only progress in bruteforce
// verbose lvl 2 : cout only progress in piece gen
// verbose lvl 3 : cout both progress
void findSolutions(puzzleVector& solutions, const fastCubeVectorVector& uniquePieces,
                   const shapeVector& sv, size_t wantedSolutions = -1, 
                   bool genUniqueSolutions = false, bool genAllRotations = false,
                   unsigned verboseLevel = 0){
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
    
    if(verboseLevel >= 2){
        cout << "All shapes generated" << endl;
        for(size_t i = 0; i < pieces.size(); i++)
            cout << "shape " << sv.at(i) << " locations and orientations : " 
                 << pieces.at(i).size() << endl;
    }
    
    // here we compute, according to the shapes used, the number of duplications by
    // permutation of 2 similar shapes
    unsigned duplicateSolutions = 1;
    shapeVector tmp_sv(sv);
    
    while(!tmp_sv.empty()){
        unsigned duplicates;
        auto it = remove(tmp_sv.begin(), tmp_sv.end(), tmp_sv.at(0));
        duplicates = distance(it, tmp_sv.end());
        duplicateSolutions *= factorial(duplicates); 
        tmp_sv.resize(it - tmp_sv.begin());
    }
    // if the currently wanted solutions is not max and we want unique solutions, we
    // multiply the wanted solutions by the amount of duplicates existing, otherwise
    // we leave it as is.
    wantedSolutions *= (wantedSolutions == (unsigned)-1 || !genUniqueSolutions)?
                                                            1 :  duplicateSolutions;
    
    bruteforceSolutions(solutions, pieces, sv, p, pieces.size(), wantedSolutions,
                        verboseLevel == 1 || verboseLevel == 3);
    
    // if we are not generating solutions with all rotations, we divide the final result
    // by 2, because of a 180° rotation in the axis 0,0,0 -> 1,1,1 , which is a pain to detect.
    
    cout << endl << solutions.size() / (1 + !genAllRotations) << " solutions found in " 
         << (time(NULL) - start) << " seconds" << endl;
    
    start = time(NULL);
    // if we want the unique solutions, then we compute them and set them in the solutions slot
    if(genUniqueSolutions){
        puzzleVector uniqueSolutions;
        uniqueSolutions.reserve(solutions.size() / duplicateSolutions);
        copyUniqueSolutions(solutions, uniqueSolutions, 
                            verboseLevel == 1 || verboseLevel == 3);
        solutions = uniqueSolutions;
        // we divide the solution number due to the spacial rotation we can't detect
        // if we don't want the rotations
        cout << endl << "unique solutions : " 
             << uniqueSolutions.size() / (1 + !genAllRotations) << " found in "
             << (time(NULL) - start) << " seconds" << endl;
    }
}

// messy code used to solve the last 2 questions, takes roughly 3-4 minutes of run time
void bruteforceAllShapeCombinations(){
    puzzleVector solutions;
    solutions.reserve(500000);
    
    // piece vectors (add any different shaped pieces to these).
    fastCubeVector fcvC{FCC}, fcvT{FCT}, fcvS{FCS1,FCS2}, fcvL{FCL1,FCL2};
    
    // piece vectors vector (vector containing all vectors defined above).
    fastCubeVectorVector InitLists {fcvL, fcvC, fcvT, fcvS};
    
    // vector containing piece combinations signatures, used to differentiate between solutions
    vector<unsigned> signatures;
    vector<string> permutationsWithSolutions;
    
    // we initialize the lowest solution count at the biggest unsigned value
    size_t LowestSolutionCount = -1; 
    // the best permutation
    string bestSolution = "";
    
    size_t size = InitLists.size();

    time_t start = time(NULL);
    for(size_t i = 0; i < size; ++i)
        for(size_t j = 0; j < size; ++j)
            for(size_t k = 0; k < size; ++k)
                for(size_t l = 0; l < size; ++l)
                    for(size_t m = 0; m < size; ++m)
                        for(size_t n = 0; n < size; ++n)
                            for(size_t o = 0; o < size; ++o){
                                // the count of small cubes in all current pieces
                                unsigned cubeCount = 0;
                                // the signature of the current combination of pieces,
                                // each different combination of pieces has a different
                                // signature due to using prime numbers to sign each piece.
                                unsigned signature = 1;
                                
                                for(int val : {i,j,k,l,m,n,o}){
                                    cubeCount += SHAPE_TO_CUBES_COUNT[(int)Shape(val)];
                                    signature *= SHAPE_TO_SIGNATURE[(int)Shape(val)];
                                }
                                
                                // if the sum of cubes in the selected pieces is not
                                // length^3 (in current case, 3^3) or if the list of
                                // signatures already contains the current piece
                                // permutation, we skip this for loop
                                if(cubeCount != length*length*length ||
                                   find(signatures.begin(), signatures.end(), signature)
                                   != signatures.end())
                                   continue;
                                signatures.push_back(signature);
                                string combination = SHAPE_TO_STRING[i] + 
                                                     SHAPE_TO_STRING[j] +
                                                     SHAPE_TO_STRING[k] +
                                                     SHAPE_TO_STRING[l] +
                                                     SHAPE_TO_STRING[m] +
                                                     SHAPE_TO_STRING[n] +
                                                     SHAPE_TO_STRING[o];
                                cout << "Testing combination : "; 
                                cout << combination << endl;
                                
                                // here we try to find all solutions to the current
                                // piece combination (we could eventually optimise it
                                // to only search 1 solution and implement another
                                // function to get the permutation with the smallest)
                                // solutions count
                                findSolutions(solutions, 
                                    {InitLists.at(i), InitLists.at(j), InitLists.at(k),
                                     InitLists.at(l), InitLists.at(m), InitLists.at(n),
                                     InitLists.at(o)},
                                    {Shape(i), Shape(j), Shape(k), Shape(l), 
                                     Shape(m), Shape(n), Shape(o)},
                                     LowestSolutionCount + 1, true, false, 2);
                                     
                                cout << endl;
                                if(solutions.size()){
                                    cout << "Solutions exist for combination : ";
                                    permutationsWithSolutions.push_back(combination);
                                    if(LowestSolutionCount > solutions.size()){
                                        bestSolution = combination;
                                        LowestSolutionCount = solutions.size();
                                    }
                                }
                                else 
                                    cout << "No solutions for combination : "; 
                                cout << combination << endl << endl;
                                solutions.resize(0);
                            }
    cout << (time(NULL) - start) << " seconds to compute all solutions and unique solutions for all possible combinations of pieces that fit in a 3x3x3 cube.";
    
    cout << endl << endl << "combinations with solutions : " << endl;
    for (const string& s : permutationsWithSolutions)
        cout << "    " << s << endl;
    cout << "combination with lowest solution count : " << endl << bestSolution
         << " (solution count : " << LowestSolutionCount / 2 << ")";
}


int main() {
    const bool findAllShapeCombinations = false;
    const unsigned solutionsWanted = 4;
    // verbose lvl 0 : cout only results
    // verbose lvl 1 : cout progress in bruteforce + results
    // verbose lvl 2 : cout progress in piece gen + results
    // verbose lvl 3 : cout both progresses + results
    const unsigned verboseLevel = 3;
    
    // if we want to find all shape combinations with solutions, we do that, and quit
    if(findAllShapeCombinations){
        bruteforceAllShapeCombinations();    
        cout << "Press ENTER to quit : ";
        CLEAR_BUFFER;
        return 0;
    }
        
    puzzleVector solutions;
    solutions.reserve(500000);
    
    // piece vectors (add any different shaped pieces to these).
    fastCubeVector fcvC{FCC}, fcvT{FCT}, fcvS{FCS1,FCS2}, fcvL{FCL1,FCL2};
    Shape Sc = Shape::C, St = Shape::T, Ss = Shape::S, Sl = Shape::L;
    
    // piece vectors vector (vector containing all vectors defined above).
    findSolutions(// puzzleVector to store the solutions in
                  solutions,
                  // fastCubeVectorVector containing all piece sets to be used
                  {fcvL, fcvL, fcvL, fcvL, fcvS, fcvT, fcvC}, 
                  // shapeVector containing the shapes of each set used
                  {Sl, Sl, Sl, Sl, Ss, St, Sc}, 
                  // the number of solutions wanted 
                  solutionsWanted,
                  // unique solution switch
                  true, 
                  // all rotation switch (messing with both switches allows to get answers to questions 2->5)
                  false, 
                  // verbose level, from 0 to 3.
                  verboseLevel);
                  
    for(size_t i = 0; i < solutionsWanted && i < solutions.size() ; i++)
        cout << "Solution " << i + 1 << " " << toString(solutions.at(i).getCodedCube())
             << endl << solutions.at(i).getCodedCube();
             
    cout << "Press ENTER to quit : ";
    CLEAR_BUFFER;
   return 0;
}