#ifndef MAX_AEVUM_IEVENTPERIOD
#define MAX_AEVUM_IEVENTPERIOD


namespace Max { namespace Aevum {

		ref class HighResolutionTimer;
		ref class TimerEventTicket;

		public interface class IEventPeriod
		{
			TimerEventTicket^ Apply(HighResolutionTimer^ timer, System::Action^ action);
		};
	}
}
#endif