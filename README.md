FLAG IDENTIFIER
CSS 487 Computer Vision
Fall 2021
Prof. Clark Olson








Produced by:
Andy Tran		bakein@uw.edu
Joseph Lan		jelan@uw.edu
Kevin Xu		kxu11@uw.edu


Table of Contents
Table of Contents	2
What we are Trying to Accomplish	3
Background	3
Project Objectives	3
Key Project Features	3
What did we Accomplish	4
Accomplishment Overview	4
Project Algorithm Run Through	4
Most Common Color (MCC) Filter	4
MCC Ratio Filter	5
Test 2 Program Run Through	6
Edge Counting	7
Quadrant Filtering	7
Short Cutting	8
What Were Our Results	9
Result Data	9
Table 1: Input to Output	9
Color Bucket Information	10
Color Ratio Information, Edge Ratio Information	11
Lessons Learned	12
Design	12
Filters	12
Future Plans	13
How to Run the Program	14
ZIP folder contents	14
Running the Program with Default Script (run.bat)	14
How the Program Runs	14
BAT File Options (run.bat)	15
Changing BAT file to run tests	15
Addition BAT files	15
Schedule	16


What we are Trying to Accomplish
Background
The idea for this project came from one of our team members Andy, who plays online trivia games. These games often ask image recognition questions such as asking what country or state a flag is from. When asked these questions, the game only allows at most 10 seconds to quickly get an answer.

Some contestants game the system by trying to google the image quickly or run through some programs which can identify flags if given an input image. Some applications exist that try to create a flag recognition program, but the programs tested have many issues. They either take too long, or they give too long a list of possible flags from an input image.

We created this project as an answer to this situation. A high speed flag recognizer that can accurately and quickly present a flag given an input flag image.
 
Project Objectives
The objective of the project is to create a flag identifier application. Unlike traditional reverse image search tools, our program leans towards fast image processing and accurate returns. Our program should be able to take in any flag image as an input, and with a combination of filtering methods, determine the most likely candidate result.

Some methods that we considered were color matching with similar colors and how to implement that. Pass that we could use color percentages to determine how dominant certain colors are. Pattern matching and template matching was an initial tool to determine what types of shapes are in a flag. However we avoided these ideas to avoiding n^2 * m^2 time for every combination of images. We instead chose to work with modular filters that could work by slowly narrowing down possible flags in stages.
 
Key Project Features
Identification of U.S. state flags when given as an input image.
Uses sequential filters with color information and edge information.
Majority of filtering occurs in O(1) time
Following filters have longer O(n^2) times, but with much fewer flags.
Returns a resultant image and name of the state flag found.
Creates a database of flag image metadata using histogram information.
What did we Accomplish
Accomplishment Overview
The program can take in a rectangular U.S. state flag and recognize it against a bank of images to search against.
The program uses color information, ratio information, edge information, and image quadrant calculations to determine the flag.
The program would take x number of input test flags, provided that the number of flags is given, and the names of the test images that correlate with the test images in the directory.
See more in "How to Run the Program"
The program is currently built for U.S. state flags only. More filters need to be added to all world flags. For the purpose of the program, flags are provided in a directory for detection. Variations of flags can be detected (for slightly different versions of the same flag) if more flag data is input into the program first (needs more image metadata).

Project Algorithm Run Through
Most Common Color (MCC) Filter
To run through what the program does step by step, the program starts by showing the input test image.

Figure 1: Test Image

 
The first filter runs the image through is our MCC (most common color) filter which finds the most common color using histogram buckets and RBG values. The image MCC metadata is compared to database image metadata using a map structure with acceptable error built in. The result after this step is a list of flags with similar most common colors.

The program finds the MCC as RBG {7, 7, 7} which correlates to a majority white image. This means the bucket in the histogram (3-dimensional with 8 buckets) with the highest count was {7, 7, 7}. If we were to stop here, the following image shows all the flags whose most common bucket was also {7, 7, 7}. It also shows all flags in our acceptable MCC bucket error of +/- 1. This means all flags within {{6, 6, 6}, {6, 6, 7}, {6, 7, 6}, {7, 6, 6}, {7, 6, 7}, {7, 7, 6}, {7, 7, 7}} are added to the possible list.
 
Figure 2: Possible Flags after MCC filter

 
MCC Ratio Filter
The next filter we run is the color ratio filter. First we calculate what ratio of MCC pixels in the input image end up in bucket {7, 7, 7}, then we calculate that same ratio for each of the 11 remaining possible flags. The ratios of pixels in MCC bucket to total pixels are compared between the input image and each image in the index. After removing all images that fall outside our +/- 0.6% margin of error, the only flag that remains was Texas.
 
Figure 3: Possible Flags after second filtering (Color ratio)

 
For this example, having the flag of Texas as an input image would correctly return Texas as an output only after applying the first two filters. However, this was not the case for all state flags.
 
Test 2 Program Run Through
Given a different image test, we input this second test flag.
 
Figure 4: Second Test Flag (Vermont)

 
The following image shows the 25 possible flags returned after the MCC filter.
 
Figure 5: Second Test Post MCC Filter

 

