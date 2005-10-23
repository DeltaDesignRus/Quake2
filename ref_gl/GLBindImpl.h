// Autogenerated file: do not modify

#if _WIN32
#	define NUM_GLFUNCS	61
#	define NUM_EXTFUNCS	7
#	define NUM_EXTENSIONS	15
#elif __linux__
#	define NUM_GLFUNCS	53
#	define NUM_EXTFUNCS	6
#	define NUM_EXTENSIONS	14
#else
#	define NUM_GLFUNCS	53
#	define NUM_EXTFUNCS	6
#	define NUM_EXTENSIONS	14
#endif


static const char *GLNames[NUM_GLFUNCS + NUM_EXTFUNCS] = {
	"glAlphaFunc",
	"glBegin",
	"glBindTexture",
	"glBlendFunc",
	"glClear",
	"glClearColor",
	"glColor3f",
	"glColor3fv",
	"glColor4f",
	"glColor4ubv",
	"glColorPointer",
	"glCullFace",
	"glDeleteTextures",
	"glDepthFunc",
	"glDepthMask",
	"glDepthRange",
	"glDisable",
	"glDisableClientState",
	"glDrawElements",
	"glEnable",
	"glEnableClientState",
	"glEnd",
	"glFinish",
	"glFogf",
	"glFogfv",
	"glGetIntegerv",
	"glGetString",
	"glLoadIdentity",
	"glLoadMatrixf",
	"glMatrixMode",
	"glOrtho",
	"glPolygonMode",
	"glPolygonOffset",
	"glPopMatrix",
	"glPushMatrix",
	"glReadPixels",
	"glRotatef",
	"glScissor",
	"glShadeModel",
	"glTexCoord2f",
	"glTexCoordPointer",
	"glTexEnvf",
	"glTexEnvfv",
	"glTexEnvi",
	"glTexImage2D",
	"glTexParameteri",
	"glTexSubImage2D",
	"glTranslatef",
	"glVertex2f",
	"glVertex3f",
	"glVertex3fv",
	"glVertexPointer",
	"glViewport"
#if _WIN32

,
	"wglChoosePixelFormat",
	"wglDescribePixelFormat",
	"wglSetPixelFormat",
	"wglSwapBuffers",
	"wglCreateContext",
	"wglDeleteContext",
	"wglGetProcAddress",
	"wglMakeCurrent"
#endif

,
	"glSelectTextureSGIS",
	"glSelectTextureCoordSetSGIS",
	"glActiveTextureARB",
	"glClientActiveTextureARB",
	"glLockArraysEXT",
	"glUnlockArraysEXT"
#if _WIN32

,
	"wglSwapIntervalEXT"
#elif __linux__

#endif


};

#if !NO_GL_LOG


