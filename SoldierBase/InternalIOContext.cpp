#include "pch.h"
#include "InternalIOContext.h"

void InternalIOContext::AddTask(const IOTaskPtr& task)
{
	std::unique_lock<std::mutex> lock(list_lock_);
	task_list_.push_back(task);
	cv_list_.notify_all();
}

void InternalIOContext::Stop()
{
	std::unique_lock<std::mutex> lock(list_lock_);
	stop_ = true; //此处可能会有多线程问题，但相比于对stop_加锁，这个更划算一些
	cv_list_.notify_all();
}

InternalIOContext::InternalIOContext()
{
	std::thread([this]
		{
			ExecTask();
		}).detach();
}

void InternalIOContext::ExecTask()
{
	while (!stop_)
	{
		decltype(task_list_) tmep_list;
		{
			std::unique_lock<std::mutex> lock(list_lock_);
			cv_list_.wait(lock, [this]
				{
					return !task_list_.empty() || stop_;
				});
			if (stop_)break;

			tmep_list = std::move(task_list_);
			task_list_.clear();
		}

		for (const auto& item : tmep_list)
		{
			item->Run();
		}
	}
}
