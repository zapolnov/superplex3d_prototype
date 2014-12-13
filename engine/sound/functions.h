#ifndef __SOUND__FUNCTIONS_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __SOUND__FUNCTIONS_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <math/vector3.h>


/* Functions */

EXTERN_C_BEGIN

/** Initializes the sound library. */
SOUNDAPI void SND_Init();

/** Shuts down the sound library. */
SOUNDAPI void SND_Shutdown();

/**
 * Sets position of the listener.
 * @param pos Position of the listener.
 */
SOUNDAPI void SND_SetListenerPosition(const Vector3 & pos);

/**
 * Sets orientation of the listener.
 * @param vec Orientation of the listener.
 */
SOUNDAPI void SND_SetListenerOrientation(const Vector3 & vec);

/**
 * Starts playing of the specified music track.
 * @param name Name of the track
 */
SOUNDAPI void SND_PlayMusic(const QString & name);

/** Stops playing of any music track that is currently playing. */
SOUNDAPI void SND_StopMusic();

/**
 * Preloads the specified sound sample.
 * @param name Name of the sound file.
 */
SOUNDAPI void SND_PreloadSound(const QString & name);

/**
 * Plays the specified sound.
 * @param name Name of the sound file.
 * @param pos Position of the sound.
 */
SOUNDAPI void SND_PlaySound(const QString & name, const Vector3 & pos = Vector3(0.0f, 0.0f, 0.0f));

EXTERN_C_END

#endif // __SOUND__FUNCTIONS_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
