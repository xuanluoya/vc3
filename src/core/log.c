#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core/log.h"

static LogLevel current_level = LOG_LEVEL_INFO;

static void get_localtime(char *time_buf, size_t buf_size) {
  time_t now;
  time(&now);

  struct tm *tm_info = localtime(&now);
  if (!tm_info) {
    strcpy(time_buf, "unknown-time");
    time_buf[buf_size - 1] = '\0';
  } else {
    strftime(time_buf, buf_size, "%Y-%m-%d %H:%M:%S", tm_info);
  }
}

void log_set_level(LogLevel level) { current_level = level; }

void log_message(LogLevel level, const char *fmt, ...) {
  if (level > current_level) {
    return;
  }

  const char *level_str = NULL;
  switch (level) {
  case LOG_LEVEL_ERROR:
    level_str = "ERROR";
    break;
  case LOG_LEVEL_WARN:
    level_str = "WARN";
    break;
  case LOG_LEVEL_INFO:
    level_str = "INFO";
    break;
  case LOG_LEVEL_DEBUG:
    level_str = "DEBUG";
    break;
  default:
    level_str = "LOG";
    break;
  }

  char time_buf[26];
  get_localtime(time_buf, 26);

  fprintf(stderr, "[%s] [%s] ", time_buf, level_str);

  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fprintf(stderr, "\n");
  fflush(stderr);
}
