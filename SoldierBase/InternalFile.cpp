#include "pch.h"
#include "InternalFile.h"

InternalFile::~InternalFile()
{
	if (open_)
	{
		file_.close();
	}
}

bool InternalFile::Open(const wchar_t* path)
{
	if (!path)
		return false;

	file_.open(path);
	open_ = file_.is_open();

	return open_;
}

size_t InternalFile::Read(void* buffer, size_t buffer_size)
{
	if (!open_)return 0;
	file_.read((char*)buffer, buffer_size);
	return file_.gcount() >0 && std::ios_base::goodbit == file_.rdstate();;
}

size_t InternalFile::Write(const void* buffer, size_t buffer_size)
{
	if (!open_)return 0;
	file_.write((const char*)buffer, buffer_size);
	return std::ios_base::goodbit == file_.rdstate();
}

bool InternalFile::Close()
{
	if (!open_)return true;
	file_.close();
	return !file_.is_open();
}
