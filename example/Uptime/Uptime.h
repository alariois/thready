/*
 * Uptime.h
 *
 * Created: 18.04.2017 13:37.10
 *  Author: alari
 */

#ifndef UPTIME_H_
#define UPTIME_H_

#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

// Windows implementation using GetTickCount64
static inline uint64_t uptimeGetMs(void) {
  return GetTickCount64(); // Returns milliseconds since system boot
}

#else
#include <time.h>

// Unix-like implementation using clock_gettime
static inline uint64_t uptimeGetMs(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts); // Use CLOCK_MONOTONIC for uptime
  return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000); // Convert to milliseconds
}
#endif

#endif /* UPTIME_H_ */
