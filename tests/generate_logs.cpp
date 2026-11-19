#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>

int main(int argc, char* argv[]) {
    int totalEntries = 1000000;

    if (argc > 2) {
        std::cerr << "Usage: generate_logs [number_of_entries]\n";
        return 1;
    }

    if (argc == 2) {
        try {
            std::size_t charactersRead = 0;
            totalEntries = std::stoi(argv[1], &charactersRead);
            if (charactersRead != std::string(argv[1]).size() ||
                totalEntries <= 0) {
                throw std::invalid_argument("Invalid entry count");
            }
        } catch (const std::exception&) {
            std::cerr << "Please provide a positive number of entries.\n";
            return 1;
        }
    }

    std::ofstream file("data/large_server.log");
    if (!file.is_open()) {
        std::cerr << "Could not create data/large_server.log.\n";
        return 1;
    }

    const std::string levels[] = {
        "INFO", "WARNING", "ERROR", "CRITICAL"
    };

    for (int i = 0; i < totalEntries; ++i) {
        file << "2026-11-13 10:30:15 "
             << levels[i % 4]
             << " Server event number " << i + 1 << '\n';
    }

    if (!file) {
        std::cerr << "An error occurred while writing the log file.\n";
        return 1;
    }

    std::cout << "Generated " << totalEntries
              << " log entries in data/large_server.log.\n";
    return 0;
}
