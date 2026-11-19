#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>
#include <omp.h>

#include "LogReader.h"
#include "SequentialAnalyzer.h"
#include "ParallelAnalyzer.h"

namespace {
bool resultsMatch(const AnalysisResult& first, const AnalysisResult& second) {
    return first.totalEntries == second.totalEntries &&
           first.infoCount == second.infoCount &&
           first.warningCount == second.warningCount &&
           first.errorCount == second.errorCount &&
           first.criticalCount == second.criticalCount &&
           first.unknownCount == second.unknownCount;
}
}

int main() {
    const int repetitions = 10;
    const int numberOfThreads = 4;
    omp_set_num_threads(numberOfThreads);

    // File reading is outside the timed sections.
    LogReader reader;
    const std::vector<LogRecord> logs =
        reader.readLogs("data/large_server.log");

    if (logs.empty()) {
        std::cerr << "No benchmark log entries loaded.\n";
        return 1;
    }

    SequentialAnalyzer sequential;
    ParallelAnalyzer parallel;

    // Warm-up both implementations before collecting measurements.
    sequential.analyze(logs);
    parallel.analyze(logs);

    double sequentialTotal = 0.0;
    double parallelTotal = 0.0;
    AnalysisResult sequentialResult;
    AnalysisResult parallelResult;

    for (int i = 0; i < repetitions; ++i) {
        auto start = std::chrono::steady_clock::now();
        sequentialResult = sequential.analyze(logs);
        auto end = std::chrono::steady_clock::now();

        sequentialTotal +=
            std::chrono::duration<double, std::milli>(end - start).count();

        start = std::chrono::steady_clock::now();
        parallelResult = parallel.analyze(logs);
        end = std::chrono::steady_clock::now();

        parallelTotal +=
            std::chrono::duration<double, std::milli>(end - start).count();
    }

    const double sequentialAverage = sequentialTotal / repetitions;
    const double parallelAverage = parallelTotal / repetitions;
    const bool correct = resultsMatch(sequentialResult, parallelResult);

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "===== PERFORMANCE BENCHMARK =====\n\n";
    std::cout << "Log entries: " << logs.size() << '\n';
    std::cout << "Threads: " << numberOfThreads << '\n';
    std::cout << "Repetitions: " << repetitions << '\n';
    std::cout << "Warm-up runs: 1 per implementation\n";
    std::cout << "File reading: excluded from measurement\n\n";
    std::cout << "Sequential average: " << sequentialAverage << " ms\n";
    std::cout << "Parallel average: " << parallelAverage << " ms\n";

    if (parallelAverage > 0.0) {
        const double speedup = sequentialAverage / parallelAverage;
        const double efficiency = speedup / numberOfThreads * 100.0;
        std::cout << "Speedup: " << speedup << "x\n";
        std::cout << "Parallel efficiency: " << efficiency << "%\n";
    }

    std::cout << "\nResult comparison: "
              << (correct ? "PASSED" : "FAILED") << '\n';

    return correct ? 0 : 1;
}
