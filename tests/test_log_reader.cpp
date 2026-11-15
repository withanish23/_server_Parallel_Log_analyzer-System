
#include <iostream>
#include "LogReader.h"

int main() {

    LogReader reader;

    std::vector<LogRecord> logs = reader.readLogs("data/server.log");

    std::cout << "Total log entries: " << logs.size() << "\n\n";

    for (const LogRecord& log : logs) {

        std::cout << "Timestamp: " << log.timestamp << "\n";
        std::cout << "Level: " << log.level << "\n";
        std::cout << "Message: " << log.message << "\n";

        std::cout << "-----------------------------\n";
    }

    return 0;
}
