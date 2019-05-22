#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <windows.h> //console text handler
#include "Square.cpp"

//Checks for single permutation lines (eg. [X-X-X]) and single numbers with unambiguous spaces (eg. 4 -> [-XXX-])
void first_pass_rows(std::vector< std::vector<int> > &axis, std::vector< std::vector<Square> > &nonogram)
{
    int black_space_count = 0;
    int offset = 0;

    for(int i = 0; i < axis.size(); ++i)
    {
        for(int j = 0; j < axis[i].size(); ++j)
        {
            //Row is known to be empty
            if(axis[i][j] == 0)
            {
                for(int k = 0; k < nonogram[0].size(); ++k)
                {
                    nonogram[i][k].state = 1;
                }
            }

            //If the number of black spaces in a row equals the row's width
            else if(axis[i][j] == nonogram[0].size())
            {
                for(int k = 0; k < nonogram[0].size(); ++k)
                {

                    nonogram[i][k].state = 2;
                }
            }

            //Now check for unambiguous middle spaces, eg. [XXX--] [-XXX-] [--XXX], in all posible cases the middle is definitely a black space
            //[XXXX-] [-XXXX], middle three are definitely black.
            //The amount of unsure spaces to leave on either size is given by (puzzle length - total number of black space)
            //The amount in the middle is total blacks - (puzzle length - total blacks)

            else if((axis[i].size() == 1) && (axis[i][j] > (nonogram[0].size() / 2)))
            {
                for(int p = 0; p < (axis[i][j] - (nonogram[0].size() - axis[i][j])); ++p)
                {
                    nonogram[i][p + (nonogram[0].size() - axis[i][j])].state = 2;
                }
            }
            else
            {
                black_space_count += axis[i][j];
            }
        }
        //For patterns that can only fit in the puzzle one way, eg. [X-X-X] or [XX-XX]
        if(black_space_count + (axis[i].size() - 1) == nonogram[0].size())
        {
            for(int n = 0; n < axis[i].size(); ++n) //As it traverses row i...
            {
                for(int o = 0; o < axis[i][n]; ++o) //it loops for the amount of black spaces specified in the vector
                {
                    nonogram[i][n + offset].state = 2;
                    ++offset;
                }

                if((n + offset) < nonogram[0].size()){
                    nonogram[i][n + offset].state = 1;
                }
            }
        }
        offset = 0;
        black_space_count = 0;
    }
}

//Chose to give columns their own separate function for easier readability and debugging
void first_pass_cols(std::vector< std::vector<int> > &axis, std::vector< std::vector<Square> > &nonogram)
{
    int black_space_count = 0;
    int offset = 0;

    for(int i = 0; i < axis.size(); ++i)
    {
        for(int j = 0; j < axis[i].size(); ++j)
        {
            //Column is empty
            if(axis[i][j] == 0)
            {
                for(int k = 0; k < nonogram.size(); ++k)
                {
                    nonogram[k][i].state = 1;
                }
            }

            //If the number of black spaces in a column equals the column's width
            else if(axis[i][j] == nonogram.size())
            {
                for(int k = 0; k < nonogram.size(); ++k)
                {
                    nonogram[k][i].state = 2;
                }
            }

            //Now check for unambiguous middle spaces, eg. [XXX--] [-XXX-] [--XXX], in all posible cases the middle is definitely a black space
            //[XXXX-] [-XXXX], middle three are definitely black.
            //The amount of unsure spaces to leave on either size is given by (puzzle length - total number of black space)
            //The amount in the middle is total blacks - (puzzle length - total blacks)

            else if((axis[i].size() == 1) && (axis[i][j] > (nonogram.size() / 2)))
            {
                for(int p = 0; p < (axis[i][j] - (nonogram.size() - axis[i][j])); ++p)
                {
                    nonogram[p + (nonogram.size() - axis[i][j])][i].state = 2;
                }
            }
            else
            {
                black_space_count += axis[i][j];
            }
        }
        //For patterns that can only fit in the puzzle one way, eg. [X-X-X] or [XX-XX]
        if(black_space_count + (axis[i].size() - 1) == nonogram.size())
        {
            for(int n = 0; n < axis[i].size(); ++n) //As it goes through column i...
            {
                for(int o = 0; o < axis[i][n]; ++o) //it loops for the amount specified in the vector
                {
                    nonogram[n + offset][i].state = 2;
                    ++offset;
                }

                if((n + offset) < nonogram.size())
                {
                    nonogram[n + offset][i].state = 1;
                }
            }
        }

        offset = 0;
        black_space_count = 0;
    }
}

