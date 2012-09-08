#include "HighResolutionTimer.h"

#ifndef MAX_AEVUM_EVERY
#define MAX_AEVUM_EVERY


namespace Max { namespace Aevum {	
		public ref class Every : public IEventPeriod
		{
		public:
			static IEventPeriod^ Milliseconds(unsigned int milliseconds);
			virtual TimerEventTicket^ Apply(HighResolutionTimer^ timer, System::Action^ action);

		internal:
			Every(unsigned int milliseconds);

		private:
			unsigned int period;
		};
	}
}

#endif