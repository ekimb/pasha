#include <iostream>
#include "decycling.h"
#include "graph.h"
//Defining main commands
#define DECYCLING  "decycling"
#define GENERATE  "generate"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void printDecyclingHelp(string argError)
{
  cout << "Usage: pasha " << DECYCLING << " -k <kmerlength> <outputfile>" << endl;
  cout << "-k <kmerlength [int 5-12]> k-mer length (required)." << endl;
  if (argError != "") {
    cout << "Input Error: "; printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", argError.c_str());
    cout << "Please follow the specifications above." << endl << endl;
  }
  exit(0);
}

int main(int argc, char* argv[]) {
  int k;
  string decyclingFile;
  const int ALPHABET_SIZE = 4;
  const double PI = 3.1415926;
  const string ALPHABET = "ACGT";
  ofstream outputStream;
  cout << endl;
  if (argc == 1 || string(argv[1]) == "-h" || string(argv[1]) == "--help" || string(argv[1]) == "help"){
    cout << "Usage: pasha <command> [options]" << endl << endl;
    cout << "Command: " << DECYCLING << "\t Calculate only the decycling set for k-mer length k." << endl;
    cout << "         " << GENERATE << "\t Calculate the hitting sets using the options specified (the decycling set will automatically be generated)." << endl;
    cout << endl << "For help on either command, run 'pasha <command>' without any options." << endl << endl;
    exit(0);
  }
  string argFirst = string(argv[1]);
  if (argFirst == DECYCLING){
    //k-mer length (required)
    //output file
    if (argc <= 2 || string(argv[2]) == "--help" || string(argv[2]) == "help" || string(argv[2]) == "-h"){
      printDecyclingHelp("");
    }
    else {
      for (int i = 2; i < argc;){
        string argNext = string(argv[i]);
        if (argNext[0] == '-' && argc < i+2){
          printDecyclingHelp("Missing parameter for argument" + argNext + ".");
        }
        if (argNext == "-k"){
          k = atoi(argv[i+1]);
          if (k < 5 || k > 12){
            printDecyclingHelp("Pasha only supports k-mer lengths between 5 and 12.");
            i += 3;
          }
          i += 2;
        }
        else {
          if (argNext[0] == '-') //Incorrect argument
              printDecyclingHelp("Incorrect argument " + argNext+ ".");
          if (argc != i+1) //Too many arguments
              printDecyclingHelp("Too many arguments.");
          decyclingFile = string(argv[i]); //If no dash, it's output file
          cout << "Decycling set will be saved to: " << decyclingFile << endl;
          i += 1;
        }
      }
      if (decyclingFile == ""){
        printDecyclingHelp("No output file name specified.");
      }
      //Create directory here
      //Open file to write
    }
  }
  graph newGraph = graph(k, ALPHABET_SIZE, ALPHABET);
  newGraph.generateGraph(k);
  decycling newDecycling;
  vector<int> decyclingSet = newDecycling.computeDecyclingSet(k, ALPHABET_SIZE, ALPHABET); // Generate decycling set of order k
  outputStream.open(decyclingFile);
  for (int i = 0; i < decyclingSet.size(); i++) {
            cout << decyclingSet.at(i) << endl;
            string label = newGraph.getLabel(decyclingSet.at(i), k, ALPHABET_SIZE, ALPHABET); 
            cout << "Writing " << label << endl;
            outputStream << label << "\n";
  }
  outputStream.close();
  return 0;
}