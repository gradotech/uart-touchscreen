#include "umt.h"
#include "umt_watchdog.h"

UMTWatchdog::UMTWatchdog(bool enabled, int interval)
{
	this->enabled  = enabled;
	this->interval = std::chrono::milliseconds(interval);
}

bool UMTWatchdog::watch(int id)
{
	if (!enabled)
		return true;

	auto now = std::chrono::high_resolution_clock::now();

	if (active_elements.count(id)) {
		UMT_LOG(UMT_DBG_LOG, "Element %d has been reset\n", id);
		active_elements[id] = now;
	} else {
		UMT_LOG(UMT_DBG_LOG, "Element %d is now being watched\n", id);
		active_elements.insert(std::pair<int, chrono_clok_t>(id, now));
	}

	return true;
}

bool UMTWatchdog::unwatch(int id)
{
	if (!enabled)
		return true;

	if (active_elements.count(id)) {
		active_elements.erase(id);
		UMT_LOG(UMT_DBG_LOG, "Element %d has been unwatched\n", id);
		return true;
	}

	return false;
}

int UMTWatchdog::expired()
{
	if (enabled) {
		auto now = std::chrono::high_resolution_clock::now();
		auto itr = active_elements.begin();

		while (itr != active_elements.end()) {
			auto elapsed = std::chrono::duration_cast<
				std::chrono::milliseconds>(now - itr->second);

			if (elapsed >= interval) {
				int id = itr->first;

				UMT_LOG(UMT_DBG_LOG, "Element %d expired\n", itr->first);
				active_elements.erase(itr->first);
				return id;
			}

			itr++;
		}
	}

	return UMT_WD_NO_EXPIRED;
}
