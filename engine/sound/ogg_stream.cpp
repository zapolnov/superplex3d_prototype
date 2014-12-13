#include <core/logger.h>
#include <sound/openal_context.h>
#include <sound/ogg_stream.h>
#include <interfaces/file_system.h>

/* OggStream methods */

//
// Constructor
//
OggStream::OggStream(const QString & file)
	: m_VorbisInfo(NULL), m_VorbisComment(NULL), m_Initialized(false)
{
	ov_callbacks cb;

	// Read settings
	//Settings settings;
	m_StreamBufferSize = 262144;//settings.value("sound/streaming_buffer_size", 262144).toInt();
	m_StreamBufferSize = Q_MIN(Q_MAX(m_StreamBufferSize, 10240), 1048576);

	// Open the file
	try
	{
		m_File = fileSystem()->openFile(file);
	}
	catch (std::exception & e)
	{
		logger << LOG_ERROR << "%s" << QString::fromLocal8Bit(e.what());
	}

	// Setup callback pointers
	memset(&cb, 0, sizeof(cb));
	cb.read_func = vorbisRead;
	cb.close_func = vorbisClose;
	cb.seek_func = vorbisSeek;
	cb.tell_func = vorbisTell;
	if (unlikely(ov_open_callbacks(this, &m_Stream, NULL, 0, cb) != 0))
		logger << LOG_ERROR << "%s: file processing failed" << file;
	else
	{
		// Collect file info
		m_VorbisInfo = ov_info(&m_Stream, -1);
		m_VorbisComment = ov_comment(&m_Stream, -1);

		// Display file information
		logger << LOG_INFO << "%s: %dHz, %d channel(s)" << file << m_VorbisInfo->rate << m_VorbisInfo->channels;

		// Setup audio format
		m_IsStereo = m_VorbisInfo->channels > 1;
		if (unlikely(m_VorbisInfo->channels > 2))
			logger << LOG_ERROR << "%s: Too many channels" << file;

		// Create the sound buffers
		m_SoundBuffers[0] = openal()->createBuffer();
		m_SoundBuffers[1] = openal()->createBuffer();

		// Create the sound source
		m_SoundSource = openal()->createSource();
		openal()->setSourceOption(m_SoundSource, AL_POSITION, Vector3(0.0f, 0.0f, 0.0f));
		openal()->setSourceOption(m_SoundSource, AL_VELOCITY, Vector3(0.0f, 0.0f, 0.0f));
		openal()->setSourceOption(m_SoundSource, AL_DIRECTION, Vector3(0.0f, 0.0f, 0.0f));
		openal()->setSourceOption(m_SoundSource, AL_ROLLOFF_FACTOR, 0.0f);
		openal()->setSourceOption(m_SoundSource, AL_SOURCE_RELATIVE, true);

		m_Initialized = true;
	}

	// Allocate memory
	m_StreamBuffer = new char[m_StreamBufferSize];
}

//
// Destructor
//
OggStream::~OggStream()
{
	stop();

	openal()->destroyBuffer(m_SoundBuffers[0]);
	openal()->destroyBuffer(m_SoundBuffers[1]);
	openal()->destroySource(m_SoundSource);

	ov_clear(&m_Stream);
	delete[] m_StreamBuffer;

	m_File.release();
}

//
// Starts the playback of the stream
//
bool OggStream::play()
{
	if (unlikely(!m_Initialized))
		return false;
	if (playing())
		return true;

	if (unlikely(!stream(m_SoundBuffers[0])))
		return false;
	if (unlikely(!stream(m_SoundBuffers[1])))
		return false;

	openal()->alSourceQueueBuffers(m_SoundSource, 2, m_SoundBuffers);
	openal()->validateCall("alSourceQueueBuffers");

	openal()->playSource(m_SoundSource);

    return true;
}

//
// Stops the playback of the stream
//
void OggStream::stop()
{
	if (unlikely(!m_Initialized))
		return;

	openal()->stopSource(m_SoundSource);
	openal()->clearSourceBuffersQueue(m_SoundSource);

	ov_time_seek(&m_Stream, 0.0f);
}

