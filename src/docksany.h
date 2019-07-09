#ifndef DOCKSANY_H
#define DOCKSANY_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <iomanip>
#include <cstdint>
#include <omp.h>
using namespace std;
using byte = uint8_t;
class DOCKSany {
    public:
        bool* finished;
        bool* used;
        double* hittingNumArray;
        double* maxHittingNum;
        int* imaxHittingNum;
        float** D;
        float** F;
        int ALPHABET_SIZE;
        double edgeCount;
        double edgeNum;
        int k;
        int curr;
        int l;
        int total;
        int vertexCount; 
        int vertexExp;
        int vertexExp2;
        int vertexExp3;
        int vertexExpMask;
        int vertexExp_1;
        byte* edgeArray;
        byte* topoSort;
        static map<char, int> alphabetMap;
        string ALPHABET;
    DOCKSany (int argK) {
    /**
    Definition of a graph object. Generates a graph of order k, creates an empty
    edge index array, calculates number of edges, builds a character-index map.
    @param argK: Argument passed as k-mer length.
    */
        ALPHABET = "ACGT";
        ALPHABET_SIZE = 4;
        k = argK;
        edgeNum = pow(ALPHABET_SIZE, k);
        edgeArray = new byte[(int)edgeNum];
        generateGraph(k);
        map<char, int> alphabetMap;
        for (int i = 0; i < ALPHABET_SIZE; i++) alphabetMap.insert(pair<char,int>(ALPHABET[i], i));
    }

    void generateGraph(int k) {  
    /**
    Generates a complete de Bruijn graph of order k.
    @param k: Desired k-mer length (order of complete graph).
    */
        for (int i = 0; i < edgeNum; i++) edgeArray[i] = 1;
        edgeCount = edgeNum;
        vertexCount = edgeNum / ALPHABET_SIZE; 
    }

    char getChar(int i) {
    /**
    Gets alphabet character from index.
    @param i: Index of character.
    @return The character in the alphabet.
    */
        return ALPHABET[i];
    }

