#ifndef __UMT_WATCHDOG_H__
#define __UMT_WATCHDOG_H__

#include <chrono>
#include <map>

#define UMT_WD_NO_EXPIRED -1

typedef std::chrono::time_point<std::chrono::high_resolution_clock> chrono_clok_t;

class UMTWatchdog {
public:
	UMTWatchdog(bool enabled, int interval);
	bool watch(int id);
	bool unwatch(int id);
	int expired();

private:
	bool enabled;
	std::chrono::milliseconds interval;
	std::map<int, chrono_clok_t> active_elements;
};

#endif /* __UMT_WATCHDOG_H__ */
