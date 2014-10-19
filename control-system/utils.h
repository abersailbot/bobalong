/*
	utils.h

	Provides some general utility functions
 */

#ifndef UTILS_H
#define UTILS_H

#define DEBUG_LEVEL_MINOR		2
#define DEBUG_LEVEL_IMPORTANT	1
#define DEBUG_LEVEL_CRITICAL	0

/**********************************************************************************
 * Prints a debug message across the Arduino serial port. The message is only
 * printed if that debug level has been set to that level or lower.
 * 
 * @param msg         		Message to print
 * @param debug_level 		The debug level of the message
 * 
 *********************************************************************************/
void debug_print(char* msg, int debug_level);

/**********************************************************************************
* Returns the heading difference between two headings in a 0-359 degree range.
* 
*********************************************************************************/
int GetHeadingDiff(int heading0, int heading1);

#endif