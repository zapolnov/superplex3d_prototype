#ifndef __BALL_H__
#define __BALL_H__

/* Includes */

#include <core/common.h>
#include <engine/render.h>
#include <math/vector3.h>
#include <vector>


/* Classes */

/** Base class for all balls. */
class Ball
{
public:
	/** Constructor. */
	Ball();

	/** Updates the ball. */
	void update();

	/** Renders the ball. */
	void render() const;

	/** Renders the ball without applying any animation. */
	void renderStatic() const;

	/**
	 * Enables or disables ball jumping.
	 * @param flag True, to enable jumping.
	 */
	void setJumping(bool flag);

	/** Returns true if ball is still falling. */
	inline bool isFalling() const
	{
		return m_FirstFall;
	}

	/** Returns true if ball is still jumping. */
	inline bool isJumping() const
	{
		return m_Jumping;
	}

	/** Returns color index of the ball. */
	inline int colorIndex() const
	{
		return m_ColorIndex;
	}

private:
	MeshPtr m_Sphere;		/**< Sphere mesh. */
	int m_ColorIndex;		/**< Color index of the ball. */
	float m_JumpHeight;		/**< Current height the ball have jumped. */
	float m_JumpStep;		/**< Current jump step. */
	bool m_Jumping;			/**< True, if ball is currently jumping. */
	bool m_NewJumping;		/**< Jumping state from the program. */
	bool m_FirstFall;		/**< True for first fall of the ball. */
};

#endif // __BALL_H__
