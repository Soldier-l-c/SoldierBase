#include "pch.h"
#include "InternalFileHash.h"
#include "InternalHash.h"
#include "InternalFile.h"

void InternalFileHash::Init()
{
}

smart_result InternalFileHash::CalcFileHash(const wchar_t* file_path, NsHashData::HashType type, NsHashData::HashData& data)
{
	InitHashData(data);

	data.length = sizeof(data.data);
	const auto obj_item = InternalHash::instance().QueryObjectItem(type);
	VERIFY_PTR_R(obj_item, false);

	const auto& hash_object = obj_item->CreateHashObject();
	InternalFile file;

	if (!file.Open(file_path))
	{
		return false;
	}

	hash_object->Init();

	const auto buffer_len = 1024 * 5;
	char buffer[buffer_len]{ 0 };

	while (true)
	{
		auto read_size = file.Read(buffer, buffer_len);
		if (read_size <= 0)
			break;;
		hash_object->Update(buffer, read_size);
	}

	hash_object->Final(data);

	data.length = InternalHash::instance().GetHashLength(type);

	return true;
}
