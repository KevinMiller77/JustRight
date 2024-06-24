#include "common.test.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

int randomizeFileLinesOutputtingNew(std::string filepath, std::string output) {
    // Open the file, randomize the lines, and write it to a temp file called <filename>.tmp.<extension>
    std::fstream file(filepath, std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filepath << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    // No access to random_shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(lines.begin(), lines.end(), g);

    file.open(output, std::ios::out);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << output << std::endl;
        return 1;
    }

    for (auto &line : lines) {
        file << line << std::endl;
    }
    file.close();

    return 0;
}

int getFileLines(std::string filepath, std::vector<std::string>& out) {
    std::fstream file(filepath, std::ios::in);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filepath << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        out.push_back(line);
    }

    file.close();
    return 0;
}

int getFileLinesWithoutCommentsOrWhitespace(std::string filepath, std::vector<std::string>& out) {
    getFileLines(filepath, out);
    // Remove comment lines (//), empty lines, and all whitespace
    out.erase(std::remove_if(out.begin(), out.end(), [](std::string &line) {
        return line.empty() || line.find("//") != std::string::npos || std::all_of(line.begin(), line.end(), isspace);
    }), out.end());

    // Remove whitespace from indivual lines
    return 0;
}