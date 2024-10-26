Partial Function is a project where you can read a binary file with one of the 5 types of partial functions. 
The user chooses points and the program will print the result of the function. 

All binary files should contain :
   - unsigned 16 bits number N
   - unsigned 16 bits number T, where T represents the type of the partial function:
The 5 types of Partial functions:

1) Function 0
   -following 2N 32bit numbers, where the first N represent the points and the second N are their custom results
   3 0      // N = 3; T = 0
   1 2 4
   5 8 -2   //f(1) = 5; f(2) = 8; f(4) = -2; f(3) = UNDEFINED

2) Function 1
   -following N 32bit numbers, where all of them exclude them from the domain of the function //Undefined
   (For the rest of the points the function is the Indentity f(x) = x)
    5 1          // N = 5; T = 1
   -1 3 2 6 8    // f(-1)  = UNDEFINED; f(0) = 0; f(1) = 1; f(2) = UNDEFINED

3) Function 2 //Defined at every point
   -following N 32bit numbers, where  if x is one of the points given the function returns 1, else the function returns 0
    2 2     // N = 2; T = 2
   -1 0     // f(-1)  = 1; f(0) = 1; f(1) = 0; f(2) = 0

4) Function Max
   -following N names of binary files with partial functions
   f(x) returns number if x is defined in all subfunctions in max;
   -the number f(x) is the maximum of the results of his subfunctions
   
5) Function Min
   -like Function Max, but returns the minimum of the rsults of his subfunctions
