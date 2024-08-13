#pragma once

struct IOTask
{
	virtual void Run() = 0;
};

using IOTaskPtr = std::shared_ptr<IOTask>;

class InternalIOContext
{
	SINGLE_INSTANCE(InternalIOContext);

public:
	void AddTask(const IOTaskPtr& task);

	void Stop();

private:
	InternalIOContext();
	
	void ExecTask();

private:
	std::vector<IOTaskPtr> task_list_;
	std::mutex list_lock_;
	std::condition_variable cv_list_;
	bool stop_{ false };
};

