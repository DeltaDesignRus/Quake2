#include "OpenGLDrv.h"
#include "gl_image.h"
#include "gl_frontend.h"		//?? for fog params only ?


namespace OpenGLDrv {


glconfig_t	gl_config;
glstate_t	gl_state;


/*-----------------------------------------------------------------------------
	Some tables for GL_TexEnv()
-----------------------------------------------------------------------------*/

#define STD_MASK		(TEXENV_FUNC_MASK)
#define ARB_MASK		(TEXENV_FUNC_MASK|TEXENV_MUL2|TEXENV_SRC0_MASK|TEXENV_SRC1_MASK)


struct texEnvInfo_t
{
	unsigned mask;
	GLint	mode1;
	GLint	mode2;
};

struct texEnvSource_t
{
	GLenum	src;
	GLenum	op_rgb;
	GLenum	op_a;
};


// Used fields for TexEnv for each function
static const texEnvInfo_t texEnvInfo[] = {
	{(unsigned)-1, 0, 0},
	{STD_MASK, GL_REPLACE, 0},
	{STD_MASK, GL_MODULATE, 0},
	{STD_MASK, GL_ADD, 0},
	{ARB_MASK, GL_COMBINE_ARB, GL_MODULATE},
	{ARB_MASK, GL_COMBINE_ARB, GL_ADD},
	{ARB_MASK|TEXENV_SRC2_MASK, GL_COMBINE_ARB, GL_INTERPOLATE_ARB},
	{ARB_MASK|TEXENV_SRC2_MASK|TEXENV_SRC3_MASK, GL_COMBINE4_NV, GL_ADD},
	{ARB_MASK|TEXENV_SRC2_MASK, GL_COMBINE_ARB, GL_MODULATE_ADD_ATI}
};

static const texEnvSource_t texEnvSource[] = {
	{(GLenum)-1, (GLenum)-1, (GLenum)-1},		// "-1" is differs from "GL_ZERO"
	// texture
	{GL_TEXTURE, GL_SRC_COLOR, GL_SRC_ALPHA},
	{GL_TEXTURE, GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA},
	// texture alpha
	{GL_TEXTURE, GL_SRC_ALPHA, GL_SRC_ALPHA},
	{GL_TEXTURE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA},
	// previous
	{GL_PREVIOUS_ARB, GL_SRC_COLOR, GL_SRC_ALPHA},
	{GL_PREVIOUS_ARB, GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA},
	// previous alpha
	{GL_PREVIOUS_ARB, GL_SRC_ALPHA, GL_SRC_ALPHA},
	{GL_PREVIOUS_ARB, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA},
	// constant
	{GL_CONSTANT_ARB, GL_SRC_COLOR, GL_SRC_ALPHA},
	{GL_CONSTANT_ARB, GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA},
	// primary color
	{GL_PRIMARY_COLOR_ARB, GL_SRC_COLOR, GL_SRC_ALPHA},
	{GL_PRIMARY_COLOR_ARB, GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA},
	// one/zero
	{GL_ZERO, GL_SRC_COLOR, GL_SRC_ALPHA},
	{GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA}
};


/*-----------------------------------------------------------------------------
	Lock/unlock
-----------------------------------------------------------------------------*/


void GL_Lock()
{
	if (gl_state.locked) return;

#define COPY(f1, f2)	memcpy(gl_state.f1, gl_state.f2, sizeof(gl_state.f1))
	COPY(newBinds, currentBinds);
	COPY(newEnv, currentEnv);
	COPY(newTexCoordEnabled, texCoordEnabled);
	COPY(newTextureTarget, textureTarget);
	COPY(newEnvColor, texEnvColor);
	COPY(newMipBias, mipBias);

	memset(gl_state.newTCPointer, 0, sizeof(gl_state.newTCPointer));
#undef COPY

	gl_state.locked = true;
}


void GL_Unlock()
{
	if (!gl_state.locked) return;
	gl_state.locked = false;

	int startTmu = gl_state.currentTmu;
	for (int i = -1; i < gl_config.maxActiveTextures; i++)
	{
		int		tmu, n;
		bool	f;
		color_t	c;
		float	v;
		GLenum	tgt;

		if (i == -1)
			tmu = startTmu;		// process current TMU first (iteration "-1")
		else if (i == startTmu)
			continue;			// and skip it later
		else
			tmu = i;

		if (gl_state.newBinds[tmu] != gl_state.currentBinds[tmu])
		{
			GL_SelectTexture(tmu);
			GL_Bind(gl_state.newBinds[tmu]);
		}
		else if ((tgt = gl_state.newTextureTarget[tmu]) != gl_state.textureTarget[tmu])
		{
			GL_SelectTexture(tmu);
			if (gl_state.textureTarget[tmu])
				glDisable(gl_state.textureTarget[tmu]);
			if (tgt) glEnable(tgt);
			gl_state.textureTarget[tmu] = tgt;
		}

		if ((n = gl_state.newEnv[tmu]) != gl_state.currentEnv[tmu])
		{
			if ((n ^ gl_state.currentEnv[tmu]) & texEnvInfo[n & TEXENV_FUNC_MASK].mask)
			{
				GL_SelectTexture(tmu);
				GL_TexEnv(n);
			}
		}
		if ((v = gl_state.newMipBias[tmu]) != gl_state.mipBias[tmu])
		{
			GL_SelectTexture(tmu);
			gl_state.mipBias[tmu] = v;
			glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, v);
		}
		if ((f = gl_state.newTexCoordEnabled[tmu]) != gl_state.texCoordEnabled[tmu])
		{
			GL_SelectTexture(tmu);
			gl_state.texCoordEnabled[tmu] = f;
			if (f)	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			else	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if (gl_state.newTCPointer[tmu])
		{
			GL_SelectTexture(tmu);
			glTexCoordPointer(2, GL_FLOAT, 0, gl_state.newTCPointer[tmu]);
		}
		if (gl_state.newEnv[tmu] & TEXENV_ENVCOLOR && (c.rgba = gl_state.newEnvColor[tmu].rgba) != gl_state.texEnvColor[tmu].rgba)
		{
			GL_SelectTexture(tmu);
			GL_TexEnvColor(&c);
		}
	}
}


/*-----------------------------------------------------------------------------
	Bind
-----------------------------------------------------------------------------*/

void GL_Bind(const image_t *tex)
{
	int		tmu;

	if (tex && tex->flags & IMAGE_MIPMAP)	//?? remove GL_TexMipBias() ?
	{
		float	f;

		if (tex->flags & IMAGE_WORLD)
			f = gl_texMipBias->value;
		else if (tex->flags & IMAGE_SKIN)
			f = gl_skinMipBias->value;
		else
			f = 0;							//?? gl_defMipBias; mipmaps for GUI image?
		GL_TexMipBias(f);
	}

	if (tex && gl_nobind->integer && (gl_nobind->integer != 2 || tex->Name != "pics/conchars.pcx"))
		tex = gl_dlightImage;		//?? need a better way to detect force-bind images

	if (gl_state.locked)
	{
		tmu = gl_state.newTmu;
		if (tex)
		{
			gl_state.newBinds[tmu] = tex;
			gl_state.newTextureTarget[tmu] = tex->target;
		}
		else
			gl_state.newTextureTarget[tmu] = 0;
		return;
	}

	tmu = gl_state.currentTmu;

	if (!tex)
	{
		if (gl_state.textureTarget[tmu])
		{
			glDisable(gl_state.textureTarget[tmu]);
			gl_state.textureTarget[tmu] = 0;
		}
	}
	else
	{
		if (gl_state.textureTarget[tmu] != tex->target)
		{
			// switch texture target
			if ((gl_config.platformId & HW_GF2MX) &&
				tex->alphaType && tex != gl_state.currentBinds[tmu]) // BUG001 FIX
			{
				LOG_STRING("// fix BUG001");
				// bind any TEXTURE_2D texture within alpha channel
				glBindTexture(GL_TEXTURE_2D, gl_dlightImage->texnum);
			}
			if (gl_state.textureTarget[tmu])
				glDisable(gl_state.textureTarget[tmu]);
			glEnable(tex->target);
			gl_state.textureTarget[tmu] = tex->target;
		}
		if (tex == gl_state.currentBinds[tmu]) return;

		gl_state.currentBinds[tmu] = tex;
		LOG_STRING(va("// GL_Bind(%s)", *tex->Name));
		glBindTexture(tex->target, tex->texnum);
#if MAX_DEBUG
		GLenum err = glGetError();
		if (err) appWPrintf("Bind: error with %s\n", *tex->Name);
#endif
		STAT(gl_stats.numBinds++);
	}
}


// Bind image even if nobind active (i.e. for uploading image)
void GL_BindForce(const image_t *tex)
{
	int tmu = gl_state.currentTmu;

	if (gl_state.textureTarget[tmu] != tex->target)
	{
		if (gl_state.textureTarget[tmu])
			glDisable(gl_state.textureTarget[tmu]);
		glEnable(tex->target);
		gl_state.textureTarget[tmu] = tex->target;
	}

	if (gl_state.currentBinds[tmu] == tex) return;

	gl_state.currentBinds[tmu] = tex;
	LOG_STRING(va("// GL_Bind(%s)", *tex->Name));
	glBindTexture(tex->target, tex->texnum);
#if MAX_DEBUG
	GLenum err = glGetError();
	if (err) appWPrintf("BindForce: error with %s\n", *tex->Name);
#endif
	STAT(gl_stats.numBinds++);
}


/*-----------------------------------------------------------------------------
	Multitexturing
-----------------------------------------------------------------------------*/

void GL_TexEnv(unsigned env)
{
	static const GLenum sourceRgb[4] = {GL_SOURCE0_RGB_ARB, GL_SOURCE1_RGB_ARB, GL_SOURCE2_RGB_ARB, GL_SOURCE3_RGB_NV};
	static const GLenum sourceAlpha[4] = {GL_SOURCE0_ALPHA_ARB, GL_SOURCE1_ALPHA_ARB, GL_SOURCE2_ALPHA_ARB, GL_SOURCE3_ALPHA_NV};
	static const GLenum operandRgb[4] = {GL_OPERAND0_RGB_ARB, GL_OPERAND1_RGB_ARB, GL_OPERAND2_RGB_ARB, GL_OPERAND3_RGB_NV};
	static const GLenum operandAlpha[4] = {GL_OPERAND0_ALPHA_ARB, GL_OPERAND1_ALPHA_ARB, GL_OPERAND2_ALPHA_ARB, GL_OPERAND3_ALPHA_NV};

	if (gl_state.locked)
	{
		gl_state.newEnv[gl_state.newTmu] = env;
		return;
	}

	int tmu = gl_state.currentTmu;
	int func = env & TEXENV_FUNC_MASK;
	const texEnvInfo_t *info = &texEnvInfo[func];
	unsigned diff = (env ^ gl_state.currentEnv[tmu]) & info->mask;
	if (!diff) return;

	gl_state.currentEnv[tmu] ^= diff;		// this will not update fields, which can be unchanged

#if 0
	//!! DEBUG: always update all registers
	diff = env & info->mask;
#endif

	if (diff & TEXENV_FUNC_MASK)
	{
		// func is changed
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, info->mode1);
		if (!info->mode2) return;

		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, info->mode2);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, info->mode2);
	}

	if (diff & TEXENV_MUL2)
		glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, (env & TEXENV_MUL2 ? 2 : 1));

	unsigned mask = TEXENV_SRC0_MASK;
	int shift = TEXENV_SRC0_SHIFT;
	for (int i = 0; i < 4; i++)
	{
		if (diff & mask)
		{
			const texEnvSource_t *src, *prevSrc;

			src = &texEnvSource[(env & mask) >> shift];
			prevSrc = &texEnvSource[((env ^ diff) & mask) >> shift];

			if (src->src != prevSrc->src)
			{
				glTexEnvi(GL_TEXTURE_ENV, sourceRgb[i], src->src);
				glTexEnvi(GL_TEXTURE_ENV, sourceAlpha[i], src->src);
			}
			if (src->op_rgb != prevSrc->op_rgb)
				glTexEnvi(GL_TEXTURE_ENV, operandRgb[i], src->op_rgb);
			if (src->op_a != prevSrc->op_a)
				glTexEnvi(GL_TEXTURE_ENV, operandAlpha[i], src->op_a);
		}
		mask >>= TEXENV_SRC_BITS;
		shift -= TEXENV_SRC_BITS;
	}
}


