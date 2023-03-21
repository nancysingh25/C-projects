//This program takes 2 strings as program argument and runs the needleman_wunsch algorithm and otput the
//alignment for the strings

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
//using namespace std;
int match=1;
int gap=-1;
int mismatch= -1;
//functionto calculate the alignment score
 int needleman_wunsch(std::string A, std::string B, std::vector<std::vector<int>> &matrix) {
     int a_l = A.length();
     int b_l = B.length();
     //intializing the matrix
     for (int i = 0; i <= a_l; i++) {
         matrix[i][0] = matrix[0][i] = -i * gap;
     }
     for (int j = 0; j <= b_l; j++) {
         matrix[0][j] = matrix[0][j] = -j * gap;
     }
     //computing the alignment score
     for (int i = 1; i <= a_l; i++) {
         for (int j = 1; j <= b_l; j++) {
             int s;
             if(A[i-1]==B[j-1]){
                 s= match;
             }else{
                 s= -mismatch;
             }
             matrix[i][j] = std::max(matrix[i - 1][j - 1] + s,
                                     std::max(matrix[i - 1][j] - gap, matrix[i][j - 1] - gap));

         }
         return matrix[a_l][b_l];
     }
 }


int main(int argc, const char *argv[])
{
    std::string s1;
    std::string s2;
    if(argc==3){//passing strings as program arguments
       s1= argv[1];
       s2= argv[2];
       std::string a, b;
      int m= s1.length();
      int n = s2.length();
        std::vector<std::vector<int>> matrix(m+1, std::vector<int>(n+1, 0));
      //cout<< m <<" "<< n<< endl;
      std::cout<< needleman_wunsch(s1, s2, matrix)<<std::endl;

    }else{
        std::cerr<<"Need 2 sequences"<< std::endl;
    }
  return 0;
}
