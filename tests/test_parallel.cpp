#include <iostream>
#include <vector>
#include <omp.h>

#include "LogReader.h"
#include "ParallelAnalyzer.h"
#include "SequentialAnalyzer.h"

int main() {
    omp_set_num_threads(4);

    LogReader reader;
    const std::vector<LogRecord> logs = reader.readLogs("data/server.log");

    if (logs.empty()) {
        std::cerr << "No sample log entries loaded.\n";
        return 1;
    }

    ParallelAnalyzer parallelAnalyzer;
    SequentialAnalyzer sequentialAnalyzer;

    const AnalysisResult parallel = parallelAnalyzer.analyze(logs);
    const AnalysisResult sequential = sequentialAnalyzer.analyze(logs);

    const std::vector<LogRecord> unknownLog = {
        {"2026-11-13 10:00:00", "DEBUG", "Unrecognised severity"}
    };
    const AnalysisResult unknownResult =
        parallelAnalyzer.analyze(unknownLog);
    const AnalysisResult emptyResult = parallelAnalyzer.analyze({});

    const bool correct =
        parallel.totalEntries == sequential.totalEntries &&
        parallel.infoCount == sequential.infoCount &&
        parallel.warningCount == sequential.warningCount &&
        parallel.errorCount == sequential.errorCount &&
        parallel.criticalCount == sequential.criticalCount &&
        parallel.unknownCount == sequential.unknownCount &&
        parallel.totalEntries ==
            parallel.infoCount + parallel.warningCount +
            parallel.errorCount + parallel.criticalCount +
            parallel.unknownCount &&
        unknownResult.totalEntries == 1 &&
        unknownResult.unknownCount == 1 &&
        emptyResult.totalEntries == 0;

    std::cout << "===== PARALLEL LOG ANALYSIS =====\n\n";
    std::cout << "Maximum configured threads: "
              << omp_get_max_threads() << '\n';
    std::cout << "Total entries: " << parallel.totalEntries << '\n';
    std::cout << "INFO: " << parallel.infoCount << '\n';
    std::cout << "WARNING: " << parallel.warningCount << '\n';
    std::cout << "ERROR: " << parallel.errorCount << '\n';
    std::cout << "CRITICAL: " << parallel.criticalCount << '\n';
    std::cout << "UNKNOWN: " << parallel.unknownCount << '\n';
    std::cout << "Unknown-level and empty-input checks: "
              << (correct ? "PASSED" : "FAILED") << '\n';
    std::cout << "Sequential/parallel comparison: "
              << (correct ? "PASSED" : "FAILED") << '\n';

    return correct ? 0 : 1;
}
