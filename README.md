# Logic-Optimization-by-Quine-McCluskey-Algorithm
#### Description
- This is a project from *CS313000,Introduction to Computer-Aided Design of Integrated Circuits* course at NTHU.
- This is a two-level logic optimization program. Optimized logic by Quine-McCluskey approach and Petric's method. Minimized the literal count.
- Input: sum of product term (SOP)
- Output: optimized product terms

#### Compile and Execute
```
    $ make
    $ ./pa1 <.in file> <.out file>
```

#### Testcase format
- Sample input:
```
    6         // total number of variables = 6
    7         // total number of product terms = 7
    101110    // product term = ab’cdef’ 
    -10101    // product term = bc’de’f 
    10-01-
    --0111
    10-111
    100-1-
    1-0111
```
- Sample output:
```
    11         // literal count = 11
    3          // total number of product terms = 3 
    10--1-     // optimized product term = ab’e -101-1
    --0111
```