After applying MCC Ratios Filter we end up with only 3 flags.
 
Figure 6: Second Test Post MCC Ratio Filter

 
Edge Counting
This is where we apply our edge counting filter. Once we resize the images and compare the standardized edge pixel count between our input image and the remaining shown above, we are left with one remaining flag after filtering out those outside our +/- 0.6% margin of error.
 
Figure 7: Possible flag after Edge Counting

 
Quadrant Filtering
Along with the previous filters, we also implemented a fourth filter. This is our quadrant filter that uses the previous modular filters to perform filtering with a subset of the test image pixels against any remaining flags. If any further filtering is required after the canny edge detection filter, we run the algorithm again with the upper left quadrant of the input test image and remaining images.

Shown below in Figure 8 are images that are being analyzed when the initial test image matches multiple flags with very similar MCC, color ratio, and edge counts. Figure 9 shows the steps the program took to get to that point.

Figure 8: Flag Test Quadrants


Figure 9: Example of Quadrant Filtering with Nebraska

 
Once the quadrant image is obtained, we run the modular filters created before for most common color, ratio, and edge detection to close in on the closest flag.
 
Short Cutting
If, at any time, a filter results in one or less flags possibly remaining, calculation stops to save computational time. The result flag is returned and displayed with the state name as well as the name is displayed on the console.
 


What Were Our Results
Result Data
We tested our results using 50 test input images of the U.S. state flags against the 50 template images from the state flags. We were able to get the program to identify each state flag presented.

Table 1: Input to Output
Test Image
Program Output


Test Image
Program Output


Test Image
Program Output
alaska
alaska


louisiana
louisiana


ohio
ohio
alabama
alabama


massachusetts
massachusetts


oklahoma
oklahoma
arkansas
arkansas


maryland
maryland


oregon
oregon
arizona
arizona


maine
maine


pennsylvania
pennsylvania
california
california


michigan
michigan


rhode_island
rhode_island
colorado
colorado


minnesota
minnesota


south_carolina
south_carolina
connecticut
connecticut


missouri
missouri


south_dakota
south_dakota
delaware
delaware


mississippi
mississippi


tennessee
tennessee
florida
florida


montana
montana


texas
texas
georgia
georgia


north_carolina
north_carolina


utah
utah
hawaii
hawaii


north_dakota
north_dakota


virginia
virginia
iowa
iowa


nebraska
nebraska


vermont
vermont
idaho
idaho


new_hampshire
new_hampshire


washington
washington
illinois
illinois


new_jersey
new_jersey


wisconsin
wisconsin
indiana
indiana


new_mexico
new_mexico


west_virginia
west_virginia
kansas
kansas


nevada
nevada


wyoming
wyoming
kentucky
kentucky


new_york
new_york









Color Bucket Information
To store image metadata, we use a number of maps to store information. The most important of these is the color bucket map structure shown below in Figure 2.

Table 2: Color Buckets Per Flag
States within the same MCC color bucket are labeled by color. When searching for the closest flags matched by MCC, adjacent flags in 3D are considered to account for the margin of error.


To store this information, we created a 3-dimensional map to store the names of flags and connect them to the RBG histogram information as keys in R, B, G buckets.

When an image is taken in as an input to the program, a similar color histogram is created using the same process. We get the bucket containing the highest count which represents the most common color.

Color Ratio Information, Edge Ratio Information
Image color ratio is stored at the same time as the MCC color is found within an object ColorBucket. This facilitates storage of the field as metadata instead of requiring execution time calculations. Color ratio uses an acceptable error or +/- 6%.

If color ratio is not enough, canny edge detection is applied to the image and possible images to count edges. Edge counts are then standardized and applied using an acceptable error of +/- 6%.

Table 3: MCC Ratio and Edge Ratios
MCC color ratios shown for each flag. Edge ratios shown for flags that require canny edge filtering.

 
Lessons Learned
Design
Our initial idea of template matching was too slow to work. Comparing every flag in the database on execution time took too long to get results.
Our ideas of shape recognition also fell into a similar trap for algorithmic time. We had to decide on algorithms that relied on stored data (metadata).
While we could have done the 2nd most common colors and further, we thought using only colors would provide inadequate filtering. We decided to go with different filter types. For further accuracy and filtering, we could add a second most common color.
 
Filters
Using any filtering method requires some level of acceptable error. We initially tried to get super accurate filtering and cut down the most flags at each step, but found it was more important to go from broad filtering to more specific filtering. We opened up our acceptable errors to allow more flags.
Image processing and filtering requires a lot of standardization to handle images of different sizes. OpenCV helps with this a lot with its resizing, however problems sometimes occur with the interpolation method used. This is another reason we need acceptable margins for error.
 
Stretch Goals and Expectations
We had high expectations initially for our algorithm and although our filters would work, we found that image filtering and processing was much more difficult than we anticipated. Margins of errors and slightly different colorations between different images make image searching fairly difficult.
 

