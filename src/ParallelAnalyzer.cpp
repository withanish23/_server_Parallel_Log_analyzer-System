#include "ParallelAnalyzer.h"

#include <omp.h>

AnalysisResult ParallelAnalyzer::analyze(
    const std::vector<LogRecord>& logs) {

    int info = 0;
    int warning = 0;
    int error = 0;
    int critical = 0;
    int unknown = 0;

    #pragma omp parallel for reduction(+:info, warning, error, critical, unknown)
    for (int i = 0; i < static_cast<int>(logs.size()); ++i) {
        const std::string& level = logs[static_cast<std::size_t>(i)].level;

        if (level == "INFO") {
            ++info;
        } else if (level == "WARNING") {
            ++warning;
        } else if (level == "ERROR") {
            ++error;
        } else if (level == "CRITICAL") {
            ++critical;
        } else {
            ++unknown;
        }
    }

    AnalysisResult result;
    result.totalEntries = static_cast<int>(logs.size());
    result.infoCount = info;
    result.warningCount = warning;
    result.errorCount = error;
    result.criticalCount = critical;
    result.unknownCount = unknown;

    return result;
}
