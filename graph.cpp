#include "graph.h"
using namespace std;
class graph {
	public:
	const int ALPHABET_SIZE = 4;
	const string ALPHABET = "ACGT";
	static int edgeArray[];
	static map<char, int> alphabetMap;
    double [][] F;
    double [] allHit;
    int k;
    int l = L-k+1;
    int edgeNum = Math.pow(ALPHABET_SIZE, k);
    int edgeArray[edgeNum];
	int edgeCount = edgeNum;
    int vertexCount = edgeNum / ALPHABET_SIZE; 
    double D[l+1][vertexCount];
    double F[l+1][vertexCount];
    double epsilon;
    double delta;
    int vertexExp = Math.pow(ALPHABET_SIZE, k-1);
    int vertexExp2 = vertexExp * 2;
    int vertexExp3 = vertexExp * 3;
    int vertexExpMask = vertexExp - 1;
    int vertexExp_1 = Math.pow(ALPHABET_SIZE, k-2);
    int vertexLog = (int)(Math.log(ALPHABET_SIZE)/Math.log(2)); 





	void generateGraph(int k) {
			fill_n(edgeArray, edgeNum, 1);
    }
    static char getChar(int i) {
    	return ALPHABET[i];
    }
    string getEdgeLabel(int i) {
    		return getString(i, k, ALPHABET_SIZE);
    }
    static string getString(int a, int k, int ALPHABET_SIZE) {
		string finalString = "";
		for (int i = 0; i < k; i++) {
			finalString = getChar(a % ALPHABET_SIZE) + finalString;
			a = a / ALPHABET_SIZE;
		}
		return finalString;
	}
	graph(int _k, const int ALPHABET_SIZE, string ALPHABET) {
		k = _k;
		generateGraph(k);
		map<char, int> alphabetMap;
		for (int i = 0; i < ALPHABET_SIZE; i++)
			mapAlphabet.insert(pair<char,int>(ALPHABET[i], i));
    	}







};
