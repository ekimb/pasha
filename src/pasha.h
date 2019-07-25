#ifndef PASHA_H
#define PASHA_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <map>
#include <cstdlib>
#include <iomanip>
#include <cstdint>
#include <omp.h>
#include <limits>
#include "half.hpp"
using namespace std;
//using half_float::half;
using byte = uint8_t;
class PASHA {
    public:
        byte* finished;
        byte* pick;
        byte* used;
        double delta;
        double epsilon;
        double* hittingNumArray;
        float** D;
        float* Fcurr;
        float* Fprev;
        //byte** Dexp;
        //byte** Dval;
        unsigned int ALPHABET_SIZE;
        double edgeCount;
        double edgeNum;
        unsigned int k;
        unsigned int l;
        unsigned int h;
        double count;
        unsigned int total;
        unsigned int curr;
        unsigned int vertexCount; 
        unsigned int vertexExp;
        unsigned int vertexExp2;
        unsigned int vertexExp3;
        unsigned int vertexExpMask;
        unsigned int vertexExp_1;
        byte* edgeArray;
        byte* stageArray;
        int* topoSort;
        static map<char, unsigned int> alphabetMap;
        string ALPHABET;
        vector<unsigned int> stageVertices;
    PASHA (unsigned int argK) {
    /**
    Definition of a graph object. Generates a graph of order k, creates an empty
    edge index array, calculates number of edges, builds a character-index map.
    @param argK: Argument passed as k-mer length.
    */
        ALPHABET = "ACGT";
        ALPHABET_SIZE = 4;
        k = argK;
        edgeNum = pow(ALPHABET_SIZE, k);
        edgeArray = new byte[(unsigned int)edgeNum];
        generateGraph(k);
        map<char, unsigned int> alphabetMap;
        for (unsigned int i = 0; i < ALPHABET_SIZE; i++) alphabetMap.insert(pair<char,unsigned int>(ALPHABET[i], i));
    }

    void generateGraph(unsigned int k) {  
    /**
    Generates a complete de Bruijn graph of order k.
    @param k: Desired k-mer length (order of complete graph).
    */
        for (unsigned int i = 0; i < edgeNum; i++) edgeArray[i] = 1;
        edgeCount = edgeNum;
        vertexCount = edgeNum / ALPHABET_SIZE; 
    }

    char getChar(unsigned int i) {
    /**
    Gets alphabet character from index.
    @param i: Index of character.
    @return The character in the alphabet.
    */
        return ALPHABET[i];
    }

    string getLabel(unsigned int i) {
    /**
    Gets label of the input edge index.
    @param i: Index of edge.
    @return The label of the edge.
    */
        string finalString = "";
        for (unsigned int j = 0; j < k; j++) {
            finalString = getChar((i % ALPHABET_SIZE)) + finalString;
            i = i / ALPHABET_SIZE;
        }
        return finalString;
    }
    int maxLength() {
    /**
    Calculates the length of the maximum length path in the graph.
    @return maxDepth: Maximum length.
    */
        vector<int> depth(vertexExp);
        int maxDepth = -1;
        for (unsigned int i = 0; i < vertexExp; i++) {
            int maxVertDepth = -1;
            for (unsigned int j = 0; j < ALPHABET_SIZE; j++) {
                unsigned int edgeIndex = topoSort[i] + j * vertexExp;
                unsigned int vertexIndex = edgeIndex / ALPHABET_SIZE;
                if ((depth[vertexIndex] > maxVertDepth) && (edgeArray[edgeIndex] == 1)) maxVertDepth = depth[vertexIndex];
            }
            depth[topoSort[i]] = maxVertDepth + 1;
            if (depth[topoSort[i]] > maxDepth) {maxDepth = depth[topoSort[i]];}
        }
        return maxDepth;
    }

    void removeEdge(unsigned int i) {
    /**
    Removes an edge from the graph.
    @param i: Index of edge.
    */
        if (edgeArray[i] == 1) edgeCount--;
        edgeArray[i] = 0;
    }

