#pragma once
class InternalFile
{
public:

	~InternalFile();

	bool Open(const wchar_t* path, uint32_t mode = std::ios::in | std::ios::binary);

	size_t Read(void* buffer, size_t buffer_size);

	size_t Write(const void* buffer, size_t buffer_size);

	bool Close();

private:
	bool open_{ false };
	std::fstream file_;
};

