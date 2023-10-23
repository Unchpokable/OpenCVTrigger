#include "Config.h"

using namespace utrigger_config;

std::unordered_map<std::string, int>* utrigger_config::ReadCmdArgs(
	int argc,
	char** raw,
	const char& sep)
{
	const auto result = new std::unordered_map<std::string, int>();

	for (auto i = 1; i < argc; i++)
	{
		std::string arg = raw[i];

		auto spos = arg.find(sep);

		if (spos != std::string::npos)
		{
			auto key = arg.substr(0, spos);
			auto value = std::stoi(arg.substr(spos + 1));
			result->insert_or_assign(key, value);
		}
	}

	return result;
}

void Config::SetScanWindowSize(int size)
{
	if (size > 0)
		m_wnd_size = size;
}

void Config::SetMinimalResponseDelay(int val)
{
	if (val > 0)
		m_response_min = val;
}

void Config::SetMaximalResponseDelay(int val)
{
	if (val < 0)
		return;

	m_response_max = val > m_response_min ? val : m_response_min + 1;
}

void Config::SetThreshold(int val)
{
	if (val > 0)
		m_diff_threshold = val;
}


int Config::GetScanWindowSize() const noexcept
{
	return m_wnd_size;
}

int Config::GetMinimalResponseDelay() const noexcept
{
	return m_response_min;
}

int Config::GetMaximalResponseDelay() const noexcept
{
	return m_response_max;
}

int Config::GetThreshold() const noexcept
{
	return m_diff_threshold;
}


