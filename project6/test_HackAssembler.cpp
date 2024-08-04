#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "HackAssembler.hpp"

TEST(AssemblerTest, AssembleTest) {
    // Create a temporary input file
    std::ofstream inputFile("input.asm");
    inputFile << "@R0  \n"
            << "D=M \n"
            << "@END\n"
            << "D;JLE\n"
            << "@n\n"
            << "M=D\n"
            << "@SCREEN\n"
            << "D=A\n"
            << "@addr\n"
            << "M=D\n"
            << "(LOOP)\n"
            << "@addr\n"
            << "A=M\n"
            << "M=-1\n"
            << "@addr\n"
            << "D=M\n"
            << "@32\n"
            << "D=D+A\n"
            << "@addr\n"
            << "M=D\n"
            << "@n\n"
            << "MD=M-1\n"
            << "@LOOP\n"
            << "D;JGT\n"
            << "(END)\n"
            << "@END\n"
            << "0;JMP\n";
    inputFile.close();

    // Call the assemble function
    assemble("input.asm", "output.hack");

    // Read the generated output file
    std::ifstream outputFile("output.hack");
    std::stringstream buffer;
    buffer << outputFile.rdbuf();
    std::string output = buffer.str();

    // Verify the expected output
std::string expectedOutput = "0000000000000000\n"
                             "1111110000010000\n"
                             "0000000000010111\n"
                             "1110001100000110\n"
                             "0000000000010000\n"
                             "1110001100001000\n"
                             "0100000000000000\n"
                             "1110110000010000\n"
                             "0000000000010001\n"
                             "1110001100001000\n"
                             "0000000000010001\n"
                             "1111110000100000\n"
                             "1110111010001000\n"
                             "0000000000010001\n"
                             "1111110000010000\n"
                             "0000000000100000\n"
                             "1110000010010000\n"
                             "0000000000010001\n"
                             "1110001100001000\n"
                             "0000000000010000\n"
                             "1111110010011000\n"
                             "0000000000001010\n"
                             "1110001100000001\n"
                             "0000000000010111\n"
                             "1110101010000111\n";
    EXPECT_EQ(output, expectedOutput);

    // Clean up the temporary files
    outputFile.close();
    std::remove("input.asm");
    std::remove("output.hack");
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}