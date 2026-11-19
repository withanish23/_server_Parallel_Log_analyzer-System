#include "SequentialAnalyzer.h"

AnalysisResult SequentialAnalyzer::analyze(
    const std::vector<LogRecord>& logs) {

    AnalysisResult result;

    for (const LogRecord& log : logs) {
        ++result.totalEntries;

        if (log.level == "INFO") {
            ++result.infoCount;
        } else if (log.level == "WARNING") {
            ++result.warningCount;
        } else if (log.level == "ERROR") {
            ++result.errorCount;
        } else if (log.level == "CRITICAL") {
            ++result.criticalCount;
        } else {
            ++result.unknownCount;
        }
    }

    return result;
}
