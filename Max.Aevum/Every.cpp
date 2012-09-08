#include "Every.h"

namespace Max { namespace Aevum {	
		IEventPeriod^ Every::Milliseconds(unsigned int milliseconds)
		{
			return gcnew Every(milliseconds);
		}

		TimerEventTicket^ Every::Apply(HighResolutionTimer^ timer, System::Action^ action)
		{
			return timer->AddIntervalEvent(period, action);
		}

		Every::Every(unsigned int milliseconds) : period(milliseconds)
		{ }
	}
}