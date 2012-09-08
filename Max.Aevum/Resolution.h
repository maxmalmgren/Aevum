#ifndef MAX_AEVUM_RESOLUTION
#define MAX_AEVUM_RESOLUTION

namespace Max { namespace Aevum {

		public value class Resolution
		{
		public:
			unsigned int GetMinimumResolution()
			{
				return minRes;
			}

			unsigned int GetMaximumResolution()
			{
				return maxRes;
			}

		internal:
			Resolution(unsigned int minRes, unsigned int maxRes) : minRes(minRes), maxRes(maxRes)
			{ }
		private:
			unsigned int minRes;
			unsigned int maxRes;
		};
	}
}
#endif