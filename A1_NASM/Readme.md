# Assignment 1

    Name - Anindya Vijayvargeeya
    Roll - 200101015


- A Makefile has been provided to easily run commands along with codes. Running 
  ```
    $ make
  ```
  will build both programs (run this in the same directory as the code files and the makefile).

- I'm using 64-bit mode in this assignment.

# Set A : Q11
> To print the list of characters along with their frequencies in a document


Build using
```
$ make clean
$ make A1A_200101015
```
After building, run using 
```
$ ./A1A_200101015
```
Use file redirection to provide input if it is to supplied from a file (I've created a sample input file input_doc.txt)
```
$ ./A1A_200101015 < input_doc.txt
```
Note - the input should not exceed 1000 characters.

# Set B : Q6
> To print the transpose of an N*N matrix.


Build using 
```
$ make clean
$ make A1B_200101015
```
After building, run using
```
$ ./A1B_200101015
```
To provide input either type the matrix - first value of N, then N*N numbers for the values of each entry, or provide input using redirection (I've created a sample input file matrix.txt)
```
$ ./A1B_200101015 < matrix.txt
```

Note - value of N should not exceed 10.

