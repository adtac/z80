#ifndef _LOGGING_HPP_
#define _LOGGING_HPP_

#include <stdio.h>
#include <string.h>

#define LEVEL_ERROR 0
#define LEVEL_WARN  1
#define LEVEL_INFO  2
#define LEVEL_DEBUG 3

#define LOG_LEVEL LEVEL_DEBUG

#define basename(x) (strrchr(x, '/') ? strrchr(x, '/') + 1 : x)

#if defined(LOG_LEVEL) && LOG_LEVEL > 0
#define ERROR(fmt, ...)                                             \
  do {                                                              \
    fprintf(stderr, "[error] %s:%d: %s(): " fmt "\n",               \
            basename(__FILE__), __LINE__, __func__, ##__VA_ARGS__); \
  } while (0)
#else
#define WARN(fmt, args...)
#endif

#if defined(LOG_LEVEL) && LOG_LEVEL > 0
#define WARN(fmt, ...)                                              \
  do {                                                              \
    fprintf(stdout, "[warning] %s:%d: %s(): " fmt "\n",             \
            basename(__FILE__), __LINE__, __func__, ##__VA_ARGS__); \
  } while (0)
#else
#define WARN(fmt, args...)
#endif

#if defined(LOG_LEVEL) && LOG_LEVEL > 1
#define INFO(fmt, ...)                                              \
  do {                                                              \
    fprintf(stdout, "[info] %s:%d: %s(): " fmt "\n",                \
            basename(__FILE__), __LINE__, __func__, ##__VA_ARGS__); \
  } while (0)
#else
#define INFO(fmt, args...)
#endif

#if defined(LOG_LEVEL) && LOG_LEVEL > 2
#define DEBUG(fmt, ...)                                             \
  do {                                                              \
    fprintf(stdout, "[debug] %s:%d: %s(): " fmt "\n",               \
            basename(__FILE__), __LINE__, __func__, ##__VA_ARGS__); \
  } while (0)
#else
#define DEBUG(fmt, args...)
#endif

#endif
