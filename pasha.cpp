// my first program in C++
#include <iostream>
//Defining main commands
#define DECYCLING = "decycling"
#define GENERATE = "generate"
void parseArguments(int argc, char* argv[]){
  //Parsing main command line arguments
  cout << endl;
  if (argc == 1 || string(argv[1]) == "-h" || string(argv[1]) == "--help" || string(argv[1]) == "help"){
    cout << "Usage: pasha <command> [options]" << endl << endl;
    cout << "Command: " << DECYCLING << "\t Calculate only the decycling set for k-mer length k." << endl;
    cout << "         " << GENERATE << "\t Calculate the hitting sets using the options specified (the decycling set will automatically be generated)." << endl;
    cout << endl << "For help on either command, run 'pasha <command>' without any options." << endl << endl;
    exit(0);
  }
  string firstArgument(argv[1]);
  if (argFirst == DECYCLING){
    //k-mer length (required)
    //output file
    if (argc <= 2 || string(argv[2]) == "--help" || string(argv[2]) == "help" || string(argv[2]) == "-h"){
      printDecyclingHelp("");
    }
    else {
      for (int i = 2; i < argc;){
        string argNext = string(argv[i]);
        if (argNext[0] == '-' && argc < i+1){
          printDecyclingHelp("Missing parameter for argument" + argNext + ".");
        }
        if (argNext == "-k"){
            argK = atoi(argv[i+1]);
            if (argK < 5 || argK > 12){
              printDecyclingHelp("Pasha only supports k-mer lengths between 5 and 12.");
              i += 2;
            }
          }


      }

    }


  }




}
