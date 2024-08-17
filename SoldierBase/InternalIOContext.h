#pragma once

struct IOTask
{
	virtual bool Run() = 0;
};

using IOTaskPtr = std::shared_ptr<IOTask>;

using IOTaskFunc = std::function<void()>;

class InternalIOContext
{
	SINGLE_INSTANCE(InternalIOContext);

public:
	void AddTask(const IOTaskPtr& task);

	void AddTask(const IOTaskFunc& func);

	void Stop();

private:
	InternalIOContext();
	
	void ExecTask();

	void EraseTask(const IOTaskPtr& task);

private:
	std::vector<IOTaskPtr> task_list_;

	std::vector<IOTaskFunc> func_list_;

	std::mutex list_lock_;
	std::condition_variable cv_list_;
	bool stop_{ false };
};

