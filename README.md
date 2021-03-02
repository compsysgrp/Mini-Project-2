## Running the code
For the purposes of testing the code it is reccomended that you use the following line to compile the main.c file:

```gcc main.c –o main -lz```

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
The read function is relatively straightforward, it simply opens a file, determines where it should read from, creates a buffer, then reads and stores the string into an array called "read_arr" which will be used for compression.
### Compression:
The compress function uses functions and data structions from the standard zlib library (http://zlib.net). The function gets the information from the "read_arr", uses the deflate functions to compress, and than stores the string into an array called "comp_arr". This array will be used in the write function.
### Writer:
The write function is the most straightforward function of all. It takes a string from the compression array, "comp_arr", then appends it to the end of the output file.
There were some alternatives to allow writing out of order, but they did not increase efficiency given that we could only write one thing at a time. 
This causes a very minor bottleneck at the end of the sequence, but nothing substantial as the reading and compression are greatly sped up by multithreading.


## Test Results
CPU: Intel Core i5 4690 @ 3.50GHz
| File Size | 3 Threads | 5 Threads | 10 Threads | 25 Threads | 50 Threads | 100 Threads | 200 Threads | 500 Threads |
| --------- |-----------|-----------|------------|------------|------------|-------------|-------------|-------------|
| 1 MB      | 4.797s    | 7.741s    | 2.3s       |  2.667s    | 1.347s     | -           | -           | -           |
| 50 MB     | 113.409s  | 8.667s    | 5.239s     |  5.35s     | 5.961s     | -           | -           | -           |
| 100 MB    | 20.186s	  | 14.284s	  | 8.178s	   |  8.615s    |	8.656s     | -           | -           | -           |
| 500 MB    | 192.226s	| 139.683s	| 132.557s	 |  103.737s	| 94.563s	   |  78.285s	   |  67.662s	   |  68.207s    |
| 1 GB      | -         | -         | -          | -          | 859.927s   | -           | -           | -           |
| 1.5 GB    | -         | -         | -          | -          | 2818.228s  | -           | -           | -           |

*higher thread counts for smaller file sizes were not used as there was little significant decrease in runtime

*thread counts for bigger files were not used due to hardware limitations

![image](https://user-images.githubusercontent.com/71051737/109592595-830d9a80-7add-11eb-9b77-edb1c293d2b2.png)

![image](https://user-images.githubusercontent.com/71051737/109592609-87d24e80-7add-11eb-87a7-dc4f5fd64eaf.png)

![image](https://user-images.githubusercontent.com/71051737/109592619-8bfe6c00-7add-11eb-9371-35ac425fbb69.png)

## Conclusions
As expected, multithreading greatly increased the efficiency of our code. The bigger the file size, the more impactful having a higher thread count was (assuming that the thread count is within hardware limitations). Our runtimes were a bit slow due to the hardware limitations, but still showed great improvement by increasing number of threads depending on the size of the file. There is a "maximum" number of useful threads based on runtime that also varies with different devices and file sizes so this is something that testers may be interested in exploring.