//Checks if picross is potentially able to be solved in its current state
bool is_valid(std::vector< std::vector<int> > &rows, std::vector< std::vector<int> > &cols, std::vector< std::vector<Square> > &nonogram)
{
    //Checking rows
    int total_blacks = 0;
    //Looping downwards
    for(int i = 0; i < rows.size(); ++i)
    {
        int cur_black_count = 0;
        int cur_x_count = 0;

        for(int j = 0; j < rows[i].size(); j++)
        {
            total_blacks += rows[i][j];
        }

        //Looping left to right over squares
        for(int k = 0; k < nonogram[0].size(); ++k)
        {
            if(abs(nonogram[i][k].state) == 1)
            {
                cur_x_count++;
            }

            if(abs(nonogram[i][k].state) == 2)
            {
                cur_black_count++;
            }
        }

        if(cur_black_count > total_blacks || cur_x_count > (nonogram[0].size() -  total_blacks) )
        {
            return false;
        }
    }

    //Checking rows
    total_blacks = 0;
    //Looping downwards
    for(int i = 0; i < cols.size(); ++i)
    {
        int cur_black_count = 0;
        int cur_x_count = 0;

        for(int j = 0; j < cols[i].size(); j++)
        {
            total_blacks += cols[i][j];
        }

        //Looping left to right over squares
        for(int k = 0; k < nonogram.size(); ++k)
        {
            if(abs(nonogram[k][i].state) == 1)
            {
                cur_x_count++;
            }

            if(abs(nonogram[k][i].state) == 2)
            {
                cur_black_count++;
            }
        }

        if(cur_black_count > total_blacks || cur_x_count > (nonogram.size() -  total_blacks) )
        {
            return false;
        }
    }

    return true;
}

bool is_solved(std::vector< std::vector<int> > &rows, std::vector< std::vector<int> > &cols, std::vector< std::vector<Square> > &nonogram)
{
    //Checking rows
    //Looping downwards
    for(int i = 0; i < rows.size(); ++i)
    {
        int cur_black_count = 0;
        int cur_param_index = 0;
        int cur_param = rows[i][cur_param_index];

        //Looping left to right over squares
        for(int j = 0; j < nonogram[0].size(); ++j)
        {
            //If empty space, puzzle isn't even filled out yet
            if(abs(nonogram[i][j].state) == 0)
            {
                return false;
            }

            //If black space, increment counter
            else if(abs(nonogram[i][j].state) == 2)
            {
                cur_black_count++;
            }

            //If white space, compare cur_black_count with cur_param
            else if(abs(nonogram[i][j].state) == 1)
            {
                if(cur_black_count != cur_param && abs(nonogram[i][j - 1].state) == 2)
                {
                    return false;
                }

                //If all row params are met, set cur_param to zero since all remaining spaces in row should be white spaces
                if(cur_param_index == rows[i].size())
                {
                    cur_param = 0;
                }
                //Only move to next param if previous square was black
                else if(j != 0 && abs(nonogram[i][j - 1].state) == 2)
                {
                    cur_param_index++;
                    cur_param = rows[i][cur_param_index];
                }
                cur_black_count = 0;
            }
        }
    }

    //Checking columns
    //Looping over each column
    for(int i = 0; i < cols.size(); ++i)
    {
        int cur_black_count = 0;
        int cur_param_index = 0;
        int cur_param = cols[i][cur_param_index];

        //Looping downwards over squares
        for(int j = 0; j < nonogram.size(); ++j)
        {
            //We know puzzle must be filled out here, or else the function would have returned false while checking rows

            //If black space, increment counter
            if(abs(nonogram[j][i].state) == 2)
            {
                cur_black_count++;
            }

            //If white space, compare cur_black_count with cur_param
            else if(abs(nonogram[j][i].state) == 1)
            {
                if(cur_black_count != cur_param && j != 0 && abs(nonogram[j - 1][i].state) == 2)
                {
                    return false;
                }

                //If all column params are met, set cur_param to zero since all remaining spaces in column should be white spaces
                if(cur_param_index == cols[i].size())
                {
                    cur_param = 0;
                }
                //Only move to next param if previous square was black
                else if(j != 0 && abs(nonogram[j - 1][i].state) == 2)
                {
                    cur_param_index++;
                    cur_param = cols[i][cur_param_index];
                }
                cur_black_count = 0;
            }
        }
    }

    return true;
}