void GL_TexMipBias(float f)
{
	if (!GL_SUPPORT(QGL_EXT_TEXTURE_LOD_BIAS))
		return;

	if (gl_state.locked)
	{
		gl_state.newMipBias[gl_state.newTmu] = f;
		return;
	}
	if (gl_state.mipBias[gl_state.newTmu] == f) return;
	gl_state.mipBias[gl_state.newTmu] = f;
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, f);
}


void GL_TexEnvColor(const color_t *c)
{
	if (gl_state.locked)
	{
		if (gl_state.newEnv[gl_state.newTmu] & TEXENV_ENVCOLOR)
			gl_state.newEnvColor[gl_state.newTmu].rgba = c->rgba;
		return;
	}

	int tmu = gl_state.currentTmu;
	if (!(gl_state.newEnv[tmu] & TEXENV_ENVCOLOR) || gl_state.texEnvColor[tmu].rgba == c->rgba)
		return;

//	LOG_STRING(va("// GL_TexEnvColor(%d, %d, %d, %d)", c->c[0], c->c[1], c->c[2], c->c[3]));
	gl_state.texEnvColor[tmu].rgba = c->rgba;
	float	color[4];
	color[0] = c->c[0] / 255.0f;
	color[1] = c->c[1] / 255.0f;
	color[2] = c->c[2] / 255.0f;
	color[3] = c->c[3] / 255.0f;
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color);
}


