#include "HighResolutionTimer.h"
#include "EventBuilder.h"
#include "MultiMediaTimerException.h"
#include <algorithm>

using System::Runtime::InteropServices::GCHandle;

namespace Max { namespace Aevum {
		Resolution HighResolutionTimer::GetResolution()
		{
			DWORD error;
			auto res = get_resolution(error);

			if(error != 0)
				throw gcnew MultiMediaTimerException(error);

			return Resolution(res.first, res.second);
		}

		EventBuilder^ HighResolutionTimer::Execute(System::Action^ action)
		{
			return gcnew EventBuilder(this, action);
		}

		void HighResolutionTimer::StopEvent(TimerEventTicket^ ticket)
		{
			timer->cancel_event(ticket->handle);
		}

		action Convert(System::Action^ callback)
		{
			return (void(*)())System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(callback).ToPointer();
		}

		TimerEventTicket^ HighResolutionTimer::AddIntervalEvent(unsigned int period, System::Action^ action)
		{
			actions->Add(action); //Keep a GC reference to the action.

			DWORD error;
			auto handle = timer->add_interval_event(period, Convert(action), error);

			if(error != 0)
				throw gcnew MultiMediaTimerException(error);

			return gcnew TimerEventTicket(handle);
		}

		TimerEventTicket^ HighResolutionTimer::AddRepeatedlyWithDelay(unsigned int milliseconds, System::Action^ action)
		{
			DWORD error;
			auto handle = timer->add_repeatedly_with_delay(milliseconds, Convert(action), error);

			if(error != 0)
				throw gcnew MultiMediaTimerException(error);

			return gcnew TimerEventTicket(handle);
		}

		HighResolutionTimer::HighResolutionTimer(unsigned int resolution) : timer(new high_resolution_timer(resolution)), actions(gcnew System::Collections::Generic::List<System::Action^>)
		{ }

		HighResolutionTimer::~HighResolutionTimer()
		{
			delete timer;
		}
	}
}
