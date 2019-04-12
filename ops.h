#include "decycling.h"
#include "graph.h"
int graph::calculateHittingNumber(int L) {
	cout << "calchit" << endl;
	int maxHittingNum = 0;
	int imaxHittingNum = -1;
	vector<int> hittingNumVector (edgeVector.size(), 0);
    for (int i = 0; i < edgeVector.size(); i++) {
        int hittingNum = 0;
        for (int j = (1 - edgeVector[i]) * L; j < L; j++) {
        	hittingNum = hittingNum + F[j][i % vertexExp] * D[(L-j-1)][i / ALPHABET_SIZE];
			hittingNumVector[i] = hittingNum;
        }
        if (hittingNum > maxHittingNum) {maxHittingNum = hittingNum; imaxHittingNum = i;}
    }
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
		cout << "l is" << L << j << endl;
		for (int i = 0; i < vertexExp; i++) {
			cout << "calcpaths" << vertexExp << edgeVector.size() << endl;
			int index = (i * 4);
            F[j][i] = edgeVector[index] * F[j-1][index & vertexExpMask] + edgeVector[index + 1] * F[j-1][index + 1 & vertexExpMask] + edgeVector[index + 2] * F[j-1][index + 2 & vertexExpMask] + edgeVector[index + 3] * F[j-1][index + 3 & vertexExpMask];
            D[j][i] = edgeVector[i] * D[j-1][i >> 2] + edgeVector[i + vertexExp] * D[j-1][(i + vertexExp >> 2)] + edgeVector[i + vertexExp2] * D[j-1][(i + vertexExp2 >> 2)] + edgeVector[i + vertexExp3] * D[j-1][(i + vertexExp3 >> 2)];
        }
	}
cout << "calcpaths" << endl;
cout << "calcpaths2 before return" << endl;
return 0;
}
int graph::depthFirstSearch(vector<bool> used, vector<bool> finished, vector<int> res, int index, int u) {
	cout << u << endl;
	used[u] = true;
	bool cycle = false;
	cout << "here dfs" << endl;
	for (int v : getAdjacent(u)) {
		// Return true for cycle
		if (used[v] && !finished[v]) cycle = true;
		if (!used[v]) {
			index = depthFirstSearch(used, finished, res, index, v);
			cycle = cycle || (index == -1);
		}
	}
	finished[u] = true;
	res[index] = u;
	if (cycle){	cout << "dfs done cycle " << u << endl; return -1;}
	else {	cout << "dfs done normal" << endl; return index + 1;}
}
vector<int> graph::getAdjacent(int v) {
	int count = 0;
	vector<int> adjVertex(ALPHABET_SIZE);
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		int index = v + i * vertexExp;
		if (edgeVector[index] == 1)
			adjVertex[count++] = index / ALPHABET_SIZE;
	}
	vector<int> rc(count);
	for (int i = 0; i < count; i++) {
		rc[i] = adjVertex[i];
	}
	return rc;
}
int graph::Hitting(int L, string hittingFile) {
	vertexExp = pow(ALPHABET_SIZE, k-1);
    int imaxHittingNum = -1;
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    cout << l << endl;
    cout << vertexExp << endl;
    D.resize(l + 1, vector<int>(vertexExp));
    cout << "here" << endl;
    F.resize(l + 1, vector<int>(vertexExp));
    cout << "here" << endl;
    calculatePaths(l);
    cout << "here after path" << endl;
    topoSort = topologicalSort();
    cout << "here after topo" << endl;
    imaxHittingNum = calculateHittingNumber(l);
    while (calculatePaths(l) && (imaxHittingNum = calculateHittingNumber(l) >= 0)) {
    	cout << "in while" << endl;
        if (imaxHittingNum > -1) {
                removeEdge(imaxHittingNum);
	            string label = getLabel(imaxHittingNum);
	            hittingStream.open(hittingFile); 
	            cout << "Writing " << label << endl;
	            hittingStream << label << "\n";
	            hittingCount++;
	            hittingStream.close();

        }
    }
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
	int n = vertexCount;
	cout << vertexCount << endl;
	vector<int> nullVector(n, 0);
	vector<bool> used(n);
	vector<bool> finished(n);
	vector<int> res(n);
	int index = 0;
	for (int i = 0; i < n; i++) {
		cout << "topo running" << endl;
		if (!used[i]) {
			cout << i << endl;
			index = depthFirstSearch(used, finished, res, index, i);
			cout << "after dfs" << endl;
			if (index == -1) {cout << "null return" << endl; return nullVector;}
		}
	}
	vector<int> rc(res.size());
	for (int i = 0; i < rc.size(); i++)
		rc[i] = res[res.size()-i-1];
	return res;
}










