#include "HighResolutionTimer.h"

#ifndef MAX_AEVUM_EVENTBUILDER
#define MAX_AEVUM_EVENTBUILDER

namespace Max { namespace Aevum {

		public ref class EventBuilder
		{
		public:
			TimerEventTicket^ At(IEventPeriod^ period);
			TimerEventTicket^ RepeatedlyWithDelay(unsigned int milliseconds);

		internal:
			EventBuilder(HighResolutionTimer^ timer, System::Action^ action);

		private:
			HighResolutionTimer^ timer;
			System::Action^ action;
		};
	}
}

#endif