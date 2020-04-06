/*           Sudoku   solution
This source code is to solve 9x9 sudoku.
Input file is sudoku.txt.
It is a 9x9 matrix, use 0 to represent unknown blocks.
*/

#include <iostream>
#include <fstream>

using namespace std;

//declaration of functions and global values
void init_matrix(int matrix[10][10]);
int  input_matrix();
void update_availability(int matrix[10][10], int line[10][10], int row[10][10], int sub[10][10]);
int  solve_sudoku(int matrix[10][10]);
int  check_sudoku_solution(int matrix[10][10]);
void output_matrix(int matrix[10][10], int bAll);

int org_matrix[10][10];

//main function
int main()
{
    int i, j, val, matrix[10][10], rslt;

    //init values
    init_matrix(org_matrix);

    //input matrix and print input
    rslt = input_matrix();
    
    if(rslt)
    {
        cout << "*******Input********" << endl;
        output_matrix(org_matrix, 1);
        cout << "********************" << endl;

        for(i = 1; i <= 9; ++i)
            for(j = 1; j <= 9; ++j)
                matrix[i][j] = org_matrix[i][j];

        cout << endl;
        //calculate result
        if(!solve_sudoku(matrix))
             cout << "Solution fail" << endl;
        else
        {
            cout << "*******Output********" << endl;
            output_matrix(matrix, 0);
            cout << "*********************" << endl;
        }
    }
    
    system("PAUSE");
    return EXIT_SUCCESS;
}


//initialize matrix.
void init_matrix(int matrix[10][10])
{
    int i, j;

    for(i = 1; i <= 9; ++i)
        for(j = 1; j <= 9; ++j)
            matrix[i][j] = 0;
}

//input original matrix from d:\sudoku.txt
int input_matrix()
{
    //open input file
    ifstream input("sudoku.txt");
    if(!input)
    {
        cout << "No input file" << endl;
        return 0;
    }

    //input matrix
    char  ss[100];
    int   i = 0, j;
    while(input.good())
    {
        ++i;
        input.getline(ss, 100);
        sscanf(ss, "%d %d %d %d %d %d %d %d %d\n",
            &org_matrix[i][1], &org_matrix[i][2], &org_matrix[i][3],
            &org_matrix[i][4], &org_matrix[i][5], &org_matrix[i][6],
            &org_matrix[i][7], &org_matrix[i][8], &org_matrix[i][9]);
        if(i >= 9)
            break;
    }
    return 1;
}

//calculate availability of each line, each row and each sub_matrix
// sub_matrix number is
// 1   2   3
// 4   5   6
// 7   8   9
void update_availability(int matrix[10][10], int line[10][10], int row[10][10], int sub[10][10])
{
    int i, j, val;

    for(i = 1; i <= 9; ++i)
        for(j = 1; j <= 9; ++j)
            line[i][j] = row[i][j] = sub[i][j] = j;
            
    for(i = 1; i <= 9; ++i)
    {
        for(j = 1; j <= 9; ++j)
        {
            if(matrix[i][j])
            {
                val = matrix[i][j];
                line[i][val] = 0;
                row[j][val] = 0;
                sub[((i-1)/3)*3 + (j+2)/3][val] = 0;
            }
        }
    }
}

//solve sudoku
 int solve_sudoku(int matrix[10][10])
 {
     int i, j, ii, jj, m, val, avail_val[10], num, org_num, best_i, best_j, best_num, best_val[10];
     int line[10][10], row[10][10], sub[10][10];

     if(check_sudoku_solution(matrix))
          return 1;
          
     best_i = best_j = best_num = 0;
     update_availability(matrix, line, row, sub);
     
     //check each (i,j), find out how many possibility is there
     //if only one possibility, set value and keep on next solve_sudoku().
     for(i = 1; i <= 9; ++i)
     {
           for(j = 1; j <= 9; ++j)
           {
                if(matrix[i][j])
                    continue;
                    
                num = 0;
                //if one value is in line, row, and sub_matrix, treat it as an possibility
                for(ii = 1; ii <= 9; ++ii)
                {
                       val = line[i][ii];
                       if(val == 0) continue;
                       
                       org_num = num;
                       for(jj = 1; jj <= 9; ++jj)
                       {
                              for(m = 1; m <= 9; ++m)
                              {
                                     if(val == row[j][jj] &&
                                        val == sub[((i-1)/3)*3 + (j+2)/3][val])
                                     {
                                            avail_val[++num] = val;
                                            break;
                                     }
                              }
                              if(num != org_num)
                                     break;
                       }
                }

                if(num == 0) return 0;//no possibility means fail
                
                if(num == 1)//only possibility, so set value and iterate
                {
                       matrix[i][j] = avail_val[1];                       
                       return solve_sudoku(matrix);
                }
                
                if(best_num == 0 || num < best_num)//remember minimal possibilities
                {
                       best_num = num;
                       best_i = i;
                       best_j = j;
                       for(ii = 1; ii <= num; ++ii)
                            best_val[ii] = avail_val[ii];
                }
          }
     }
     
     //more than one possibilities at any (i,j)
     //use temp_matrix to set value at (i,j), then keep on using solve_sudoku(temp_matrix)
     for(m = 1; m <= best_num; ++m)
     {
         int temp_matrix[10][10];
         for(i = 1; i <= 9; ++i)
             for(j = 1; j <= 9; ++j)
                  temp_matrix[i][j] = matrix[i][j];
         
         temp_matrix[best_i][best_j] = best_val[m];
         if(solve_sudoku(temp_matrix))
         {
             for(i = 1; i <= 9; ++i)
                for(j = 1; j <= 9; ++j)
                   matrix[i][j] = temp_matrix[i][j];
                   
             return 1;              
         }
     }
     return 0;
}

//check solution
int check_sudoku_solution(int matrix[10][10])
{
    int i, j;
    int line[10][10], row[10][10], sub[10][10];

    update_availability(matrix, line, row, sub);
    for(i = 1; i <= 9; ++i)
    {
          for(j = 0; j <= 9; ++j)
          {
                if(line[i][j] || row[i][j] || sub[i][j])
                     return 0;
          }
    } 

    return 1;   
}

//Ouptup current matrix
void output_matrix(int matrix[10][10], int bAll)
{
     int i, j;
     
     for(i = 1; i <= 9; ++i)
     {
           for(j = 1; j <=9; ++j)
           {
                 if(!bAll && org_matrix[i][j])
                     cout << " " << " ";
                 else
                     cout << matrix[i][j] << " ";
           }
           cout << endl;
     }
}
