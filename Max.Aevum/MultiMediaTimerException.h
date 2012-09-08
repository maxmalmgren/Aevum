namespace Max { namespace Aevum {
		public ref class MultiMediaTimerException : public System::Exception
		{
		private:
			int errorCode;

		public:
			MultiMediaTimerException(int errorCode) : Exception("Error occured while executing timer function: " + errorCode)
			{
				this->errorCode = errorCode;
			}

			property int ErrorCode
			{
				int get()
				{
					return errorCode;
				}
			}
		};
	}
}