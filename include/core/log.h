#ifndef VC3_CORE_LOG_H
#define VC3_CORE_LOG_H

typedef enum LogLevel {
  LOG_LEVEL_ERROR = 0,
  LOG_LEVEL_WARN,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG
} LogLevel;

void log_set_level(LogLevel level);
void log_message(LogLevel level, const char *fmt, ...);

#endif
