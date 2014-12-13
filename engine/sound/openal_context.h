#ifndef __SOUND__OPENAL_CONTEXT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __SOUND__OPENAL_CONTEXT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <math/vector3.h>

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <AL/alext.h>


/* Macros */

#ifndef ALchar
 #define ALchar char
#endif

#ifndef ALCchar
 #define ALCchar char
#endif

#define openal() (OpenALContext::instance())


/* Types */

typedef void (ALAPIENTRY * PFNALENABLE)(ALenum capability);
typedef void (ALAPIENTRY * PFNALDISABLE)(ALenum capability); 
typedef ALboolean (ALAPIENTRY * PFNALISENABLED)(ALenum capability); 
typedef const ALchar * (ALAPIENTRY * PFNALGETSTRING)(ALenum param);
typedef void (ALAPIENTRY * PFNALGETBOOLEANV)(ALenum param, ALboolean * data);
typedef void (ALAPIENTRY * PFNALGETINTEGERV)(ALenum param, ALint * data);
typedef void (ALAPIENTRY * PFNALGETFLOATV)(ALenum param, ALfloat * data);
typedef void (ALAPIENTRY * PFNALGETDOUBLEV)(ALenum param, ALdouble * data);
typedef ALboolean (ALAPIENTRY * PFNALGETBOOLEAN)(ALenum param);
typedef ALint (ALAPIENTRY * PFNALGETINTEGER)(ALenum param);
typedef ALfloat (ALAPIENTRY * PFNALGETFLOAT)(ALenum param);
typedef ALdouble (ALAPIENTRY * PFNALGETDOUBLE)(ALenum param);
typedef ALenum (ALAPIENTRY * PFNALGETERROR)(void);
typedef ALboolean (ALAPIENTRY * PFNALISEXTENSIONPRESENT)(const ALchar * extname);
typedef void * (ALAPIENTRY * PFNALGETPROCADDRESS)(const ALchar * fname);
typedef ALenum (ALAPIENTRY * PFNALGETENUMVALUE)(const ALchar * ename);
typedef void (ALAPIENTRY * PFNALLISTENERF)(ALenum param, ALfloat value);
typedef void (ALAPIENTRY * PFNALLISTENER3F)(ALenum param, ALfloat value1, ALfloat value2, ALfloat value3);
typedef void (ALAPIENTRY * PFNALLISTENERFV)(ALenum param, const ALfloat * values);
typedef void (ALAPIENTRY * PFNALLISTENERI)(ALenum param, ALint value);
typedef void (ALAPIENTRY * PFNALGETLISTENERF)(ALenum param, ALfloat * value);
typedef void (ALAPIENTRY * PFNALGETLISTENER3F)(ALenum param, ALfloat * value1, ALfloat * value2, ALfloat * value3);
typedef void (ALAPIENTRY * PFNALGETLISTENERFV)(ALenum param, ALfloat * values);
typedef void (ALAPIENTRY * PFNALGETLISTENERI)(ALenum param, ALint * value);
typedef void (ALAPIENTRY * PFNALGENSOURCES)(ALsizei n, ALuint * sources); 
typedef void (ALAPIENTRY * PFNALDELETESOURCES)(ALsizei n, const ALuint * sources);
typedef ALboolean (ALAPIENTRY * PFNALISSOURCE)(ALuint sid); 
typedef void (ALAPIENTRY * PFNALSOURCEF)(ALuint sid, ALenum param, ALfloat value); 
typedef void (ALAPIENTRY * PFNALSOURCE3F)(ALuint sid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3);
typedef void (ALAPIENTRY * PFNALSOURCEFV)(ALuint sid, ALenum param, const ALfloat * values);
typedef void (ALAPIENTRY * PFNALSOURCEI)(ALuint sid, ALenum param, ALint value); 
typedef void (ALAPIENTRY * PFNALGETSOURCEF)(ALuint sid, ALenum param, ALfloat * value);
typedef void (ALAPIENTRY * PFNALGETSOURCE3F)(ALuint sid, ALenum param, ALfloat * value1, ALfloat* value2, ALfloat* value3);
typedef void (ALAPIENTRY * PFNALGETSOURCEFV)(ALuint sid, ALenum param, ALfloat * values);
typedef void (ALAPIENTRY * PFNALGETSOURCEI)(ALuint sid, ALenum param, ALint * value);
typedef void (ALAPIENTRY * PFNALSOURCEPLAYV)(ALsizei ns, const ALuint * sids);
typedef void (ALAPIENTRY * PFNALSOURCESTOPV)(ALsizei ns, const ALuint * sids);
typedef void (ALAPIENTRY * PFNALSOURCEREWINDV)(ALsizei ns, const ALuint * sids);
typedef void (ALAPIENTRY * PFNALSOURCEPAUSEV)(ALsizei ns, const ALuint * sids);
typedef void (ALAPIENTRY * PFNALSOURCEPLAY)(ALuint sid);
typedef void (ALAPIENTRY * PFNALSOURCESTOP)(ALuint sid);
typedef void (ALAPIENTRY * PFNALSOURCEREWIND)(ALuint sid);
typedef void (ALAPIENTRY * PFNALSOURCEPAUSE)(ALuint sid);
typedef void (ALAPIENTRY * PFNALSOURCEQUEUEBUFFERS)(ALuint sid, ALsizei numEntries, const ALuint * bids);
typedef void (ALAPIENTRY * PFNALSOURCEUNQUEUEBUFFERS)(ALuint sid, ALsizei numEntries, ALuint * bids);
typedef void (ALAPIENTRY * PFNALGENBUFFERS)(ALsizei n, ALuint * buffers);
typedef void (ALAPIENTRY * PFNALDELETEBUFFERS)(ALsizei n, const ALuint * buffers);
typedef ALboolean (ALAPIENTRY * PFNALISBUFFER)(ALuint bid);
typedef void (ALAPIENTRY * PFNALBUFFERDATA)(ALuint bid, ALenum format, const ALvoid * data, ALsizei size, ALsizei freq);
typedef void (ALAPIENTRY * PFNALGETBUFFERF)(ALuint bid, ALenum param, ALfloat * value);
typedef void (ALAPIENTRY * PFNALGETBUFFERI)(ALuint bid, ALenum param, ALint * value);
typedef void (ALAPIENTRY * PFNALDOPPLERFACTOR)(ALfloat value);
typedef void (ALAPIENTRY * PFNALDOPPLERVELOCITY)(ALfloat value);
typedef void (ALAPIENTRY * PFNALDISTANCEMODEL)(ALenum distanceModel);

