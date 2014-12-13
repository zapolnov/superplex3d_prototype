#include <core/exception.h>
#include <core/logger.h>
#include "openal_context.h"

/* OpenALContext variables */

OpenALContext * OpenALContext::m_Instance;


/* OpenALContext methods */

//
// Constructor
//
OpenALContext::OpenALContext()
	: QLibrary(""), m_Device(NULL), m_Context(NULL)
{
  #ifndef __APPLE__
	resetEntryPointsToNull();
  #else
	// On APPLE we use the native OpenAL library and link to it statically
	alEnable = ::alEnable;
	alDisable = ::alDisable;
	alIsEnabled = ::alIsEnabled;
	alGetBoolean = ::alGetBoolean;
	alGetInteger = ::alGetInteger;
	alGetFloat = ::alGetFloat;
	alGetDouble = ::alGetDouble;
	alGetBooleanv = ::alGetBooleanv;
	alGetIntegerv = ::alGetIntegerv;
	alGetFloatv = ::alGetFloatv;
	alGetDoublev = ::alGetDoublev;
	alGetString = ::alGetString;
	alGetError = ::alGetError;
	alIsExtensionPresent = ::alIsExtensionPresent;
	alGetProcAddress = ::alGetProcAddress;
	alGetEnumValue = ::alGetEnumValue;
	alListeneri = ::alListeneri;
	alListenerf = ::alListenerf;
	alListener3f = ::alListener3f;
	alListenerfv = ::alListenerfv;
	alGetListeneri = ::alGetListeneri;
	alGetListenerf = ::alGetListenerf;
	alGetListener3f = ::alGetListener3f;
	alGetListenerfv = ::alGetListenerfv;
	alGenSources = ::alGenSources;
	alDeleteSources = ::alDeleteSources;
	alIsSource = ::alIsSource;
	alSourcei = ::alSourcei;
	alSourcef = ::alSourcef;
	alSource3f = ::alSource3f;
	alSourcefv = ::alSourcefv;
	alGetSourcei = ::alGetSourcei;
	alGetSourcef = ::alGetSourcef;
	alGetSourcefv = ::alGetSourcefv;
	alSourcePlayv = ::alSourcePlayv;
	alSourceStopv = ::alSourceStopv;
	alSourcePlay = ::alSourcePlay;
	alSourcePause = ::alSourcePause;
	alSourceStop = ::alSourceStop;
	alGenBuffers = ::alGenBuffers;
	alDeleteBuffers = ::alDeleteBuffers;
	alIsBuffer = ::alIsBuffer;
	alBufferData = ::alBufferData;
	alGetBufferi = ::alGetBufferi;
	alGetBufferf = ::alGetBufferf;
	alSourceQueueBuffers = ::alSourceQueueBuffers;
	alSourceUnqueueBuffers = ::alSourceUnqueueBuffers;
	alDistanceModel = ::alDistanceModel;
	alDopplerFactor = ::alDopplerFactor;
	alDopplerVelocity = ::alDopplerVelocity;
	alcGetString = ::alcGetString;
	alcGetIntegerv = ::alcGetIntegerv;
	alcOpenDevice = ::alcOpenDevice;
	alcCloseDevice = ::alcCloseDevice;
	alcCreateContext = ::alcCreateContext;
	alcMakeContextCurrent = ::alcMakeContextCurrent;
	alcProcessContext = ::alcProcessContext;
	alcGetCurrentContext = ::alcGetCurrentContext;
	alcGetContextsDevice = ::alcGetContextsDevice;
	alcSuspendContext = ::alcSuspendContext;
	alcDestroyContext = ::alcDestroyContext;
	alcGetError = ::alcGetError;
	alcIsExtensionPresent = ::alcIsExtensionPresent;
	alcGetProcAddress = ::alcGetProcAddress;
	alcGetEnumValue = ::alcGetEnumValue;
  #endif

	m_Instance = this;
}

