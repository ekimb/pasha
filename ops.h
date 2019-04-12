#include "decycling.h"
#include "graph.h"
#include <cstdlib>
#include <iomanip>
#include <omp.h>
int graph::calculateForEach(int i, int L){
	int hittingNum = 0;
    for (int j = (1 - edgeVector[i]) * L; j < L; j++) {
    	hittingNum = hittingNum + F[j][i % vertexExp] * D[(L-j-1)][i / ALPHABET_SIZE];
		//cout << hittingNum << endl;
    }
    hittingNumVector[i] = hittingNum;
}
int graph::calculateHittingNumber(int L) {
	//cout << "calchit" << endl;
	int maxHittingNum = 0;
	int imaxHittingNum = -1;
	hittingNumVector.resize(edgeVector.size(), 0);
    for (int i = 0; i < edgeVector.size(); i++) {
        int hittingNum = 0;
        for (int j = (1 - edgeVector[i]) * L; j < L; j++) {
        	hittingNum = hittingNum + F[j][i % vertexExp] * D[(L-j-1)][i / ALPHABET_SIZE];
			hittingNumVector[i] = hittingNum;
			//cout << hittingNum << endl;
        }
        if (hittingNum > maxHittingNum) {maxHittingNum = hittingNum; imaxHittingNum = i;}
    }
    //cout << imaxHittingNum << endl;
    return imaxHittingNum;
}
int graph::calculateHittingNumberParallel(int L) {
	omp_set_dynamic(0);
	//cout << "calchit" << endl;
	int maxHittingNum = 0;
	int imaxHittingNum = -1;
	//cout << vertexExp << ALPHABET_SIZE << F.size() << D.size() << edgeVector.size() << endl;
	hittingNumVector.resize(edgeVector.size(), 0);
  	#pragma omp parallel for num_threads(16)
    for (int i = 0; i < edgeVector.size(); i++) {
    	calculateForEach(i, L);
    }
    for (int i = 0; i < edgeVector.size(); i++) {
    	if (hittingNumVector[i]*edgeVector[i] > maxHittingNum) {maxHittingNum = hittingNumVector[i]; imaxHittingNum = i;};
    }
    //cout << imaxHittingNum << endl;
    return imaxHittingNum;
}
int graph::calculatePaths(int L) {
	vertexExp2 = vertexExp * 2;
    vertexExp3 = vertexExp * 3;
    vertexExpMask = vertexExp - 1;
    vertexExp_1 = pow(ALPHABET_SIZE, k-2);
   	//cout << "calcpaths" << vertexExp << ALPHABET_SIZE << endl;
	for (int i = 0; i < vertexExp; i++) {D[0][i] = 1; F[0][i] = 1;}
	for (int j = 1; j <= L; j++) {
		for (int i = 0; i < vertexExp; i++) {
			int index = (i * 4);
            F[j][i] = edgeVector[index]*F[j-1][index & vertexExpMask] + edgeVector[index + 1]*F[j-1][(index + 1) & vertexExpMask] + edgeVector[index + 2]*F[j-1][(index + 2) & vertexExpMask] + edgeVector[index + 3]*F[j-1][(index + 3) & vertexExpMask];
            D[j][i] = edgeVector[i]*D[j-1][(i >> 2)] + edgeVector[i + vertexExp]*D[j-1][((i + vertexExp) >> 2)] + edgeVector[i + vertexExp2]*D[j-1][((i + vertexExp2) >> 2)] + edgeVector[i + vertexExp3]*D[j-1][((i + vertexExp3) >> 2)];
        }
	}
cout << "Calculated paths." << endl;
return 0;
}
int graph::depthFirstSearch(int index, int u) {
	//cout << u << endl;
	used[u] = 1;
	bool cycle = false;
	//cout << "here dfs" << endl;
	for (int v : getAdjacent(u)) {
		// Return true for cycle
		if (used[v] == 1 && finished[v] == 0) {cycle = true;}
		if (used[v] == 0) {
			index = depthFirstSearch(index, v);
			cycle = cycle || (index == -1);
		}
	}
	//cout << "dfs done" << endl;
	finished[u] = 1;
	//cout << "finished assign" << endl;
	res[index] = u;
	//cout << "finished res" << endl;
	if (cycle){	/*cout << "dfs done cycle " << u << endl;*/ return -1;}
	else { return index + 1;}
}
vector<int> graph::getAdjacent(int v) {
	int count = 0;
	int adjVertex[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		int index = v + i * vertexExp;
		if (edgeVector[index] == 1) adjVertex[count++] = index / ALPHABET_SIZE;
	}
	vector<int> rc(count);
	for (int i = 0; i < count; i++) {
		rc[i] = adjVertex[i];
	}
	return rc;
}
int graph::Hitting(int L, string hittingFile) {
	vertexExp = pow(ALPHABET_SIZE, k-1);
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    //cout << l << endl;
    //cout << vertexExp << endl;
    D.resize(l + 1, vector<int>(vertexExp));
    cout << "D initialized." << endl;
    F.resize(l + 1, vector<int>(vertexExp));
    cout << "F initialized." << endl;
    hittingStream.open(hittingFile); 
    while (calculatePaths(l)){
    	int imaxHittingNum = calculateHittingNumber(l);
    	if (imaxHittingNum < 0) break;
    	//cout << imaxHittingNum << endl;
        removeEdge(imaxHittingNum);
        string label = getLabel(imaxHittingNum);
        //cout << "Writing " << label << endl;
        hittingStream << label << "\n";
        hittingCount++;
   	}
   	hittingStream.close();
    topoSort = topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << endl;
    return hittingCount;
}
int graph::HittingParallel(int L, string hittingFile) {
	vertexExp = pow(ALPHABET_SIZE, k-1);
    int imaxHittingNum = -1;
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    //cout << l << endl;
    //cout << vertexExp << endl;
    D.resize(l + 1, vector<int>(vertexExp));
    cout << "D initialized." << endl;
    F.resize(l + 1, vector<int>(vertexExp));
    cout << "F initialized." << endl;
    hittingStream.open(hittingFile); 
    while (calculatePaths(l)){
    	int imaxHittingNum = calculateHittingNumberParallel(l);
    	if (imaxHittingNum < 0) break;
    	//cout << imaxHittingNum << endl;
        removeEdge(imaxHittingNum);
        string label = getLabel(imaxHittingNum);
        //cout << "Writing " << label << endl;
        hittingStream << label << "\n";
        hittingCount++;
   	}
   	hittingStream.close();
    topoSort = topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << endl;
    return hittingCount;
}

