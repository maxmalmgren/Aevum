#include <algorithm>
#include "high_resolution_timer.h"

namespace Max { namespace Aevum {

		DWORD high_resolution_timer::cancel_event(PHANDLE handle)
		{
			timers.erase(std::remove(timers.begin(), timers.end(), *handle), timers.end());

			if(DeleteTimerQueueTimer(NULL, *handle, NULL) == 0)
				return GetLastError();
		}

		std::pair<unsigned int, unsigned int> get_resolution(DWORD &error)
		{
			TIMECAPS tc;
			error = 0;

			if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
				error = GetLastError();

			return std::pair<unsigned int, unsigned int>(tc.wPeriodMin, tc.wPeriodMax);
		}

		callback_data* construct_callback_data(high_resolution_timer &timer, std::function<void()> callback)
		{
			timer.callback_datas.push_back(callback_data(&timer, callback));
			return &timer.callback_datas.back();
		}

		void remove_callback_data(high_resolution_timer &timer, callback_data *data)
		{
			timer.callback_datas.erase(std::remove_if(timer.callback_datas.begin(), timer.callback_datas.end(), [&](callback_data &el) { return data == &el; }), timer.callback_datas.end());
		}

		void CALLBACK on_time_elapsed(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
		{
			auto data = (callback_data*)lpParameter;

			data->callback();
		}

		high_resolution_timer::~high_resolution_timer()
		{
			timeEndPeriod(res);

			for(auto i = timers.begin(); i != timers.end(); i++)
				DeleteTimerQueueTimer(NULL, *i, NULL);
		}

		PHANDLE add_timer(high_resolution_timer &timer)
		{
			timer.timers.push_back(HANDLE());
			return &timer.timers.back();
		}

		std::function<void()> combine(std::function<void()> first, std::function<void()> second)
		{
			return [=]() 
			{
				first();
				second();
			};
		}

		std::function<void()> wrap(action callback)
		{
			return [=](){ callback(); };
		}

		PHANDLE high_resolution_timer::add_non_repeating_event(unsigned int delay, std::function<void()> callback, DWORD &error)
		{
			auto timer = add_timer(*this);
			error = 0;

			if(CreateTimerQueueTimer(timer, NULL, &on_time_elapsed, construct_callback_data(*this, callback), delay, 0, WT_EXECUTEONLYONCE) == 0)
				error = GetLastError();

			return timer;
		}

		action convert(std::function<void()> &func)
		{
			return *func.target<void(*)()>();
		}

		PHANDLE high_resolution_timer::add_repeatedly_with_delay(unsigned int delay, std::function<void()> callback, DWORD &error)
		{
			auto timer = add_timer(*this);
			error = 0;

			auto combined = combine(std::function<void()>(callback), [=]() {
																			DWORD error;
																			add_repeatedly_with_delay(delay, callback, error); 
																		  });

			return add_non_repeating_event(delay, combined, error);
		}

		PHANDLE high_resolution_timer::add_interval_event(unsigned int period, std::function<void()> callback, DWORD &error)
		{
			auto timer = add_timer(*this);
			error = 0;

			if(CreateTimerQueueTimer(timer, NULL, &on_time_elapsed, construct_callback_data(*this, callback), period, period, WT_EXECUTEDEFAULT) == 0)
				error = GetLastError();

			return timer;
		}

		void assert_is_within_bounds(unsigned int resolution)
		{
			DWORD error;
			auto res = get_resolution(error);

			if(res.first > resolution || res.second < resolution)
				throw gcnew System::Exception();
		}

		high_resolution_timer::high_resolution_timer(unsigned int resolution) : res(resolution), timers(), callback_datas(), action_cache()
		{
			assert_is_within_bounds(res);
			timeBeginPeriod(res);
		}
	}
}