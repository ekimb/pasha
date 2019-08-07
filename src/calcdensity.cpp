  #include <iostream>
  #include <sys/resource.h> 
  #include <sys/time.h> 
  #include <unistd.h> 
  #include <stdio.h> 
  #include <time.h>
  #include<bits/stdc++.h>
  #include <sys/stat.h> 
  #include <sys/types.h> 
  #include "pdocks.h"
  #include <unordered_set>
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
  using byte = uint8_t;

char* genRandomSeq(const int len) {
    srand(time(0));
    char* seq = new char[len];
    static const char alphabet[] =
        "A"
        "C"
        "G"
        "T";

    for (int i = 0; i < len; ++i) {
        seq[i] = alphabet[rand() % (sizeof(alphabet) - 1)];
        //cout << seq[i] << endl;

    }
    return seq;
}

int getIdx(string kmer) {
  int idx = 0;
  for (int i = 0; i < kmer.length(); i++) {
    int base;
    if (kmer[i] == 'A') base = 0;
    if (kmer[i] == 'C') base = 1;
    if (kmer[i] == 'G') base = 2;
    if (kmer[i] == 'T') base = 3;
    idx += pow(4, kmer.length()-i-1) * base;
  }
  return idx;
}

string findTestKmer(char* window, const int k, int pos) {
  string kmer = "";
  for(int i = 0; i < k; i++) {
    kmer = kmer + window[pos+i];
    //cout << kmer << i << endl;
  }

  return kmer;
}
char* genWindow (char* seq, const int k, const int w, int count, const int len) {
  int foo = 0;
  char* window;
  window = new char[w];
  for (int i = 0; i < w; i++) {
    window[i] = seq[i + count];
    //cout << window[i];
    foo++;
  }
  //cout << endl;
  //cout << foo << endl;
  return window;
}
vector<int> findMin (char* window, const int k, const int L, int count, const int len, byte* UHS) {
  unordered_set<string>:: iterator check;
  string min = "TTTTTTTTTTTTTTTTTTT";
  int minPos = 0;
  int UHScount = 0;
  for (int pos = 0; pos < L - k + 1; pos++) {
    string testKmer = findTestKmer(window, k, pos);
    //cout << "Testing " << testKmer << endl;
    int idx = getIdx(testKmer);
    if (UHS[idx] == 1) {
      //cout << "kmer in UHS" << endl;
      UHScount++;
      if (getIdx(min) > getIdx(testKmer)) {
        min = testKmer;
        minPos = pos;
      }
    }

  }
  cout << "Min: " << min << " in position: " << minPos+count << endl;
  //if (UHScount == 1) cout << "UHS count: " << UHScount << endl;
  vector<int> res;
  res.push_back(minPos+count);
  res.push_back(UHScount);
  return res;

}
int findUmer (char* window, const int k, const int L, int count, const int len, byte* UHS) {
  unordered_set<string>:: iterator check;
  int umerCount = 0;
  for (int pos = 0; pos < L - k + 1; pos++) {
    string testKmer = findTestKmer(window, k, pos);
    int idx = getIdx(testKmer);
    //cout << "Testing " << testKmer << endl;
    if (UHS[idx] == 1) {
      umerCount++;
    }
  }
  cout << "Universal k-mer count: " << umerCount << endl;
  return umerCount;

}
double calcSparse(vector<int> umerVector, int count) {
  int sparseCount = 0;
  for (int i = 0; i < umerVector.size(); i++) {
    if (umerVector[i] == 1) sparseCount++;
  }
  double sparsity = (double)sparseCount / (double)count;
  return sparsity;
}

 vector< vector<int> > findMinSeq(char* seq, const int k, const int L, const int len, byte* UHS) {

  vector<int> minPosVector;
  vector<int> umerVector;
  vector<int> res;
  vector< vector<int> > res2;
  int count = len - L + 1;
  for (int i = 0; i < count; i++) {
    //cout << "Window " << i+1 << endl;
    char* testWindow = genWindow(seq, k, L, i, len);
    res = findMin(testWindow, k, L, i, len, UHS);
    minPosVector.push_back(res[0]);
    umerVector.push_back(res[1]);
  }
  res2.push_back(minPosVector);
  res2.push_back(umerVector);

  return res2;
}

int main (int argc, char* argv[]) {
  const int len = atoi(argv[1]);
  const int k = atoi(argv[2]);
  const int L = atoi(argv[3]);
  const int w = L - k + 1;
  //const char* seqFile = argv[1];
  //unsigned int size = (unsigned int)pow(4, k+w) + k+w - 1;
  //cout << size << endl;
  //char* seq = new char[size];
  //ifstream dbg(seqFile);
  //int i = 0;
  //cout << i << endl;
  //if (dbg.is_open()) {
  //  while (!dbg.eof()) {
  //    dbg >> seq[i];
      //cout << seq[i] << endl;
  //    i++;
  //  } 
  //}     
  //while (getline(dbg, dbgstr)) strcpy(seq, dbgstr.c_str());
  //cout << seq[0] << endl;
  char* seq = genRandomSeq(len);
  cout << seq << endl;
  const char* UHSfile = argv[4];
  const char* decycfile = argv[5];
  unsigned int edgeNum = static_cast<unsigned int>(pow(4, k));
  byte* UHS = new byte[static_cast<unsigned int>(edgeNum)];
  for (int i = 0; i < edgeNum; i++) UHS[i] = 0;
  //PDOCKS pdocks = PDOCKS(k);
  string UHSkmer;
  string decyckmer;
  ifstream fin(UHSfile);
  ifstream fin2(decycfile);
  unordered_set<string>:: iterator check;
  while (getline(fin, UHSkmer)) {
    int idx = getIdx(UHSkmer);
    UHS[idx] = 1;
  }
  while(getline(fin2, decyckmer)) {
    int idx = getIdx(decyckmer);
    UHS[idx] = 1;
  }
  cout << "UHS complete" << endl;
  vector< vector <int> > resVector = findMinSeq(seq, k, L, len, UHS);
  vector<int> minPosVector = resVector[0];
  vector<int> umerVector = resVector[1];
  double sparsity = calcSparse(umerVector, len - L - 1);
  //cout << minPosVector.size() << endl;
  sort(minPosVector.begin(), minPosVector.end());
  int uniqueCount = set<int>(minPosVector.begin(), minPosVector.end()).size();
  double density = (double)uniqueCount / (len - w - k);
  cout << "Density: " << density << " Sparsity: " << sparsity << endl;

}