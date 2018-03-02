# LZW

## What is this code?
The Lempel–Ziv–Welch (LZW) is a data compression algorithm.  I did my own version of the LZW compressor and decompressor. I used 12-bit fixed with codes.

## Input Data
There are three examples of my code running:
- file1.txt <-> file1.z
- file2.txt <-> file2.z
- file3.txt <-> file3.z

## Compressor
### How to execute the program
If you use g++ to execute your program, you should go to the folder where is the `compressor.cpp` file. Then, you should write the line `g++ compressor.cpp –o compressor -std=c++11`. Then, you should write `compressor name_of_file_you_want_to_compress`. For instance, `compressor /LZWInputData/file1.txt`.
If you use a program like Eclipe, Notepad, … you should add the name of the file to compress as a parameter in the running options.
In order to compress a file named `file1.txt` you should execute the program with the full filename path as an argument.

### The LzwStream class
There are :
1. Private attributes:
- a dictionary (map)
- a position (integer - indicates the position in the file)
- an OldCode (char)
2. Public attributes:
- a constructor
- a method compressedToStream, which does the compression
- a method initializeDictionary, which creates the dictionary at the beginning of the program



## Decompressor
### How to execute the program
If you use g++ to execute your program, you should just like you did before with the compressor but instead of writting `compressor`, you should write `decompressor`.

### The LzwStream class
There are :
1. Private attributes:
- a dictionary (map)
- a position (integer - the position in the file to decompress, that is to say the number of codes we have already read)
- an OldByte (unsigned char - it memorizes the last byte we read in the file)
2. Public attributes:
-	A constructor which uses the ifstream constructor
-	readCode : read the next code in the file to decompress
-	DecompressToStream: do the decompression, using the readCode method
-	initializeDictionary: initialize (or reinitialize) the dictionary when needed

### To-do list
- the code is written for a 12-bit fixed with codes. I would like to improve it and make it possible to use with any number of bits.
