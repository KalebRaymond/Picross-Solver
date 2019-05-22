#include <iostream>
#include <stdlib.h>
#include <vector>
#include <windows.h> //console text handler
#include "Square.cpp"

void print_puzzle(std::vector< std::vector<Square> > nonogram)
{
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE); //h_console is a console command handler, used to change text color in the terminal

    for(int m = 0; m < nonogram.size(); ++m)
    {
        for(int n = 0; n < nonogram[0].size(); ++n){
            switch( abs(nonogram[m][n].state))
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
            std::cout << abs(nonogram[m][n].state)<< " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    SetConsoleTextAttribute(h_console, 15);
}

//Checks if picross is potentially able to be solved in its current state
bool is_valid(std::vector< std::vector<int> > &rows, std::vector< std::vector<int> > &cols, std::vector< std::vector<Square> > &nonogram)
{
    //Checking rows
    //Looping downwards
    for(int i = 0; i < rows.size(); ++i)
    {
        int cur_black_count = 0;
        int cur_x_count = 0;
        int total_blacks = 0;

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

    //Checking cols
    //Looping left to right
    for(int i = 0; i < cols.size(); ++i)
    {
        int cur_black_count = 0;
        int cur_x_count = 0;
        int total_blacks = 0;

        for(int j = 0; j < cols[i].size(); j++)
        {
            total_blacks += cols[i][j];
        }

        //Looping downwards over squares
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

std::vector< std::vector<Square> > extend(std::vector< std::vector<int> > rows, std::vector< std::vector<int> > cols, std::vector< std::vector<Square> > nonogram)
{
    if(is_solved(rows, cols, nonogram))
    {
        return std::vector< std::vector<Square> >(); //Return empty vector
    }

    std::vector< std::vector<Square> > extension = nonogram;
    for(int i = 0; i < extension.size(); i++)
    {
        for(int j = 0; j < extension[0].size(); j++)
        {
            if(extension[i][j].state == 0)
            {
                extension[i][j].state = -1;
                return extension;
            }
        }
    }

    return std::vector< std::vector<Square> >();
}

std::vector< std::vector<Square> > next(std::vector< std::vector<Square> > nonogram)
{
    std::vector< std::vector<Square> > next_nonogram = nonogram;

    for(int i = next_nonogram.size() - 1; i >= 0; i--)
    {
        for(int j = next_nonogram[0].size() - 1; j >= 0; j--)
        {
            if(next_nonogram[i][j].state == -1)
            {
                --next_nonogram[i][j].state;
                return next_nonogram;
            }

            if(next_nonogram[i][j].state == -2)
            {
                next_nonogram[i][j].state = 0;
                return std::vector< std::vector<Square> >(); //Return empty vector
            }
        }
    }

    return std::vector< std::vector<Square> >();
}


std::vector< std::vector<Square> > solve_picross(std::vector< std::vector<int> > &rows, std::vector< std::vector<int> > &cols, std::vector< std::vector<Square> > &nonogram)
{
    print_puzzle(nonogram);

    if(!is_valid(rows, cols, nonogram))
          return std::vector< std::vector<Square> >(); //Return empty vector

    if(is_solved(rows, cols, nonogram))
          return nonogram;

    std::vector< std::vector<Square> > attempt = extend(rows, cols, nonogram);
    while(!attempt.empty())
    {
        std::vector< std::vector<Square> > solution = solve_picross(rows, cols, attempt);
        if(!solution.empty())
        {
            return solution;
        }

        attempt = next(attempt);
    }

    return std::vector< std::vector<Square> >();
}

int main()
{
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

    puzzle = solve_picross(rows, columns, puzzle);
}
