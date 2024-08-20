#include "pch.h"
#include "TimerMgr.h"

TimerItem::TimerItem(uint32_t timer_id, void* context, BASE_TIMER_CALLBACK callback, uint32_t interval, int32_t count):
	timer_id(timer_id),context(context), interval(interval), count(count),callback(callback)
{
	last_call_time = helper::time::get_steady_tick_inms();
}

bool TimerItem::Check(const int64_t& cur_time)
{
	if (cur_time - last_call_time < interval)
		return true;

	last_call_time = cur_time;

	auto res = callback(timer_id, context, count, index++);

	return res && (count == -1 || index < count);
}

TimerMgr::TimerMgr()
{
	Init();
}

void TimerMgr::AddTimer(uint32_t time_id, BASE_TIMER_CALLBACK call_back, void* context, uint32_t interval, int32_t call_count)
{
	auto timer = std::make_shared<TimerItem>(time_id, context, call_back, interval, call_count);

	std::unique_lock<std::mutex> lock(list_lock_);
	timer_list_.push_back(std::move(timer));
	cv_list_.notify_all();
}

void TimerMgr::Stop()
{
	std::unique_lock<std::mutex> lock(list_lock_);
	stop_ = true; //此处可能会有多线程问题，但相比于对stop_加锁，这个更划算一些
	cv_list_.notify_all();
}

void TimerMgr::Init()
{
	std::thread([this] 
		{
			CheckFunc();
		}).detach();
}

void TimerMgr::CheckFunc()
{
	while (!stop_)
	{
		decltype(timer_list_) tmep_list;
		{
			std::unique_lock<std::mutex> lock(list_lock_);
			cv_list_.wait(lock, [this]
				{
					return !timer_list_.empty() || stop_;
				});
			if (stop_)break;

			tmep_list = timer_list_;
		}

		auto cur_time = helper::time::get_steady_tick_inms();
		for (const auto& item : tmep_list)
		{
			if (!item->Check(cur_time))
			{
				EraseTimer(item);
			}
		}

		helper::time::sleep(100);
	}
}

void TimerMgr::EraseTimer(const TimerItemPtr& timer)
{
	std::unique_lock<std::mutex> lock(list_lock_);
	auto iter = std::find(timer_list_.begin(), timer_list_.end(), timer);
	if (iter != timer_list_.end())
	{
		timer_list_.erase(iter);
	}
}
