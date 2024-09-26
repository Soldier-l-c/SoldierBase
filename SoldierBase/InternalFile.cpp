#include "pch.h"
#include "InternalFile.h"

InternalFile::~InternalFile()
{
	if (open_)
	{
		file_.close();
	}
}

bool InternalFile::Open(const wchar_t* path, uint32_t mode)
{
	if (!path)
		return false;

	file_.open(path, mode);
	open_ = file_.is_open();

	return open_;
}

size_t InternalFile::Read(void* buffer, size_t buffer_size)
{
	if (!open_)return 0;
	
	return file_.read((char*)buffer, buffer_size).gcount();
}

size_t InternalFile::Write(const void* buffer, size_t buffer_size)
{
	if (!open_)return 0;

	auto before = file_.tellg();
	file_.write((const char*)buffer, buffer_size);
	auto cur = file_.tellg();

	return std::ios_base::goodbit == file_.rdstate() ? cur - before : 0;
}

bool InternalFile::Close()
{
	if (!open_)return true;
	file_.close();
	open_ = !file_.is_open();
	return open_;
}