//
// Constructor
//
OpenALContext::OpenALContext(const QString & name)
	: QLibrary(name), m_Device(NULL), m_Context(NULL)
{
	Q_ASSERT(!m_Instance);

	resetEntryPointsToNull();

	if (unlikely(!load()))
	{
		logger << LOG_INFO << "%s" << errorString();
		m_Instance = this;
		return;
	}

	try
	{
		alEnable = (PFNALENABLE)getProcAddress("alEnable");
		alDisable = (PFNALDISABLE)getProcAddress("alDisable");
		alIsEnabled = (PFNALISENABLED)getProcAddress("alIsEnabled");
		alGetBoolean = (PFNALGETBOOLEAN)getProcAddress("alGetBoolean");
		alGetInteger = (PFNALGETINTEGER)getProcAddress("alGetInteger");
		alGetFloat = (PFNALGETFLOAT)getProcAddress("alGetFloat");
		alGetDouble = (PFNALGETDOUBLE)getProcAddress("alGetDouble");
		alGetBooleanv = (PFNALGETBOOLEANV)getProcAddress("alGetBooleanv");
		alGetIntegerv = (PFNALGETINTEGERV)getProcAddress("alGetIntegerv");
		alGetFloatv = (PFNALGETFLOATV)getProcAddress("alGetFloatv");
		alGetDoublev = (PFNALGETDOUBLEV)getProcAddress("alGetDoublev");
		alGetString = (PFNALGETSTRING)getProcAddress("alGetString");
		alGetError = (PFNALGETERROR)getProcAddress("alGetError");
		alIsExtensionPresent = (PFNALISEXTENSIONPRESENT)getProcAddress("alIsExtensionPresent");
		alGetProcAddress = (PFNALGETPROCADDRESS)getProcAddress("alGetProcAddress");
		alGetEnumValue = (PFNALGETENUMVALUE)getProcAddress("alGetEnumValue");
		alListeneri = (PFNALLISTENERI)getProcAddress("alListeneri");
		alListenerf = (PFNALLISTENERF)getProcAddress("alListenerf");
		alListener3f = (PFNALLISTENER3F)getProcAddress("alListener3f");
		alListenerfv = (PFNALLISTENERFV)getProcAddress("alListenerfv");
		alGetListeneri = (PFNALGETLISTENERI)getProcAddress("alGetListeneri");
		alGetListenerf = (PFNALGETLISTENERF)getProcAddress("alGetListenerf");
		alGetListener3f = (PFNALGETLISTENER3F)getProcAddress("alGetListener3f");
		alGetListenerfv = (PFNALGETLISTENERFV)getProcAddress("alGetListenerfv");
		alGenSources = (PFNALGENSOURCES)getProcAddress("alGenSources");
		alDeleteSources = (PFNALDELETESOURCES)getProcAddress("alDeleteSources");
		alIsSource = (PFNALISSOURCE)getProcAddress("alIsSource");
		alSourcei = (PFNALSOURCEI)getProcAddress("alSourcei");
		alSourcef = (PFNALSOURCEF)getProcAddress("alSourcef");
		alSource3f = (PFNALSOURCE3F)getProcAddress("alSource3f");
		alSourcefv = (PFNALSOURCEFV)getProcAddress("alSourcefv");
		alGetSourcei = (PFNALGETSOURCEI)getProcAddress("alGetSourcei");
		alGetSourcef = (PFNALGETSOURCEF)getProcAddress("alGetSourcef");
		alGetSourcefv = (PFNALGETSOURCEFV)getProcAddress("alGetSourcefv");
		alSourcePlayv = (PFNALSOURCEPLAYV)getProcAddress("alSourcePlayv");
		alSourceStopv = (PFNALSOURCESTOPV)getProcAddress("alSourceStopv");
		alSourcePlay = (PFNALSOURCEPLAY)getProcAddress("alSourcePlay");
		alSourcePause = (PFNALSOURCEPAUSE)getProcAddress("alSourcePause");
		alSourceStop = (PFNALSOURCESTOP)getProcAddress("alSourceStop");
		alGenBuffers = (PFNALGENBUFFERS)getProcAddress("alGenBuffers");
		alDeleteBuffers = (PFNALDELETEBUFFERS)getProcAddress("alDeleteBuffers");
		alIsBuffer = (PFNALISBUFFER)getProcAddress("alIsBuffer");
		alBufferData = (PFNALBUFFERDATA)getProcAddress("alBufferData");
		alGetBufferi = (PFNALGETBUFFERI)getProcAddress("alGetBufferi");
		alGetBufferf = (PFNALGETBUFFERF)getProcAddress("alGetBufferf");
		alSourceQueueBuffers = (PFNALSOURCEQUEUEBUFFERS)getProcAddress("alSourceQueueBuffers");
		alSourceUnqueueBuffers = (PFNALSOURCEUNQUEUEBUFFERS)getProcAddress("alSourceUnqueueBuffers");
		alDistanceModel = (PFNALDISTANCEMODEL)getProcAddress("alDistanceModel");
		alDopplerFactor = (PFNALDOPPLERFACTOR)getProcAddress("alDopplerFactor");
		alDopplerVelocity = (PFNALDOPPLERVELOCITY)getProcAddress("alDopplerVelocity");
		alcGetString = (PFNALCGETSTRING)getProcAddress("alcGetString");
		alcGetIntegerv = (PFNALCGETINTEGERV)getProcAddress("alcGetIntegerv");
		alcOpenDevice = (PFNALCOPENDEVICE)getProcAddress("alcOpenDevice");
		alcCloseDevice = (PFNALCCLOSEDEVICE)getProcAddress("alcCloseDevice");
		alcCreateContext = (PFNALCCREATECONTEXT)getProcAddress("alcCreateContext");
		alcMakeContextCurrent = (PFNALCMAKECONTEXTCURRENT)getProcAddress("alcMakeContextCurrent");
		alcProcessContext = (PFNALCPROCESSCONTEXT)getProcAddress("alcProcessContext");
		alcGetCurrentContext = (PFNALCGETCURRENTCONTEXT)getProcAddress("alcGetCurrentContext");
		alcGetContextsDevice = (PFNALCGETCONTEXTSDEVICE)getProcAddress("alcGetContextsDevice");
		alcSuspendContext = (PFNALCSUSPENDCONTEXT)getProcAddress("alcSuspendContext");
		alcDestroyContext = (PFNALCDESTROYCONTEXT)getProcAddress("alcDestroyContext");
		alcGetError = (PFNALCGETERROR)getProcAddress("alcGetError");
		alcIsExtensionPresent = (PFNALCISEXTENSIONPRESENT)getProcAddress("alcIsExtensionPresent");
		alcGetProcAddress = (PFNALCGETPROCADDRESS)getProcAddress("alcGetProcAddress");
		alcGetEnumValue = (PFNALCGETENUMVALUE)getProcAddress("alcGetEnumValue");
	}
	catch (std::exception &)
	{
		unload();
		throw;
	}

	m_Instance = this;
}

