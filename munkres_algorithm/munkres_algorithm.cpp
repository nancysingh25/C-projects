#include "munkres_algorithm.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

void reduced_row(Matrix<int>& v) { //step 1: finding minimum element in the row and substracting it from other elements
    int row_min;
    for (auto i = 0; i < v.nrows(); i++) {
        row_min = v(i, 0);
        for (auto j = 1; j < v.ncols(); j++) {

            if (v(i, j) < row_min) {
                row_min = v(i, j);
            }
        }
        for (auto j = 0; j < v.ncols(); j++) {
            v(i, j) = v(i, j) - row_min;
        }
    }

}

void reduced_col(Matrix<int>& v) {//step 2: Finding the minimum element in the column and substracting it with other elements
    int col_min;
    for(auto i =0; i<v.nrows(); i++) {
        col_min = v(0,i);
        for (auto j =1; j < v.ncols(); j++) {

            if(v(j,i)< col_min){
                col_min= v(j,i);
            }

        }
        for (auto j = 0; j < v.ncols(); j++) {
            v(j, i) = v(j, i) - col_min;
        }
    }
}
void starred(int& step, Matrix<int>& v, Matrix<int>& matrix, std::vector<int>& row, std::vector<int>& col ) {// marking zero with star
    reduced_row(v);
    reduced_col(v);
    for (auto i = 0; i < v.nrows(); i++) { //for all zeros i in matrix, mark i with a star if there is no starred in row or column
        for (auto j = 0; j < v.ncols(); j++) {
            if ((v(i, j) == 0) && (row[i] == 0) && (col[j] == 0)) {
                matrix(i, j) = 1;
                row[i] = 1;
                col[j] = 1;
            }
        }
    }
    for (auto i = 0; i < v.nrows(); i++) {
        row[i] = 0; //uncovering row
    }
    for (auto j = 0; j < v.ncols(); j++) {
        col[j] = 0; //uncovering column
    }
    step = 1; //then go to step1

}


void check_starred(int& step, Matrix<int>& matrix, std::vector<int>& col){
    for(auto i =0; i<matrix.nrows();i++){ //for every column with starred zero cover that column
        for(auto j=0; j< matrix.ncols();j++){
            if(matrix(i,j)==1){
                col[j]= 1;
            }

        }

    }
    if(find(col.begin(),col.end(),0)==col.end()){ //if n column are covered then got to step which is done
        //done = true;
        step = 5;
    }
    else{
        step = 2; //else go to step 2
    }

}
int uncovered_zero(Matrix<int>& v, Matrix<int>& matrix, std::vector<int>& row, std::vector<int>& col){ //to check the uncover column with zero and prime it
    int prime = -1;
    for(auto i=0; i<v.nrows();i++){
        //row[i] = 0;// i added
        for(auto j=0; j<v.ncols();j++){
            if((v(i,j)==0)&&(row[i]==0)&&(col[j]==0)){
                prime =i;
                matrix(i,j)=2;
                return prime;
            }
        }
    }
    return prime;

}
int starred_Zero_row(Matrix<int>& v, Matrix<int>& matrix, int prime){ //function to check the starred zero in row. calling this function step2
    int star_col=-1;
    for(auto j=0; j<v.ncols();j++){
        if(matrix(prime,j)==1){
            star_col=j;
        }

    }
    return star_col;
}

int algo_step2(int& step,Matrix<int>& v, Matrix<int>& matrix, std::vector<int>& row, std::vector<int>& col, int& e_min){
    int prime = uncovered_zero(v,matrix, row,col); //findind uncovered zero and priming it
    if(prime!=-1){
        int star_col= starred_Zero_row(v,matrix,prime); // if there is no zero in the row Z0
        if(star_col==-1) {
            //step3
            step = 3;
        }else{//cover the row and uncovering the column then go to step2
            row[prime]=1;
            col[star_col]=0;
            //step2;
            step = 2;
        }
    }else{
        std::vector<int> e;
        for(auto i=0; i<v.nrows();i++){
            for(auto j=0; j<v.ncols();j++){
                if(row[i]==0 && col[j]==0){
                    int a= v(i,j);
                    e.push_back(a);
                }
            }
        }
        e_min= *std::min_element(e.begin(),e.end()); //finding minimum e_min value and then go to step4
        //  step4(mat, row, col, e_min);
        step = 4;
    }

    return prime;
}

