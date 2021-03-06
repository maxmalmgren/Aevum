#include "Resolution.h"
#include "high_resolution_timer.h"
#include "IEventPeriod.h"
#include <vector>
#include <Windows.h>
#include <Mmsystem.h>

#ifndef MAX_AEVUM_HIGHRESOLUTIONTIMER
#define MAX_AEVUM_HIGHRESOLUTIONTIMER

namespace Max { namespace Aevum {

		ref class EventBuilder;

		public ref class TimerEventTicket
		{
		internal:
			HANDLE *handle;
			TimerEventTicket(HANDLE *handle) : handle(handle)
			{ }
		};

		public ref class HighResolutionTimer
		{
		public:
			static Resolution GetResolution();

			HighResolutionTimer(unsigned int resolution);
			~HighResolutionTimer();

			EventBuilder^ Execute(System::Action^ action);
			void StopEvent(TimerEventTicket^ ticket);
		internal:
			TimerEventTicket^ AddRepeatedlyWithDelay(unsigned int delay, System::Action^ action);
			TimerEventTicket^ AddIntervalEvent(unsigned int period, System::Action^ action);
		private:
			high_resolution_timer *timer;
			System::Collections::Generic::List<System::Action^>^ actions;
		};
	}
}

#endif