static const char *EnumName (GLenum v)
{
	switch (v)
	{
	case GL_LINE_LOOP:
		return "LINE_LOOP";
	case GL_LINE_STRIP:
		return "LINE_STRIP";
	case GL_TRIANGLES:
		return "TRIANGLES";
	case GL_TRIANGLE_STRIP:
		return "TRIANGLE_STRIP";
	case GL_TRIANGLE_FAN:
		return "TRIANGLE_FAN";
	case GL_QUADS:
		return "QUADS";
	case GL_QUAD_STRIP:
		return "QUAD_STRIP";
	case GL_POLYGON:
		return "POLYGON";
	case GL_NEVER:
		return "NEVER";
	case GL_LESS:
		return "LESS";
	case GL_EQUAL:
		return "EQUAL";
	case GL_LEQUAL:
		return "LEQUAL";
	case GL_GREATER:
		return "GREATER";
	case GL_NOTEQUAL:
		return "NOTEQUAL";
	case GL_GEQUAL:
		return "GEQUAL";
	case GL_ALWAYS:
		return "ALWAYS";
	case GL_ZERO:
		return "ZERO";
	case GL_ONE:
		return "ONE";
	case GL_SRC_COLOR:
		return "SRC_COLOR";
	case GL_ONE_MINUS_SRC_COLOR:
		return "ONE_MINUS_SRC_COLOR";
	case GL_SRC_ALPHA:
		return "SRC_ALPHA";
	case GL_ONE_MINUS_SRC_ALPHA:
		return "ONE_MINUS_SRC_ALPHA";
	case GL_DST_ALPHA:
		return "DST_ALPHA";
	case GL_ONE_MINUS_DST_ALPHA:
		return "ONE_MINUS_DST_ALPHA";
	case GL_DST_COLOR:
		return "DST_COLOR";
	case GL_ONE_MINUS_DST_COLOR:
		return "ONE_MINUS_DST_COLOR";
	case GL_SRC_ALPHA_SATURATE:
		return "SRC_ALPHA_SATURATE";
	case GL_COMBINE_ARB:
		return "COMBINE_ARB";
	case GL_COMBINE_RGB_ARB:
		return "COMBINE_RGB_ARB";
	case GL_COMBINE_ALPHA_ARB:
		return "COMBINE_ALPHA_ARB";
	case GL_RGB_SCALE_ARB:
		return "RGB_SCALE_ARB";
	case GL_SOURCE0_RGB_ARB:
		return "SOURCE0_RGB_ARB";
	case GL_OPERAND0_RGB_ARB:
		return "OPERAND0_RGB_ARB";
	case GL_SOURCE0_ALPHA_ARB:
		return "SOURCE0_ALPHA_ARB";
	case GL_OPERAND0_ALPHA_ARB:
		return "OPERAND0_ALPHA_ARB";
	case GL_SOURCE1_RGB_ARB:
		return "SOURCE1_RGB_ARB";
	case GL_OPERAND1_RGB_ARB:
		return "OPERAND1_RGB_ARB";
	case GL_SOURCE1_ALPHA_ARB:
		return "SOURCE1_ALPHA_ARB";
	case GL_OPERAND1_ALPHA_ARB:
		return "OPERAND1_ALPHA_ARB";
	case GL_SOURCE2_RGB_ARB:
		return "SOURCE2_RGB_ARB";
	case GL_OPERAND2_RGB_ARB:
		return "OPERAND2_RGB_ARB";
	case GL_SOURCE2_ALPHA_ARB:
		return "SOURCE2_ALPHA_ARB";
	case GL_OPERAND2_ALPHA_ARB:
		return "OPERAND2_ALPHA_ARB";
	case GL_INTERPOLATE_ARB:
		return "INTERPOLATE_ARB";
	case GL_CONSTANT_ARB:
		return "CONSTANT_ARB";
	case GL_PREVIOUS_ARB:
		return "PREVIOUS_ARB";
	case GL_PRIMARY_COLOR_ARB:
		return "PRIMARY_COLOR_ARB";
	case GL_SOURCE3_RGB_NV:
		return "SOURCE3_RGB_NV";
	case GL_OPERAND3_RGB_NV:
		return "OPERAND3_RGB_NV";
	case GL_SOURCE3_ALPHA_NV:
		return "SOURCE3_ALPHA_NV";
	case GL_OPERAND3_ALPHA_NV:
		return "OPERAND3_ALPHA_NV";
	case GL_COMBINE4_NV:
		return "COMBINE4_NV";
	case GL_MODULATE_ADD_ATI:
		return "MODULATE_ADD_ATI";
	case GL_FRONT:
		return "FRONT";
	case GL_BACK:
		return "BACK";
	case GL_MODELVIEW:
		return "MODELVIEW";
	case GL_PROJECTION:
		return "PROJECTION";
	case GL_TEXTURE:
		return "TEXTURE";
	case GL_TEXTURE_ENV:
		return "TEXTURE_ENV";
	case GL_NEAREST:
		return "NEAREST";
	case GL_LINEAR:
		return "LINEAR";
	case GL_NEAREST_MIPMAP_NEAREST:
		return "NEAREST_MIPMAP_NEAREST";
	case GL_LINEAR_MIPMAP_NEAREST:
		return "LINEAR_MIPMAP_NEAREST";
	case GL_NEAREST_MIPMAP_LINEAR:
		return "NEAREST_MIPMAP_LINEAR";
	case GL_LINEAR_MIPMAP_LINEAR:
		return "LINEAR_MIPMAP_LINEAR";
	case GL_TEXTURE_MAG_FILTER:
		return "TEXTURE_MAG_FILTER";
	case GL_TEXTURE_MIN_FILTER:
		return "TEXTURE_MIN_FILTER";
	case GL_TEXTURE_FILTER_CONTROL_EXT:
		return "TEXTURE_FILTER_CONTROL_EXT";
	case GL_TEXTURE_LOD_BIAS_EXT:
		return "TEXTURE_LOD_BIAS_EXT";
	case GL_TEXTURE_WRAP_S:
		return "TEXTURE_WRAP_S";
	case GL_TEXTURE_WRAP_T:
		return "TEXTURE_WRAP_T";
	case GL_CLAMP:
		return "CLAMP";
	case GL_REPEAT:
		return "REPEAT";
	case GL_MODULATE:
		return "MODULATE";
	case GL_DECAL:
		return "DECAL";
	case GL_REPLACE:
		return "REPLACE";
	case GL_ADD:
		return "ADD";
	case GL_FRONT_AND_BACK:
		return "FRONT_AND_BACK";
	case GL_FILL:
		return "FILL";
	case GL_LINE:
		return "LINE";
	case GL_TEXTURE_ENV_MODE:
		return "TEXTURE_ENV_MODE";
	case GL_TEXTURE_ENV_COLOR:
		return "TEXTURE_ENV_COLOR";
	case GL_FOG_MODE:
		return "FOG_MODE";
	case GL_FOG_START:
		return "FOG_START";
	case GL_FOG_END:
		return "FOG_END";
	case GL_FOG_COLOR:
		return "FOG_COLOR";
	case GL_FOG_DENSITY:
		return "FOG_DENSITY";
	case GL_FOG_DISTANCE_MODE_NV:
		return "FOG_DISTANCE_MODE_NV";
	case GL_EYE_RADIAL_NV:
		return "EYE_RADIAL_NV";
	case GL_RGB5:
		return "RGB5";
	case GL_RGB8:
		return "RGB8";
	case GL_RGBA4:
		return "RGBA4";
	case GL_RGB5_A1:
		return "RGB5_A1";
	case GL_RGBA8:
		return "RGBA8";
	case GL_RGB4_S3TC:
		return "RGB4_S3TC";
	case GL_COMPRESSED_RGB_ARB:
		return "COMPRESSED_RGB_ARB";
	case GL_COMPRESSED_RGBA_ARB:
		return "COMPRESSED_RGBA_ARB";
	case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
		return "COMPRESSED_RGB_S3TC_DXT1_EXT";
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		return "COMPRESSED_RGBA_S3TC_DXT1_EXT";
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		return "COMPRESSED_RGBA_S3TC_DXT3_EXT";
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		return "COMPRESSED_RGBA_S3TC_DXT5_EXT";
	case GL_ACTIVE_TEXTURE_ARB:
		return "ACTIVE_TEXTURE_ARB";
	case GL_CLIENT_ACTIVE_TEXTURE_ARB:
		return "CLIENT_ACTIVE_TEXTURE_ARB";
	case GL_MAX_TEXTURE_UNITS_ARB:
		return "MAX_TEXTURE_UNITS_ARB";
	case GL_TEXTURE0_ARB:
		return "TEXTURE0_ARB";
	case GL_TEXTURE1_ARB:
		return "TEXTURE1_ARB";
	case GL_TEXTURE2_ARB:
		return "TEXTURE2_ARB";
	case GL_TEXTURE3_ARB:
		return "TEXTURE3_ARB";
	case GL_TEXTURE4_ARB:
		return "TEXTURE4_ARB";
	case GL_TEXTURE5_ARB:
		return "TEXTURE5_ARB";
	case GL_TEXTURE6_ARB:
		return "TEXTURE6_ARB";
	case GL_TEXTURE7_ARB:
		return "TEXTURE7_ARB";
	case GL_MAX_TEXTURES_SGIS:
		return "MAX_TEXTURES_SGIS";
	case GL_TEXTURE0_SGIS:
		return "TEXTURE0_SGIS";
	case GL_TEXTURE1_SGIS:
		return "TEXTURE1_SGIS";
	case GL_FOG:
		return "FOG";
	case GL_TEXTURE_2D:
		return "TEXTURE_2D";
	case GL_CULL_FACE:
		return "CULL_FACE";
	case GL_ALPHA_TEST:
		return "ALPHA_TEST";
	case GL_BLEND:
		return "BLEND";
	case GL_DEPTH_TEST:
		return "DEPTH_TEST";
	case GL_VERTEX_ARRAY:
		return "VERTEX_ARRAY";
	case GL_COLOR_ARRAY:
		return "COLOR_ARRAY";
	case GL_TEXTURE_COORD_ARRAY:
		return "TEXTURE_COORD_ARRAY";
	case GL_TEXTURE_RECTANGLE_NV:
		return "TEXTURE_RECTANGLE_NV";
	case GL_BYTE:
		return "BYTE";
	case GL_UNSIGNED_BYTE:
		return "UNSIGNED_BYTE";
	case GL_SHORT:
		return "SHORT";
	case GL_UNSIGNED_SHORT:
		return "UNSIGNED_SHORT";
	case GL_INT:
		return "INT";
	case GL_UNSIGNED_INT:
		return "UNSIGNED_INT";
	case GL_FLOAT:
		return "FLOAT";
	case GL_DOUBLE:
		return "DOUBLE";
	default:
		return va("UNK_%X", v);
	}
}