void GL_SelectTexture(int tmu)
{
	if (gl_state.locked)
	{
		gl_state.newTmu = tmu;
		return;
	}

	if (tmu == gl_state.currentTmu)
		return;

	int tex;
	if (GL_SUPPORT(QGL_ARB_MULTITEXTURE))
	{
		// ARB_multitexture
		tex = GL_TEXTURE0_ARB + tmu;
		glActiveTextureARB(tex);
		glClientActiveTextureARB(tex);	// affects gl[Enable|Disable]ClientState(GL_TEXTURE_COORD_ARRAY) and glTexCoordPointer() only
	}
	else
	{
		// SGIS_multitexture
		tex = GL_TEXTURE0_SGIS + tmu;
		glSelectTextureSGIS(tex);
		glSelectTextureCoordSetSGIS(tex);
	}

	gl_state.currentTmu = tmu;
}


void GL_TexCoordPointer(void *ptr)
{
	if (gl_state.locked)
	{
		gl_state.newTCPointer[gl_state.newTmu] = ptr;
		return;
	}

	glTexCoordPointer(2, GL_FLOAT, 0, ptr);
}


void GL_SetMultitexture(int level)
{
	int		i;

	if (level > gl_config.maxActiveTextures)
		appError("R_SetMultitexture(%d > %d)", level, gl_config.maxActiveTextures);

	if (gl_state.locked)
	{
		for (i = 0; i < gl_config.maxActiveTextures; i++)
			if (i < level)
			{
				gl_state.newTexCoordEnabled[i] = true;
			}
			else
			{
				gl_state.newTexCoordEnabled[i] = false;
				gl_state.newTextureTarget[i] = 0;
			}
		return;
	}

	for (i = level; i < gl_config.maxActiveTextures; i++)
		if (gl_state.textureTarget[i] || gl_state.texCoordEnabled[i])
		{
			GL_SelectTexture(i);
			GL_Bind(NULL);
			if (gl_state.texCoordEnabled[i])
			{
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				gl_state.texCoordEnabled[i] = false;
			}
		}
	for (i = level - 1; i >= 0; i--)
		if (!gl_state.texCoordEnabled[i])
		{
			GL_SelectTexture(i);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			gl_state.texCoordEnabled[i] = true;
		}
	GL_SelectTexture(0);
}