typedef ALCcontext * (ALCAPIENTRY * PFNALCCREATECONTEXT) (ALCdevice * device, const ALCint * attrlist);
typedef ALCboolean (ALCAPIENTRY * PFNALCMAKECONTEXTCURRENT)(ALCcontext * context);
typedef void (ALCAPIENTRY * PFNALCPROCESSCONTEXT)(ALCcontext * context);
typedef void (ALCAPIENTRY * PFNALCSUSPENDCONTEXT)(ALCcontext * context);
typedef void (ALCAPIENTRY * PFNALCDESTROYCONTEXT)(ALCcontext * context);
typedef ALCcontext * (ALCAPIENTRY * PFNALCGETCURRENTCONTEXT)(void);
typedef ALCdevice * (ALCAPIENTRY * PFNALCGETCONTEXTSDEVICE)(ALCcontext * context);
typedef ALCdevice * (ALCAPIENTRY * PFNALCOPENDEVICE)(const ALCchar * devicename);
typedef ALCboolean (ALCAPIENTRY * PFNALCCLOSEDEVICE)(ALCdevice * device);
typedef ALCenum (ALCAPIENTRY * PFNALCGETERROR)(ALCdevice * device);
typedef ALCboolean (ALCAPIENTRY * PFNALCISEXTENSIONPRESENT)(ALCdevice * device, const ALCchar * extname);
typedef void * (ALCAPIENTRY * PFNALCGETPROCADDRESS)(ALCdevice * device, const ALCchar * funcname);
typedef ALCenum (ALCAPIENTRY * PFNALCGETENUMVALUE)(ALCdevice * device, const ALCchar * enumname);
typedef const ALCchar * (ALCAPIENTRY * PFNALCGETSTRING)(ALCdevice * device, ALCenum param);
typedef void (ALCAPIENTRY * PFNALCGETINTEGERV)(ALCdevice * device, ALCenum param, ALCsizei size, ALCint * dest);


