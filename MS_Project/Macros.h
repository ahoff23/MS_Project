#ifndef MACROS_H
#define MACROS_H

/* Uncomment if you wish to run the regular search program (no test or world/agent generation) */
//#define RUN_PROGRAM 1

/* Uncomment if automated tests should be run rather than regular program */
//#define TEST 1

/* 
* Uncomment if you want to generate worlds and agent file,
* define the number as the number of tests you wish to generate.
* Uncomment the second macro if you wish to time test the tests.
* The number chosen for the second variable (NUM_TESTS) equals the number
* of tests you want to run.
*/
#define NUM_GEN_TESTS 100
#define NUM_TESTS 100

/* Uncomment if test should stop after a set period of time (value in seconds) */
#define TIME_LIMIT 60

/* Uncomment if the size of the CLOSED and OPEN list should be displayed when they are copied */
//#define DISPLAY_LIST_SIZES 1

/* Uncomment if you wish to display the size of the PCA* search */
//#define PCA_STAR_SIZE 1

/*
* Uncomment if you wish to display the agent depth of each agent, 
* the total number of nodes in each OPEN list,
* the total number of nodes popped from the OPEN list in a given search
* and the total number of nodes added to each OPEN list in a given search
*/
//#define OPEN_LIST_DATA 1

/* Uncomment if you wish to display information about each element expanded in an A* search */
//#define A_STAR_SEARCH_DATA 1

/* Uncomment this if you wish to display conflict data */
//#define CONFLICT_DATA 1

/* Uncomment if the search should not use PCA* and use the classic CBS algorithm */
#define CBS_CLASSIC 1

/* Set the deth search limit */
#define SEARCH_DEPTH 30000

#endif