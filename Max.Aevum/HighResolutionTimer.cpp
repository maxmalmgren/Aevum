#include "HighResolutionTimer.h"
#include "EventBuilder.h"
#include "MultiMediaTimerException.h"
#include <algorithm>

using System::Runtime::InteropServices::GCHandle;

namespace Max { namespace Aevum {
		Resolution HighResolutionTimer::GetResolution()
		{
			TIMECAPS tc;
			UINT     wTimerRes;

			if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
				throw gcnew MultiMediaTimerException(GetLastError());

			UINT wPeriodMin;
			UINT wPeriodMax;

			return Resolution(tc.wPeriodMin, tc.wPeriodMax);
		}

		EventBuilder^ HighResolutionTimer::Execute(System::Action^ action)
		{
			return gcnew EventBuilder(this, action);
		}

		class CallbackData
		{
		public:
			CallbackData(void(*action)()) : action(action)
			{ }

			void(*action)();
		};

		std::vector<CallbackData> callbackDatas;
		
		CallbackData* ConstructCallbackData(System::IntPtr timer, System::Action^ action)
		{
			auto stuff = (void(*)())System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(action).ToPointer();

			CallbackData data(stuff);
			callbackDatas.push_back(data);
			return &callbackDatas.back();
		}

		void RemoveCallbackData(CallbackData *data)
		{
			callbackDatas.erase(std::remove_if(callbackDatas.begin(), callbackDatas.end(), [&](CallbackData & el) { return data == &el; }), callbackDatas.end());
		}

		void CALLBACK OnTimeElapsed(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
		{
			auto data = (CallbackData*)lpParameter;

			data->action();
		}

		void HighResolutionTimer::StopEvent(TimerEventTicket^ ticket)
		{
			timers->erase(std::remove(timers->begin(), timers->end(), *ticket->handle), timers->end());

			if(DeleteTimerQueueTimer(NULL, *ticket->handle, NULL) == 0)
				throw gcnew MultiMediaTimerException(GetLastError());
		}

		TimerEventTicket^ HighResolutionTimer::AddIntervalEvent(unsigned int period, System::Action^ action)
		{
			timers->push_back(HANDLE());

			actions->Add(action); //Keep a GC reference to the action.

			if(CreateTimerQueueTimer(&timers->back(), NULL, &OnTimeElapsed, ConstructCallbackData(this_ptr, action), period, period, WT_EXECUTEDEFAULT) == 0)
				throw gcnew MultiMediaTimerException(GetLastError());

			return gcnew TimerEventTicket(&timers->back());
		}

		TimerEventTicket^ HighResolutionTimer::AddRepeatedlyWithDelay(unsigned int milliseconds, System::Action^ action)
		{
			return AddIntervalEvent(milliseconds, action);
		}

		void assert_is_within_bounds(unsigned int resolution)
		{
			Resolution res = HighResolutionTimer::GetResolution();

			if(res.GetMinimumResolution() > resolution || res.GetMaximumResolution() < resolution)
				throw gcnew System::Exception();
		}

		HighResolutionTimer::HighResolutionTimer(unsigned int resolution) : res(resolution),
																			timers(new std::vector<HANDLE>()),
																			actions(gcnew System::Collections::Generic::List<System::Action^>)
		{
			assert_is_within_bounds(res);
			timeBeginPeriod(res);
		}

		HighResolutionTimer::~HighResolutionTimer()
		{
			timeEndPeriod(res);

			for(auto i = timers->begin(); i != timers->end(); i++)
				DeleteTimerQueueTimer(NULL, *i, NULL);

			delete timers;
		}
	}
}
