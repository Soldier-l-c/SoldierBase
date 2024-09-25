#pragma once
class InternalFile
{
public:

	~InternalFile();

	bool Open(const wchar_t* path);

	size_t Read(void* buffer, size_t buffer_size);

	size_t Write(const void* buffer, size_t buffer_size);

	bool Close();

private:
	bool open_{ false };
	std::fstream file_;
};

