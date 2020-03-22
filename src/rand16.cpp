    #include <sys/resource.h> 
    #include <sys/time.h> 
    #include <unistd.h> 
    #include <stdio.h> 
    #include <time.h>
    //#include<bits/stdc++.h>
    #include <sys/stat.h> 
    #include <sys/types.h> 
    #include "decycling.h"
    #include "pasha.h"

    int main(int argc, char* argv[])
    {
        struct timespec start, finish;
        double elapsed;
        size_t pos;
        unsigned_int k = stoi(argv[1], &pos);
        unsigned_int L = stoi(argv[2], &pos);
        unsigned_int threads = stoi(argv[3], &pos);
        const char *decyclingPath = argv[4];
        const char *hittingPath = argv[5];
        string decyclingFile;
        string hittingFile;
        const double PI = 3.14159;
        string directory;
        //ofstream decyclingStream(decyclingPath);
        PASHA pasha = PASHA(k);
        cout << "Graph OK" << endl;
        //decycling newDecycling;
        //vector<unsigned int> decyclingSet = newDecycling.computeDecyclingSet(k);
        //cout << "Decycling max size: " << decyclingSet.max_size() << "Decycling size: " << decyclingSet.size() << endl;
        //for (unsigned_int i = 0; i < decyclingSet.size(); i++) {
            //cout << "Removing decycling edge " << decyclingSet[i] << endl;
        //    string label = pasha.getLabel(decyclingSet[i]);
            //cout << label << " Index: " << decyclingSet[i] << endl;
        //    pasha.removeEdge(decyclingSet[i]);
        //    //cout << label << endl;
        //    decyclingStream << label << "\n";
        //}
        //unsigned_int decyclingSize = decyclingSet.size();
        //cout << "Decycling set size: " << decyclingSize << endl;
        //decyclingStream.close();
        ifstream decycFile;
        string label;
        string line;
        ifstream myfile ("decyc8.txt");
        if (myfile.is_open()){
            while(getline(myfile, label)){ //read data from file object and put it into string.
                //cout << "Removing " + label << "\n"; //print the data of the string
                pasha.removeEdge(pasha.getIndex(label));
            }
        }
        myfile.close(); //close the file object.
        clock_gettime(CLOCK_MONOTONIC, &start);
        unsigned_int hittingSize = pasha.HittingRandomParallel(L, hittingPath, threads);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        cout << elapsed << " seconds." << endl; 
        cout << "Hitting set size: " << hittingSize << endl << endl;
    }