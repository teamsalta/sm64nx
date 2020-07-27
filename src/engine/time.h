#pragma once

class Duration
{
	public:
	Duration();
	Duration(s16 duration);
	operator s16() const;
	operator s16&();

	protected:
	s16 m_value;
};