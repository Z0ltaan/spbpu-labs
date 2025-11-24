#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#  define DEBUG_TEST 1
#  include<time.h>
static clock_t debug_tmp_clock;
static char* debug_env_var = 0;
#else
#  define DEBUG_TEST 0
#endif

#define DEBUG_INIT \
  do { \
    debug_env_var = getenv("DEBUG"); \
  } while(0)

#define DEBUG_MSG(fmt, ...) \
  do { \
    if (debug_env_var) { \
      fprintf(stderr, fmt, __VA_ARGS__); \
    } \
  } while(0)
#define DEBUG_DONE \
  do { \
    if (debug_env_var) { \
      fprintf(stderr, "...done\n"); \
    } \
  } while(0)
#define DEBUG_TIMED_MSG(fmt, ...) \
  do { \
    if (debug_env_var) { \
      fprintf(stderr, fmt, __VA_ARGS__); \
      debug_tmp_clock = clock(); \
    } \
  } while(0)
#define DEBUG_TIMED_DONE \
  do { \
    if (debug_env_var) { \
      long double time_spent = \
        (long double)(clock() - debug_tmp_clock)/CLOCKS_PER_SEC; \
      fprintf(stderr, "...done: %Lf secs\n", time_spent); \
    } \
  } while(0)

#endif
