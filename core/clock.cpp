#include <core/clock.h>

/* Clock variables */

#ifndef _PG_ENABLE_QT4
Clock * Clock::m_Instance;
#endif


/* Clock methods */

//
// Constructor
//
Clock::Clock()
	: m_FrameDurationHistoryOffset(0), m_FrameDurationHistoryLength(1), m_CurrentTime(0), m_FrameTime(0.0f)
{
#ifndef _PG_ENABLE_QT4
	Q_ASSERT(!m_Instance);
	m_Instance = this;
#endif

	m_FrameDurationHistory[0] = 0.03;
	m_FrameTime = getPredictedFrameDuration();
}

//
// Destructor
//
Clock::~Clock()
{
#ifndef _PG_ENABLE_QT4
	Q_ASSERT(m_Instance);
	m_Instance = NULL;
#endif
}

//
// Updates the current time.
//
void Clock::update(double delta)
{
	double frameTime, exactLastFrameDuration;

	exactLastFrameDuration = delta;
	if (unlikely(exactLastFrameDuration > 0.200))
		exactLastFrameDuration = m_FrameDurationHistory[m_FrameDurationHistoryOffset];

	addToFrameHistory(exactLastFrameDuration);

	frameTime = getPredictedFrameDuration();
	m_CurrentTime += frameTime;

	if (likely(frameTime > 0.0))
		m_FrameTime = frameTime;
}

//
// Returns the average duration of the previous time frames.
//
double Clock::getPredictedFrameDuration() const
{
	double totalFrameTime = 0;

	Q_ASSERT(m_FrameDurationHistoryLength > 0);
	Q_ASSERT(m_FrameDurationHistoryLength <= m_FilteringWindow);
	Q_ASSERT(m_FrameDurationHistoryOffset < m_FilteringWindow);

	for (uint i = 0; i < m_FrameDurationHistoryLength; i++)
		totalFrameTime += m_FrameDurationHistory[i];

	return totalFrameTime / m_FrameDurationHistoryLength;
}

//
// Adds duration of the current frame to the history.
//
void Clock::addToFrameHistory(const double exactFrameDuration)
{
	m_FrameDurationHistoryOffset = (m_FrameDurationHistoryOffset + 1) % m_FilteringWindow;

	if (unlikely(m_FrameDurationHistoryLength < m_FilteringWindow))
	 	m_FrameDurationHistoryLength++;

	m_FrameDurationHistory[m_FrameDurationHistoryOffset] = exactFrameDuration;
}
