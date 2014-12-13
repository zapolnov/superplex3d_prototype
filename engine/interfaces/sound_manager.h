#ifndef __INTERFACES__SOUND_MANAGER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __INTERFACES__SOUND_MANAGER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/object.h>
#include <interfaces/sound_source.h>
#include <interfaces/sound_buffer.h>
#include <game/client.h>


/* Classes */

/** Interface to the sound manager. */
class ISoundManager
{
public:
	/**
	 * Starts playing the specified track. If another track is playing, it will be smoothly replaced.
	 * @param track Name of the track.
	 */
	virtual void playMusic(const QString & track) = 0;

	/**
	 * Sets volume of the music.
	 * @param volume Volume (in range 0.0 - 1.0).
	 */
	virtual void setMusicVolume(float volume) = 0;

	/** Returns current volume of the music. */
	virtual float getMusicVolume() const = 0;

	/** Creates the sound source. */
	virtual SoundSource createSoundSource() = 0;

	/** Creates the sound buffer. */
	virtual SoundBuffer createSoundBuffer() = 0;
};

#endif // __INTERFACES__SOUND_MANAGER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
