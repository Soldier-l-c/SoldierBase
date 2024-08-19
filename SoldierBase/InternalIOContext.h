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

	void Init();

	void UnInit();

	void AddTask(const IOTaskPtr& task);

	void AddTask(const IOTaskFunc& func);

	boost::asio::io_context& IOContext();

private:
	void InternalInit();

	void InternalUinit();

private:

	using IOWrokPtr = std::unique_ptr<boost::asio::io_context::work>;

	IOWrokPtr io_work_;
	boost::asio::io_context io_context_;
	std::once_flag int_once_flag_;
};

