#pragma once
#include "types.h"
#include <string>

extern "C"
{
	struct duk_hthread;
}

namespace sm64::script
{
	class Js
	{
	public:
		Js();
		Js(const char* src);
		virtual ~Js();
		void close();
		bool open(const char* src);
		s64 exec();
	protected:
		std::string m_src;
		duk_hthread* m_ctx;
	};
}
