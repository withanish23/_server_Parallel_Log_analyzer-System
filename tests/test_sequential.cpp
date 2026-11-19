#include <iostream>
#include <vector>

#include "LogReader.h"
#include "SequentialAnalyzer.h"

int main() {
    LogReader reader;
    const std::vector<LogRecord> logs = reader.readLogs("data/server.log");

    if (logs.empty()) {
        std::cerr << "No sample log entries loaded.\n";
        return 1;
    }

    SequentialAnalyzer analyzer;
    const AnalysisResult result = analyzer.analyze(logs);
    const std::vector<LogRecord> unknownLog = {
        {"2026-11-13 10:00:00", "DEBUG", "Unrecognised severity"}
    };
    const AnalysisResult unknownResult = analyzer.analyze(unknownLog);
    const AnalysisResult emptyResult = analyzer.analyze({});

    const bool correct =
        result.totalEntries == 10 &&
        result.infoCount == 4 &&
        result.warningCount == 2 &&
        result.errorCount == 3 &&
        result.criticalCount == 1 &&
        result.unknownCount == 0 &&
        unknownResult.totalEntries == 1 &&
        unknownResult.unknownCount == 1 &&
        emptyResult.totalEntries == 0;

    std::cout << "===== SEQUENTIAL LOG ANALYSIS =====\n\n";
    std::cout << "Total entries: " << result.totalEntries << '\n';
    std::cout << "INFO: " << result.infoCount << '\n';
    std::cout << "WARNING: " << result.warningCount << '\n';
    std::cout << "ERROR: " << result.errorCount << '\n';
    std::cout << "CRITICAL: " << result.criticalCount << '\n';
    std::cout << "UNKNOWN: " << result.unknownCount << '\n';
    std::cout << "Unknown-level and empty-input checks: "
              << (correct ? "PASSED" : "FAILED") << '\n';
    std::cout << "Sequential test: "
              << (correct ? "PASSED" : "FAILED") << '\n';

    return correct ? 0 : 1;
}
