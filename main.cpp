/*
------------
Introduction
------------

This is a tutorial, implementation, and demonstration showing how to
        generate random numbers in C++.

To be random means to be unpredictable. Therefore, a random number
        should be an unpredictable number. However, since computers are among
the most predictable devices ever invented by humans, it will be
difficult to get a computer to create a truly random number. For this
reason, we will satisfy ourselves with getting the computer to create
so-called "pseudo random numbers", where "pseudo" essentially means
"not genuine", or to put it more bluntly, "fake". Most programming
languages have a Pseudo Random Number Generator (PRNG) for this
purpose. If we knew enough about exactly how the PRNG worked, we could
        predict the pseudo random numbers it generates. However, in practice,
        we don't take the time and expense to discover this, making the pseudo
random numbers good enough for our purposes. (If we needed random
numbers for a more serious purpose, for example to generate passwords
or implement encryption, we'd need a more sophisticated process than
the one described below.)

Although a PRNG can create only pseudo random numbers, we will
continue to use the phrase "random numbers" in the discussion below.


---------------------
Random Numbers in C++
---------------------

In C++, the PRNG can be accessed via the rand() function, which is
found in the cstdlib library. When rand() is called, it returns one
random number. It does this by performing a series of complicated and
obscure mathematical operations. We can think of rand() essentially as
a black box, like this:

+------+
| rand |---> a random number
+------+

The PRNG needs a value as an input for its mathematical operations,
called the random number generator seed:

                          +------+
                 seed --->| rand |---> a random number
                          +------+

In C++, the seed defaults to a starting value of 0.

If the seed never changed, then the random numbers that the PRNG
generated would always be the same, and therefore highly predictable,
defeating our purpose. To avoid this, the PRNG uses the last random
number generated as the next seed:

                          +------+
                 seed --->| rand |---> a random number ---+
                  ^       +------+                        |
                  |                                       |
                  +---------------------------------------+

When a C++ program is executed using the above set-up, it can generate
a series of random numbers. However, if the program is executed a
second time, it will generate the same series of random numbers, again
 defeating our purpose. Why does it do that?  Because, as noted above,
the seed defaults to a starting value of 0.

To change the default starting value of the seed, we can use another
function called srand(), which is also found in the cstdlib library.
srand() is used to set the value of the seed. (There is some
disagreement about what the "s" in srand() stands for -- some say it
means "seed", as in "seed the PRNG", others say it means "setup", as
in "setup the PRNG".) Our diagram now looks like this:

              +-------+
              | srand |
              +-------+
                  |
                  |
                  v       +------+
                 seed --->| rand |---> a random number ---+
                  ^       +------+                        |
                  |                                       |
                  +---------------------------------------+

The problem is that, in order to work properly, srand() needs to be
given its own unpredictable starting value! This gives us a
chicken-and-egg problem (as in "Which came first, the chicken or the
egg?"). srand() can help rand() create random numbers, but to do so it
needs a random number of its own, so that it can help rand() to create
random numbers, but to do so... etc., etc.

To solve this problem, we resort to using the system clock, via the
time() function, found in the ctime library. If we give the time
function an argument of 0, it will return the number of seconds that
have elapsed since the so-called Unix Epoch, which was midnight on
January 1st, 1970. (From the viewpoint of the early twenty-first
century, that's a lot of seconds!)

Now our setup looks like this:

              +------+
              | time |
              +------+
                  |
                  |
                  v
              +-------+
              | srand |
              +-------+
                  |
                  |
 v       +------+
                 seed --->| rand |---> a random number ---+
                  ^       +------+                        |
                  |                                       |
                  +---------------------------------------+

We're almost there. Since the value that the time() function returns
is very large, its data type is long int. This is somewhat unfortunate
for our purposes, since the srand() function is expecting to be given
a value whose data type is int. To fix this, we can use a so-called
type-cast, via the built-in int() function, to turn the long int value
into an int value. Our final setup looks like this:

              +------+
              | time |
              +------+
                  |
                  |
                  v
               +-----+
               | int |
               +-----+
                  |
                  |
                  v
              +-------+
              | srand |
              +-------+
                  |
                  |
                  v       +------+
                 seed --->| rand |---> a random number ---+
                  ^       +------+                        |
                  |                                       |
                  +---------------------------------------+

Putting this all together gives us the following statement in C++ to
set the seed:

        srand(int(time(0)));

This statement should be executed only once per execution of a C++
program, and also should be executed before any calls to rand().

Once the seed has been set, we can now call rand() as many times as we
want, for example:

        cout << "Here is one random number: " << rand() << endl;


------------------------------------------------
Getting a Random Number Within a Specified Range
 ------------------------------------------------

The C++ rand() function returns a random number within the range of 0
to RAND_MAX, inclusive. RAND_MAX is a constant defined in the cstdlib
library, and is dependant on the specific compiler, operating system,
and base hardware being used. Using GNU C++ on District Unix, RAND_MAX
is 2,147,483,647 -- just a little over 2 billion.

This might be suitable in some circumstances, but often we want
something a bit smaller, perhaps within a specified range. We can make
this happen by applying just a little bit of arithmetic.

Say we want to generate a random number between a given low value and
a given high value, inclusive. First, let's create some int variables:

        int low;
        int high;
        int random1;
        int range;
        int random2;
        int random3;
        int random;

Let's set low and high to be a couple of example values:

        low = 20;
        high = 30;

You can change low and high to be any whole numbers that you like.
Just be careful to make sure that low <= high. If high were ever to
become the same as (low - 1), we'd get a runtime error (division by
zero) below.

Now call rand(), as follows:

        random1 = rand();

The random1 variable will have value between 0 and 2,147,483,647.

Now we want to use that random number to create another random value,
but between low and high. Notice that there are exactly (high - low +
1) values in this range. (If you don't believe me, count on your
fingers how many values there are between 20 and 30.  You'll find that
there are 11 of them, not 10.) This gives us:

        range = (high - low + 1);

In our example, where low is 20 and high is 30, range will be 11.

We now use the mod operator to find the remainder of dividing random1
by the range, as follows:

random2  = random1 % range;

Due to the properties of mod, random2 will be a value between 0 and
(high - low). In our example, where low is 20 and high is 30, random2
will be between 0 and 10.

To get a value between low and high, simply add the low back in:

        random3 = random2 + low;

In our example, where low is 20 and high is 30, random3 will be a
value between 20 and 30. Viola!

We can do all of the above in a single statement:

        random = (rand() % (high - low + 1) + low);


--------------------------------------------------------------------
Implementation and Demonstration of Generating Random Numbers in C++
--------------------------------------------------------------------

C++ code demonstrating these random number concepts can be found
below.


----------------
Review Questions
----------------

1. What is a random number?

2. Can computers easily generate true random numbers?

3. What does PRNG stand for?

4. What does "pseudo" mean?

5. What function in C++ is used to generate a random number?

6. What is the purpose of the random number generator seed?

7. What function in C++ is used to set this seed?

8. How can the time() and int() functions help srand() to set this
seed?

9. When was the Unix Epoch?

10. What does the Unix Epoch have to do with the time() function?

11. True or False: it is preferable to call the rand() function before
calling the srand() function.

12. True or False: it is preferable to call the the srand() function
several times during a single execution of a C++ program.

13. What is RAND_MAX?

14. What is the value of RAND_MAX for GNU C++ on District Unix?

15. What is the arithmetic formula for creating a random number within
a specified range?

*/


