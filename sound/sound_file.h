#ifndef __SOUND__SOUND_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __SOUND__SOUND_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <AL/al.h>


/* Classes */

class MemoryFile;
class RiffFile;
struct RiffWaveFormat;

/* Sound file. */
class SoundFile
{
public:
	/**
	 * Constructor.
	 * @param name Name of the file.
	 */
	SoundFile(const QString & name);

	/** Returns frequency. */
	inline uint frequency() const
	{
		return m_Frequency;
	}

	/** Returns format of the data. */
	inline uint format() const
	{
		return m_Format;
	}

	/** Returns number of channels. */
	inline uint numChannels() const
	{
		switch (m_Format)
		{
		case AL_FORMAT_MONO8:
		case AL_FORMAT_MONO16:
			return 1;
		case AL_FORMAT_STEREO8:
		case AL_FORMAT_STEREO16:
			return 2;
		}	

		Q_ASSERT(false);
		return 0;
	}

	/** Returns number of bits per sample. */
	inline uint bitsPerSample() const
	{
		switch (m_Format)
		{
		case AL_FORMAT_MONO8:
		case AL_FORMAT_STEREO8:
			return 8;
		case AL_FORMAT_MONO16:
		case AL_FORMAT_STEREO16:
			return 16;
		}	

		Q_ASSERT(false);
		return 0;
	}

	/** Returns size of the data. */
	inline uint dataSize() const
	{
		return m_SoundData.length();
	}

	/** Returns pointer to the data. */
	inline const char * data() const
	{
		return m_SoundData.constData();
	}

	/** Returns duration of the sound in seconds. */
	inline double duration() const
	{
		return m_Duration;
	}

	/** Creates OpenAL buffer from the sound. */
	ALuint createBuffer();

private:
	uint m_Frequency;			/**< Frequency. */
	uint m_Format;				/**< OpenAL format of the sound data. */
	QByteArray m_SoundData;		/**< Sound data. */
	double m_Duration;			/**< Duration in seconds. */

	/**
	 * Loads the RIFF waveform file.
	 * @param file Pointer to the file.
	 */
	void loadRiffWaveform(MemoryFile * file);

	/**
	 * Loads the RIFF waveform data in PCM format.
	 * @param file Pointer to the file.
	 * @param fmt Reference to the format information.
	 * @param riff Reference to the RIFF reader.
	 */
	void loadRiffWaveformPCM(MemoryFile * file, const RiffWaveFormat & fmt, RiffFile & riff);
};

#endif // __SOUND__SOUND_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
