
#include <iostream>
#include "LogReader.h"
#include "SequentialAnalyzer.h"

int main() {

    // Read server logs
    LogReader reader;
    std::vector<LogRecord> logs = reader.readLogs("data/server.log");

    // Perform sequential analysis
    SequentialAnalyzer analyzer;
    AnalysisResult result = analyzer.analyze(logs);

    // Display results
    std::cout << "===== SEQUENTIAL LOG ANALYSIS =====\n\n";

    std::cout << "Total Entries: " << result.totalEntries << "\n";
    std::cout << "INFO: " << result.infoCount << "\n";
    std::cout << "WARNING: " << result.warningCount << "\n";
    std::cout << "ERROR: " << result.errorCount << "\n";
    std::cout << "CRITICAL: " << result.criticalCount << "\n";
    std::cout << "UNKNOWN: " << result.unknownCount << "\n";

    return 0;
}
