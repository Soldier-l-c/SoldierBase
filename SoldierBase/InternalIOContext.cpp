#include "pch.h"
#include "InternalIOContext.h"

void InternalIOContext::Init()
{
	std::call_once(int_once_flag_, [this] {InternalInit(); });
}

void InternalIOContext::UnInit()
{
    InternalUinit();
}

void InternalIOContext::AddTask(const IOTaskPtr& task)
{
    io_context_.post([this, task] 
        {
            if (task->Run())
            {
                this->AddTask(task);
            }
        });
}

void InternalIOContext::AddTask(const IOTaskFunc& func)
{
    io_context_.post(func);
}

boost::asio::io_context& InternalIOContext::IOContext()
{
    return io_context_;
}

void InternalIOContext::InternalInit()
{
    io_work_.reset(new boost::asio::io_context::work(io_context_));

    std::thread t([this]()
        {
            io_context_.run();
        }
    );

    t.detach();
}

void InternalIOContext::InternalUinit()
{
    if (io_work_ == nullptr) return;

    io_work_.reset();
    io_context_.stop();
    io_context_.reset();
}
