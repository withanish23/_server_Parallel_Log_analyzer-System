
#ifndef LOGREADER_H
#define LOGREADER_H

#include <string>
#include <vector>
#include "LogRecord.h"

class LogReader {
public:
    std::vector<LogRecord> readLogs(const std::string& filename);
};

#endif
