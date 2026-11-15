
#ifndef LOGRECORD_H
#define LOGRECORD_H

#include <string>

struct LogRecord {
    std::string timestamp;
    std::string level;
    std::string message;
};

#endif