    void topologicalSort() {
    /**
    Traverses the graph in topological order.
    */
        for (unsigned int i = 0; i < vertexExp; i++) {used[i] = false; finished[i] = false;}
        int index = 0;
        for (unsigned int i = 0; i < vertexExp; i++) {
            if (used[i] == false) {
                index = depthFirstSearch(index, i);
                if (index == -1) {topoSort = NULL; return;}
            }
        }
       // int rc[vertexExp];
        //for (int i = 0; i < vertexExp; i++) rc[i] = topoSort[vertexExp-i-1];
    }
    int depthFirstSearch(int index, unsigned int u) {
    /**
    Depth-first search of a given index of an edge.
    @param index: Depth of recursion, u: Index of edge.
    @return -1: The search cycles, index+1: Current depth.
    */
        used[u] = true;
        bool cycle = false;
        for (unsigned int v : getAdjacent(u)) {
            if (used[v] == true && finished[v] == false) cycle = true;
            if (used[v] == false) {
                index = depthFirstSearch(index, v);
                cycle = cycle || (index == -1);
            }
        }
        finished[u] = true;
        topoSort[index] = u;
        if (cycle) return -1;
        else return index + 1;
    }
    vector<unsigned int> getAdjacent(unsigned int v) {
    /**
    Get adjacent vertices to a given index of a vertex.
    @param v: Index of vertex.
    @return rc: Array of adjacent vertices.
    */
        unsigned int count = 0;
        unsigned int adjVertex[ALPHABET_SIZE];
        for (unsigned int i = 0; i < ALPHABET_SIZE; i++) {
            unsigned int index = v + i * vertexExp;
            if (edgeArray[index] == 1) adjVertex[count++] = index / ALPHABET_SIZE;
        }
        vector<unsigned int> rc(count);
        for (unsigned int i = 0; i < count; i++) {
            rc[i] = adjVertex[i];
        }
        return rc;
    }

