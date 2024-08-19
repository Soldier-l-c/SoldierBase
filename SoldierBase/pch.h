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
#include <shared_mutex>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <vector>
#include <functional>

#include <helper/util_time.h>
#include <helper/util_string.h>
#include <helper/util_path.h>
#include <helper/single_instance.h>
#include <helper/uninheritable.h>

#include <curl/curl_helper.h>
#include <thread/thread_pool.h>

#include <public/result_code.h>
#include <interface/smart_base.h>
#include <interface/smart_object.h>
#include <interface/smart_ptr.h>
#include <interface/object_creater.h>
#include <base/IDataBundle.h>
#include <base/DataBundleHelper.h>
#include <base/INetHelper.h>
#include <base/NetHelper.h>
#include <base/ITimer.h>
#include <base/ISoldierBase.h>
#include <base/IDemo.h>
#include <base/IPipe.h>
#include <base/PipeHelper.h>
#include <logger/ILogger.h>

#include <boost/asio.hpp>
#include <boost/process/pipe.hpp>
using boost_stream_handle = boost::asio::windows::stream_handle;
using stream_handle_ptr = std::shared_ptr<boost_stream_handle>;

#include "LoggerDef.h"

using ReadLock = std::shared_lock<std::shared_mutex>;
using WriteLock = std::unique_lock<std::shared_mutex>;

using INTERNAL_HANDLE = void*;

#ifdef WIN32
#ifndef errno
#define errno ::GetLastError()
#endif
#endif

#endif //PCH_H