int graph::maxLength() {
	vector<int> depth(topoSort.size());
	int maxDepth = -1;
	for (int i = 0; i < topoSort.size(); i++) {
		int maxVertDepth = -1;
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			int edgeIndex = topoSort[i] + j * vertexExp;
			int vertexIndex = edgeIndex / ALPHABET_SIZE; // >> log;
			if (depth[vertexIndex] > maxVertDepth && edgeVector[edgeIndex] == 1) maxVertDepth = depth[vertexIndex];
		}
		depth[topoSort[i]] = maxVertDepth + 1;
		if (depth[topoSort[i]] > maxDepth) {maxDepth = depth[topoSort[i]];}
	}
	depth.shrink_to_fit();
	return maxDepth;
}
void graph::removeEdge(int i) {
	if (edgeVector[i] == 1) {
			edgeCount--;
			//if (S != null){
			//	S[i] = -1;
			//}
			//if (P != null){
			//	P[i] = -1;
			//}
			//if (pick != null){
			//	pick[i] = true;
			//}
			//System.out.println("Removing edge " + getEdgeLabel(i) + ", " + numEdges + " edges remaining");
	}
	edgeVector[i] = 0;
}
vector<int> graph::topologicalSort() {
	int n = vertexExp;
	vector<int> nullVector(n, 0);
	used.resize(n, 0);
	finished.resize(n, 0);
	res.resize(n);
	int index = 0;
	for (int i = 0; i < n; i++) {
		if (used[i] == 0) {
			index = depthFirstSearch(index, i);
			if (index == -1) {cout << "null return" << endl; return nullVector;}
		}
	}
	vector<int> rc(res.size());
	for (int i = 0; i < rc.size(); i++)
		rc[i] = res[res.size()-i-1];
	rc.shrink_to_fit();
	used.shrink_to_fit();
	finished.shrink_to_fit();
	return res;
}