static void APIENTRY logAlphaFunc (GLenum func, GLclampf ref)
{
	LogFile->Printf ("%s (GL_%s, %g)\n", "glAlphaFunc", EnumName(func), ref);
	lib.AlphaFunc (func, ref);
}

static void APIENTRY logBegin (GLenum mode)
{
	LogFile->Printf ("%s (GL_%s)\n", "glBegin", EnumName(mode));
	lib.Begin (mode);
}

static void APIENTRY logBindTexture (GLenum target, GLuint texture)
{
	LogFile->Printf ("%s (GL_%s, %d)\n", "glBindTexture", EnumName(target), texture);
	lib.BindTexture (target, texture);
}

static void APIENTRY logBlendFunc (GLenum sfactor, GLenum dfactor)
{
	LogFile->Printf ("%s (GL_%s, GL_%s)\n", "glBlendFunc", EnumName(sfactor), EnumName(dfactor));
	lib.BlendFunc (sfactor, dfactor);
}

static void APIENTRY logClear (GLbitfield mask)
{
	LogFile->Printf ("%s\n", "glClear");
	lib.Clear (mask);
}

static void APIENTRY logClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	LogFile->Printf ("%s (%g, %g, %g, %g)\n", "glClearColor", red, green, blue, alpha);
	lib.ClearColor (red, green, blue, alpha);
}

