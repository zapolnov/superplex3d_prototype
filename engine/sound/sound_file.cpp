#include <core/memory_file.h>
#include <core/logger.h>
#include <core/exception.h>
#include <interfaces/file_system.h>
#include <sound/openal_context.h>
#include <sound/riff.h>
#include <sound/sound_file.h>

/* SoundFile methods */

//
// Constructor
//
SoundFile::SoundFile(const QString & name)
	: m_Format(AL_FORMAT_MONO8), m_Frequency(0)
{
	QByteArray fileData = fileSystem()->getFileContents(name);

	MemoryFile file(name, fileData);
	file.open(MemoryFile::ReadOnly);

	if (fileData.length() > 4 && fileData[0] == 'R' && fileData[1] == 'I' &&
			fileData[2] == 'F' && fileData[3] == 'F')
		loadRiffWaveform(&file);
	else
		throw Exception(QString("File \"%1\" has unsupported format.").arg(name));
}

//
// Creates OpenAL buffer from the sound
//
ALuint SoundFile::createBuffer()
{
	ALuint handle = openal()->createBuffer();

	openal()->alBufferData(handle, m_Format, m_SoundData.constData(), m_SoundData.length(), m_Frequency);
	openal()->validateCall("alBufferData");

	return handle;
}

//
// Loads the RIFF waveform file
//
void SoundFile::loadRiffWaveform(MemoryFile * file)
{
	RiffFile riff(file->fileName(), file);
	RiffWaveFormat fmt;

	if (unlikely(riff.format() != RiffWave))
		throw Exception(QString("%1: Not a waveform audio file.").arg(file->fileName()));

	if (unlikely(!riff.seekToChunk(FOURCC_fmt)))
		throw Exception(QString("%1: Chunk 'fmt' was not found in the file.").arg(file->fileName()));
	if (unlikely(riff.currentChunkSize() < sizeof(fmt)))
		throw Exception(QString("%1: Chunk 'fmt' is too small.").arg(file->fileName()));
	if (unlikely(file->read((char *)&fmt, sizeof(fmt)) != sizeof(fmt)))
		throw Exception(QString("%1: I/O error.").arg(file->fileName()));

	const char * formatName;
	switch (fmt.wFormatTag)
	{
	case RiffWavePCM:
		formatName = "PCM";
		loadRiffWaveformPCM(file, fmt, riff);
		break;

	default:
		throw Exception(QString("%1: Unsupported data encoding.").arg(file->fileName()));
	}

	m_Duration = (double)m_SoundData.length()  * 8.0f / (double)bitsPerSample() / (double)m_Frequency;

	logger << LOG_INFO << "%s: %s, %dHz, %d channel(s), duration %.2f seconds"
		<< file->fileName() << formatName << m_Frequency << numChannels()
		<< m_Duration;
}

//
// Loads the RIFF waveform data in PCM format
//
void SoundFile::loadRiffWaveformPCM(MemoryFile * file, const RiffWaveFormat & fmt, RiffFile & riff)
{
	RiffWavePcmSpecific pcm;

	// Read PCM-specific header
	if (unlikely(riff.currentChunkSize() < sizeof(RiffWaveFormat) + sizeof(pcm)))
		throw Exception(QString("%1: Chunk 'fmt' is too small.").arg(file->fileName()));
	if (unlikely(file->read((char *)&pcm, sizeof(pcm)) != sizeof(pcm)))
		throw Exception(QString("%1: I/O error.").arg(file->fileName()));

	// Read waveform data
	if (unlikely(!riff.seekToChunk(FOURCC_data)))
		throw Exception(QString("%1: Chunk 'data' was not found in the file.").arg(file->fileName()));
	unsigned size = riff.currentChunkSize();
	m_SoundData.resize(size);
	if (unlikely(file->read(m_SoundData.data(), size) != size))
		throw Exception(QString("%1: I/O error.").arg(file->fileName()));

	// Determine file format
	if (pcm.wBitsPerSample == 8 && fmt.wChannels == 1)
		m_Format = AL_FORMAT_MONO8;
	else if (pcm.wBitsPerSample == 16 && fmt.wChannels == 1)
		m_Format = AL_FORMAT_MONO16;
	else if (pcm.wBitsPerSample == 8 && fmt.wChannels == 2)
		m_Format = AL_FORMAT_STEREO8;
	else if (pcm.wBitsPerSample == 16 && fmt.wChannels == 2)
		m_Format = AL_FORMAT_STEREO16;
	else
	{
		throw Exception(QString("File \"%1\" has unsupported bits/channels combination.")
			.arg(file->fileName()));
	}

	m_Frequency = fmt.dwSamplesPerSec;
}
