#ifndef COLLEXT_H
#define COLLEXT_H

#include <unordered_map>
#include <stdexcept>

template<typename Type, typename Key>
bool TryFind(std::unordered_map<Key, Type> origin, const Key& key, Type* ret)
{
	auto it = origin.find(key);
	if (it != origin.end())
	{
		if (ret == nullptr)
		{
			throw std::invalid_argument("ret pointer should be not nullptr");
		}
		*ret = it->second;
		return true;
	}

	return false;
}

#endif