static void APIENTRY logColor3f (GLfloat red, GLfloat green, GLfloat blue)
{
	LogFile->Printf ("%s (%g, %g, %g)\n", "glColor3f", red, green, blue);
	lib.Color3f (red, green, blue);
}

static void APIENTRY logColor3fv (const GLfloat *v)
{
	LogFile->Printf ("%s\n", "glColor3fv");
	lib.Color3fv (v);
}

static void APIENTRY logColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	LogFile->Printf ("%s (%g, %g, %g, %g)\n", "glColor4f", red, green, blue, alpha);
	lib.Color4f (red, green, blue, alpha);
}

static void APIENTRY logColor4ubv (const GLubyte *v)
{
	LogFile->Printf ("%s\n", "glColor4ubv");
	lib.Color4ubv (v);
}

static void APIENTRY logColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	LogFile->Printf ("%s (%d, GL_%s, %d, $%X)\n", "glColorPointer", size, EnumName(type), stride, (unsigned)pointer);
	lib.ColorPointer (size, type, stride, pointer);
}

static void APIENTRY logCullFace (GLenum mode)
{
	LogFile->Printf ("%s (GL_%s)\n", "glCullFace", EnumName(mode));
	lib.CullFace (mode);
}

static void APIENTRY logDeleteTextures (GLsizei n, const GLuint *textures)
{
	LogFile->Printf ("%s (%d, $%X)\n", "glDeleteTextures", n, (unsigned)textures);
	lib.DeleteTextures (n, textures);
}

static void APIENTRY logDepthFunc (GLenum func)
{
	LogFile->Printf ("%s (GL_%s)\n", "glDepthFunc", EnumName(func));
	lib.DepthFunc (func);
}

static void APIENTRY logDepthMask (GLboolean flag)
{
	LogFile->Printf ("%s (%d)\n", "glDepthMask", flag);
	lib.DepthMask (flag);
}

static void APIENTRY logDepthRange (GLclampd zNear, GLclampd zFar)
{
	LogFile->Printf ("%s (%g, %g)\n", "glDepthRange", zNear, zFar);
	lib.DepthRange (zNear, zFar);
}

