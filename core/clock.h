#ifndef __CORE__CLOCK_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__CLOCK_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>


/* Macros */

#ifdef _PG_ENABLE_QT4
 /** Returns pointer to the instance of the clock. */
 #define clock() (Application::clock())
#else
 /** Returns pointer to the instance of the clock. */
 #define clock() (Clock::instance())
#endif


/* Classes */

/** Clock implementation. */
class GCC_PUBLIC Clock : public QObject
{
	Q_OBJECT
	Q_PROPERTY(double framesPerSecond READ getCurrentFPS() STORED false)
	Q_PROPERTY(double currentTime READ getCurrentTime() STORED false)

public:
	/** Constructor. */
	COREAPI Clock();

	/** Destructor. */
	COREAPI ~Clock();

	/** Returns the current number of frames per second. */
	inline double getCurrentFPS() const
	{
		return 1.0 / m_FrameTime;
	}

	/** Returns length of the last time frame. */
	inline double getLastFrameTime() const
	{
		return m_FrameTime;
	}

	/** Returns the current time in seconds from the start of the program. */
	inline double getCurrentTime() const
	{
		return m_CurrentTime;
	}

	/**
	 * Updates the clock.
	 * @param delta Number of seconds passed from the previous call.
	 */
	COREAPI void update(double delta);

#ifndef _PG_ENABLE_QT4

	/** Returns pointer to the instance of the clock. */
	static inline Clock * instance()
	{
		return m_Instance;
	}

protected:
	COREAPI static Clock * m_Instance;					/**< Pointer to the instance of the clock. */

#endif

private:
	static const uint m_FilteringWindow = 100;			/**< Number of frames to use in filtering of the clock drift. */
	double m_FrameDurationHistory[m_FilteringWindow];	/**< Lengths of the previous frames. */
	uint m_FrameDurationHistoryOffset;					/**< Current offset in the frame duration history buffer. */
	uint m_FrameDurationHistoryLength;					/**< Number of the previous frames. */
	double m_CurrentTime;								/**< Number of seconds passed from the start of the program. */
	double m_FrameTime;									/**< Length of the last time frame. */

	/** Returns the average duration of the previous time frames. */
	double getPredictedFrameDuration() const;

	/**
	 * Adds duration of the current frame to the history.
	 * @param exactFrameDuraction Duration of the current frame.
	 */
	void addToFrameHistory(const double exactFrameDuration);
};

#endif // __CORE__CLOCK_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
