#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <regex>
#include <getopt.h>
#include <cstddef>

// 符号表
std::unordered_map<std::string, int> symbolTable = {
    {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4},
    {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5},
    {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9}, {"R10", 10},
    {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
    {"SCREEN", 16384}, {"KBD", 24576}
};

const std::unordered_map<std::string, std::string> compTable = {
    {"0", "0101010"}, {"1", "0111111"}, {"-1", "0111010"}, {"D", "0001100"},
    {"A", "0110000"}, {"M", "1110000"}, {"!D", "0001101"}, {"!A", "0110001"},
    {"!M", "1110001"}, {"-D", "0001111"}, {"-A", "0110011"}, {"-M", "1110011"},
    {"D+1", "0011111"}, {"A+1", "0110111"}, {"M+1", "1110111"}, {"D-1", "0001110"},
    {"A-1", "0110010"}, {"M-1", "1110010"}, {"D+A", "0000010"}, {"D+M", "1000010"},
    {"D-A", "0010011"}, {"D-M", "1010011"}, {"A-D", "0000111"}, {"M-D", "1000111"},
    {"D&A", "0000000"}, {"D&M", "1000000"}, {"D|A", "0010101"}, {"D|M", "1010101"}
};

const std::unordered_map<std::string, std::string> destTable = {
    {"", "000"}, {"M", "001"}, {"D", "010"}, {"MD", "011"}, {"A", "100"},
    {"AM", "101"}, {"AD", "110"}, {"AMD", "111"}
};

const std::unordered_map<std::string, std::string> jumpTable = {
    {"", "000"}, {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"}, {"JLT", "100"},
    {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}
};
int varSymbol = 0;

std::string parseAInstruction(const std::string& line) {
    std::string value = line.substr(1);
    int address;
    if (isdigit(value[0])) {
        address = std::stoi(value);
    } else {
        if (symbolTable.find(value) == symbolTable.end()) {
            symbolTable[value] = 16 + varSymbol;
            // std::cout << "Met with new variable symbol: " << value << ", Set its value to " << symbolTable[value] << std::endl;
            varSymbol++;
        }
        address = symbolTable[value];
    }
    // std::cout << "A instruction: " << value <<", Address: " << address << std::endl;
    return "0" + std::bitset<15>(address).to_string();
}

std::string parseCInstruction(const std::string& line) {
    std::string dest, comp, jump;
    std::smatch match;
    std::string pre = "";

    std::size_t semicolonPos = line.find(';');
    if (semicolonPos != std::string::npos) {
        pre = line.substr(0, semicolonPos);
        jump = line.substr(semicolonPos + 1);
    } else {
        pre = line;
        jump = "";
    }
    // std::cout << line << std::endl;
    // std::cout << "Pre: "<<pre << " Jump: "<<jump << std::endl;

    std::size_t equalsPos = pre.find('=');
    if (equalsPos != std::string::npos) {
        dest = pre.substr(0, equalsPos);
        comp = pre.substr(equalsPos + 1);
    } else {
        dest = "";
        comp = pre;
    }
    // std::cout << "Dest: "<<dest << " Comp: "<<comp << std::endl;

    std::string result = "111";

    if (compTable.find(comp) != compTable.end()) {
        result += compTable.at(comp);
    } else {
        throw std::runtime_error("Invalid comp field: " + comp);
    }

    if (destTable.find(dest) != destTable.end()) {
        result += destTable.at(dest);
    } else {
        throw std::runtime_error("Invalid dest field: " + dest);
    }
    if(jump == "JLE" ) std::cout << "JLE" << std::endl;
    if (jumpTable.find(jump) != jumpTable.end()) {
        result += jumpTable.at(jump);
    } else {
        throw std::runtime_error("Invalid jump field: " + jump);
    }

    return result;
}

void assemble(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile || !outputFile) {
        std::cerr << "Error: cannot open file" << std::endl;
        throw std::runtime_error("File open error");
    }

    std::string line;
    int lineCounter = 0;
    
    while (std::getline(inputFile >> std::ws, line)) {
        line = line.substr(0, line.find("//"));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());
        std::istringstream inputStream(line);
        if (line.empty() || line[0] == '/') {
            continue;
        }
        if (line[0] == '(') {
            // std::cout << "Met with new line symbol: " << line << ", Set its value to " << lineCounter <<std::endl;
            symbolTable[line.substr(1, line.size() - 2)] = lineCounter;
        } else {
            lineCounter++;
        }
    }

    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);

    while (std::getline(inputFile >> std::ws, line)) {
        line = line.substr(0, line.find("//"));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());
        if (line.empty() || line[0] == '/' || line[0] == '(') {
            continue;
        }
        if (line[0] == '@') {
            outputFile << parseAInstruction(line) << std::endl;
        } else {
            outputFile << parseCInstruction(line) << std::endl;
        }
    }

    inputFile.close();
    outputFile.close();
}