static void APIENTRY logDisable (GLenum cap)
{
	LogFile->Printf ("%s (GL_%s)\n", "glDisable", EnumName(cap));
	lib.Disable (cap);
}

static void APIENTRY logDisableClientState (GLenum array)
{
	LogFile->Printf ("%s (GL_%s)\n", "glDisableClientState", EnumName(array));
	lib.DisableClientState (array);
}

static void APIENTRY logDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	LogFile->Printf ("%s (GL_%s, %d, GL_%s, $%X)\n", "glDrawElements", EnumName(mode), count, EnumName(type), (unsigned)indices);
	lib.DrawElements (mode, count, type, indices);
}

static void APIENTRY logEnable (GLenum cap)
{
	LogFile->Printf ("%s (GL_%s)\n", "glEnable", EnumName(cap));
	lib.Enable (cap);
}

static void APIENTRY logEnableClientState (GLenum array)
{
	LogFile->Printf ("%s (GL_%s)\n", "glEnableClientState", EnumName(array));
	lib.EnableClientState (array);
}

static void APIENTRY logEnd (void)
{
	LogFile->Printf ("%s\n", "glEnd");
	lib.End ();
}

static void APIENTRY logFinish (void)
{
	LogFile->Printf ("%s\n", "glFinish");
	lib.Finish ();
}

static void APIENTRY logFogf (GLenum pname, GLfloat param)
{
	LogFile->Printf ("%s (GL_%s, %g)\n", "glFogf", EnumName(pname), param);
	lib.Fogf (pname, param);
}

static void APIENTRY logFogfv (GLenum pname, const GLfloat *params)
{
	LogFile->Printf ("%s (GL_%s, $%X)\n", "glFogfv", EnumName(pname), (unsigned)params);
	lib.Fogfv (pname, params);
}

static void APIENTRY logGetIntegerv (GLenum pname, GLint *params)
{
	LogFile->Printf ("%s\n", "glGetIntegerv");
	lib.GetIntegerv (pname, params);
}

static const GLubyte * APIENTRY logGetString (GLenum name)
{
	LogFile->Printf ("%s\n", "glGetString");
	return lib.GetString (name);
}

static void APIENTRY logLoadIdentity (void)
{
	LogFile->Printf ("%s\n", "glLoadIdentity");
	lib.LoadIdentity ();
}

static void APIENTRY logLoadMatrixf (const GLfloat *m)
{
	LogFile->Printf ("%s\n", "glLoadMatrixf");
	lib.LoadMatrixf (m);
}

static void APIENTRY logMatrixMode (GLenum mode)
{
	LogFile->Printf ("%s (GL_%s)\n", "glMatrixMode", EnumName(mode));
	lib.MatrixMode (mode);
}

static void APIENTRY logOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	LogFile->Printf ("%s (%g, %g, %g, %g, %g, %g)\n", "glOrtho", left, right, bottom, top, zNear, zFar);
	lib.Ortho (left, right, bottom, top, zNear, zFar);
}

static void APIENTRY logPolygonMode (GLenum face, GLenum mode)
{
	LogFile->Printf ("%s (GL_%s, GL_%s)\n", "glPolygonMode", EnumName(face), EnumName(mode));
	lib.PolygonMode (face, mode);
}

static void APIENTRY logPolygonOffset (GLfloat factor, GLfloat units)
{
	LogFile->Printf ("%s (%g, %g)\n", "glPolygonOffset", factor, units);
	lib.PolygonOffset (factor, units);
}

static void APIENTRY logPopMatrix (void)
{
	LogFile->Printf ("%s\n", "glPopMatrix");
	lib.PopMatrix ();
}

static void APIENTRY logPushMatrix (void)
{
	LogFile->Printf ("%s\n", "glPushMatrix");
	lib.PushMatrix ();
}

static void APIENTRY logReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
	LogFile->Printf ("%s (%d, %d, %d, %d, GL_%s, GL_%s, $%X)\n", "glReadPixels", x, y, width, height, EnumName(format), EnumName(type), (unsigned)pixels);
	lib.ReadPixels (x, y, width, height, format, type, pixels);
}

