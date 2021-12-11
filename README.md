# Project Objectives
The objective of the project is to create a flag identifier application.

Unlike traditional reverse image search tools, our program leans towards fast image processing and accurate returns.

Our program should be able to take in any flag image as an input, and with a combination of filtering methods, determine the most likely candidate result.

Some methods that we considered were color matching with similar colors and how to implement that. Pass that we could use color percentages to determine how dominant certain colors are.

Pattern matching and template matching was an initial tool to determine what types of shapes are in a flag. However we avoided these ideas to avoiding n^2 * m^2 time for every combination of images. We instead chose to work with modular filters that could work by slowly narrowing down possible flags in stages.
 
# Key Project Features
- Identification of U.S.state flags when given as an input image.

- Uses sequential filters with color information and edge information.

- Majority of filtering occurs in O(1) time

- Following filters have longer O(n^2) times, but with much fewer flags.

- Returns a resultant image and name of the state flag found.

- Creates a database of flag image metadata using histogram information.

# What did we Accomplish
## Accomplishment Overview
- The program can take in a rectangular U.S. state flag and recognize it against a bank of images to search against.
- The program uses color information, ratio information, edge information, and image quadrant calculations to determine the flag.
- The program would take x number of input test flags, provided that the number of flags is given, and the names of the test images that correlate with the test images in the directory.
- See more in "How to Run the Program"
- The program is currently built for U.S. state flags only.

More filters need to be added to all world flags. For the purpose of the program, flags are provided in a directory for detection. Variations of flags can be detected (for slightly different versions of the same flag) if more flag data is input into the program first (needs more image metadata).

# How the Program Runs
Requires opencv
Requires property sheets
Requires x64 compiling

# Execute BAT file
Program will initially build the "search" images from 50 state flags in flags/ directory. (In future planning, this data should be persisted and the program cuts out build time)
The program will create a window called "test" which shows the test image.
The program waits for a user key press to continue (waitkey).
The program will run through the filters and find what the input flag image is.
Program displays the found image in a window with the name of the state which was found.
Filter information / steps and the output state name is also printed into the console.
Program waits for the user key press to continue (waitkey).
Program runs steps 3-5 for remaining tests.

Note: For the default script, image 2 is meant to be a blank purple picture to show what happens in the event that no similar flag is found. This occurs in the case that an image match has NO MCC closest color matches within acceptable error. If an image has at least one MCC color bucket match, it will attempt to make a guess at a flag from the database even if the test flag is gibberish.

# BAT File Options (run.bat)
The provided .bat file contains the script to run the code. The default script runs with five of the provided test files.

The arguments in order to the program into argv[] are:

[0] : Flag-Identifier_OPENCV.exe		// Name of the program
[1] : 5						// # of images to run this execution
[2] : test1					// Name of first test image
[3] : test2					// Name of second test image
[4] : test3					// Name of third test image
[5] : test4					// Name of fourth test image
[6] : test5					// Name of fifth test image

Note: all test images must be in “.jpg” format. (e.g. test1.jpg, test2.jpg, anytest.jpg)

# Changing BAT file to run tests
If you want to run tests separately you can edit the bat file by changing the arguments to suit your desired test.

Change argv[1] to the number of test files you want to test.
e.g. Testing 50 flags means this would be the number 50
Change argv[2 - n] to the file names for your test images. These names must not include spaces and must be jpg. Do not include extensions.
e.g. test1, test2, test3, test4, vt, ak, wa, ca
Run the bat file

# Addition BAT files
The file "runall.bat" is provided which can be used to run through all 50 flags, if desired.