//
// Destructor
//
OpenALContext::~OpenALContext()
{
	Q_ASSERT(m_Instance);
	m_Instance = NULL;

	shutdownDevice();
}

//
// Returns name of the default sound device
//
QByteArray OpenALContext::defaultDevice() const
{
	if (!alcIsExtensionPresent)
		return QByteArray();

	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
		return alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
	else if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
		return alcGetString(NULL, ALC_DEVICE_SPECIFIER);

	return QByteArray();
}

//
// Initializes the specified device
//
void OpenALContext::initDevice(const char * name)
{
	ALCcontext * context = NULL;
	ALCdevice * device = NULL;

	if (!alcOpenDevice)
		throw Exception(tr("Missing OpenAL entry point."));

	try
	{
		device = alcOpenDevice(name);
		if (unlikely(!device))
			throw Exception(tr("Unable to open sound device '%1'").arg(QString::fromLocal8Bit(name)));

		context = alcCreateContext(device, NULL);
		if (unlikely(!context))
			throw Exception(tr("Unable to create OpenAL context"));

		shutdownDevice();

		if (!alcMakeContextCurrent(context))
			throw Exception(tr("Unable to activate the OpenAL context"));
	}
	catch (...)
	{
		if (context)
			alcDestroyContext(context);
		if (device)
			alcCloseDevice(device);

		throw;
	}

	m_Device = device;
	m_Context = context;

	logger << LOG_INFO << "Using OpenAL device: %s" << alcGetString(m_Device, ALC_DEVICE_SPECIFIER);
	logger << LOG_INFO << "OpenAL version: %s" << alGetString(AL_VERSION);
	logger << LOG_INFO << "OpenAL vendor: %s" << alGetString(AL_VENDOR);
	logger << LOG_INFO << "OpenAL renderer: %s" << alGetString(AL_RENDERER);
}

