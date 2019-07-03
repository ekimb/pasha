	#include <iostream>
    #include <sys/resource.h> 
    #include <sys/time.h> 
    #include <unistd.h> 
    #include <stdio.h> 
    #include <time.h>
    #include<bits/stdc++.h>
    #include <sys/stat.h> 
    #include <sys/types.h> 
    #include "decycling.h"
    #include "pdocks.h"
    int main(int argc, char* argv[])
    {
        struct timespec start, finish;
        double elapsed;
        size_t pos;
        int k = stoi(argv[1], &pos);
        int L = stoi(argv[2], &pos);
        int threads = stoi(argv[3], &pos);
        const char *decyclingPath = argv[4];
        const char *hittingPath = argv[5];
        const double PI = 3.14159;
        string directory;
        ofstream decyclingStream(decyclingPath);
        PDOCKS pdocks = PDOCKS(k);
        decycling newDecycling;
        vector<int> decyclingSet = newDecycling.computeDecyclingSet(k);
        for (int i = 0; i < decyclingSet.size(); i++) {
            string label = pdocks.getLabel(decyclingSet[i]);
            pdocks.removeEdge(decyclingSet[i]);
            decyclingStream << label << "\n";
        }
        int decyclingSize = decyclingSet.size();
        cout << "Decycling set size: " << decyclingSize << endl;
        decyclingStream.close();
        clock_gettime(CLOCK_MONOTONIC, &start);
        int hittingSize = pdocks.HittingParallel(L, hittingPath, threads);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        cout << elapsed << " seconds." << endl; 
        cout << "Hitting set size: " << hittingSize << endl << endl;
    }