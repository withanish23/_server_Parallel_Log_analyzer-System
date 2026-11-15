
#include "LogReader.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::vector<LogRecord> LogReader::readLogs(const std::string& filename) {

    std::vector<LogRecord> logs;

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return logs;
    }

    std::string line;

    while (std::getline(file, line)) {

        std::istringstream iss(line);

        LogRecord record;
        std::string date;
        std::string time;

        if (iss >> date >> time >> record.level) {

            record.timestamp = date + " " + time;

            std::getline(iss, record.message);

            // Remove leading spaces from the message
            size_t start = record.message.find_first_not_of(" \t");

            if (start != std::string::npos) {
                record.message = record.message.substr(start);
            }

            logs.push_back(record);
        }
    }

    file.close();

    return logs;
}
