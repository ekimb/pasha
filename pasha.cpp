/**
    Pasha: Parallel Algorithms for Approximating Compact Universal Hitting Sets
    pasha.cpp
    Main file for parsing arguments, running decycling set calculations, and
    outputting hitting sets.
    @author Baris Ekim
    @version 1.0 4/15/19
*/
#include <iostream>
#include <sys/resource.h> 
#include <sys/time.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <time.h>
#include <bits/stdc++.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include "decycling.h"
#include "graph.h"
#include "ops.h"
#include "hitting.h"
#define DECYCLING  "decycling"
#define GENERATE  "generate"
#define BENCHMARK "benchmark"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void printDecyclingHelp(string argError) {
/**
Prints help log for decycling set.
@param argError: Error message.
*/
    cout << "Usage: pasha " << DECYCLING << " -k <kmerlength>" << endl;
    cout << "-k <kmerlength [int 5-12]> K-mer length (required)." << endl;
    if (argError != "") {
        cout << "Input Error: "; printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", argError.c_str());
        cout << "Please follow the specifications above." << endl << endl;
    }
    exit(0);
}

void printGenerateHelp(string argError) {
/**
Prints help log for hitting set.
@param argError: Error message.
*/
    cout << "Usage: pasha " << GENERATE << " [-a[<vcount>]][-p][-r] -k <kmerlength> -l <seqlength>" << endl;
    cout << "-a [<vertexcount [int 1-625]>] Calculate all paths (optional), optionally specify number of vertices to remove at a time." << endl;
    cout << "-p Parallelization (optional, required with randomization)." << endl;
    cout << "-r Randomization (optional)." << endl;
    cout << "-k <kmerlength [int 5-12]> K-mer length (required)." << endl;
    cout << "-l <seqlength [int 20-200]> Sequence length (required)." << endl;
    if (argError != "") {
        cout << "Input Error: "; printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", argError.c_str());
        cout << "Please follow the specifications above." << endl << endl;
    }
    exit(0);
}
void printBenchmarkHelp(string argError) {
/**
Prints help log for benchmarks.
@param argError: Error message.
*/
    cout << "Usage: pasha " << BENCHMARK << " -k <kmerlength> -l <seqlength>" << endl;
    cout << "-k <kmerlength [int 5-12]> k-mer length (required)." << endl;
    cout << "-l <seqlength [int 20-200]> sequence length (required)." << endl;
    if (argError != "") {
        cout << "Input Error: "; printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", argError.c_str());
        cout << "Please follow the specifications above." << endl << endl;
    }
    exit(0);
}