Future Plans
Create data persistence for all flag metadata to eliminate the build time that we currently have.
Migrate data for mobile application for camera
Requires finding a flag in an image
Using flag images that are imperfect (not rectangular, off-color)
Expand flag detection to real world flags (able to detect flags that are not ideal images)
Better UI for the user (Getting user to input directly into the program with snippet tools or similar)
How to Run the Program
ZIP folder contents
Our ZIP folder contains the following files and directories:
run.bat			// Script to run program, with default options
runall.bat			// Script to run program through all 50 flags, if desired.
driver.cpp			// main file, houses main()
CommonColorFinder.h	// Required object header
CommonColorFinder.cpp	// Object definition
ColorBucket.h		// Required object header
ColorBucket.cpp		// Object definition
flags/				// Directory containing files used to populate metadata
					//	and files used for testing

All the files should be placed in the code directory.

Running the Program with Default Script (run.bat)
You can run a version of the program with the default parameters by running the run.bat file provided.

The default options are that the program takes in 5 arguments, each argument corresponding to a name of an image in the flag/ folder.

How the Program Runs
Execute BAT file
Program will initially build the "search" images from 50 state flags in flags/ directory. (In future planning, this data should be persisted and the program cuts out build time)
The program will create a window called "test" which shows the test image.
The program waits for a user key press to continue (waitkey).
The program will run through the filters and find what the input flag image is.
Program displays the found image in a window with the name of the state which was found.
Filter information / steps and the output state name is also printed into the console.
Program waits for the user key press to continue (waitkey).
Program runs steps 3-5 for remaining tests.

Note: For the default script, image 2 is meant to be a blank purple picture to show what happens in the event that no similar flag is found. This occurs in the case that an image match has NO MCC closest color matches within acceptable error. If an image has at least one MCC color bucket match, it will attempt to make a guess at a flag from the database even if the test flag is gibberish.

BAT File Options (run.bat)
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

Changing BAT file to run tests
If you want to run tests separately you can edit the bat file by changing the arguments to suit your desired test.

Change argv[1] to the number of test files you want to test.
e.g. Testing 50 flags means this would be the number 50
Change argv[2 - n] to the file names for your test images. These names must not include spaces and must be jpg. Do not include extensions.
e.g. test1, test2, test3, test4, vt, ak, wa, ca
Run the bat file

Addition BAT files
The file "runall.bat" is provided which can be used to run through all 50 flags, if desired.


Schedule
Week 1 (Week of 11/14)
Form team. Propose a project to me. Review necessary OpenCV code. (Full understanding is not necessary, yet.)
Week 2 (Week of 11/21)
Finalize desired objectives. First pass implementation coded (resulting in an end-to-end system that can be tested, but might not meet final objectives, yet). Preliminary comparison of new methods with previous techniques.
Week 3 (Week of 11/28)
Second iteration that extends methods to (or past) objectives. Variations are tested to improve performance. Test scripts/configurations finalized and run to generate (near) final results.
Week 4 (Week of 12/4)
Final code modifications and tests. Prepare write-up and presentation. Submit results on Canvas.




Sunday
Monday
Tuesday
Wednesday
Thursday
Friday
Saturday
11/7-
11/13


11/7


11/8


11/9


11/10


11/11


11/12


11/13


11/14-
11/20
Project Planning. Start researching opencv methods. Decide on a project plan.
11/14
Decide on what methods to use
11/15
Decide on what methods to use
11/16
Decide on what methods to use
11/17
Decide on what methods to use
11/8
Meeting on project methods
11/19
Meeting on project methods
11/20
Start Implementation
11/21-
11/27
Finalize desired objectives. First pass implementation coded. Tested.
11/21
Implement & Refactor
11/22
Implement & Refactor
11/23
Implement & Refactor
11/24
Implement & Refactor
11/25
Implement & Refactor
11/26
Implement & Refactor
11/27
WORKING CODE DUE (MINIMUM REQUIREMENTS). INITIAL TESTING COMPLETE.
11/28-
12/4
Second pass iteration, extend methods or past objectives. Test performance. Write test scripts.
11/28
-Fix multiple images / bucket
-Fix acceptable errors
-Add filters?
-Debug image errors


11/29
-Fix multiple images / bucket
-Fix acceptable errors
-Add filters?
-Debug image errors
11/30
-Fix multiple images / bucket
-Fix acceptable errors
-Add filters?
-Debug image errors
11/31
-Fix multiple images / bucket
-Fix acceptable error-Fix multiple images / bucket


12/1
-Fix multiple images / bucket
-Fix acceptable errors
-Add filters?
-Debug image errors
12/2
-Fix multiple images / bucket
-Fix acceptable errors
-Add filters?
-Debug image errors
12/3
SECOND PASS REQUIREMENTS (IMPROVED PERFORMANCE OR SCALE). FULL TESTED.
12/5-
12/11
Presentation Week

Final modifications and test. Present.
12/4
Debug + Accuracy checks
12/5
Debug + Accuracy checks
12/6
Debug + Accuracy checks
12/7
Final editing
Code Cleanup
Finish Project Writeup
12/8
Final editing
Code Cleanup
Finish Project Writeup
12/9
Final editing
Code Cleanup
Finish Project Writeup
12/10
PROJECT DUE