static void APIENTRY logRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	LogFile->Printf ("%s (%g, %g, %g, %g)\n", "glRotatef", angle, x, y, z);
	lib.Rotatef (angle, x, y, z);
}

static void APIENTRY logScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
	LogFile->Printf ("%s (%d, %d, %d, %d)\n", "glScissor", x, y, width, height);
	lib.Scissor (x, y, width, height);
}

static void APIENTRY logShadeModel (GLenum mode)
{
	LogFile->Printf ("%s (GL_%s)\n", "glShadeModel", EnumName(mode));
	lib.ShadeModel (mode);
}

static void APIENTRY logTexCoord2f (GLfloat s, GLfloat t)
{
	LogFile->Printf ("%s (%g, %g)\n", "glTexCoord2f", s, t);
	lib.TexCoord2f (s, t);
}

static void APIENTRY logTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	LogFile->Printf ("%s (%d, GL_%s, %d, $%X)\n", "glTexCoordPointer", size, EnumName(type), stride, (unsigned)pointer);
	lib.TexCoordPointer (size, type, stride, pointer);
}

static void APIENTRY logTexEnvf (GLenum target, GLenum pname, GLfloat param)
{
	LogFile->Printf ("%s (GL_%s, GL_%s, %g)\n", "glTexEnvf", EnumName(target), EnumName(pname), param);
	lib.TexEnvf (target, pname, param);
}

static void APIENTRY logTexEnvfv (GLenum target, GLenum pname, const GLfloat *params)
{
	LogFile->Printf ("%s (GL_%s, GL_%s, $%X)\n", "glTexEnvfv", EnumName(target), EnumName(pname), (unsigned)params);
	lib.TexEnvfv (target, pname, params);
}

static void APIENTRY logTexEnvi (GLenum target, GLenum pname, GLenum param)
{
	LogFile->Printf ("%s (GL_%s, GL_%s, GL_%s)\n", "glTexEnvi", EnumName(target), EnumName(pname), EnumName(param));
	lib.TexEnvi (target, pname, param);
}

static void APIENTRY logTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
	LogFile->Printf ("%s (GL_%s, %d, GL_%s, %d, %d, %d, GL_%s, GL_%s, $%X)\n", "glTexImage2D", EnumName(target), level, EnumName(internalformat), width, height, border, EnumName(format), EnumName(type), (unsigned)pixels);
	lib.TexImage2D (target, level, internalformat, width, height, border, format, type, pixels);
}

static void APIENTRY logTexParameteri (GLenum target, GLenum pname, GLint param)
{
	LogFile->Printf ("%s (GL_%s, GL_%s, %d)\n", "glTexParameteri", EnumName(target), EnumName(pname), param);
	lib.TexParameteri (target, pname, param);
}

static void APIENTRY logTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
	LogFile->Printf ("%s (GL_%s, %d, %d, %d, %d, %d, GL_%s, GL_%s, $%X)\n", "glTexSubImage2D", EnumName(target), level, xoffset, yoffset, width, height, EnumName(format), EnumName(type), (unsigned)pixels);
	lib.TexSubImage2D (target, level, xoffset, yoffset, width, height, format, type, pixels);
}

static void APIENTRY logTranslatef (GLfloat x, GLfloat y, GLfloat z)
{
	LogFile->Printf ("%s (%g, %g, %g)\n", "glTranslatef", x, y, z);
	lib.Translatef (x, y, z);
}

static void APIENTRY logVertex2f (GLfloat x, GLfloat y)
{
	LogFile->Printf ("%s (%g, %g)\n", "glVertex2f", x, y);
	lib.Vertex2f (x, y);
}

static void APIENTRY logVertex3f (GLfloat x, GLfloat y, GLfloat z)
{
	LogFile->Printf ("%s (%g, %g, %g)\n", "glVertex3f", x, y, z);
	lib.Vertex3f (x, y, z);
}

static void APIENTRY logVertex3fv (const GLfloat *v)
{
	LogFile->Printf ("%s\n", "glVertex3fv");
	lib.Vertex3fv (v);
}

static void APIENTRY logVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	LogFile->Printf ("%s (%d, GL_%s, %d, $%X)\n", "glVertexPointer", size, EnumName(type), stride, (unsigned)pointer);
	lib.VertexPointer (size, type, stride, pointer);
}

