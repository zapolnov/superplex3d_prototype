#include "ball.h"
#include <opengl/context.h>
#include <engine/system.h>
#include <engine/render.h>
#include <sound/functions.h>
#include <stdlib.h>

/* Private variables */

static Vector3 g_Colors[] =
{
	Vector3(1.0f, 1.0f, 1.0f),
	Vector3(1.0f, 0.0f, 0.0f),
	Vector3(0.0f, 1.0f, 0.0f),
	Vector3(0.0f, 0.0f, 1.0f),
	Vector3(1.0f, 1.0f, 0.0f),
	Vector3(0.0f, 1.0f, 1.0f),
	Vector3(1.0f, 0.0f, 1.0f),
};


/* Ball methods */

//
// Constructor
//
Ball::Ball()
	: m_JumpHeight(10.0f), m_JumpStep(-3.0f), m_Jumping(true), m_NewJumping(false), m_FirstFall(true)
{
	m_Sphere = R_LoadMesh("sphere.obj");
	m_ColorIndex = rand() % (sizeof(g_Colors) / sizeof(g_Colors[0]));
}

//
// Updates the ball
//
void Ball::update()
{
	if (m_NewJumping)
		m_Jumping = m_NewJumping;
	if (!m_Jumping)
		return;

	// Calculate new position of the ball
	float newHeight = m_JumpHeight + (float)(m_JumpStep * Sys_GetLastFrameTime() *
		(m_FirstFall ? 8.0f : (m_NewJumping ? 1.0f : 2.0f)));

	// Play the jump sound
	if ((m_JumpHeight >= 0.0f && newHeight < 0.0f) ||
			(m_JumpHeight > 0.0f && newHeight <= 0.0f))
		SND_PlaySound("jump.wav", Vector3(0.0f, 16.0f, 20.0f));

	// Check for end of animation
	if (m_JumpHeight < 0.0f && newHeight > 0.0f && !m_NewJumping)
	{
		m_Jumping = false;
		m_JumpHeight = 0.0f;
		m_JumpStep = fabs(m_JumpStep);
		return;
	}

	// Calculate new direction of the ball
	m_JumpHeight = newHeight;
	if (m_JumpHeight < -0.5f)
	{
		m_JumpHeight = -0.5f;
		m_JumpStep = fabs(m_JumpStep);
		m_FirstFall = false;
	}
	else if (m_JumpHeight > 0.5f && !m_FirstFall) 
	{
		m_JumpHeight = 0.5f;
		m_JumpStep = -fabs(m_JumpStep);
	}
}

//
// Renders the ball
//
void Ball::render() const
{
	if (m_JumpHeight >= 0.0f)
		glTranslatef(0, m_JumpHeight, 0);
	else
	{
		float delta = fabs(m_JumpHeight);
		glScalef(1.0f + delta * 0.25f, 1.0f - delta, 1.0f + delta * 0.25f);
		glTranslatef(0, m_JumpHeight, 0);
	}

	renderStatic();
}

//
// Renders the ball without applying any animation
//
void Ball::renderStatic() const
{
	glColor3fv(g_Colors[m_ColorIndex].ptr());
	R_RenderMeshWithoutMaterial(m_Sphere);
}

//
// Enables or disables ball jumping
//
void Ball::setJumping(bool flag)
{
	m_NewJumping = flag;
}