/* Classes */

/** OpenAL library context */
class OpenALContext : public QLibrary
{
	Q_OBJECT

public:
	PFNALENABLE alEnable;
	PFNALDISABLE alDisable;
	PFNALISENABLED alIsEnabled;
	PFNALGETBOOLEAN alGetBoolean;
	PFNALGETINTEGER alGetInteger;
	PFNALGETFLOAT alGetFloat;
	PFNALGETDOUBLE alGetDouble;
	PFNALGETBOOLEANV alGetBooleanv;
	PFNALGETINTEGERV alGetIntegerv;
	PFNALGETFLOATV alGetFloatv;
	PFNALGETDOUBLEV alGetDoublev;
	PFNALGETSTRING alGetString;
	PFNALGETERROR alGetError;
	PFNALISEXTENSIONPRESENT alIsExtensionPresent;
	PFNALGETPROCADDRESS alGetProcAddress;
	PFNALGETENUMVALUE alGetEnumValue;
	PFNALLISTENERI alListeneri;
	PFNALLISTENERF alListenerf;
	PFNALLISTENER3F alListener3f;
	PFNALLISTENERFV alListenerfv;
	PFNALGETLISTENERI alGetListeneri;
	PFNALGETLISTENERF alGetListenerf;
	PFNALGETLISTENER3F alGetListener3f;
	PFNALGETLISTENERFV alGetListenerfv;
	PFNALGENSOURCES alGenSources;
	PFNALDELETESOURCES alDeleteSources;
	PFNALISSOURCE alIsSource;
	PFNALSOURCEI alSourcei;
	PFNALSOURCEF alSourcef;
	PFNALSOURCE3F alSource3f;
	PFNALSOURCEFV alSourcefv;
	PFNALGETSOURCEI alGetSourcei;
	PFNALGETSOURCEF alGetSourcef;
	PFNALGETSOURCEFV alGetSourcefv;
	PFNALSOURCEPLAYV alSourcePlayv;
	PFNALSOURCESTOPV alSourceStopv;
	PFNALSOURCEPLAY alSourcePlay;
	PFNALSOURCEPAUSE alSourcePause;
	PFNALSOURCESTOP alSourceStop;
	PFNALGENBUFFERS alGenBuffers;
	PFNALDELETEBUFFERS alDeleteBuffers;
	PFNALISBUFFER alIsBuffer;
	PFNALBUFFERDATA alBufferData;
	PFNALGETBUFFERI alGetBufferi;
	PFNALGETBUFFERF alGetBufferf;
	PFNALSOURCEQUEUEBUFFERS alSourceQueueBuffers;
	PFNALSOURCEUNQUEUEBUFFERS alSourceUnqueueBuffers;
	PFNALDISTANCEMODEL alDistanceModel;
	PFNALDOPPLERFACTOR alDopplerFactor;
	PFNALDOPPLERVELOCITY alDopplerVelocity;
	PFNALCGETSTRING alcGetString;
	PFNALCGETINTEGERV alcGetIntegerv;
	PFNALCOPENDEVICE alcOpenDevice;
	PFNALCCLOSEDEVICE alcCloseDevice;
	PFNALCCREATECONTEXT alcCreateContext;
	PFNALCMAKECONTEXTCURRENT alcMakeContextCurrent;
	PFNALCPROCESSCONTEXT alcProcessContext;
	PFNALCGETCURRENTCONTEXT alcGetCurrentContext;
	PFNALCGETCONTEXTSDEVICE alcGetContextsDevice;
	PFNALCSUSPENDCONTEXT alcSuspendContext;
	PFNALCDESTROYCONTEXT alcDestroyContext;
	PFNALCGETERROR alcGetError;
	PFNALCISEXTENSIONPRESENT alcIsExtensionPresent;
	PFNALCGETPROCADDRESS alcGetProcAddress;
	PFNALCGETENUMVALUE alcGetEnumValue;