int main(int argc, char* argv[]) {
    int k = 0;
    int L = 0;
    int x;
    string decyclingFile;
    string hittingFile;
    bool parallel = false;
    bool randomized = false;
    bool any = false;
    bool seqLen = false;
    bool kmerLen = false;
    const int ALPHABET_SIZE = 4;
    const double PI = 3.14159;
    string directory;
    const string ALPHABET = "ACGT";
    ofstream decyclingStream;
    ofstream hittingStream;
    cout << endl;
    if (argc == 1 || string(argv[1]) == "-h" || string(argv[1]) == "--help" || string(argv[1]) == "help") {
        cout << "Usage: pasha <command> [options]" << endl << endl;
        cout << "Command: " << DECYCLING << "\t Calculate only the decycling set for k-mer length k." << endl;
        cout << "         " << GENERATE << "\t Calculate the hitting sets using the options specified (the decycling set will automatically be generated)." << endl;
        cout << endl << "For help on either command, run 'pasha <command>' without any options." << endl << endl;
        exit(0);
    }
    string argFirst = string(argv[1]);
    if (argFirst == DECYCLING) {
        if (argc <= 2 || string(argv[2]) == "--help" || string(argv[2]) == "help" || string(argv[2]) == "-h") printDecyclingHelp("");
        else {
            for (int i = 2; i < argc;) {
                string argNext = string(argv[i]);
                if (argc != 4) printDecyclingHelp("Incorrect number of arguments or values.");
                else if (argNext == "-k") {
                    char *end;
                    k = strtol(argv[i+1], &end, 10);
                    if (*end != '\0') printDecyclingHelp("Non-integer or unspecified k-mer length.");
                    directory = "pasha_" + string(argv[i+1]);
                    decyclingFile = "pasha_" + string(argv[i+1]) + "/d" + string(argv[i+1]) + ".txt";
                    if (k < 5 || k > 12) printDecyclingHelp("Pasha only supports k-mer lengths between 5 and 12.");
                    i += 2;
                }
                else printDecyclingHelp("Incorrect argument or value " + argNext+ "."); 
            }
        }
    }
    else if (argFirst == GENERATE) {
        if (argc <= 2 || string(argv[2]) == "--help" || string(argv[2]) == "help" || string(argv[2]) == "-h") printGenerateHelp("");
        else {
            for (int i = 2; i < argc;) {
                string argNext = string(argv[i]);
                if (argc < 6 || argc > 10) printGenerateHelp("Incorrect number of arguments or values.");
                else if (argNext == "-a") {
                    any = true;
                    x = 1;
                    if (string(argv[i+1]) != "-p" && string(argv[i+1]) != "-r" && string(argv[i+1]) != "-k" && string(argv[i+1]) != "-l") {
                        char *end;
                        x = strtol(argv[i+1], &end, 10);
                        if (*end != '\0' || x <= 0) printGenerateHelp("Negative, 0, or non-integer number of vertices to remove.");
                        i += 2;
                    }
                    else i += 1;
                }
                else if (argNext == "-p" || argNext == "-r") {
                    if (argNext == "-p") parallel = true;
                    else if (argNext == "-r") {
                        if (any == true) printGenerateHelp("Pasha requires calculating paths of L-k+1 for randomization. Do not use -a when using -r.");
                        if (parallel == false) printGenerateHelp("Pasha requires parallelization for randomization. Use -p before using -r.");
                        randomized = true;
                    }
                    i += 1;
                }
                else if (argNext == "-k") {
                    kmerLen = true;
                    char *end;
                    k = strtol(argv[i+1], &end, 10);
                    if (*end != '\0') printGenerateHelp("Non-integer or unspecified k-mer length.");
                    directory = "pasha_" + string(argv[i+1]);
                    decyclingFile = "pasha_" + string(argv[i+1]) + "/d" + string(argv[i+1]) + ".txt";
                    if (k < 5 || k > 12) printGenerateHelp("Pasha only supports k-mer lengths between 5 and 12.");
                    i += 2;
                } 
                else if (argNext == "-l") {
                    seqLen = true;
                    char *end;
                    L = strtol(argv[i+1], &end, 10);
                    if (*end != '\0') printGenerateHelp("Non-integer or unspecified sequence length.");
                    directory = "pasha_" + string(argv[i+1]);
                    decyclingFile = "pasha_" + string(argv[i+1]) + "/d" + string(argv[i+1]) + ".txt";
                    if (L < 20 || L > 200) printGenerateHelp("Pasha only supports sequence lengths between 20 and 200.");
                    i += 2;
                }
                else printGenerateHelp("Incorrect argument or value " + argNext+ ".");      
            }
            if (kmerLen == false) printGenerateHelp("Please provide a k-mer length.");
            if (seqLen == false) printGenerateHelp("Please provide a sequence length.");
        }
    }
    else if (argFirst == BENCHMARK) {
        if (argc <= 2 || string(argv[2]) == "--help" || string(argv[2]) == "help" || string(argv[2]) == "-h") printBenchmarkHelp("");
        else {
            for (int i = 2; i < argc;) {
                string argNext = string(argv[i]);
                if (argNext == "-k") {
                    directory = "pasha_" + string(argv[i+1]);
                    k = atoi(argv[i+1]);
                    if (k < 5 || k > 12) printBenchmarkHelp("Pasha only supports k-mer lengths between 5 and 12.");
                    i += 2;
                }
                else if (argNext == "-l") {
                    L = atoi(argv[i+1]);
                    hittingFile =  to_string(k) + string(argv[i+1]);
                    if (L < 20 || L > 200) printBenchmarkHelp("Pasha only supports sequence lengths between 20 and 200.");
                    i += 2;
                }
                else printDecyclingHelp("Incorrect argument or value " + argNext+ ".");
            }
        }
    }

    if (mkdir(directory.c_str(), 0777) == -1) cout << strerror(errno) << "." << endl; 
    else cout << "Directory created." << endl; 
    if (argFirst == DECYCLING) cout << "Decycling set will be saved to: " << decyclingFile << endl; 
    graph newGraph = graph(k);
    decycling newDecycling;
    vector<int> decyclingSet = newDecycling.computeDecyclingSet(k);
    decyclingFile = "pasha_" + to_string(k) + "/" + decyclingFile;
    decyclingStream.open(decyclingFile);
    for (int i = 0; i < decyclingSet.size(); i++) {
        string label = newGraph.getLabel(decyclingSet[i]);
        newGraph.removeEdge(decyclingSet[i]);
        decyclingStream << label << "\n";
    }
    int decyclingSize = decyclingSet.size();
    decyclingStream.close();
    int hittingSize = 0;
    if (argFirst == GENERATE) {
        struct timespec start, finish;
        double elapsed;
        clock_gettime(CLOCK_MONOTONIC, &start);  
        if (any == true) {
            if (parallel == true) {
                if (randomized == true) {
                    //hittingSize = HittingRandomParallelAny(newGraph, L, x, hittingFile);
                }
                else {
                    hittingFile = "pasha_" + to_string(k) + "/" + hittingFile + "ap.txt";
                    cout << "Decycling set will be saved to: " << decyclingFile << endl;
                    cout << "Hitting set will be saved to: " << hittingFile << endl;
                    hittingSize = newGraph.HittingParallelAny(L, x, hittingFile);
                }
            }
            else {
                hittingFile = "pasha_" + to_string(k) + "/" + hittingFile + "a.txt";
                cout << "Decycling set will be saved to: " << decyclingFile << endl;
                cout << "Hitting set will be saved to: " << hittingFile << endl;
                hittingSize = newGraph.HittingAny(L, x, hittingFile);
            }
        }
        else {
            if (parallel == true) {
                if (randomized == true) {
                    hittingFile = "pasha_" + to_string(k) + "/" + hittingFile + "r.txt";
                    cout << "Decycling set will be saved to: " << decyclingFile << endl;
                    cout << "Hitting set will be saved to: " << hittingFile << endl;
                    hittingSize = newGraph.HittingRandomParallel(L, hittingFile);
                }
                else { 
                    hittingFile = "pasha_" + to_string(k) + "/" + hittingFile + "p.txt";
                    cout << "Decycling set will be saved to: " << decyclingFile << endl;
                    cout << "Hitting set will be saved to: " << hittingFile << endl;
                    hittingSize = newGraph.HittingParallel(L, hittingFile);
                }
            }
            else {
                hittingFile = "pasha_" + to_string(k) + "/" + hittingFile + ".txt";
                cout << "Decycling set will be saved to: " << decyclingFile << endl;
                cout << "Hitting set will be saved to: " << hittingFile << endl;
                hittingSize = newGraph.Hitting(L, hittingFile);
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        cout << elapsed << " seconds." << endl;
    }
    if (argFirst == GENERATE) cout << "Hitting set size: " << hittingSize << endl;
    if (argFirst == DECYCLING) cout << "Decycling set size: " << decyclingSize << endl;
    return hittingSize + decyclingSize;
}