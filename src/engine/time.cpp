#include "sm64.h"
#include "time.h"

Duration::Duration() : m_value(0)
{
}

Duration::Duration(s16 duration) : m_value(duration == 0x7FFF || duration < 0 ? duration : (duration * FRAME_RATE_SCALER_INV))
{
}

Duration::operator s16() const
{
	return m_value;
}

Duration::operator s16&()
{
	return m_value;
}