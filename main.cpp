#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <windows.h> //console text handler

//Checks for single permutation lines (eg. [X-X-X]) and single numbers with unambiguous spaces (eg. 4 -> [-XXX-])
void preliminary_search_rows(std::vector< std::vector<int> > &axis, std::vector< std::vector<picrossSquare> > &nonogram)
{
    int black_space_count = 0,
        offset = 0;

    std::cout << "";
    for(int i = 0; i < axis.size(); ++i)
    {
        for(int j = 0; j < axis[i].size(); ++j)
        {
            if(axis[i][j] == 0)
            {
                for(int k = 0; k < nonogram[0].size(); ++k)
                {
                    nonogram[i][k].state = '*';
                }
            }

            //If the number of black spaces in a row equals the row's width
            else if(axis[i][j] == nonogram[0].size())
            {
                for(int k = 0; k < nonogram[0].size(); ++k)
                {

                    nonogram[i][k].state = 'X';
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
                    nonogram[i][p + (nonogram[0].size() - axis[i][j])].state = 'X';
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
            for(int n = 0; n < axis[i].size(); ++n) //while it goes through row i...
            {
                for(int o = 0; o < axis[i][n]; ++o) //it loops for the amount specified in the vector
                {
                    nonogram[i][n + offset].state = 'X';
                    ++offset;
                }

                if((n + offset) < nonogram[0].size()){
                    nonogram[i][n + offset].state = '*';
                }
            }
        }
        offset = 0;
        black_space_count = 0;
    }
}

//Chose to give columns their own separate function for easier readability and fewer bugs than trying to integrate a system for columns and columns into one function
void preliminary_search_columns(std::vector< std::vector<int> > &axis, std::vector< std::vector<picrossSquare> > &nonogram)
{
    int black_space_count = 0,
        offset = 0;

    std::cout << "";
    for(int i = 0; i < axis.size(); ++i)
    {
        for(int j = 0; j < axis[i].size(); ++j)
        {
            if(axis[i][j] == 0)
            {
                for(int k = 0; k < nonogram.size(); ++k)
                {
                    nonogram[k][i].state = '*';
                }
            }

            //If the number of black spaces in a column equals the column's width
            else if(axis[i][j] == nonogram.size())
            {
                for(int k = 0; k < nonogram.size(); ++k)
                {
                    //columnAccess and columnAccess dictate wether to fill in square across a column or down a column
                    //One must be 0 and the other be 1. If columnAcess == 1 and columnAccess == 0, the array coordinates below will simplify to
                    //nonogram[i][k], and thus the squares will be filled in across column #i
                    nonogram[k][i].state = 'X';
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
                    nonogram[p + (nonogram.size() - axis[i][j])][i].state = 'X';
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
            for(int n = 0; n < axis[i].size(); ++n) //while it goes through column i...
            {
                for(int o = 0; o < axis[i][n]; ++o) //it loops for the amount specified in the vector
                {
                    nonogram[n + offset][i].state = 'X';
                    ++offset;
                }

                if((n + offset) < nonogram.size())
                {
                    nonogram[n + offset][i].state = '*';
                }
            }
        }

        offset = 0;
        black_space_count = 0;
    }
}

void secondary_search_rows(std::vector< std::vector<int> > &axis, std::vector< std::vector<picrossSquare> > &nonogram)
{
    int black_space_count = 0,
        total_black_count = 0,
        cross_count = 0,
        //offset = 0;
        left_boundary = 0,
        right_boundary = 0;

    for(int i = 0; i < nonogram.size(); ++i)
    {
        //Add up the integers in rows[i] to get the total number of black spaces in the row
        //Since the number of elements in the rows vector represent the number of rows in the nonogram, int i can be used here
        for(int m = 0; m < axis[i].size(); ++m)
        {
            total_black_count += axis[i][m];
        }

        //Add up the current number of black spaces filled
        for(int j = 0; j < nonogram[0].size(); ++j)
        {
            if(nonogram[i][j].state == 'X')
            {
                ++black_space_count;
            }
            else if (nonogram[i][j].state == '*')
            {
                ++cross_count;
            }
        }

        //If all non-black spaces have been found, the row can be filled in
        if(cross_count == (nonogram[0].size() - total_black_count))
        {
            for(int k = 0; k < nonogram[0].size(); ++k)
            {
                if(nonogram[i][k].state == '-')
                {
                    nonogram[i][k].state = 'X';
                }
            }
        }

        //If all black spaces have been found, the row can be crossed out
        if(black_space_count == total_black_count)
        {
            for(int k = 0; k < nonogram[0].size(); ++k)
            {
                if(nonogram[i][k].state == '-')
                {
                    nonogram[i][k].state = '*';
                }
            }
        }
        //This is a more advanced version of checking for unambiguous blocks. Here it sets a left and right boundary by filling in
        //imaginary blocks in the leftmost and righmost spaces, and finds the unambiguity of the remaining spaces
        //ex. in a size fifteen row {1, 2, 5, 2} checking for the five, it would calculate the row like this: [X*XX*-------*XX]
        //Within the seven dashes, the middle three are definitely black spaces
        //This even works when there's only one element :^))))))
         else if(total_black_count > (nonogram[0].size() / 2))
        {
            for(int n = 0; n < axis[i].size(); ++n)
            {
                left_boundary = 0;
                right_boundary = 0;

                //The number of interest right now is axis[m][n]. We must now find the left & right boundaries using elements in axis
                for(int o = 0; o < n; ++o)
                {
                    left_boundary += (axis[i][o] + 1);
                }

                for(int p = (axis[i].size() - 1); p > n; --p)
                {
                    right_boundary += (axis[i][p] + 1);
                }

                //With the boundaries now set, find the unambiguous spaces if there are any. axis[m][n] must be greater than half of the
                //space between the boundaries.
                if(axis[i][n] > (nonogram[0].size() - (right_boundary + left_boundary)) / 2)
                {
                    for(int q = 0; q < (axis[i][n] - (((nonogram[0].size() - (right_boundary + left_boundary)) - axis[i][n]))); ++q)
                    {
                        nonogram[i][left_boundary + ((nonogram[0].size() - (right_boundary + left_boundary) - axis[i][n])) + q].state = 'X';
                    }
                }
            }
        }

        black_space_count = 0;
        total_black_count = 0;
    }
}

void secondary_search_columns(std::vector< std::vector<int> > &axis, std::vector< std::vector<picrossSquare> > &nonogram)
{
    int black_space_count = 0,
        total_black_count = 0,
        cross_count = 0,
        //offset = 0;
        left_boundary = 0,
        right_boundary = 0;

    for(int i = 0; i < nonogram[0].size(); ++i)
    {
        //Add up the integers in column[i] to get the total number of black spaces
        //Since the number of elements in the column vector represent the number of columns in the nonogram, int i can be used here
        for(int m = 0; m < axis[i].size(); ++m)
        {
            total_black_count += axis[i][m];
        }

        //Add up the current number of black spaces filled
        for(int j = 0; j < nonogram.size(); ++j)
        {
            if(nonogram[j][i].state == 'X')
            {
                ++black_space_count;
            }
            else if (nonogram[j][i].state == '*')
            {
                ++cross_count;
            }
        }

        //If all non-black spaces have been found, the column can be filled in
        if(cross_count == (nonogram.size() - total_black_count))
        {
            for(int k = 0; k < nonogram.size(); ++k)
            {
                if(nonogram[k][i].state == '-')
                {
                    nonogram[k][i].state = 'X';
                }
            }
        }

        //If all black spaces have been found, the column can be crossed out
        if(black_space_count == total_black_count){
            for(int k = 0; k < nonogram.size(); ++k)
            {
                if(nonogram[k][i].state == '-')
                {
                    nonogram[k][i].state = '*';
                }
            }
        }

        //This is a more advanced version of checking for unambiguous blocks. Here it sets a left and right boundary by filling in
        //imaginary blocks in the leftmost and righmost spaces, and finds the unambiguity of the remaining spaces
        //ex. in a size fifteen row {1, 2, 5, 2} checking for the five, it would calculate the row like this: [X*XX*-------*XX]
        //Within the seven dashes, the middle three are definitely black spaces
        //This even works when there's only one element :^))))))
         else if(total_black_count > (nonogram.size() / 2))
        {
            for(int n = 0; n < axis[i].size(); ++n)
            {
                left_boundary = 0;
                right_boundary = 0;

                //The number of interest right now is axis[m][n]. We must now find the left & right boundaries using elements in axis
                for(int o = 0; o < n; ++o)
                {
                    left_boundary += (axis[i][o] + 1);
                }

                for(int p = (axis[i].size() - 1); p > n; --p)
                {
                    right_boundary += (axis[i][p] + 1);
                }

                //With the boundaries now set, find the unambiguous spaces if there are any. axis[m][n] must be greater than half of the
                //space between the boundaries.
                if(axis[i][n] > (nonogram.size() - (right_boundary + left_boundary)) / 2)
                {
                    for(int q = 0; q < (axis[i][n] - (((nonogram.size() - (right_boundary + left_boundary)) - axis[i][n]))); ++q)
                    {
                        nonogram[left_boundary + ((nonogram.size() - (right_boundary + left_boundary) - axis[i][n])) + q][i].state = 'X';
                    }
                }
            }
        }

        black_space_count = 0;
        total_black_count = 0;
    }
}

int main()
{
    srand( time(NULL));

    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE); //h_console is a console command handler

    //puzzle is a 2d vector of 10 columns, 3 rows, containing picrossSquare types
    //std::vector< std::vector<picrossSquare> > puzzle(number of rows, std::vector<picrossSquare>(number of columns));
    std::vector< std::vector<picrossSquare> > puzzle( 4, std::vector<picrossSquare>(10));

    std::vector< std::vector<int> > rows {{2, 2, 3},
                                          {1, 1, 1},
                                          {8},
                                          {1, 1, 1}};

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


    preliminary_search_rows(rows, puzzle);
    preliminary_search_columns(columns, puzzle);

    secondary_search_rows(rows, puzzle);
    secondary_search_columns(columns, puzzle);

    for(int m = 0; m < 4; ++m){
        for(int n = 0; n < 10; ++n){
            switch( puzzle[m][n].state)
            {
                case '-' :
                    SetConsoleTextAttribute(h_console, 255); //white
                    break;

                case 'X' :
                    SetConsoleTextAttribute(h_console, 150); //blue
                    break;

                case '*' :
                    SetConsoleTextAttribute(h_console, 200); //red
                    break;
            }
            std::cout << puzzle[m][n].state << " ";
        }
        std::cout << "\n";
    }

    SetConsoleTextAttribute(h_console, 15);
}