    unsigned int HittingRandomParallel(unsigned int L, const char *hittingPath, unsigned int threads) {
    /**
    Performs hitting set calculations with parallelization
    and with randomization, counting L-k+1-long paths.
    @param L: Sequence length, hittingFile: Output file destination.
    @return hittingCount: Size of hitting set.
    */
        omp_set_dynamic(0);
        vertexExp = pow(ALPHABET_SIZE, k-1);
        ofstream hittingStream(hittingPath);
        int hittingCount = 0;
        l = L-k+1;
        delta = 1/(double)l;
        epsilon = (1-8*(delta))/4;
        if (epsilon == 0) epsilon = 0.083333;
        double alpha = 1 - 4*delta -2*epsilon;
        cout << "Alpha: " << 1/alpha << endl;
        cout << "Delta: " << delta << endl;
        cout << "Epsilon: " << epsilon << endl;
        unsigned int i;
        unsigned int j;
        hittingNumArray = new double[(unsigned int)edgeNum];
        stageArray = new byte[(unsigned int)edgeNum];
        used = new byte[vertexExp];
        finished = new byte[vertexExp];
        pick = new byte[(unsigned int)edgeNum];
        topoSort = new int[vertexExp];
        D = new float*[l + 1];
        //Dexp = new byte*[l + 1];
       //float* Dpool = new float[(l+1)* vertexExp];
        for(unsigned int i = 0; i < l+1; i++) {D[i] = new float[vertexExp];}
        //hittingStream.open(hittingFile); 
        Fcurr = new float[vertexExp];
        Fprev = new float[vertexExp];
        //double* Fpool = new double[(l+1)* vertexExp];
       // for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;
        calculatePaths(l, threads);
        int imaxHittingNum = calculateHittingNumberParallel(l, false, threads);
        cout << "Max hitting number: " << hittingNumArray[imaxHittingNum] << endl;
        h = findLog((1.0+epsilon), hittingNumArray[imaxHittingNum]);
        double prob = delta/(double)l;
        while (h > 0) {
            total = 0;
            unsigned int hittingCountStage = 0;
            double pathCountStage = 0;
            calculatePaths(l, threads);
            if (calculateHittingNumberParallel(l, true, threads) < 0) break;
            stageVertices = pushBackVector();
            #pragma omp parallel for num_threads(threads)
            for (unsigned int it = 0; it < stageVertices.size(); it++) {
                i = stageVertices[it];
                #pragma omp critical
                if ((pick[i] == false) && (hittingNumArray[i] > (pow(delta, 3) * total))) {
                    stageArray[i] = 0;
                    pick[i] = true;
                    hittingCountStage++;
                    pathCountStage += hittingNumArray[i];
                }
            }
            #pragma omp parallel for collapse (2) num_threads(threads) 
            for (unsigned int it = 0; it < stageVertices.size(); it++) {
                for (unsigned int jt = 0; jt < stageVertices.size(); jt++) {
                    i = stageVertices[it];
                    #pragma omp critical
                    if (pick[i] == false) {
                        if (((double) rand() / (RAND_MAX)) <= prob) {
                            stageArray[i] = 0;
                            pick[i] = true;
                            hittingCountStage += 1;
                            pathCountStage += hittingNumArray[i];
                        }
                        j = stageVertices[jt];
                        if (pick[j] == false) {
                            if (((double) rand() / (RAND_MAX)) <= prob) {
                                stageArray[j] = 0;
                                pick[j] = true;
                                hittingCountStage += 1;
                                pathCountStage += hittingNumArray[j];

                            }
                        }
                    }
                }
            }
            hittingCount += hittingCountStage;
            #pragma omp barrier
            if (pathCountStage >= hittingCountStage * pow((1.0 + epsilon), h) * (1 - 4*delta - 2*epsilon)) {
                for (unsigned int it = 0; it < stageVertices.size(); it++) {
                    i = stageVertices[it];
                    if (pick[i] == true) {
                        removeEdge(i);
                        string label = getLabel(i);
                        hittingStream << label << "\n";
                        //cout << label << endl;
                    }
                }
                h--;
            }
            else hittingCount -= hittingCountStage;
        }
        hittingStream.close();
        topologicalSort();
        cout << "Length of longest remaining path: " <<  maxLength() << "\n";
        return hittingCount;
    }
    //void calculateForEach(int i, int L) {
/**
Calculates hitting number for an edge of specified index with respect to a specified
sequence length, counting paths of length L-k+1.
@param i: Index of edge, L: Sequence length.
*/
       // double hittingNum = 0;
       // for (int j = (1 - edgeArray[i]) * L; j < L; j++) hittingNum = hittingNum + F[j][i % vertexExp] * D[(L-j-1)][i / ALPHABET_SIZE];
        //hittingNumArray[i] = hittingNum;
    //}
    int calculateHittingNumberParallel(unsigned int L, bool random, unsigned int threads) {
/**
Calculates hitting number of all edges, counting paths of length L-k+1, in parallel.
@param L: Sequence length.
@return imaxHittingNum: Index of vertex with maximum hitting number.
*/  
        omp_set_dynamic(0);
        double maxHittingNum = 0;
        int imaxHittingNum = -1;
        unsigned int count = 0;
        #pragma omp parallel for num_threads(threads)
        for (unsigned int i = 0; i < (unsigned int)edgeNum; i++) {
          //  calculateForEach(i, L);
            if (random == true) {
                if (((hittingNumArray[i]) >= pow((1.0+epsilon), h-1)) && ((hittingNumArray[i]) <= pow((1.0+epsilon), h))) {
                    stageArray[i] = 1;
                    pick[i] = false;
                    total += hittingNumArray[i] * stageArray[i];
                    count++;
                }
                else {
                    stageArray[i] = 0;
                    pick[i] = false;
                }   
            }
        }
        for (unsigned int i = 0; i < (unsigned int)edgeNum; i++) {
            if ((hittingNumArray[i])*edgeArray[i] > maxHittingNum) {
                maxHittingNum = hittingNumArray[i]; imaxHittingNum = i;
                //cout << maxHittingNum << endl;
            }
        }
        return imaxHittingNum;
    }

