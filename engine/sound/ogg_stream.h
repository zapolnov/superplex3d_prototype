#ifndef __SOUND__OGG_STREAM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __SOUND__OGG_STREAM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/trolltech.h>
#include <interfaces/file_system.h>
#include <vorbis/vorbisfile.h>
#include <AL/al.h>


/* Classes */

/** Ogg/Vorbis audio stream */
class OggStream : public QObject, public RefCounted
{
	Q_OBJECT

public:
	/**
	 * Constructor.
	 * @param file Path to the file.
	 */
	OggStream(const QString & file);

	/** Destructor. */
	~OggStream();

	/** Starts the playback of the stream. */
	bool play();

	/** Stops the playback of the stream. */
	void stop();

	/** Returns true if stream is currently playing. */
	bool playing();

	/** Feeds sound buffers with a fresh data. */
	bool update();

	/**
	 * Sets volume of the stream.
	 * @param volume Volume.
	 */
	void setVolume(float volume);

private:	
	FilePtr m_File;							/**< Handle of the file. */
	OggVorbis_File m_Stream;				/**< OGG/Vorbis stream. */
	vorbis_info * m_VorbisInfo;				/**< Vorbis information. */
	vorbis_comment * m_VorbisComment;		/**< Vorbis comment. */
	bool m_IsStereo;						/**< True if sound has stereo format. */
	ALuint m_SoundBuffers[2];				/**< Handles of sound buffers. */
	ALuint m_SoundSource;					/**< Handle of the sound source. */
	bool m_Initialized;						/**< True if initialization was successful. */
	char * m_StreamBuffer;					/**< Streaming buffer. */
	int m_StreamBufferSize;					/**< Size of the streaming buffer. */

	/**
	 * Streams data from OGG into the OpenAL buffer.
	 * @param buffer Buffer handle.
	 */
	bool stream(ALuint buffer);

	/**
	 * Callback from the libvorbis. Reads data from the input file.
	 * @param ptr Pointer to the destination buffer.
	 * @param blockSize Block size.
	 * @param numBlocks Number of blocks to read.
	 * @param user Pointer to the user-specific data.
	 */
	static size_t vorbisRead(void * ptr, size_t blockSize, size_t numBlocks, void * user);

	/**
	 * Callback from the libvorbis. Closes the input file.
	 * @param user Pointer to the user-specific data.
	 */
	static int vorbisClose(void * user);

	/**
	 * Callback from the libvorbis. Moves file pointer in the input file.
	 * @param user Pointer to the user-specific data.
	 * @param offset New offset in the file.
	 * @param whence Seek mode.
	 */
	static int vorbisSeek(void * user, ogg_int64_t offset, int whence);

	/**
	 * Callback from the libvorbis. Returns current position in the input file.
	 * @param user Pointer to the user-specific data.
	 */
	static long vorbisTell(void * user);
};

#endif // __SOUND__OGG_STREAM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
