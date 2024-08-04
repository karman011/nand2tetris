#include "HackAssembler.hpp"

int main(int argc, char* argv[]) {
    std::string inputFileName;
    std::string outputFileName;

    int opt;
    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                inputFileName = optarg;
                break;
            case 'o':
                outputFileName = optarg;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -i inputfile -o outputfile" << std::endl;
                return 1;
        }
    }

    if (inputFileName.empty() || outputFileName.empty()) {
        std::cerr << "Usage: " << argv[0] << " -i inputfile -o outputfile" << std::endl;
        return 1;
    }

    try {
        assemble(inputFileName, outputFileName);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
