
#ifndef SEQUENTIALANALYZER_H
#define SEQUENTIALANALYZER_H

#include <vector>
#include "LogRecord.h"

struct AnalysisResult {
    int totalEntries = 0;
    int infoCount = 0;
    int warningCount = 0;
    int errorCount = 0;
    int criticalCount = 0;
    int unknownCount = 0;
};

class SequentialAnalyzer {
public:
    AnalysisResult analyze(const std::vector<LogRecord>& logs);
};

#endif
