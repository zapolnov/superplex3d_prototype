#include <sound/functions.h>
#include <sound/openal_context.h>
#include <sound/thread.h>
#include <core/logger.h>

/* Private variables */

static OpenALContext * g_Context;		/**< Pointer to the OpenAL context. */
static SoundThread * g_Thread;			/**< Pointer to the sound thread. */


/* Public functions */

//
// Initializes the sound library
//
void SND_Init()
{
  #ifndef __APPLE__
	static const char * dlls[] = { "openal32", "openal", "pg_openal", NULL };

	for (int i = 0; dlls[i]; i++)
	{
		OpenALContext * context = NULL;

		try
		{
			context = new OpenALContext(dlls[i]);
			context->initDevice(NULL);
			g_Context = context;
			break;
		}
		catch (std::exception & e)
		{
			logger << LOG_ERROR << "%s" << QString::fromLocal8Bit(e.what());
			delete context;
		}
	}
  #endif

	if (!g_Context)
	{
		g_Context = new OpenALContext();
	  #ifdef __APPLE__
		g_Context->initDevice(NULL);
	  #endif
	}

	g_Thread = new SoundThread;
	g_Thread->start();
}

//
// Shuts down the sound library
//
void SND_Shutdown()
{
	delete g_Thread;
	g_Thread = NULL;

	delete g_Context;
	g_Context = NULL;
}

//
// Sets position of the listener
//
void SND_SetListenerPosition(const Vector3 & pos)
{
	if (!g_Context || !g_Thread)
		return;

	g_Context->alListenerfv(AL_POSITION, pos.ptr());
	g_Context->validateCall("alListenerfv");
}

//
// Sets orientation of the listener
//
void SND_SetListenerOrientation(const Vector3 & vec)
{
	if (!g_Context || !g_Thread)
		return;

	g_Context->alListenerfv(AL_ORIENTATION, vec.ptr());
	g_Context->validateCall("alListenerfv");
}

//
// Starts playing of the specified music track
//
void SND_PlayMusic(const QString & name)
{
	if (!g_Context || !g_Thread)
		return;

	g_Thread->playMusic(name);
}

//
// Stops playing of any music track that is currently playing
//
void SND_StopMusic()
{
	if (!g_Context || !g_Thread)
		return;
}

//
// Preloads the specified sound sample
//
void SND_PreloadSound(const QString & name)
{
	if (!g_Context || !g_Thread)
		return;

	g_Thread->loadSound(name);
}

//
// Plays the specified sound
//
void SND_PlaySound(const QString & name, const Vector3 & pos)
{
	if (!g_Context || !g_Thread)
		return;

	g_Thread->playSound(name, pos);
}
