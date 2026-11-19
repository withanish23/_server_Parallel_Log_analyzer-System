#include "LogReader.h"

#include <fstream>
#include <sstream>

std::vector<LogRecord> LogReader::readLogs(const std::string& filename) {
    std::vector<LogRecord> logs;
    std::ifstream file(filename);

    if (!file.is_open()) {
        return logs;
    }

    std::string line;

    while (std::getline(file, line)) {
        // Remove the carriage return from Windows CRLF line endings.
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        std::istringstream input(line);
        LogRecord record;
        std::string date;
        std::string time;

        // Expected format: YYYY-MM-DD HH:MM:SS LEVEL message
        if (!(input >> date >> time >> record.level)) {
            continue;
        }

        record.timestamp = date + " " + time;
        std::getline(input, record.message);

        const std::size_t firstCharacter =
            record.message.find_first_not_of(" \t");

        if (firstCharacter == std::string::npos) {
            record.message.clear();
        } else {
            record.message.erase(0, firstCharacter);
        }

        logs.push_back(record);
    }

    return logs;
}
