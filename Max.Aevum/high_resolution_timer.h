#ifndef MAX_AEVUM_HIGH_RESOLUTION_TIMER
#define MAX_AEVUM_HIGH_RESOLUTION_TIMER

#include <Windows.h>
#include <Mmsystem.h>
#include <utility>
#include <vector>
#include <functional>

namespace Max { namespace Aevum {

		std::pair<unsigned int, unsigned int> get_resolution(DWORD &error);

		typedef void(*action)();

		class high_resolution_timer;

		struct callback_data
		{
			callback_data(high_resolution_timer *timer, std::function<void()> callback) : timer(timer), callback(callback)
			{ }

			high_resolution_timer *timer;
			std::function<void()> callback;
		};

		class high_resolution_timer
		{
		public:
			high_resolution_timer(unsigned int resolution);
			~high_resolution_timer();

			PHANDLE add_repeatedly_with_delay(unsigned int delay, std::function<void()> callback, DWORD &error);
			PHANDLE add_non_repeating_event(unsigned int delay, std::function<void()> callback, DWORD &error);
			PHANDLE add_interval_event(unsigned int period, std::function<void()> callback, DWORD &error);
			DWORD cancel_event(PHANDLE ticket);

		private:
			std::vector<HANDLE> timers;
			std::vector<callback_data> callback_datas;
			std::vector<std::function<void()>> action_cache;
			unsigned int res;

			PHANDLE high_resolution_timer::add_repeatedly_with_delay_int(unsigned int delay, std::function<void()> function, DWORD &error);

			friend PHANDLE add_timer(high_resolution_timer &timer);
			friend callback_data* construct_callback_data(high_resolution_timer &timer, std::function<void()> callback);
			friend void remove_callback_data(high_resolution_timer &timer, callback_data *data);
		};

	}
}
#endif