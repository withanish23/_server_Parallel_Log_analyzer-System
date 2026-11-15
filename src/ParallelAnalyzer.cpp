
#include "ParallelAnalyzer.h"
#include <omp.h>

AnalysisResult ParallelAnalyzer::analyze(
    const std::vector<LogRecord>& logs) {

    AnalysisResult result;

    int info = 0;
    int warning = 0;
    int error = 0;
    int critical = 0;
    int unknown = 0;

    #pragma omp parallel for reduction(+:info, warning, error, critical, unknown)
    for (int i = 0; i < static_cast<int>(logs.size()); i++) {

        if (logs[i].level == "INFO") {
            info++;
        }
        else if (logs[i].level == "WARNING") {
            warning++;
        }
        else if (logs[i].level == "ERROR") {
            error++;
        }
        else if (logs[i].level == "CRITICAL") {
            critical++;
        }
        else {
            unknown++;
        }
    }

    result.totalEntries = static_cast<int>(logs.size());
    result.infoCount = info;
    result.warningCount = warning;
    result.errorCount = error;
    result.criticalCount = critical;
    result.unknownCount = unknown;

    return result;
}
