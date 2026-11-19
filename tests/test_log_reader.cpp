#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include "LogReader.h"

int main() {
    LogReader reader;
    const std::vector<LogRecord> logs = reader.readLogs("data/server.log");

    bool passed = logs.size() == 10;

    if (passed) {
        passed = logs[0].timestamp == "2026-11-13 10:30:15" &&
                 logs[0].level == "INFO" &&
                 logs[0].message == "Server started successfully" &&
                 logs[8].level == "CRITICAL";
    }

    // Check that incomplete lines are skipped and unknown levels are retained.
    const char* temporaryFile = "build/test_reader_input.tmp";
    {
        std::ofstream output(temporaryFile);
        output << "incomplete line\n";
        output << "2026-11-13 10:40:00 NOTICE Maintenance scheduled\n";
        output << "2026-11-13 10:41:00 ERROR\n";
    }

    const std::vector<LogRecord> edgeCases =
        reader.readLogs(temporaryFile);
    std::remove(temporaryFile);

    passed = passed && edgeCases.size() == 2 &&
             edgeCases[0].level == "NOTICE" &&
             edgeCases[0].message == "Maintenance scheduled" &&
             edgeCases[1].level == "ERROR" &&
             edgeCases[1].message.empty();

    std::cout << "Log reader test: "
              << (passed ? "PASSED" : "FAILED") << '\n';
    std::cout << "Sample entries loaded: " << logs.size() << '\n';

    return passed ? 0 : 1;
}
