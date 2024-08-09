// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"
#include <iostream>

#include <helper/util_time.h>
#include <helper/util_string.h>
#include <helper/util_path.h>
#include <helper/single_instance.h>
#include <helper/uninheritable.h>

#include <curl/curl_helper.h>
#include <thread/thread_pool.h>
#include <logger/console_logger.h>

#include <public/result_code.h>
#include <interface/smart_base.h>
#include <interface/smart_object.h>
#include <interface/smart_ptr.h>
#include <interface/object_creater.h>
#include <base/IDataBundle.h>
#include <base/DataBundleHelper.h>
#include <base/INetHelper.h>
#include <base/NetHelper.h>

#define USEGLOG
#ifdef USEGLOG
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GLOG_EXPORT
#define GLOG_NO_EXPORT
#define GLOG_DEPRECATED
#include <glog/logging.h>
#else 
#include <logger/console_logger.h>
extern console_logger::ILoggerPtr g_logger;
#define LOG LOG_CMD
#endif
#include <logger/ILogger.h>

#endif //PCH_H