void GL_DisableTexCoordArrays()
{
	if (gl_state.locked)
	{
		memset(gl_state.newTexCoordEnabled, 0, sizeof(gl_state.newTexCoordEnabled));
		return;
	}

	for (int i = 0; i < gl_config.maxActiveTextures; i++)
		if (gl_state.texCoordEnabled[i])
		{
			GL_SelectTexture(i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			gl_state.texCoordEnabled[i] = false;
		}
}


/*-----------------------------------------------------------------------------
	Miscellaneous
-----------------------------------------------------------------------------*/


void GL_CullFace(gl_cullMode_t mode)
{
	if (gl_state.inverseCull && mode != CULL_NONE)
		mode = (gl_cullMode_t)(mode ^ CULL_FRONT ^ CULL_BACK);

	if (gl_state.currentCullMode == mode)
		return;

	if (mode == CULL_NONE)
		glDisable(GL_CULL_FACE);
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(mode == CULL_FRONT ? GL_FRONT : GL_BACK);
	}

	gl_state.currentCullMode = mode;
}


void GL_DepthRange(gl_depthMode_t mode)
{
	static const float n[] = {0,	0,		0,		1};
	static const float f[] = {0,  1.0/3,	1,		1};

	if (gl_state.currentDepthMode == mode)
		return;

	glDepthRange(n[mode], f[mode]);

	gl_state.currentDepthMode = mode;
}


