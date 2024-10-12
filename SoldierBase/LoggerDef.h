#pragma once

#define USE_INTERNAL_LOG

#ifndef USE_INTERNAL_LOG
#ifdef RELEASE_MD
#define USEGLOG
#else
#define USE_INTERNAL_LOG
#endif
#endif

#ifdef USEGLOG
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GLOG_EXPORT
#define GLOG_NO_EXPORT
#define GLOG_DEPRECATED
#include <glog/logging.h>

#ifdef WIN32
#ifdef _WIN64
#pragma comment(lib, "glog-x64.lib")
#else
#pragma comment(lib, "glog-x86.lib")
#endif
#endif
#else
extern ILoggerPtr g_logger;
#define LOG ILOG
#endif
