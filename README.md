## Running the code
For the purposes of testing the code it is reccomended that you use the following line to compile the main.c file:

```put thingy here```

The code has been simplified as much as possible in order to make it more readable and to make testing easier. 
Once you run that file with the command:

```./main```

You will then be prompted to provide the number of threads you would like to run the code with. Once the code is finished running, the compressed output file will be called 1GB_output.txt

By default, the code runs with the input "1GB.txt" a 1 gigabyte input file of random text. 
If changing the input is desired, right below the #include statements you can change the input_file name as well as the output_file name

## Data Structure
The data structure used is simply arrays of char* (strings). The input file is parsed into 4000 character strings, with a remainder string of less than 4000 at the end if necessary.
There are some data type modifications for the purpose of compression, and there is also use of a buffer when reading and writing. 

## Functions:
### Main:
Our main function handles the multithreading as well as iterators to determine what stage of reading, compressing, and writing we are at. This is important 
in the case of multithreading because order matters especially for writing, or trying to compress/write before the read/compress is complete.
These iterators as well as thread tracking are essentially all that happens in the main file seperate from basic declarations, timing, and function calls.
### Reader:
The read function is relatively straightforward, it simply opens a file, determines where it should read from, creates a buffer, 
then reads and stores the string into an array called "read_arr" which will be used for compression.
### Compression:

### Writer:
The write function is the most straightforward function of all. It takes a string from the compression array, "comp_arr", then appends it to the end of the output file.
There were some alternatives to allow writing out of order, but they did not increase efficiency given that we could only write one thing at a time. 
This causes a very minor bottleneck at the end of the sequence, but nothing substantial as the reading and compression are greatly sped up by multithreading.


## Test Results
PUT PROCESSOR AND TEST RESULTS HERE.


## Conclusions
As expected, multithreading greatly increased the efficiency of our code. Our runtimes were a bit slow due to the hardware limitations,
but still showed great improvement by increasing number of threads depending on the size of the file. There is a "maximum" number of useful threads based on runtime that also varies
with different devices and file sizes so this is something that testers may be interested in exploring.
