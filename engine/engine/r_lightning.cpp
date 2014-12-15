#include <engine/render.h>
#include <stdlib.h>

/* Structures */

/** Lightning info. */
struct LightningImpl : public Lightning
{
	Vector3 from;				/**< Source point. */
	Vector3 to;					/**< Destination point. */
	float fracLevel;			/**< Fractalisation level. */
	float fracDec;				/**< Fractalisation decrease. */
	uint maxFracDepth;			/**< Max depth of fractalisation. */
	Vector3 fractal[1026];		/**< Fractal points of the lightning. */
	int numPoints;				/**< Number of fractal points. */
};


/* Private functions */

//
// Generates random number in range [0;1)
//
static
inline double frand()
{
	return (double)rand() / ((double)(RAND_MAX) + 1.0);
}

//
// Adds point to the list of fractal points
//
static
void pushFractal(LightningImpl * lightning, const Vector3 & v)
{
	Q_ASSERT(lightning->numPoints < (int)(sizeof(lightning->fractal) / sizeof(lightning->fractal[0])));
	lightning->fractal[++lightning->numPoints] = v;
}

//
// Generates fractal points for lightning
//
static
void fract(LightningImpl * lightning, const Vector3 & a, const Vector3 & b, float factor,
	float fracDec, byte maxFracDepth, byte fracCount)
{
	if (++fracCount >= maxFracDepth)
	{
		pushFractal(lightning, b);
		return;
	}

	Vector3 dir = a - b;
	Vector3 planeU = dir ^ Vector3(0.0f, 1.0f, 0.0f);
	Vector3 planeV = planeU ^ dir;

	planeU.normalize();
	planeV.normalize();

	Vector3 c
	(
		(float)(0.5f * (a.x() + b.x()) + factor * (planeU.x() * (frand() - 0.5f) + planeV.x() * (frand() - 0.5f))),
		(float)(0.5f * (a.y() + b.y()) + factor * (planeU.y() * (frand() - 0.5f) + planeV.y() * (frand() - 0.5f))),
		(float)(0.5f * (a.z() + b.z()) + factor * (planeU.z() * (frand() - 0.5f) + planeV.z() * (frand() - 0.5f)))
	);

	fract(lightning, a, c, factor * fracDec, fracDec, maxFracDepth, fracCount);
	fract(lightning, c, b, factor * fracDec, fracDec, maxFracDepth, fracCount);
}

//
// Generates the lightning
//
static
void generateLightning(LightningImpl * lightning)
{
	lightning->numPoints = -1;
	pushFractal(lightning, lightning->from);
	fract(lightning, lightning->from, lightning->to, lightning->fracLevel,
		lightning->fracDec, lightning->maxFracDepth, 0);
}


/* Public functions */

//
// Creates new lightning
//
LightningPtr R_CreateLightning(const Vector3 & from, const Vector3 & to, float fracLevel,
	float fracDec, uint maxFracDepth)
{
	LightningPtr result = new LightningImpl;
	LightningImpl * lightning = result.as<LightningImpl>();

	if (unlikely(maxFracDepth > sizeof(lightning->fractal) / sizeof(lightning->fractal[0])))
		maxFracDepth = sizeof(lightning->fractal) / sizeof(lightning->fractal[0]);

	lightning->from = from;
	lightning->to = to;
	lightning->fracLevel = fracLevel * (to - from).length();
	lightning->fracDec = fracDec;
	lightning->maxFracDepth = maxFracDepth;
	generateLightning(lightning);

	return lightning;
}

//
// Renders the lightning
//
void R_RenderLightning(const LightningPtr & ptr, float width, bool is3d)
{
	const LightningImpl * lightning = ptr.as<LightningImpl>();
	static const float STEP = 0.1f;
	Vector3 v;

	R_BindTexture(0, NULL);
	R_UnbindVertexShader();
	R_UnbindFragmentShader();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glLineWidth(width * 0.5f);

	glColor4f(0.5f, 0.5f, 1.0f, 0.8f);
	glBegin(GL_LINE_STRIP);
	if (is3d)
	{
		for (int i = 0; i < lightning->numPoints; i++)
			glVertex3fv(lightning->fractal[i].ptr());
	}
	else
	{
		for (int i = 0; i < lightning->numPoints; i++)
			glVertex2fv(lightning->fractal[i].ptr());
	}
	glEnd();

	glLineWidth(width * 0.7f);

	glColor4f(0.3f, 0.3f, 1.0f, 0.5f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < lightning->numPoints; i++)
	{
		v = lightning->fractal[i] + Vector3((float)frand() * STEP - (STEP * 0.5f),
			(float)frand() * STEP - (STEP * 0.5f), (float)frand() * STEP - (STEP * 0.5f));

		if (is3d)
			glVertex3fv(v.ptr());
		else
			glVertex2fv(v.ptr());
	}
	glEnd();

	glLineWidth(width);

	glColor4f(0.3f, 0.3f, 1.0f, 0.2f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < lightning->numPoints; i++)
	{
		v = lightning->fractal[i] + Vector3((float)frand() * STEP - (STEP * 0.5f),
			(float)frand() * STEP - (STEP * 0.5f), (float)frand() * STEP - (STEP * 0.5f));

		if (is3d)
			glVertex3fv(v.ptr());
		else
			glVertex2fv(v.ptr());
	}
	glEnd();

	glPopAttrib();
}

//
// Updates the lightning
//
void R_UpdateLightning(LightningPtr ptr)
{
	generateLightning(ptr.as<LightningImpl>());
}