void solveProblem(std::vector< std::vector<int> > &rows, std::vector< std::vector<int> > &cols, std::vector< std::vector<Square> > &nonogram)
{
    /*if(nonogram violates params) //check_picross(rows, cols, nonogram)
          return;

    if((is_solved(rows, cols, nonogram)
          output currentAttempt;

    for(each possible nextAttempt based on currentAttempt)
          solveProblem(nextAttempt)*/
}


int main()
{
    srand( time(NULL));

    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE); //h_console is a console command handler, used to change text color in the terminal

    //puzzle is a 2d vector of 10 columns, 3 rows, containing picrossSquare types
    //std::vector< std::vector<picrossSquare> > puzzle(number of rows, std::vector<picrossSquare>(number of columns));
    std::vector< std::vector<Square> > puzzle( 4, std::vector<Square>(10));

    std::vector< std::vector<int> > rows {{2, 2, 3},
                                          {1, 1, 1},
                                          {8},
                                          {1, 1, 1}};

    //Columns are written from {top -> bottom}
    std::vector< std::vector<int> > columns {{2, 1},
                                          {1},
                                          {3},
                                          {1, 1},
                                          {1, 2},
                                          {1},
                                          {1},
                                          {1, 1},
                                          {3},
                                          {1, 1}};


    first_pass_rows(rows, puzzle);
    first_pass_cols(columns, puzzle);

    //Testing with solved nonogram
    /*for(int i = 0; i < puzzle.size(); i++)
    {
        for(int j = 0; j < puzzle[0].size(); j++)
        {
            puzzle[i][j].state = 1;
        }
    }

    puzzle[0][0].state = 2;
    puzzle[0][1].state = 2;
    puzzle[0][3].state = 2;
    puzzle[0][4].state = 2;
    puzzle[0][7].state = 2;
    puzzle[0][8].state = 2;
    puzzle[0][9].state = 2;
    puzzle[1][0].state = 2;
    puzzle[1][2].state = 2;
    puzzle[1][8].state = 2;
    puzzle[2][2].state = 2;
    puzzle[2][3].state = 2;
    puzzle[2][4].state = 2;
    puzzle[2][5].state = 2;
    puzzle[2][6].state = 2;
    puzzle[2][7].state = 2;
    puzzle[2][8].state = 2;
    puzzle[2][9].state = 2;
    puzzle[3][0].state = 2;
    puzzle[3][2].state = 2;
    puzzle[3][4].state = 2;*/

    for(int m = 0; m < 4; ++m){
        for(int n = 0; n < 10; ++n){
            switch( abs(puzzle[m][n].state))
            {
                case 0 :
                    SetConsoleTextAttribute(h_console, 255); //white
                    break;

                case 1 :
                    SetConsoleTextAttribute(h_console, 200); //red
                    break;

                case 2 :
                    SetConsoleTextAttribute(h_console, 150); //blue
                    break;

            }
            std::cout << puzzle[m][n].state << " ";
        }
        std::cout << "\n";
    }

    SetConsoleTextAttribute(h_console, 15);

    //std::cout << is_solved(rows, columns, puzzle);
    std::cout << is_valid(rows, columns, puzzle);
}
