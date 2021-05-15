#pragma once


class AMethod
{
	private:
		Stream&		_stream_in;
		Stream&		_stream_out;
	public:
		AMethod();
		AMethod(const AMethod& ref);
		~AMethod();
		AMethod	operator=(const AMethod& ref);
};