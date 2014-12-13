#include <core/exception.h>
#include <core/logger.h>
#include <core/clock.h>
#include <sound/sound_file.h>
#include <sound/thread.h>
#include <sound/openal_context.h>

/* Constants */

#define MAX_SIMULTANEOUS_SOUNDS		32


/* SoundThread methods */

//
// Constructor
//
SoundThread::SoundThread()
	: QThread(), m_StopFlag(false)
{
	m_SoundSources.reserve(MAX_SIMULTANEOUS_SOUNDS);
	for (int i = 0; i < MAX_SIMULTANEOUS_SOUNDS; i++)
	{
		SoundSource src;

		src.source = openal()->createSource();
		openal()->setSourceOption(src.source, AL_PITCH, 1.0f);
		openal()->setSourceOption(src.source, AL_GAIN, 1.0f);
		openal()->setSourceOption(src.source, AL_VELOCITY, Vector3(0.0f, 0.0f, 0.0f));

		src.startTime = -1.0f;
		src.stopTime = -1.0f;
		m_SoundSources.push_back(src);
	}
}

//
// Destructor
//
SoundThread::~SoundThread()
{
	stop();
}

//
// Starts the thread
//
void SoundThread::start()
{
	m_StopFlag = false;
	QThread::start();
}

//
// Stops the thread
//
void SoundThread::stop()
{
	m_StopFlag = true;
	m_Signal.signal();
	wait();
}

//
// Thread body
//
void SoundThread::run()
{
	Command cmd;

	// Read options
	int timeout = 1000;//settings.value("sound/thread_sleep_time", 1000).toInt();
	int musicVolume = 100;//settings.value("sound/music_volume", 100).toInt();

	// Convert volume
	musicVolume = Q_MIN(Q_MAX(musicVolume, 0), 100);
	m_MusicVolume = (float)musicVolume / 100.0f;

	// Processing loop
	for (;;)
	{
		// Wait for event
		m_Signal.wait(timeout);

		// Check the quit flag
		if (unlikely(m_StopFlag))
			break;

		// Free unused sound sources
		double time = clock()->getCurrentTime();
		for (SoundSources::iterator jt = m_SoundSources.begin(); jt != m_SoundSources.end(); ++jt)
		{
			if (jt->stopTime > 0.0f && jt->stopTime <= time)
			{
				openal()->stopSource(jt->source);
				jt->startTime = -1.0f;
				jt->stopTime = -1.0f;
				break;
			}
		}

		// Process queue commands
		while (m_Queue.try_pop(cmd))
		{
			switch (cmd.command)
			{
			case CMD_PlayMusic:
				playMusicTrack(cmd.path);
				break;

			case CMD_StopMusic:
				// FIXME: not implemented
				break;

			case CMD_LoadSound:
				loadSoundSample(cmd.path);
				break;

			case CMD_PlaySound:
				playSoundSample(cmd.path, cmd.position);
				break;
			}
		}

		// Update music streams
		for (StreamsHash::iterator it = m_MusicTracks.begin(); it != m_MusicTracks.end(); )
		{
			MusicTrack & track = it->second;
			bool setvol = false;

			switch (track.playMode)
			{
			case NormalPlay:
				break;

			case FadeIn:
				track.volume += 0.2f;
				if (track.volume >= 1.0f)
				{
					track.volume = 1.0f;
					track.playMode = NormalPlay;
				}
				setvol = true;
				break;

			case FadeOut:
				track.volume -= 0.1f;
				if (track.volume <= 0.0f)
				{
					track.stream->stop();
					m_MusicTracks.erase(it++);
					continue;
				}
				setvol = true;
				break;
			}

			if (setvol)
				track.stream->setVolume(track.volume * m_MusicVolume);

			if (!track.stream->playing())
				track.stream->play();
			else
				track.stream->update();

			++it;
		}
	}
}

//
// Plays the specified audio track
//
void SoundThread::playMusicTrack(const QString & name)
{
	logger << LOG_TRACE << "Playing music track \"%s\"" << name;

	StreamsHash::iterator it = m_MusicTracks.find(name);
	if (it != m_MusicTracks.end())
		it->second.playMode = FadeIn;
	else
	{
		MusicTrack info;

		info.playMode = FadeIn;
		info.volume = 0.0f;
		info.stream = new OggStream(name);

		info.stream->play();
		m_MusicTracks.insert(StreamsHash::value_type(name, info));
	}

	for (it = m_MusicTracks.begin(); it != m_MusicTracks.end(); ++it)
	{
		if (it->first != name)
			it->second.playMode = FadeOut;
	}
}

//
// Loads the specified sound
//
SoundThread::SamplesCache::iterator SoundThread::loadSoundSample(const QString & name)
{
	SamplesCache::iterator it = m_SoundSamples.find(name);
	if (unlikely(it == m_SoundSamples.end()))
	{
		SoundSample sample;

		try
		{
			SoundFile file(name);
			sample.duration = file.duration();
			sample.buffer = file.createBuffer();
		}
		catch (std::exception & e)
		{
			logger << LOG_ERROR << "%s" << QString::fromLocal8Bit(e.what());
			sample.duration = 0.0f;
			sample.buffer = 0;
		}

		std::pair<SamplesCache::iterator, bool> r
			= m_SoundSamples.insert(SamplesCache::value_type(name, sample));
		it = r.first;
	}

	return it;
}

//
// Plays the specified sound
//
void SoundThread::playSoundSample(const QString & name, const Vector3 & pos)
{
	logger << LOG_TRACE << "Playing \"%s\"" << name;

	SamplesCache::iterator it = loadSoundSample(name);

	if (unlikely(it->second.duration <= 0.0f))
		return;

	double time = clock()->getCurrentTime();
	for (SoundSources::iterator jt = m_SoundSources.begin(); jt != m_SoundSources.end(); ++jt)
	{
		if (jt->stopTime <= time)
		{
			openal()->stopSource(jt->source);
			openal()->setSourceOption(jt->source, AL_BUFFER, it->second.buffer);
			openal()->setSourceOption(jt->source, AL_POSITION, pos);
			openal()->setSourceOption(jt->source, AL_LOOPING, false);
			openal()->playSource(jt->source);

			jt->startTime = time;
			jt->stopTime = time + it->second.duration;

			return;
		}
	}

	logger << LOG_NOTICE << "No free sound channels!";
}
