# Mastermind-Game-Solver
The Game has two variants
- Not Allowing repetitions of colors.
- Allowing repetitions of colors.
## Input
Program randomly initializes the four color codes i.e. it is itself the code master.
## Output
Program solves the code using togasat SAT solver based on the feedback in form of black and white pegs after every iterations. It also shows the code guessed at each iteration.
## How to use togasat as a library

At first, include the header (or do copy & paste):

``` c++
  #include "togasat.hpp"
```

Make a solver object.
``` c++
   togasat::Solver solver;
```
