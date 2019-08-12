  #ifndef DENSITY_H
  #define DENSITY_H
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
  class density {
  public:
    int UHScount;
    uint64_t minPos;
    uint64_t sparseCount;
    vector<uint64_t> minPosVector;
    uint64_t count;
    int k;
    int L;
    uint64_t len;
    int w;
    char* seq;
    uint64_t edgeNum;
    byte* UHS;

    density(int argK, int argL, int argLen, const char* seqFile, const char* UHSfile, const char* decycfile, bool random) {
      k = argK;
      L = argL;
      len = argLen;
      int i = 0;
      if (!random) {
        fstream seqs(seqFile, fstream::in);
        vector<char> seqv;
        char base;
        seqs.get(base);
        while (seqs >> noskipws >> base) {
          if(base == 'A' || base == 'C' || base == 'G' || base == 'T') {
            seqv.push_back(base);
          }
          i++;
          seqs.get(base);
        }      
        cout << seqv.size() << endl;
        len = seqv.size();
        seq = new char[len];
        for (int i = 0; i < len; i++) seq[i] = seqv[i];
      }
      else seq = genRandomSeq(len);
      w = L - k + 1;
      edgeNum = static_cast<uint64_t>(pow(4, k));
      UHS = new byte[edgeNum];
      for (int i = 0; i < edgeNum; i++) UHS[i] = 0;
  //PDOCKS pdocks = PDOCKS(k);
      string UHSkmer;
      string decyckmer;
      ifstream fin(UHSfile);
      ifstream fin2(decycfile);
      while (getline(fin, UHSkmer)) {
        int idx = getIdx(UHSkmer);
        UHS[idx] = 1;
      }
      while(getline(fin2, decyckmer)) {
        int idx = getIdx(decyckmer);
        UHS[idx] = 1;
      }
    }

    char* genRandomSeq(int len) {
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
    char* genWindow (char* seq, const int k, const int w, int count, int len) {
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
    void findMin (char* window, const int k, const int L, int count, int len, byte* UHS) {
      unordered_set<string>:: iterator check;
      string min = "TTTTTTTTTTTTTTTTTTT";
      minPos = 0;
      UHScount = 0;
      bool UHSfound = false;
      for (int pos = 0; pos < L - k + 1; pos++) {
        string testKmer = findTestKmer(window, k, pos);
        //cout << "Testing " << testKmer << endl;
        int idx = getIdx(testKmer);
        if (UHS[idx] == 1) {
          UHSfound = true;
          UHScount++;
          if (getIdx(min) > getIdx(testKmer)) {
            min = testKmer;
            minPos = pos;
          }
        }

      }
      minPosVector.push_back(minPos + count);
      //cout << "Min: " << min << " in position: " << minPos+count << endl;
  //if (UHScount == 1) cout << "UHS count: " << UHScount << endl;

    }

    void findMinSeq(char* seq, const int k, const int L, int len, byte* UHS) {
      sparseCount = 0;
      count = len - L + 1;
      for (int i = 0; i < count; i++) {
        if (i % 1000000 == 0) cout << i/1000000 << endl;
        //if (i == 10000000) cout << i << endl;
        //if (i == 100000000) cout << i << endl;
        //if (i == 1000000000) cout << i << endl;
        //cout << "Window " << i+1 << endl;
        char* testWindow = genWindow(seq, k, L, i, len);
        findMin(testWindow, k, L, i, len, UHS);
        if (UHScount == 1) sparseCount += UHScount;
      }
    }
};
#endif