//
// Returns true if stream is currently playing
//
bool OggStream::playing()
{
	if (unlikely(!m_Initialized))
		return false;

	return openal()->getSourceState(m_SoundSource) == AL_PLAYING;
}

//
// Feeds sound buffers with a fresh data
//
bool OggStream::update()
{
	bool active = true;

	if (unlikely(!m_Initialized))
		return false;

	int processed = openal()->getSourceBuffersProcessed(m_SoundSource);
	while (processed--)
	{
		ALuint buffer;

		openal()->alSourceUnqueueBuffers(m_SoundSource, 1, &buffer);
		if (unlikely(!openal()->validateCall("alSourceUnqueueBuffers")))
			active = false;
		else if (unlikely(!stream(buffer)))
			active = false;
		else
		{
			openal()->alSourceQueueBuffers(m_SoundSource, 1, &buffer);
			openal()->validateCall("alSourceQueueBuffers");
		}
	}

	return active;
}

//
// Sets volume of the stream
//
void OggStream::setVolume(float volume)
{
	if (likely(m_Initialized))
		openal()->setSourceVolume(m_SoundSource, volume);
}

//
// Streams data from OGG into the OpenALbuffer
//
bool OggStream::stream(ALuint buffer)
{
	static bool errorReported = false;
	int size = 0, section, result;
	bool didSeek = false;

	while (size < m_StreamBufferSize)
	{
		result = ov_read(&m_Stream, m_StreamBuffer + size, m_StreamBufferSize - size, 0, 2, 1, &section);

		if (likely(result > 0))
		{
			errorReported = false;
			size += result;
		}
		else if (unlikely(result < 0))
		{
			if (!errorReported)
			{
				errorReported = true;
				if (unlikely(!m_File))
					logger << LOG_ERROR << "ov_read() error: %d" << result;
				else
					logger << LOG_ERROR << "%s: ov_read() error: %d" << m_File->fileName() << result;
			}
			stop();
		}
		else
		{
			errorReported = false;

			if (unlikely(didSeek))
				break;

			logger << LOG_NOTICE << "OggStream::stream(): Restarting from the beginning";
			result = ov_time_seek(&m_Stream, 0.0f);
			if (unlikely(result < 0))
			{
				logger << LOG_ERROR << "ov_time_seek() error: %d" << result;
				errorReported = true;
				break;
			}
			didSeek = true;
		}
	}

	if (size == 0)
	{
		stop();
		return false;
	}

	openal()->setBufferData(buffer, m_StreamBuffer, size, m_VorbisInfo->rate, m_IsStereo);

	return true;
}

//
// Callback from the libvorbis. Reads data from the input file
//
size_t OggStream::vorbisRead(void * ptr, size_t blockSize, size_t numBlocks, void * user)
{
	OggStream * self = (OggStream *)user;

	if (unlikely(!self->m_File))
		return 0;

	qint64 result = self->m_File->read((char *)ptr, (qint64)(blockSize * numBlocks));
	if (unlikely(result < 0))
	{
		logger << LOG_ERROR << "%s: read failed." << self->m_File->fileName();
		return 0;
	}

	return (size_t)result;
}

//
// Callback from the libvorbis. Closes the input file
//
int OggStream::vorbisClose(void *)
{
	return 1;
}

//
// Callback from the libvorbis. Moves file pointer in the input file
//
int OggStream::vorbisSeek(void * user, ogg_int64_t offset, int whence)
{
	OggStream * self = (OggStream *)user;
	qint64 off;

	if (unlikely(!self->m_File))
		return -1;

	switch (whence)
	{
	case SEEK_SET:
		off = offset;
		break;

	case SEEK_CUR:
		off = self->m_File->pos() + offset;
		break;

	case SEEK_END:
		off = self->m_File->size() + offset;
		break;

	default:
		return -1;
	}

	if (unlikely(!self->m_File->seek(off)))
	{
		logger << LOG_ERROR << "%s: seek failed." << self->m_File->fileName();
		return -1;
	}

	return 0;
}

//
// Callback from the libvorbis. Returns current position in the input file
//
long OggStream::vorbisTell(void * user)
{
	OggStream * self = (OggStream *)user;

	if (unlikely(!self->m_File))
		return -1;

	return (long)self->m_File->pos();
}
