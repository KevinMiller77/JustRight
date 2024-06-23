#ifndef __COMMON_TEST_H__
#define __COMMON_TEST_H__

#include <string>

/**
 * @brief Create a new file randomizing all lines in the input file
 * 
 * @param filepath 
 * @return int - error code
 */
int randomizeFileLinesOutputtingNew(std::string filepath, std::string output);

/**
 * @brief Get the File Lines object
 * 
 * @param filepath 
 * @param out 
 * @return int - error code
 */
int getFileLines(std::string filepath, std::vector<std::string>& out);

/**
 * @brief Get the File Lines Without Comments Or Whitespace object
 * 
 * @param filepath 
 * @return int 
 */
int getFileLinesWithoutCommentsOrWhitespace(std::string filepath, std::vector<std::string>& out);

#endif // __COMMON_TEST_H__