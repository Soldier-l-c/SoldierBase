#pragma once
#define USE_INTERNAL_LOG

#ifndef USE_INTERNAL_LOG
#define USEGLOG
#endif

#ifdef USEGLOG
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GLOG_EXPORT
#define GLOG_NO_EXPORT
#define GLOG_DEPRECATED
#include <glog/logging.h>
#else
extern ILoggerPtr g_logger;
#define LOG ILOG
#endif