static void APIENTRY logViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
	LogFile->Printf ("%s (%d, %d, %d, %d)\n", "glViewport", x, y, width, height);
	lib.Viewport (x, y, width, height);
}


#if _WIN32

static int APIENTRY logChoosePixelFormat (HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd)
{
	LogFile->Printf ("%s\n", "wglChoosePixelFormat");
	return lib.ChoosePixelFormat (hdc, ppfd);
}

static int APIENTRY logDescribePixelFormat (HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd)
{
	LogFile->Printf ("%s\n", "wglDescribePixelFormat");
	return lib.DescribePixelFormat (hdc, iPixelFormat, nBytes, ppfd);
}

static BOOL APIENTRY logSetPixelFormat (HDC hdc, int iPixelFormat, CONST PIXELFORMATDESCRIPTOR *ppfd)
{
	LogFile->Printf ("%s\n", "wglSetPixelFormat");
	return lib.SetPixelFormat (hdc, iPixelFormat, ppfd);
}

static BOOL APIENTRY logSwapBuffers (HDC hdc)
{
	LogFile->Printf ("%s\n", "wglSwapBuffers");
	return lib.SwapBuffers (hdc);
}

static HGLRC APIENTRY logCreateContext (HDC hdc)
{
	LogFile->Printf ("%s\n", "wglCreateContext");
	return lib.CreateContext (hdc);
}

static BOOL APIENTRY logDeleteContext (HGLRC hglrc)
{
	LogFile->Printf ("%s\n", "wglDeleteContext");
	return lib.DeleteContext (hglrc);
}

static PROC APIENTRY logGetProcAddress (LPCSTR lpszProc)
{
	LogFile->Printf ("%s\n", "wglGetProcAddress");
	return lib.GetProcAddress (lpszProc);
}

static BOOL APIENTRY logMakeCurrent (HDC hdc, HGLRC hglrc)
{
	LogFile->Printf ("%s\n", "wglMakeCurrent");
	return lib.MakeCurrent (hdc, hglrc);
}


#endif

static void APIENTRY logSelectTextureSGIS (GLenum target)
{
	LogFile->Printf ("%s (GL_%s)\n", "glSelectTextureSGIS", EnumName(target));
	lib.SelectTextureSGIS (target);
}

static void APIENTRY logSelectTextureCoordSetSGIS (GLenum target)
{
	LogFile->Printf ("%s (GL_%s)\n", "glSelectTextureCoordSetSGIS", EnumName(target));
	lib.SelectTextureCoordSetSGIS (target);
}

static void APIENTRY logActiveTextureARB (GLenum texture)
{
	LogFile->Printf ("%s (GL_%s)\n", "glActiveTextureARB", EnumName(texture));
	lib.ActiveTextureARB (texture);
}

static void APIENTRY logClientActiveTextureARB (GLenum texture)
{
	LogFile->Printf ("%s (GL_%s)\n", "glClientActiveTextureARB", EnumName(texture));
	lib.ClientActiveTextureARB (texture);
}

static void APIENTRY logLockArraysEXT (GLint first, GLint count)
{
	LogFile->Printf ("%s (%d, %d)\n", "glLockArraysEXT", first, count);
	lib.LockArraysEXT (first, count);
}

static void APIENTRY logUnlockArraysEXT (void)
{
	LogFile->Printf ("%s\n", "glUnlockArraysEXT");
	lib.UnlockArraysEXT ();
}


#if _WIN32

static BOOL APIENTRY logSwapIntervalEXT (int interval)
{
	LogFile->Printf ("%s (%d)\n", "wglSwapIntervalEXT", interval);
	return lib.SwapIntervalEXT (interval);
}


#elif __linux__

#endif

