
#include <iostream>
#include "LogReader.h"
#include "ParallelAnalyzer.h"
#include <omp.h>

int main() {

    // Set number of threads
    omp_set_num_threads(4);

    // Read server logs
    LogReader reader;
    std::vector<LogRecord> logs = reader.readLogs("data/server.log");

    // Perform parallel analysis
    ParallelAnalyzer analyzer;
    AnalysisResult result = analyzer.analyze(logs);

    // Display results
    std::cout << "===== PARALLEL LOG ANALYSIS =====\n\n";

    std::cout << "Number of threads: " << omp_get_max_threads() << "\n";
    std::cout << "Total Entries: " << result.totalEntries << "\n";
    std::cout << "INFO: " << result.infoCount << "\n";
    std::cout << "WARNING: " << result.warningCount << "\n";
    std::cout << "ERROR: " << result.errorCount << "\n";
    std::cout << "CRITICAL: " << result.criticalCount << "\n";
    std::cout << "UNKNOWN: " << result.unknownCount << "\n";

    return 0;
}
