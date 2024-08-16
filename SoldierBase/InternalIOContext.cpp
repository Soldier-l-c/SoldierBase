#include "pch.h"
#include "InternalIOContext.h"

void InternalIOContext::AddTask(const IOTaskPtr& task)
{
	std::unique_lock<std::mutex> lock(list_lock_);
	task_list_.push_back(task);
	cv_list_.notify_all();
}

void InternalIOContext::AddRunOnceTask(const IOTaskPtr& task)
{
	std::unique_lock<std::mutex> lock(list_lock_);
	task_list_once_.push_back(task);
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
		decltype(task_list_) tmep_list, runonce_list;
		{
			std::unique_lock<std::mutex> lock(list_lock_);
			cv_list_.wait(lock, [this]
				{
					return !task_list_.empty() || !task_list_once_.empty() || stop_;
				});
			if (stop_)break;

			if (!task_list_.empty())
			{
				tmep_list = task_list_;
			}
			if (!task_list_once_.empty())
			{
				runonce_list = std::move(task_list_once_);
				task_list_once_.clear();
			}
		}

		for (const auto& item : runonce_list)
		{
			item->Run();
		}

		for (const auto& item : tmep_list)
		{
			if (!item->Run())
			{
				EraseTask(item);
			}
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