static const GL_t logFuncs = {
	logAlphaFunc,
	logBegin,
	logBindTexture,
	logBlendFunc,
	logClear,
	logClearColor,
	logColor3f,
	logColor3fv,
	logColor4f,
	logColor4ubv,
	logColorPointer,
	logCullFace,
	logDeleteTextures,
	logDepthFunc,
	logDepthMask,
	logDepthRange,
	logDisable,
	logDisableClientState,
	logDrawElements,
	logEnable,
	logEnableClientState,
	logEnd,
	logFinish,
	logFogf,
	logFogfv,
	logGetIntegerv,
	logGetString,
	logLoadIdentity,
	logLoadMatrixf,
	logMatrixMode,
	logOrtho,
	logPolygonMode,
	logPolygonOffset,
	logPopMatrix,
	logPushMatrix,
	logReadPixels,
	logRotatef,
	logScissor,
	logShadeModel,
	logTexCoord2f,
	logTexCoordPointer,
	logTexEnvf,
	logTexEnvfv,
	logTexEnvi,
	logTexImage2D,
	logTexParameteri,
	logTexSubImage2D,
	logTranslatef,
	logVertex2f,
	logVertex3f,
	logVertex3fv,
	logVertexPointer,
	logViewport
#if _WIN32

,
	logChoosePixelFormat,
	logDescribePixelFormat,
	logSetPixelFormat,
	logSwapBuffers,
	logCreateContext,
	logDeleteContext,
	logGetProcAddress,
	logMakeCurrent
#endif

,
	logSelectTextureSGIS,
	logSelectTextureCoordSetSGIS,
	logActiveTextureARB,
	logClientActiveTextureARB,
	logLockArraysEXT,
	logUnlockArraysEXT
#if _WIN32

,
	logSwapIntervalEXT
#elif __linux__

#endif


};

#endif // NO_GL_LOG

typedef struct {
	const char *names;				// "alias1\0alias2\0...\0" or "name\0\0"
	const char *name;				// current name (points to a primary name or to any alias)
	const char *cvar;				// name of cvar to disable extension
	short	first, count;			// positions of provided functions in name table
	unsigned require, deprecate;	// dependent extensions
} extInfo_t;

static extInfo_t extInfo[NUM_EXTENSIONS] = {
	{"GL_SGIS_multitexture\0", NULL, "gl_ext_multitexture", NUM_GLFUNCS+0, 2, 0, QGL_ARB_MULTITEXTURE},
	{"GL_ARB_multitexture\0", NULL, "gl_ext_multitexture", NUM_GLFUNCS+2, 2, 0, 0},
	{"GL_ARB_texture_env_add\0" "GL_EXT_texture_env_add\0", NULL, "gl_ext_texture_env_add", 0, 0, 0, 0},
	{"GL_EXT_texture_env_combine\0", NULL, "gl_ext_texture_env_combine", 0, 0, QGL_ARB_MULTITEXTURE, QGL_ARB_TEXTURE_ENV_COMBINE},
	{"GL_ARB_texture_env_combine\0", NULL, "gl_ext_texture_env_combine", 0, 0, QGL_ARB_MULTITEXTURE, 0},
	{"GL_NV_texture_env_combine4\0", NULL, "gl_ext_texture_env_combine_nv", 0, 0, QGL_ARB_MULTITEXTURE, 0},
	{"GL_ATI_texture_env_combine3\0" "GL_ATIX_texture_env_combine3\0", NULL, "gl_ext_texture_env_combine_ati", 0, 0, QGL_ARB_MULTITEXTURE, QGL_NV_TEXTURE_ENV_COMBINE4},
	{"GL_S3_s3tc\0", NULL, "gl_ext_texture_compression", 0, 0, 0, QGL_ARB_TEXTURE_COMPRESSION | QGL_EXT_TEXTURE_COMPRESSION_S3TC},
	{"GL_ARB_texture_compression\0", NULL, "gl_ext_texture_compression", 0, 0, 0, QGL_EXT_TEXTURE_COMPRESSION_S3TC},
	{"GL_EXT_texture_compression_s3tc\0", NULL, "gl_ext_texture_compression", 0, 0, 0, 0},
	{"GL_EXT_compiled_vertex_array\0", NULL, "gl_ext_compiled_vertex_array", NUM_GLFUNCS+4, 2, 0, 0},
	{"GL_NV_texture_rectangle\0" "GL_ARB_texture_rectangle\0" "GL_EXT_texture_rectangle\0", NULL, "gl_ext_texture_rectangle", 0, 0, 0, 0},
	{"GL_NV_fog_distance\0", NULL, "gl_ext_fog_distance_nv", 0, 0, 0, 0},
	{"GL_EXT_texture_lod_bias\0", NULL, NULL, 0, 0, 0, 0}
#if _WIN32

,
	{"WGL_EXT_swap_control\0", NULL, NULL, NUM_GLFUNCS+6, 1, 0, 0}
#elif __linux__

#endif

};

