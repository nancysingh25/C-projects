/* This program computes the gene cluster in which each gene is at most at
distance d of at least one other gene of the cluster and all being located on the same
chromosome. The clusters should be sorted in ascending coordinate order.
*/

#include <iostream>
#include <string>
#include<fstream>
#include <sstream>
#include <math.h>

using namespace std;


int main(int argc, const char *argv[]) {
    if (argc < 4) { //checking for correct program argument
        cout << "Missing arguments" << endl;
        cout << argv[0] << " sample_input.txt 2000 sample_output.txt" << endl;
        exit(EXIT_FAILURE);
    }

    ifstream input(argv[1]); //giving input file as a first argument

    long prog_distance;
    try {
        prog_distance = stoi(argv[2]);
    } catch (const invalid_argument &) {
        cout << "The second argument is not a number" << endl;
        cout << argv[0] << " sample_input.txt 2000 sample_output.txt" << endl;
        exit(EXIT_FAILURE);
    }

    ofstream out(argv[3]);

    long distance = 0;
    int middle = 0;

    int previous_mid = 0;
    string previous_chr;
    int previous_start = 0;

    string chr;
    int start = 0;
    int stop = 0;

    int cluster_no = 1;
//checking for valid input
     string line;
     while(getline(input, line)){
        istringstream s(line);

        s >> chr >> start >> stop;

        if (previous_chr == chr && (previous_start > start)) {
            cerr << "Input is not sorted" << endl;
            exit(EXIT_FAILURE);
        } else if (previous_chr > chr) {
            cerr << "Input is not sorted" << endl;
            exit(EXIT_FAILURE);
        }


//assigning genes in a cluster
        middle = (start + stop) / 2;

        if (previous_mid != 0 || previous_chr == chr) {
            distance = abs(middle - previous_mid);

            if (distance <= prog_distance) {
                out << chr << "\t" << start << "\t" << stop << "\t" << "cluster=" << cluster_no <<"\n";
            } else if (distance > prog_distance && previous_chr == chr) {
                ++cluster_no;
                out << chr << "\t" << start << "\t" << stop << "\t" << "cluster=" << cluster_no << "\n";
            } else if (distance > prog_distance && previous_chr != chr) {
                ++cluster_no;
                out << chr << "\t" << start << "\t" << stop << "\t" << "cluster=" << cluster_no <<"\n";
            }
        } else {
            out << chr << "\t" << start << "\t" << stop << "\t" << "cluster=" << cluster_no << "\n";
        }

        previous_mid = middle;
        previous_chr = chr;
        previous_start = start;
    }

    return 0;
}