//////////////////////////////////////////////////////////////////////


#include <iostream>

using namespace std;


// access the rand() and srand() functions

#include <cstdlib>


// access the time() function

#include <ctime>


// constant used to control how many sample random numbers are
// generated

const int REPETITIONS = 10;


// prototype for a function to generate a random number within a
// specified range

int rand_range(int low, int high);

//////////////////////////////////////////////////////////////////////


int main() {

    int random;     // used to hold a sample random number
    int low;        // used to hold a sample low value
    int high;       // used to hold a sample high value

    // set the random number generator seed by using the number of
    // seconds since the Unix Epoch
    srand(int(time(0)));

    // tell the user that several random numbers will be displayed
    cout << endl
         << "Displaying "
         << REPETITIONS
         << " random numbers between 0 and "
         << RAND_MAX
         << endl;

    // loop REPETITIONS times
    for (int i = 1; i <= REPETITIONS; i++) {

        // get a random number
        random = rand();

        //display that random number
        cout << random << endl;
    }

    // set sample low and high values for creating a random number
    // within a specified range
    low = 200;
    high = 300;

    // tell the user that several ranged random numbers will be
    // displayed
    cout << endl
         << "Displaying "
         << REPETITIONS
         << " random numbers between "
         << low
         << " and "
         << high
         << endl;

    // loop REPETITIONS times
    for (int i = 1; i <= REPETITIONS; i++) {

        // get a random number between low and high
        random = rand_range(low, high);

        // display that random number
        cout << random << endl;
    }

}


//////////////////////////////////////////////////////////////////////


int rand_range(int low, int high) {

    // PRE:  low and high are valid integers with low <= high, and
    //       srand() has been called
    //
    // POST: a random number between low and high (inclusive) has
    //       been returned

    // get a random number, and use it to create another random
    // value that is between low and high
    return (rand() % (high - low + 1)) + low;
}
