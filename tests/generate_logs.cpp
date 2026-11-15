
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {

    int totalEntries = 100000;

    if (argc > 1) {
        totalEntries = std::stoi(argv[1]);
    }

    std::ofstream file("data/large_server.log");

    if (!file.is_open()) {
        std::cerr << "Could not create log file.\n";
        return 1;
    }

    std::string levels[] = {
        "INFO", "WARNING", "ERROR", "CRITICAL"
    };

    for (int i = 0; i < totalEntries; i++) {

        int levelIndex = i % 4;

        file << "2026-11-13 10:30:15 "
             << levels[levelIndex]
             << " Server event number "
             << i + 1 << "\n";
    }

    file.close();

    std::cout << "Successfully generated "
              << totalEntries
              << " log entries.\n";

    return 0;
}
