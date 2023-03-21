/*
A program that takes as first argument a FASTA file, as second argument the
k-mer size and as third argument the skip size n and outputs the number of n-skip-
k-mers found in the sequences on the standard output. A n-skip-k-mer is defined as
a sequence of length k that includes one out of every n+1 bases. The output
contain thes header of the sequence and then the uppercase n-skip-k-mer and its
count (space separated), sorted lexicographically by the n-skip-k-mers.

*/


#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <ostream>
using namespace std;

void nskip_kmer(int& k, int& n, string& seq) //function to calculate the n-skip-kmer
{
    map<string, int> k_mer; //store the kmer string and count
        for(int i = 0; i < seq.size(); i++)
        {
            int end_pos = i + (n + 1) * (k - 1); // if end position exceding the string end then break the loop
           if(end_pos > seq.size() - 1){break;}
            string nk_mer;
            for(int j = 0; j < k; j++)
            {
                nk_mer += seq[i + (n + 1) * (j)];
            }

            transform(nk_mer.begin(), nk_mer.end(),nk_mer.begin(), ::toupper); //to make the lowercase to uppercase
            k_mer[nk_mer] += 1;
        }

       for(auto it : k_mer)
            cout << it.first << " " << it.second << "\n"; //printing the map containing nskip_kmer and count
     cout<<"\n";
}


int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << " Invalid program arguments";
    }

    int k, n;
    ifstream input(argv[1]);
    k = stoi(argv[2]); //kmer size
    n = stoi(argv[3]); //n skip kmer


    string line;
    string seq, header;
    std::map<string, int> k_mer;
   string first_char = "ATGCatgc";

    //input
    if (!input.eof() && !input.fail()) {
        while (std::getline(input, line)) {
            if (line[0] == '>') {
                cout << header << "\n";
                if (!seq.empty() && seq.find_first_of(first_char)==0) {// checking if the file is empty and the first letter of the string
                    nskip_kmer(k, n, seq);

                } else {
                    cerr << header <<"\n";
                }
                seq = "";
                header = line.substr(1, line.size());
            } else {
                seq += line;
            }
        }
            if(!seq.empty()&&seq.find_first_of(first_char)==0 ) //printing the result for the last sequence
            {
                cout<<header<<"\n";
                nskip_kmer(k, n,seq);
            }else{
                cerr<<header<<"\n";
            }



    }


    return 0;

}