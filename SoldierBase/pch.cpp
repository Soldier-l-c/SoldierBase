﻿// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
#ifndef USEGLOG
console_logger::ILoggerPtr g_logger;
#endif
// 当使用预编译的头时，需要使用此源文件，编译才能成功。
