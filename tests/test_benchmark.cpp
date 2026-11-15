
#include <iostream>
#include <chrono>
#include <omp.h>

#include "LogReader.h"
#include "SequentialAnalyzer.h"
#include "ParallelAnalyzer.h"

int main() {

    const int repetitions = 10;

    omp_set_num_threads(4);

    // Read dataset before benchmarking
    LogReader reader;
    std::vector<LogRecord> logs =
        reader.readLogs("data/large_server.log");

    if (logs.empty()) {
        std::cerr << "No log entries loaded.\n";
        return 1;
    }

    SequentialAnalyzer sequential;
    ParallelAnalyzer parallel;

    double sequentialTotal = 0;
    double parallelTotal = 0;

    AnalysisResult seqResult;
    AnalysisResult parResult;

    // Warm-up runs
    sequential.analyze(logs);
    parallel.analyze(logs);

    // Sequential benchmark
    for (int i = 0; i < repetitions; i++) {

        auto start = std::chrono::high_resolution_clock::now();

        seqResult = sequential.analyze(logs);

        auto end = std::chrono::high_resolution_clock::now();

        sequentialTotal +=
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();
    }

    // Parallel benchmark
    for (int i = 0; i < repetitions; i++) {

        auto start = std::chrono::high_resolution_clock::now();

        parResult = parallel.analyze(logs);

        auto end = std::chrono::high_resolution_clock::now();

        parallelTotal +=
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();
    }

    double sequentialAverage =
        sequentialTotal / repetitions;

    double parallelAverage =
        parallelTotal / repetitions;

    // Verify results
    bool correct =
        seqResult.totalEntries == parResult.totalEntries &&
        seqResult.infoCount == parResult.infoCount &&
        seqResult.warningCount == parResult.warningCount &&
        seqResult.errorCount == parResult.errorCount &&
        seqResult.criticalCount == parResult.criticalCount &&
        seqResult.unknownCount == parResult.unknownCount;

    std::cout << "\n===== PERFORMANCE BENCHMARK =====\n\n";

    std::cout << "Log entries: " << logs.size() << "\n";
    std::cout << "Threads: " << omp_get_max_threads() << "\n";
    std::cout << "Repetitions: " << repetitions << "\n\n";

    std::cout << "Sequential average: "
              << sequentialAverage << " ms\n";

    std::cout << "Parallel average: "
              << parallelAverage << " ms\n";

    if (parallelAverage > 0) {

        double speedup =
            sequentialAverage / parallelAverage;

        double efficiency = speedup / 4.0 * 100.0;

        std::cout << "Speedup: "
                  << speedup << "x\n";

        std::cout << "Parallel efficiency: "
                  << efficiency << "%\n";
    }

    std::cout << "\nResults verification: "
              << (correct ? "PASSED" : "FAILED")
              << "\n";

    return correct ? 0 : 1;
}