    int calculatePaths(unsigned int L, unsigned int threads) {
    /**
    Calculates number of L-k+1 long paths for all vertices.
    @param L: Sequence length.
    @return 1: True if path calculation completes.
    */
        omp_set_dynamic(0);
        curr = 1;
        vertexExp2 = vertexExp * 2;
        vertexExp3 = vertexExp * 3;
        vertexExpMask = vertexExp - 1;
        vertexExp_1 = pow(ALPHABET_SIZE, k-2);
        //double maxD = 0;
       // double maxF = 0;
        //Fexp(res) = max(Fexp1, Fexp2, Fexp3, Fexp4)
       // Fval(res) = [Fval1 >> (Fexp - Fexp1)] + [Fval2 >> (Fexp - Fexp2)] + [Fval3 >> (Fexp - Fexp3)] + [Fval4 >> (Fexp - Fexp4)]

        #pragma omp parallel for num_threads(threads)
        for (unsigned int i = 0; i < vertexExp; i++) {D[0][i] = 1.4e-45; Fprev[i] = 1.4e-45;}
        for (unsigned int j = 1; j <= L; j++) {
            #pragma omp parallel for num_threads(threads)
            for (unsigned int i = 0; i < vertexExp; i++) {
                //uint8_t r1;
                //uint8_t r2;
                //uint8_t r3;
                //r1 = (uint8_t)edgeArray[i]*Dexp[j-1][(i >> 2)] ^ (((uint8_t)(edgeArray[i]*Dexp[j-1][(i >> 2)]) ^ (uint8_t)edgeArray[i + vertexExp]*Dexp[j-1][((i + vertexExp) >> 2)])) & -((uint8_t)(edgeArray[i]*Dexp[j-1][(i >> 2)]) < ((uint8_t)edgeArray[i + vertexExp]*Dexp[j-1][((i + vertexExp) >> 2)]));
                //r2 = (uint8_t)edgeArray[i + vertexExp2]*Dexp[j-1][((i + vertexExp2) >> 2)] ^ (((uint8_t)(edgeArray[i + vertexExp2]*Dexp[j-1][((i + vertexExp2) >> 2)]) ^ (uint8_t)edgeArray[i + vertexExp3]*Dexp[j-1][((i + vertexExp3) >> 2)])) & -((uint8_t)(edgeArray[i]*Dexp[j-1][((i + vertexExp) >> 2)]) < ((uint8_t)edgeArray[i + vertexExp3]*Dexp[j-1][((i + vertexExp3) >> 2)]));
                //r3 = (uint8_t)r1 ^ ((r1 ^ r2) & -(r1 < r2));
                //Dexp[j][i] = r3;
                //Dval[j][i] = (Dval[j-1][(i >> 2)] >> (Dexp[j][i] - Dexp[j-1][(i >> 2)]))*edgeArray[i] + (Dval[j-1][((i + vertexExp) >> 2)] >> (Dexp[j][i] - Dexp[j-1][((i + vertexExp) >> 2)]))*edgeArray[i + vertexExp] + (Dval[j-1][((i + vertexExp2) >> 2)] >> (Dexp[j][i] - Dexp[j-1][((i + vertexExp2) >> 2)]))*edgeArray[i + vertexExp2] + (Dval[j-1][((i + vertexExp3) >> 2)] >> (Dexp[j][i] - Dexp[j-1][((i + vertexExp3) >> 2)]))*edgeArray[i + vertexExp3];
                //Dexp[j][i] = Dexp[j][i] + ((128 & Dval[j][i]) >> 7);
                //Dval[j][i] = Dval[j][i] >> ((128 & Dval[j][i]) >> 7);
                //Dexp[j][i] = Dexp[j][i] + ((64 & Dval[j][i]) >> 6);
                //Dval[j][i] = Dval[j][i] >> ((64 & Dval[j][i]) >> 6);
                D[j][i] = edgeArray[i]*D[j-1][(i >> 2)] + edgeArray[i + vertexExp]*D[j-1][((i + vertexExp) >> 2)] + edgeArray[i + vertexExp2]*D[j-1][((i + vertexExp2) >> 2)] + edgeArray[i + vertexExp3]*D[j-1][((i + vertexExp3) >> 2)];
                //cout << (float)(Dval[j][i] * pow(2, Dexp[j][i])) << endl;
                //D[j][i] = Dval[i];
                //if ((float)(Dval[j][i] * pow(2, Dexp[j][i])) > maxD) maxD = ((float)Dval[j][i] * pow(2, Dexp[j][i])); 
            }
            //if (maxD > std::numeric_limits<half>::max()/4) {
             //   for (unsigned int i = 0; i < vertexExp; i++) D[j][i] = D[j][i] * 0.5;
            //}


        }
        #pragma omp parallel for num_threads(threads)
        for (unsigned int i = 0; i < (unsigned int)edgeNum; i++) hittingNumArray[i] = 0;
        while (curr <= L) {
            #pragma omp parallel for num_threads(threads)
            for (unsigned int i = 0; i < vertexExp; i++) {
                unsigned int index = (i * 4);
                Fcurr[i] = (edgeArray[index]*Fprev[index & vertexExpMask] + edgeArray[index + 1]*Fprev[(index + 1) & vertexExpMask] + edgeArray[index + 2]*Fprev[(index + 2) & vertexExpMask] + edgeArray[index + 3]*Fprev[(index + 3) & vertexExpMask]);
                //cout << "Fexp: " << Fexp[i] << endl;
                //Fval[i] = (Fprev[index & vertexExpMask] >> (Fexp[i] - Fexp[index & vertexExpMask])) + (Fprev[(index+1) & vertexExpMask] >> (Fexp[i] - Fexp[(index+1) & vertexExpMask])) + (Fprev[(index+2) & vertexExpMask] >> (Fexp[i] - Fexp[(index+2) & vertexExpMask])) + (Fprev[(index+3) & vertexExpMask] >> (Fexp[i] - Fexp[(index+3) & vertexExpMask]));
                //if (Fval[i] > 63) {
                //    Fexp[i] = Fexp[i] + 1;
                //    Fval[i] = Fval[i] >> 1;
                //}
               //if (Fcurr[i] > maxF) maxF = Fcurr[i]; 
               //cout << Fval[i] << endl;
            }
            #pragma omp parallel for num_threads(threads)
            for (unsigned int i = 0; i < (unsigned int)edgeNum; i++) {
                //cout << Fprev[i % vertexExp] << " " << ((float)(Dval[(L-curr)][i / ALPHABET_SIZE] * pow(2, Dexp[(L-curr)][i / ALPHABET_SIZE]))) << endl;
                hittingNumArray[i] += (Fprev[i % vertexExp]/1.4e-45) * (D[(L-curr)][i / ALPHABET_SIZE]/1.4e-45);
                //cout << hittingNumArray[i] << endl;
                if (edgeArray[i] == 0) hittingNumArray[i] = 0;
            }
            #pragma omp parallel for num_threads(threads)
            for (unsigned int i = 0; i < vertexExp; i++) Fprev[i] = Fcurr[i];
            curr++;
            //cout << curr << endl;
        }
        //cout << "MaxD: " << maxD << " maxF: " << maxF << endl;
        return 1;
    }
    int findLog(double base, double x) {
    /**
    Finds the logarithm of a given number with respect to a given base.
    @param base: Base of logartihm, x: Input number.
    @return (int)(log(x) / log(base)): Integer logarithm of the number and the given base.
    */
        return (int)(log(x) / log(base));
    }
    vector<unsigned int> pushBackVector() {
        vector<unsigned int> stageVertices;
        for(unsigned int i = 0; i < (unsigned int)edgeNum; i++) {
            if (stageArray[i] == 1) stageVertices.push_back(i);
        }
        return stageVertices;
    }
};
#endif