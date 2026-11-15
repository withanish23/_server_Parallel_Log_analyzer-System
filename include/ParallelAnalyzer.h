
#ifndef PARALLELANALYZER_H
#define PARALLELANALYZER_H

#include <vector>
#include "LogRecord.h"
#include "SequentialAnalyzer.h"

class ParallelAnalyzer {
public:
    AnalysisResult analyze(const std::vector<LogRecord>& logs);
};

#endif
