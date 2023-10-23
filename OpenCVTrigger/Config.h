#ifndef CONFIG_H
#define CONFIG_H

#include <memory>

#include <string>
#include <unordered_map>
#include "collection_extenstions.h"
#include "Config.h"

namespace utrigger_config
{
	class Config;

	std::unordered_map<std::string, int>* ReadCmdArgs(int, char**, const char&);
}

class utrigger_config::Config
{
public:
	Config()
		: m_wnd_size(0), m_response_min(0), m_response_max(0), m_diff_threshold(0)
	{
	}

	Config(const std::unordered_map<std::string, int>& tmpl) :  m_wnd_size(0), m_response_min(0), m_response_max(0), m_diff_threshold(0)
	{
		TryFind(tmpl, std::string("-wnd"), &m_wnd_size);
		TryFind(tmpl, std::string("-responsemin"), &m_response_min);
		TryFind(tmpl, std::string("-responsemax"), &m_response_max);
		TryFind(tmpl, std::string("-threshold"), &m_diff_threshold);
	}
		
	void SetScanWindowSize(int);
	void SetMinimalResponseDelay(int);
	void SetMaximalResponseDelay(int);
	void SetThreshold(int);

	int GetScanWindowSize() const noexcept;
	int GetMinimalResponseDelay() const noexcept;
	int GetMaximalResponseDelay() const noexcept;
	int GetThreshold() const noexcept;

private:
	int m_wnd_size;
	int m_response_min;
	int m_response_max;
	int m_diff_threshold;
};


#endif 
