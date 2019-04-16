<p align="center">
<img align = "center" src ="pasha.png">
</p>
<h1><center>Pasha: Parallel Algorithms for Approximating Compact Universal Hitting Sets</center></h1>

**Pasha** (Parallel Algorithms for Approximating Compact Universal Hitting Sets) is a tool for finding approximations for a small universal hitting set (a small set of k-mers that hits every sequence of length L). A detailed description of the functionality of Pasha, along with results in set size, runtime, memory usage and CPU utilization are provided in:

> Ekim, B., Orenstein, Y., Berger, B. Parallel algorithms for approximating compact universal hitting sets. *19th International Workshop on Algorithms in Bioinformatics* (2019).

Pasha is a tool for research and still under development. It is not presented as accurate or free of any errors, and is provided "AS IS", without warranty of any kind.

## Installation and setup

### Installing the package

Clone the repository to your local machine, and compile via `g++` using the command:

`g++ -O3 -o pasha pasha.cpp -fopenmp`

The `-O3` or `-Ofast` flag is necessary for efficient parallelization and optimization. The `-fopenmp` flag is needed to process parallelization via OpenMP.

### Example commands

Pasha offers several different options for the calculation of decycling and hitting sets.

To compute the decycling set for a specified k-mer length, use the command:

`./pasha decycling -k <kmerlength>`

Example: `./pasha decycling -k 5` will compute the decycling set for a complete de Bruijn graph of order 5.

To compute the hitting set for a specified k-mer and sequence length, use the command:

`./pasha generate [-a[<vcount>]][-p][-r] -k <kmerlength> -l <seqlength>`

Flag `-a` enables the calculation of paths of any length going through a vertex, as opposed to paths of length L-k+1.<br> 
Flag `-p` enables parallelization.  
Flag `-r` enables randomization. 

Example: `./pasha generate -k 7 -l 40` will compute the hitting set for k-mer length 7 and sequence length 40, counting paths of length 34.<br> 
Example: `./pasha generate -p -k 6 -l 20` will compute the hitting set for k-mer length 6 and sequence length 20 in parallel, counting paths of length 15.<br> 
Example: `./pasha generate -a -k 8 -l 60` will compute the hitting set for k-mer length 8 and sequence length 60, counting paths of any length.<br> 
Example: `./pasha generate -a -p -k 10 -l 50` will compute the hitting set for k-mer length 10 and sequence length 50 in parallel, counting paths of any length.<br>
Example: `./pasha generate -a 625 -p -k 10 -l 50` will compute the hitting set for k-mer length 10 and sequence length 50 in parallel, counting paths of any length, removing the top 625 vertices at a time.<br> 
Example: `./pasha generate -p -r -k 9 -l 30` will compute the hitting set for k-mer length 9 and sequence length 30 in parallel and using randomization, counting paths of length 22.

To compare outputs in terms of CPU usage, memory usage, running time, and set size, use the command:

`./pasha benchmark -k <kmerlength> -l <seqlength>`

Example: `./pasha benchmark -k 7 -l 60` will compute hitting sets for k-mer 7 and sequence length 60 with every method, and report CPU usage, memory usage, running time, and set size for each method.

Currently, Pasha supports k-mer lengths of 5 to 12, and sequence lengths of 20 to 200. Randomization requires computing paths in parallel; hence, in order to use the flag `-r`, the flag `-p` must be included.

For help on either command, run `pasha <command>` without any options.

### Interpreting output

Before computing a decycling or hitting set, Pasha will create a directory named `pasha_<kmerlength>`, and report the filenames the sets will be saved in. If `pasha generate` or `pasha benchmark` was run, Pasha will report running times:

`usr@usr:~/pasha$ ./pasha decycling -k 5`

`Directory created.`<br> 
`Decycling set will be saved to: pasha_5/d5.txt` <br> 
`Decycling set size: 208`

`usr@usr:~/pasha$ ./pasha generate -p -r -k 5 -l 20`

`Decycling set will be saved to: pasha_5/d5.txt` <br>
`Hitting set will be saved to: pasha_5/520r.txt` <br>
`Length of longest remaining path: 15` <br>
`0.184839 seconds.` <br>
`Hitting set size: 45`

## License

Pasha is freely available under the [MIT License](https://opensource.org/licenses/MIT).

## References

If you find Pasha useful, please cite the following papers:

- Ekim, B., Orenstein, Y., Berger, B. Parallel algorithms for approximating compact universal hitting sets. *19th International Workshop on Algorithms in Bioinformatics* (2019).

- Orenstein, Y., Pellow, D., Marçais, G., Shamir, R., Kingsford, C. Compact universal k-mer sets. *16th International Workshop on Algorithms in Bioinformatics* (2016).

- Orenstein, Y., Pellow, D., Marçais, G., Shamir, R., Kingsford, C. Designing small universal k-mer hitting sets for improved analysis of high-throughput sequencing. *PLoS Computational Biology* (2017).

## Contributors

Development of Pasha is led by [Barış Ekim](http://people.csail.mit.edu/ekim/), collaboratively in the labs of [Bonnie Berger](http://people.csail.mit.edu/bab/) at the Computer Science and Artificial Intelligence Laboratory (CSAIL) at Massachusetts Institute of Technology (MIT), and [Yaron Orenstein](http://wwwee.ee.bgu.ac.il/~yaronore/) at the Department of Electrical and Computer Engineering at Ben-Gurion University of the Negev (BGU).

## Contact

All of the software executables, manuals, and quick-start scripts, in addition to the calculated universal hitting sets are provided [here](http://pasha.csail.mit.edu/). Should you have any inquiries, please contact [Barış Ekim](http://people.csail.mit.edu/ekim/) at baris [at] mit [dot] edu.