//
// Shuts down the currently initialized device
//
void OpenALContext::shutdownDevice()
{
	if (!alcMakeContextCurrent)
		return;

	alcMakeContextCurrent(NULL);

	if (m_Context)
	{
		alcDestroyContext(m_Context);
		m_Context = NULL;
	}	

	if (m_Device)
	{	
		alcCloseDevice(m_Device);
		m_Device = NULL;
	}
}

//
// Returns name of OpenAL error
//
QString OpenALContext::getErrorDescription(int code) const
{
	switch (code)
	{
	case AL_NO_ERROR:
		return "AL_NO_ERROR";
	case AL_INVALID_NAME:
		return "AL_INVALID_NAME";
	case AL_INVALID_ENUM:
		return "AL_INVALID_ENUM";
	case AL_INVALID_VALUE:
		return "AL_INVALID_VALUE";
	case AL_INVALID_OPERATION:
		return "AL_INVALID_OPERATION";
	case AL_OUT_OF_MEMORY:
		return "AL_OUT_OF_MEMORY";
	}

	char buf[16];
	snprintf(buf, sizeof(buf), "0x%04X", code);

	return buf;
}

//
// Checks for OpenAL error and logs message if it happened
//
bool OpenALContext::validateCall(const char * name) const
{
	if (!alGetError())
		return true;

	int error = alGetError();

	if (likely(error == AL_NO_ERROR))
		return true;

	logger << LOG_ERROR << "%s() error: %s" << name << getErrorDescription(error);

	return false;
}

//
// Retrieves address of the specified function
//
void * OpenALContext::getProcAddress(const char * name)
{
	void * addr = resolve(name);

	if (unlikely(!addr))
		throw Exception(tr("Symbol '%1' was not found in library '%2'.").arg(name).arg(fileName()));

	return addr;
}

//
// Resets OpenAL entry points to NULL
//
void OpenALContext::resetEntryPointsToNull()
{
	alEnable = NULL;
	alDisable = NULL;
	alIsEnabled = NULL;
	alGetBoolean = NULL;
	alGetInteger = NULL;
	alGetFloat = NULL;
	alGetDouble = NULL;
	alGetBooleanv = NULL;
	alGetIntegerv = NULL;
	alGetFloatv = NULL;
	alGetDoublev = NULL;
	alGetString = NULL;
	alGetError = NULL;
	alIsExtensionPresent = NULL;
	alGetProcAddress = NULL;
	alGetEnumValue = NULL;
	alListeneri = NULL;
	alListenerf = NULL;
	alListener3f = NULL;
	alListenerfv = NULL;
	alGetListeneri = NULL;
	alGetListenerf = NULL;
	alGetListener3f = NULL;
	alGetListenerfv = NULL;
	alGenSources = NULL;
	alDeleteSources = NULL;
	alIsSource = NULL;
	alSourcei = NULL;
	alSourcef = NULL;
	alSource3f = NULL;
	alSourcefv = NULL;
	alGetSourcei = NULL;
	alGetSourcef = NULL;
	alGetSourcefv = NULL;
	alSourcePlayv = NULL;
	alSourceStopv = NULL;
	alSourcePlay = NULL;
	alSourcePause = NULL;
	alSourceStop = NULL;
	alGenBuffers = NULL;
	alDeleteBuffers = NULL;
	alIsBuffer = NULL;
	alBufferData = NULL;
	alGetBufferi = NULL;
	alGetBufferf = NULL;
	alSourceQueueBuffers = NULL;
	alSourceUnqueueBuffers = NULL;
	alDistanceModel = NULL;
	alDopplerFactor = NULL;
	alDopplerVelocity = NULL;
	alcGetString = NULL;
	alcGetIntegerv = NULL;
	alcOpenDevice = NULL;
	alcCloseDevice = NULL;
	alcCreateContext = NULL;
	alcMakeContextCurrent = NULL;
	alcProcessContext = NULL;
	alcGetCurrentContext = NULL;
	alcGetContextsDevice = NULL;
	alcSuspendContext = NULL;
	alcDestroyContext = NULL;
	alcGetError = NULL;
	alcIsExtensionPresent = NULL;
	alcGetProcAddress = NULL;
	alcGetEnumValue = NULL;
}
