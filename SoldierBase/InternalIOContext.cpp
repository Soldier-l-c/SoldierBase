#include "pch.h"
#include "InternalIOContext.h"

void InternalIOContext::AddTask(const IOTaskPtr& task)
{
	std::unique_lock<std::mutex> lock(list_lock_);
	task_list_.push_back(task);
	cv_list_.notify_all();
}

void InternalIOContext::AddTask(const IOTaskFunc& func)
{
	std::unique_lock<std::mutex> lock(list_lock_);
	func_list_.push_back(func);
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
		decltype(func_list_) func_list;
		{
			std::unique_lock<std::mutex> lock(list_lock_);
			cv_list_.wait(lock, [this]
				{
					return !task_list_.empty() || !func_list_.empty() || stop_;
				});
			if (stop_)break;

			if (!task_list_.empty())
			{
				tmep_list = task_list_;
			}
			
			if (!func_list_.empty())
			{
				func_list = std::move(func_list_);
				func_list_.clear();
			}
		}

		for (const auto& item : tmep_list)
		{
			if (!item->Run())
			{
				EraseTask(item);
			}
		}

		for (const auto& func : func_list)
		{
			func();
		}
	}
}

void InternalIOContext::EraseTask(const IOTaskPtr& task)
{
	std::unique_lock<std::mutex> lock(list_lock_);
	auto iter = std::find(task_list_.begin(), task_list_.end(), task);
	if (iter != task_list_.end())
	{
		task_list_.erase(iter);
	}
}
