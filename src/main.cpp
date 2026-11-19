#include <iostream>
#include <string>
#include <vector>
#include <omp.h>

#include "LogReader.h"
#include "SequentialAnalyzer.h"
#include "ParallelAnalyzer.h"

namespace {
void printResult(const std::string& title, const AnalysisResult& result) {
    std::cout << title << '\n';
    std::cout << "Total entries: " << result.totalEntries << '\n';
    std::cout << "INFO: " << result.infoCount << '\n';
    std::cout << "WARNING: " << result.warningCount << '\n';
    std::cout << "ERROR: " << result.errorCount << '\n';
    std::cout << "CRITICAL: " << result.criticalCount << '\n';
    std::cout << "UNKNOWN: " << result.unknownCount << "\n\n";
}

bool resultsMatch(const AnalysisResult& first, const AnalysisResult& second) {
    return first.totalEntries == second.totalEntries &&
           first.infoCount == second.infoCount &&
           first.warningCount == second.warningCount &&
           first.errorCount == second.errorCount &&
           first.criticalCount == second.criticalCount &&
           first.unknownCount == second.unknownCount;
}
}

int main(int argc, char* argv[]) {
    const std::string filename =
        (argc > 1) ? argv[1] : "data/server.log";

    LogReader reader;
    const std::vector<LogRecord> logs = reader.readLogs(filename);

    if (logs.empty()) {
        std::cerr << "No log entries were loaded from: " << filename << '\n';
        std::cerr << "Check the file path and log file contents.\n";
        return 1;
    }

    SequentialAnalyzer sequentialAnalyzer;
    ParallelAnalyzer parallelAnalyzer;

    const AnalysisResult sequentialResult =
        sequentialAnalyzer.analyze(logs);

    omp_set_num_threads(4);
    const AnalysisResult parallelResult =
        parallelAnalyzer.analyze(logs);

    std::cout << "SERVER LOG ANALYSIS\n";
    std::cout << "Input file: " << filename << "\n\n";

    printResult("SEQUENTIAL ANALYSIS", sequentialResult);
    printResult("PARALLEL ANALYSIS", parallelResult);

    const bool match = resultsMatch(sequentialResult, parallelResult);
    std::cout << "Sequential and parallel results: "
              << (match ? "MATCH" : "DO NOT MATCH") << '\n';

    return match ? 0 : 1;
}