	/** Constructor. */
	OpenALContext();

	/**
	 * Constructor.
	 * @param name Name of the library.
	 */
	OpenALContext(const QString & name);

	/** Destructor. */
	~OpenALContext();

	/** Returns pointer to the instance of OpenAL context. */
	static inline OpenALContext * instance()
	{
		Q_ASSERT(m_Instance);
		return m_Instance;
	}

	/** Returns list of installed devices. */
//	QVector<ISoundPlugin::DeviceInfo> enumerateDevices() const;

	/** Returns name of the default sound device. */
	QByteArray defaultDevice() const;

	/**
	 * Initializes the specified device.
     * @param name Name of the device.
     */
    void initDevice(const char * name);

    /** Shuts down the currently initialized device. */
    void shutdownDevice();

    /** Creates the sound buffer. */
    inline ALuint createBuffer()
	{
		ALuint handle = 0;

		alGenBuffers(1, &handle);
		validateCall("alGenBuffers");

		return handle;
	}

    /**
     * Destroys the sound buffer.
     * @param handle Handle of the sound buffer.
     */
    inline void destroyBuffer(ALuint handle)
    {
    	alDeleteBuffers(1, &handle);
		validateCall("alDeleteBuffers");
    }

    /**
	 * Sets sound buffer data.
     * @param handle Handle of the sound buffer.
	 * @param data Pointer to the data.
	 * @param size Size of the data.
	 * @param rate Rate (in Hz).
	 * @param stereo If true, buffer contains stereo data.
	 */
	inline void setBufferData(ALuint handle, const void * data, uint size, uint rate, bool stereo)
	{
		alBufferData(handle, stereo ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data, size, rate);
		validateCall("alBufferData");
	}

    /** Creates the sound source. */
    inline ALuint createSource()
    {
		ALuint handle = 0;

		alGenSources(1, &handle);
		validateCall("alGenSources");

		return handle;
    }

    /**
     * Destroys the sound source.
     * @param handle Handle of the sound source.
     */
    inline void destroySource(ALuint handle)
    {
	    alDeleteSources(1, &handle);
		validateCall("alDeleteSources");
    }

    /**
     * Sets specified option of the sound source.
     * @param handle Handle of the sound source.
     * @param option ID of the option to set.
     * @param value Value to set.
     */
    inline void setSourceOption(ALuint handle, ALenum option, const Vector3 & value)
    {
		alSourcefv(handle, option, value.ptr());
		validateCall("alSourcefv");
    }

    /**
     * Sets specified option of the sound source.
     * @param handle Handle of the sound source.
     * @param option ID of the option to set.
     * @param value Value to set.
     */
    inline void setSourceOption(ALuint handle, ALenum option, float value)
    {
		alSourcef(handle, option, value);
		validateCall("alSourcef");
    }

    /**
     * Sets specified option of the sound source.
     * @param handle Handle of the sound source.
     * @param option ID of the option to set.
     * @param value Value to set.
     */
    inline void setSourceOption(ALuint handle, ALenum option, bool value)
    {
		alSourcei(handle, option, value ? AL_TRUE : AL_FALSE);
		validateCall("alSourcei");
    }

