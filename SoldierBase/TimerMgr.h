#pragma once
struct TimerItem
{
	void* context{ nullptr };
	uint32_t timer_id{0};
	BASE_TIMER_CALLBACK callback;
	uint32_t interval{ 0 };
	int32_t count{ 0 };
	int32_t index{ 0 };
	int64_t last_call_time{ 0 };

	TimerItem(uint32_t timer_id, void* context, BASE_TIMER_CALLBACK callback, uint32_t interval, int32_t count);

	bool Check(const int64_t& cur_time);
};

using TimerItemPtr = std::shared_ptr<TimerItem>;

class TimerMgr
{
	SINGLE_INSTANCE(TimerMgr)

	void AddTimer(uint32_t time_id, BASE_TIMER_CALLBACK call_back, void* context, uint32_t interval, int32_t call_count);

	void Stop();

private:
	TimerMgr();

	void Init();

	void CheckFunc();

	void EraseTimer(const TimerItemPtr& timer);

private:
	std::mutex list_lock_;
	std::condition_variable cv_list_;
	std::list<TimerItemPtr> timer_list_;
	bool stop_{ false };
};

