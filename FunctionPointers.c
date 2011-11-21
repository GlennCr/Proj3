#include <stdio.h>

// F is a function that takes an int parameter and
// returns an int value. This is just a declaration.
int F(int);


// now we are defining F by providing a body for it
int F(int x)
{
	return 4*x;
}

int h0(int x)
{
   return 2*x;
}

int h1(int x)
{

    return 3*x;
}


// what is f ?
// before attempting to understand this declaration,
// read the example below first
int  (*f(int (*y[])(int), int x))(int)
{
    return y[x];
}

main()
{

     // In our box-circle diagram notation
     //              ______
     //       F ----|      |
     //             |      |
     //             |______|
     //
     // so the memory associated with F has the code for 
     // F. &F is the address of the memory associated with 
     // F which is the same as the value for F in C 
        if (F == &F)
           printf("F is equal to &F\n");
        else
           printf("F is not equal to &F");

     // also F is equal to *F. The reason for this
     // is not as clear. I believe it is notation convenience.
        if (F == *F)
           printf("F is equal to *F\n");
        else
           printf("F is not equal to *F");
 
     //  note that we cannot say F = h0
     // the fact that F == &F == *F is only for 
     // functions that are not explicitly declared
     // as pointers. For functions that are declared
     // as pointers, it is not the same. See below.

     // *y is a function that takes an int and returns 
     // an int. y is a pointer to a function that takes
     // an int and returns an int. We call y as follows
     // i = (*y)(3). Notice how the call has the same 
     // form as the declaration.
     int (*y)(int);

     // h0 is a function that takes an int parameter and 
     // returns an int
     // assign h0 to y and the invoke y
     y = h0;
     printf("%d \n", (*y)(3)); 
     printf("%d \n", y(3)); 

     //  y is not the same as &y 
        if (y == &y)
           printf("y is equal to &y\n");
        else
           printf("y is not equal to &y\n");

     // but y is the the same as *y
        if (y == *y)
           printf("y is equal to *y\n");
        else
           printf("y is not equal to *y\n");


     // k is an array of function pointers
     // again note how k is called (*k[0])(3) this is of
     // the form f(x) where *k[0] is in place of f. So,
     // *k[0] is a function that has one int parameter and 
     // returns int. k[0] is a function pointer, so
     // k is an array of function pointers
     int (*k[3])(int);

     k[0] = h0;
     k[1] = h1;

     printf("%d \n", ((*f)(k,0))(3));
     printf("%d \n", ((*f)(k,1))(3));
       
    // now I can explain the declaration above for f
    // it is of the form int (X)(int). This means
    // that X is a function that takes an int parameter
    // and returns an int value.
    // Let us look at X. It is  *f(int (*y[])(int), int x)
    // This is of the form Y(a,b), so *f is a function pointer
    // of two parameter and when we apply it to its parameters
    // we get the function just described above (X). In other words,
    // the declaration int X(int) and int (*f(y,x))(int) have
    // the same form so *f(y,x) is the same as X which is a function from
    // int to int. 
    // now lut us look at the parameters y and x of *f. 
    // x is simply an int parameter
    // y is more complicated. we have the declaration
    // int (*y[])(int). This is of the form int (Y)(int) so
    // *y[] is a function from int to int.
    // y[] is a pointer to a function from int to int
    // y is an array of function pointers from int to int  
 
}