    /**
     * Sets specified option of the sound source.
     * @param handle Handle of the sound source.
     * @param option ID of the option to set.
     * @param value Value to set.
     */
    inline void setSourceOption(ALuint handle, ALenum option, int value)
    {
		alSourcei(handle, option, value);
		validateCall("alSourcei");
    }

    /**
     * Sets specified option of the sound source.
     * @param handle Handle of the sound source.
     * @param option ID of the option to set.
     * @param value Value to set.
     */
    inline void setSourceOption(ALuint handle, ALenum option, ALuint value)
    {
		alSourcei(handle, option, value);
		validateCall("alSourcei");
    }

    /**
     * Returns current state of the sound source.
     * @param handle Handle of the sound source.
     */
    inline ALenum getSourceState(ALuint handle) const
    {
		ALenum state = -1;

		alGetSourcei(handle, AL_SOURCE_STATE, &state);
		validateCall("alGetSourcei");

		return state;
	}

	/**
	 * Returns number of sound buffers that has been queued by the sound source.
	 * @param handle Handle of the sound source.
	 */
	inline int getSourceBuffersQueued(ALuint handle) const
	{
		int queued = 0;

		alGetSourcei(handle, AL_BUFFERS_QUEUED, &queued);
		validateCall("alGetSourcei");

		return queued;
	}	

	/**
	 * Returns number of sound buffers that has been processed by the sound source.
	 * @param handle Handle of the sound source.
	 */
	inline int getSourceBuffersProcessed(ALuint handle) const
	{
		int processed = 0;

		alGetSourcei(handle, AL_BUFFERS_PROCESSED, &processed);
		validateCall("alGetSourcei");

		return processed;
	}	

    /**
     * Starts playing of the specified source.
     * @param handle Handle of the sound source.
     */
    inline void playSource(ALuint handle)
    {
	   	alSourcePlay(handle);
		validateCall("alSourcePlay");
	}	

    /**
     * Stops playing of the specified source.
     * @param handle Handle of the sound source.
     */
    inline void stopSource(ALuint handle)
    {
		alSourceStop(handle);
		validateCall("alSourceStop");
	}

	/**
	 * Removes any queued buffers from the sound source.
	 * @param handle Handle of the sound source.
	 */
	inline void clearSourceBuffersQueue(ALuint handle)
	{
		int queued = getSourceBuffersQueued(handle);
		while (queued--)
		{
			ALuint buffer;

			alSourceUnqueueBuffers(handle, 1, &buffer);
			validateCall("alSourceUnqueueBuffers");
		}
	}

	/**
	 * Sets volume of the sound source.
	 * @param handle Handle of the sound source.
	 * @param volume New value of the volume.
	 */
	inline void setSourceVolume(ALuint handle, float volume)
	{
		if (unlikely(volume < 0.0f))
			volume = 0.0f;
		if (unlikely(volume > 1.0f))
			volume = 1.0f;

		alSourcef(handle, AL_GAIN, volume);
		validateCall("alSourcef");
	}

	/**
	 * Returns name of OpenAL error.
	 * @param code Error code.
	 */
	QString getErrorDescription(int code) const;

	/**
	 * Checks for OpenAL error and logs message if it happened.
	 * @param name Name of the OpenAL function.
	 * @returns True if no error has occured, false otherwise.
	 */
	bool validateCall(const char * name) const;

private:
	static OpenALContext * m_Instance;	/**< Pointer to the instance of the context. */
	ALCdevice * m_Device;				/**< Pointer to the device. */
	ALCcontext * m_Context;				/**< Pointer to the OpenAL context. */

	/**
	 * Retrieves address of the specified function.
	 * @param name Name of the function.
	 */
	void * getProcAddress(const char * name);

	/** Resets OpenAL entry points to NULL. */
	void resetEntryPointsToNull();
};

#endif // __SOUND__OPENAL_CONTEXT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
