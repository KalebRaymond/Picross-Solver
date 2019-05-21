class Square
{
    public:
        //0 = unsolved space, +/- 1 = solved empty space, +/- 2 = solved filled space
        //Positive states represent squares solved by first and second passes
        //Negative states are used by backtracking method to differentiate from positive solved states
        int state = 0;
};
