#include "EventBuilder.h"
#include "HighResolutionTimer.h"

namespace Max { namespace Aevum {

			TimerEventTicket^ EventBuilder::At(IEventPeriod^ period)
			{
				return period->Apply(timer, action);
			}

			TimerEventTicket^ EventBuilder::RepeatedlyWithDelay(unsigned int milliseconds)
			{
				return timer->AddRepeatedlyWithDelay(milliseconds, action);
			}

			EventBuilder::EventBuilder(HighResolutionTimer^ timer, System::Action^ action) : timer(timer), action(action)
			{ }
	}
}