void GL_State(unsigned state)
{
	static const GLenum blends[] = {
		0,		// unused value (used for "no blend")
		GL_ZERO, GL_ONE,
		GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
		GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA,
		GL_SRC_ALPHA_SATURATE
	};

	unsigned diff = state ^ gl_state.currentState;
	if (!diff)
		return;

	if (diff & GLSTATE_BLENDMASK)
	{
		if (state & GLSTATE_BLENDMASK)
		{
			if (!(gl_state.currentState & GLSTATE_BLENDMASK))
				glEnable(GL_BLEND);
			GLenum src = blends[(state & GLSTATE_SRCMASK) >> GLSTATE_SRCSHIFT];
			GLenum dst = blends[(state & GLSTATE_DSTMASK) >> GLSTATE_DSTSHIFT];
			glBlendFunc(src, dst);
		}
		else
			glDisable(GL_BLEND);
	}

	if (diff & GLSTATE_ALPHAMASK)
	{
		unsigned m = state & GLSTATE_ALPHAMASK;
		if (!m)
			glDisable(GL_ALPHA_TEST);
		else
		{
			if (!(gl_state.currentState & GLSTATE_ALPHAMASK))
				glEnable(GL_ALPHA_TEST);
			if (m == GLSTATE_ALPHA_GT0)
				glAlphaFunc(GL_GREATER, 0.05f);	//?? 0.0f
			else if (m == GLSTATE_ALPHA_LT05)
				glAlphaFunc(GL_LESS, 0.5f);
			else // if (m == GLSTATE_ALPHA_GE05)
				glAlphaFunc(GL_GEQUAL, 0.5f);
		}
	}

	if (diff & GLSTATE_DEPTHWRITE)
		glDepthMask(state & GLSTATE_DEPTHWRITE ? GL_TRUE : GL_FALSE);

	if (diff & GLSTATE_NODEPTHTEST)
	{
		if (state & GLSTATE_NODEPTHTEST)
			glDisable(GL_DEPTH_TEST);
		else
			glEnable(GL_DEPTH_TEST);
	}

	if (diff & GLSTATE_DEPTHEQUALFUNC)
		glDepthFunc(state & GLSTATE_DEPTHEQUALFUNC ? GL_EQUAL : GL_LEQUAL);

	if (diff & GLSTATE_POLYGON_LINE)
		glPolygonMode(GL_FRONT_AND_BACK, state & GLSTATE_POLYGON_LINE ? GL_LINE : GL_FILL);

	gl_state.currentState = state;
}