    string getLabel(int i) {
    /**
    Gets label of the input edge index.
    @param i: Index of edge.
    @return The label of the edge.
    */
        string finalString = "";
        for (int j = 0; j < k; j++) {
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
        for (int i = 0; i < vertexExp; i++) {
            int maxVertDepth = -1;
            for (int j = 0; j < ALPHABET_SIZE; j++) {
                int edgeIndex = topoSort[i] + j * vertexExp;
                int vertexIndex = edgeIndex / ALPHABET_SIZE;
                if ((depth[vertexIndex] > maxVertDepth) && (edgeArray[edgeIndex] == 1)) maxVertDepth = depth[vertexIndex];
            }
            depth[topoSort[i]] = maxVertDepth + 1;
            if (depth[topoSort[i]] > maxDepth) {maxDepth = depth[topoSort[i]];}
        }
        return maxDepth;
    }

    void removeEdge(int i) {
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
        for (int i = 0; i < vertexExp; i++) {used[i] = false; finished[i] = false;}
        int index = 0;
        for (int i = 0; i < vertexExp; i++) {
            if (used[i] == false) {
                index = depthFirstSearch(index, i);
                if (index == -1) {topoSort = NULL; return;}
            }
        }
       // int rc[vertexExp];
       // for (int i = 0; i < vertexExp; i++) rc[i] = topoSort[vertexExp-i-1];
    }
    int depthFirstSearch(int index, int u) {
    /**
    Depth-first search of a given index of an edge.
    @param index: Depth of recursion, u: Index of edge.
    @return -1: The search cycles, index+1: Current depth.
    */
        used[u] = true;
        bool cycle = false;
        for (int v : getAdjacent(u)) {
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
    vector<int> getAdjacent(int v) {
    /**
    Get adjacent vertices to a given index of a vertex.
    @param v: Index of vertex.
    @return rc: Array of adjacent vertices.
    */
        int count = 0;
        int adjVertex[ALPHABET_SIZE];
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            int index = v + i * vertexExp;
            if (edgeArray[index] == 1) adjVertex[count++] = index / ALPHABET_SIZE;
        }
        vector<int> rc(count);
        for (int i = 0; i < count; i++) {
            rc[i] = adjVertex[i];
        }
        return rc;
    }

    int HittingAny(int L, int x, const char *hittingPath) {
    /**
    Performs hitting set calculations without randomization, counting all paths.
    @param L: Sequence length, hittingFile: Output file destination.
    @return hittingCount: Size of hitting set.
    */
        vertexExp = pow(ALPHABET_SIZE, k-1);
        ofstream hittingStream;
        int hittingCount = 0;
        l = L-k+1;
        hittingNumArray = new double[(int)edgeNum];
        used = new bool[vertexExp];
        finished = new bool[vertexExp];
        topoSort = new byte[vertexExp];
        hittingStream.open(hittingPath); 
        topologicalSort();
        D = new float*[1];
        float* Dpool = new float[(1)* vertexExp];
        for(int i = 0; i < 1; i++, Dpool += vertexExp) D[i] = Dpool;
        F = new float*[1];
        float* Fpool = new float[(1)* vertexExp];
        for(int i = 0; i < 1; i++, Fpool += vertexExp) F[i] = Fpool;
        while (maxLength() >= l) { 
            calculatePathsAny();
            imaxHittingNum = calculateHittingNumberAny(x);
            for (int i = 0; i < x; i++) {
                removeEdge(imaxHittingNum[i]);
                string label = getLabel(imaxHittingNum[i]);
                hittingStream << label << "\n";
                hittingCount++;
            }
        }
        hittingStream.close();
        topologicalSort();
        cout << "Length of longest remaining path: " <<  maxLength() << "\n";
        return hittingCount;
    }

    int* calculateHittingNumberAny(int x) {
/**
Calculates hitting number of all edges, counting paths of all length.
@param L: Sequence length.
@return imaxHittingNum: Index of vertex with maximum hitting number.
*/  
        maxHittingNum = new double[x];
        imaxHittingNum = new int[x];
        for (int i = 0; i < edgeNum; i++) {
            double hittingNum = 0;
            hittingNum = edgeArray[i] * F[0][i % vertexExp] * D[0][i / ALPHABET_SIZE];
            hittingNumArray[i] = hittingNum;
            int j = 0;
            while (j < x && hittingNum <= maxHittingNum[j]) j++;
            if (j < x && hittingNum > maxHittingNum[j]) {
                maxHittingNum[j] = hittingNum; imaxHittingNum[j] = i;
            }
        }
        return imaxHittingNum;
    }


    int calculatePathsAny() {
    /**
    Calculates number of all paths for all vertices.
    @param L: Sequence length.
    @return 1: True if path calculation completes.
    */
        vertexExp2 = vertexExp * 2;
        vertexExp3 = vertexExp * 3;
        vertexExpMask = vertexExp - 1;
        vertexExp_1 = pow(ALPHABET_SIZE, k-2);
        for (int i = 0; i < vertexExp; i++) {F[0][i] = 1; D[0][i] = 1;}
        for (int i = 0; i < vertexExp; i++) {
            D[0][topoSort[i]] += edgeArray[topoSort[i]]*D[0][(topoSort[i] >> 2)] + edgeArray[topoSort[i] + vertexExp]*D[0][((topoSort[i] + vertexExp) >> 2)] + edgeArray[topoSort[i] + vertexExp2]*D[0][((topoSort[i] + vertexExp2) >> 2)] + edgeArray[topoSort[i] + vertexExp3]*D[0][((topoSort[i] + vertexExp3) >> 2)];
            int index = (topoSort[vertexExp-i-1] * 4);
            F[0][topoSort[vertexExp-i-1]] += edgeArray[index]*F[0][index & vertexExpMask] + edgeArray[index+1]*F[0][(index+1) & vertexExpMask] + edgeArray[index+2]*F[0][(index+2) & vertexExpMask] + edgeArray[index+3]*F[0][(index+3) & vertexExpMask];
        }
        return 1;
    }
};
#endif