int prime_col(Matrix<int> matrix, int primeRow){
    int primeCol = -1;
    for (auto j = 0; j < matrix.ncols(); ++j){
        if (matrix(primeRow, j) == 2) primeCol = j;
    }
    return primeCol;
}
int prime_row(Matrix<int> matrix, int primeCol){
    int primeRow = -1;
    for (auto i = 0; i < matrix.nrows(); ++i){
        if (matrix(i, primeCol) == 2) {
            primeRow = i;
            break;
        }
    }

    return primeRow;

}
int starred_row(Matrix<int> matrix, int primeCol){
    int star_row = -1;
    for (auto i = 0; i < matrix.ncols(); i++){
        if (matrix(i, primeCol) == 1) star_row = i;
    }

    return star_row;
}
int starred_col(Matrix<int> matrix, int starRow){
    int star_col = -1;
    for (auto j = 0; j < matrix.ncols(); j++){
        if (matrix(starRow, j) == 1) star_col = j;
    }

    return star_col;
}
bool starred_zero_col(Matrix<int>& matrix, int primeCol){
    bool result = false;
    for (size_t i = 0; i < matrix.nrows(); i++){
        if (matrix(i, primeCol) == 1){
            result = true;
            break;
        }
    }

    return result;
}
void algo_step3(int& step, Matrix<int>& v, Matrix<int>& matrix, std::vector<int>& row, std::vector<int>& col, int& primeRow){ // STEP 3
    std::vector<int> s;
    s.push_back(primeRow); //Insert Z0 into S

    while (starred_zero_col(matrix, prime_col(matrix, primeRow))){ // if in the column of Z0 exists a starred zero Z1 do
        int star_row = starred_row(matrix, prime_col(matrix, primeRow));
        s.push_back(star_row); // insert Z1 into S

        int primeCol = prime_col(matrix, star_row);
        primeRow = prime_row(matrix, primeCol); // replacing Z0 with the primed zero in the row of Z1. Insert Z0 into S
        s.push_back(primeRow);
    }

    for (auto i = 1; i < s.size(); i+=2){ // Unstar each starred zero in S
        matrix(s[i], starred_col(matrix, s[i])) = 0;
    }
    for (size_t i = 0; i < s.size(); i+=2){
        matrix(s[i], prime_col(matrix, s[i])) = 1; // replacing all primes with stars.
    }
    for (auto i = 0; i < matrix.nrows(); i++){ // erasing all other primes
        for (auto j = 0; j < matrix.ncols(); j++){
            if (matrix(i,j) == 2) matrix(i,j) = 0;
        }
    }
    for (auto i = 0; i < v.nrows(); i++) {
        row[i] = false; // uncover every line in C
    }
    for (auto j = 0; j<  v.ncols(); j++) {
        col[j] = false; // uncover every line in C
    }
    step = 1; // GOTO STEP 1

}

void algo_step4(int& step,Matrix<int>& v, Matrix<int>& matrix, std::vector<int>& row, std::vector<int>& col, int e_min){
    for (auto i = 0; i < row.size(); i++){// add e_min to evey element in covered rows
        if (row[i]){
            for (auto j = 0; j <matrix.ncols(); j++){
                matrix(i,j) += e_min;
            }
        }
    }
    for (size_t j = 0; j < col.size(); j++){ //subtract emin from every element in uncovered columns
        if (!col[j]){
            for (auto i = 0; i < matrix.nrows(); i++){
                matrix(i,j) -= e_min;
            }
        }
    }
    //step2(v, mat, row, col, e_min);
    step = 2;
}


Matrix<int> run_munkres_algorithm(Matrix<int> c) {
    if (c.nrows() != c.ncols()){
        std::cerr << "Matrix is not a square matrix" << std::endl;
    }
    Matrix<int> matrix(c.nrows(), c.ncols(),0);

    int e_min=0;
    bool done = false;
    int step = 0;
    int prime = 0;
    std::vector<int> row(c.nrows(), 0);
    std::vector<int> col(c.ncols(), 0);

    while (!done){

        if(step==0) {
            starred(step, c, matrix, row, col);
        }
        else if(step== 1) {
            check_starred(step, matrix, col);
        }
        else if(step==2) {
            prime = algo_step2(step, c, matrix, row, col, e_min);
        }
        else if (step==3) {
            algo_step3(step, c, matrix, row, col, reinterpret_cast<int &>(prime));
        }
        else if(step==4){
            algo_step4(step, c, matrix,row, col, e_min);
        }

        else if(step==5) {
            done = true;
        }
    }
    return matrix;
}