void GL_EnableFog(bool enable)
{
	if (!gl_fogMode || !gl_fog->integer) enable = false;
	if (enable == gl_state.fogEnabled)
		return;

	if (enable)
	{
		glEnable(GL_FOG);
//		glHint(GL_FOG_HINT, GL_NICEST);
		glFogf(GL_FOG_MODE, gl_fogMode);
		glFogfv(GL_FOG_COLOR, gl_fogColor);
		switch (gl_fogMode)
		{
		case GL_EXP:
		case GL_EXP2:
			glFogf(GL_FOG_DENSITY, gl_fogDensity);
			break;
		case GL_LINEAR:
			glFogf(GL_FOG_START, gl_fogStart);
			glFogf(GL_FOG_END, gl_fogEnd);
			break;
		}
		gl_state.fogEnabled = true;
	}
	else
	{
		glDisable(GL_FOG);
		gl_state.fogEnabled = false;
	}
}


void GL_SetDefaultState()
{
	memset(&gl_state, 0, sizeof(gl_state));

	glDisable(GL_CULL_FACE);
	glDepthRange(0, 1);
	gl_state.currentDepthMode = DEPTH_NORMAL;
	glColor4f(1, 1, 1, 1);

	// setup texturing
	for (int i = 1; i < gl_config.maxActiveTextures; i++)
	{
		GL_SelectTexture(i);
		glDisable(GL_TEXTURE_2D);
		if (GL_SUPPORT(QGL_NV_TEXTURE_RECTANGLE))
			glDisable(GL_TEXTURE_RECTANGLE_NV);
		gl_state.currentEnv[i] = 0;
		GL_TexEnv(TEXENV_REPLACE);
	}
	if (gl_config.maxActiveTextures > 1) GL_SelectTexture(0);
	glDisable(GL_TEXTURE_2D);
	if (GL_SUPPORT(QGL_NV_TEXTURE_RECTANGLE))
		glDisable(GL_TEXTURE_RECTANGLE_NV);
	gl_state.currentEnv[0] = 0;
	GL_TexEnv(TEXENV_MODULATE);

	// prepare filter values for new images (images are not exists at this point)
	GL_TextureMode(gl_texturemode->string);

	// set GL_State to a corresponding zero values
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_SCISSOR_TEST);
	glShadeModel(GL_SMOOTH);
	glEnableClientState(GL_VERTEX_ARRAY);
}


void GL_Set2DMode()
{
	if (gl_state.is2dMode)
		return;

	if (screenshotName && (screenshotFlags & SHOT_NO_2D))	// can move this to gl_main.cpp::RenderScene(), but this will wait for 3D
		PerformScreenshot();

	LOG_STRING("***** Set2DMode() *****");
	glViewport(0, 0, gl_config.width, gl_config.height);
	glScissor(0, 0, gl_config.width, gl_config.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, gl_config.width, gl_config.height, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GL_State(BLEND(S_ALPHA,M_S_ALPHA)|GLSTATE_NODEPTHTEST);
	GL_CullFace(CULL_NONE);
	gl_state.is2dMode = true;

	gl_state.inverseCull = false;	//??
//??	GL_CullFace(CULL_FRONT);
}


// Setup OpenGL as specified in active portal
void GL_Set3DMode(viewPortal_t *port)
{
	LOG_STRING("*** GL_Set3DMode() ***");
	gl_state.is2dMode = false;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&port->projectionMatrix[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&port->modelMatrix[0][0]);		// required for sky drawing (remove this comment ??)

	glViewport(port->x, port->y, port->w, port->h);
	glScissor(port->x, port->y, port->w, port->h);
	GL_State(GLSTATE_DEPTHWRITE);					// affects glClear(DEPTH)

	GLbitfield bits = GL_DEPTH_BUFFER_BIT;
	if (gl_state.useFastSky && !(port->flags & RDF_NOWORLDMODEL))
	{
		bits |= GL_COLOR_BUFFER_BIT;
		glClearColor(0, 0, 0, 1);
	}
	glClear(bits);

	gl_state.inverseCull = false;
	GL_CullFace(CULL_FRONT);
}


//!!!!! NOT HELPS
void GL_ResetState()
{
#if 0
	gl_state.currentTmu = -1;
	for (int i = 0; i < gl_config.maxActiveTextures; i++)
	{
		gl_state.currentBinds[i] = (image_t*) -1;
		* (byte*) &gl_state.texCoordEnabled[i] = 2;
		gl_state.textureTarget[i] = -1;
	}
#endif
}


} // namespace
