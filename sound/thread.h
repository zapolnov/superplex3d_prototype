#ifndef __SOUND__THREAD_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __SOUND__THREAD_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/signal.h>
#include <core/pointer.h>
#include <tbb/concurrent_queue.h>
#include <math/vector3.h>
#include <sound/ogg_stream.h>
#include <vector>
#include <map>
#include <AL/al.h>


/* Classes */

/** Sound processing thread. */
class SoundThread : public QThread
{
	Q_OBJECT

public:
	/** Constructor. */
	SoundThread();

	/** Destructor. */
	~SoundThread();

	/** Starts the thread. */
	void start();

	/** Stops the thread. */
	void stop();

	/**
	 * Plays the specified music track.
	 * @param name Name of the track.
	 */
	inline void playMusic(const QString & name)
	{
		m_Queue.push(Command(CMD_PlayMusic, name));
		m_Signal.signal();
	}

	/**
	 * Loads the specified sound.
	 * @param name Name of the file.
	 */
	inline void loadSound(const QString & name)
	{
		m_Queue.push(Command(CMD_LoadSound, name));
		m_Signal.signal();
	}

	/**
	 * Plays the specified sound.
	 * @param name Name of the file.
	 * @param pos Position of the sound.
	 */
	inline void playSound(const QString & name, const Vector3 & pos)
	{
		m_Queue.push(Command(CMD_PlaySound, name, pos));
		m_Signal.signal();
	}

private:
	/** Audio track playing mode. */
	enum PlayMode
	{
		NormalPlay = 0,				/**< Track is played normally. */
		FadeOut,					/**< Track is being faded out. */
		FadeIn						/**< Track is being faded in. */
	};

	/** Information about the music track. */
	struct MusicTrack
	{
		Pointer<OggStream> stream;	/**< Pointer to the stream. */
		PlayMode playMode;			/**< Track play mode. */
		float volume;				/**< Track volume. */
	};

	/** Information about the sound sample. */
	struct SoundSample
	{
		ALuint buffer;				/**< Handle of the OpenAL buffer. */
		double duration;			/**< Duration of the sound sample. */
	};

	/** Information about the sound source. */
	struct SoundSource
	{
		ALuint source;				/**< Handle of the OpenAL sound source. */
		double startTime;			/**< Time when playback has been started (or -1). */
		double stopTime;			/**< Time when playback should be stopped (or -1). */
	};

	/** Map of the streams currently playing. */
	typedef std::map<QString, MusicTrack> StreamsHash;

	/** Cache of the sound samples. */
	typedef std::map<QString, SoundSample> SamplesCache;

	/** List of sound sources. */
	typedef std::vector<SoundSource> SoundSources;

	/** Constants */
	enum QueueCommand
	{
		CMD_PlayMusic = 0,			/**< Starts playback of the specified music track. */
		CMD_StopMusic,				/**< Stops any music that is currently playing. */
		CMD_LoadSound,				/**< Loads the specified sound. */
		CMD_PlaySound,				/**< Plays the specified sound. */
	};

	/** Queue command. */
	struct Command
	{
		QueueCommand command;				/**< Command ID. */
		QString path;						/**< Path to the file. */
		Vector3 position;					/**< Position of the sound source. */

		/** Constructor. */
		inline Command()
		{
		}

		/**
		 * Constructor.
		 * @param _command Command ID.
		 * @param _path Path to the file.
		 * @param _position Position of the sound source.
		 */
		inline Command(QueueCommand _command, const QString & _path = QString(),
				const Vector3 & _position = Vector3())
			: command(_command), path(_path), position(_position)
		{
		}
	};

	bool m_StopFlag;						/**< If true, thread should exit. */
	Signal m_Signal;						/**< Signal. */
	float m_MusicVolume;					/**< Volume of the music. */
	StreamsHash m_MusicTracks;				/**< List of music tracks that are currently playing. */
	SamplesCache m_SoundSamples;			/**< Cache of the sound samples. */
	SoundSources m_SoundSources;			/**< List of sound sources. */
	tbb::concurrent_queue<Command> m_Queue;	/**< Thread command queue. */

	/** Thread body. */
	void run();

	/**
	 * Plays the specified music track.
	 * @param name Name of the track.
	 */
	void playMusicTrack(const QString & name);

	/**
	 * Loads the specified sound.
	 * @param name Name of the file.
	 */
	SamplesCache::iterator loadSoundSample(const QString & name);

	/**
	 * Plays the specified sound.
	 * @param name Name of the file.
	 * @param pos Position of the sound.
	 */
	void playSoundSample(const QString & name, const Vector3 & pos);
};

#endif // __SOUND__THREAD_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
