/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// cl_view.cpp -- player rendering positioning

//?? combine with cl_tent.cpp and put as cl_scene.cpp ?

#include "client.h"
#include "cmodel.h"					// for brush debug

#if GUN_DEBUG
// development tools for weapons
int		gun_frame;
CRenderModel *gun_model;
#endif

#if !NO_DEBUG
static cvar_t	*cl_testentities;
static cvar_t	*cl_testlights;
static cvar_t	*cl_testblend;

static cvar_t	*r_playerpos;
static cvar_t	*r_surfinfo;
static cvar_t	*r_showBrush;
#endif // NO_DEBUG
static cvar_t	*r_drawfps;

cvar_t			*scr_viewsize;		// non-static for menu

static int		r_numdlights;
static dlight_t	r_dlights[MAX_DLIGHTS];

static int		r_numentities;
static entity_t	r_entities[MAX_ENTITIES];

float			r_blend[4];			// rgba 0-1 full screen blend

char cl_weaponmodels[MAX_CLIENTWEAPONMODELS][MAX_QPATH];
int num_cl_weaponmodels;


static void V_ClearScene()
{
	r_numdlights  = 0;
	r_numentities = 0;
}


// player effects
static CBasicImage *disguiseShader, *irShader, *shellShader;
// railgun trails
CBasicImage *railSpiralShader, *railRingsShader, *railBeamShader;


void V_AddEntity(entity_t *ent)
{
	if (r_numentities >= MAX_ENTITIES)
		return;
	entity_t &e = r_entities[r_numentities++];
	e = *ent;
	e.pos.axis.FromEuler(e.angles);
}

// same as V_AddEntity(), but without axis computation
void V_AddEntity2(entity_t *ent)
{
	if (r_numentities >= MAX_ENTITIES)
		return;
	entity_t &e = r_entities[r_numentities++];
	e = *ent;
}

void AddEntityWithEffects2(entity_t *ent, unsigned fx)
{
#if 0
	// disguise: in rogue; alyays available (can be activated with console cmd "disguise");
	// disguise used in single-player games; originally, q2 have no 2rd person - so, can be visible
	// in coop games only; disappear when player is killed only; strange thing ...
	if ((fx & RF_USE_DISGUISE) && disguiseShader)
	{
		ent->customShader = disguiseShader;
		ent->skinnum      = 0;
		ent->skin         = NULL;
	}
	else
#endif
	if ((fx & RF_IR_VISIBLE) && (cl.refdef.rdflags & RDF_IRGOGGLES))
	{
		if (irShader)
		{
			ent->customShader = irShader;
			ent->skinnum      = 0;
			ent->skin         = NULL;
		}
		else
		{
			// shader should exists, but ...
			ent->flags |= RF_FULLBRIGHT;
		}
		ent->flags &= ~RF_GLOW;		// no glow for IR-visible
	}

	V_AddEntity2(ent);

	// color shells generate a seperate entity for the main model
	// check for COLOR_SHELL
	if (fx & (RF_SHELL_RED|RF_SHELL_GREEN|RF_SHELL_BLUE|RF_SHELL_DOUBLE|RF_SHELL_HALF_DAM))
	{
		ent->skin         = NULL;
		ent->customShader = shellShader;
		ent->color.rgba   = 0x202020;				// required for RED/GREEN/BLUE
		if (fx & RF_SHELL_HALF_DAM)
			ent->color.rgba = RGB(0.56, 0.59, 0.45);
		if (fx & RF_SHELL_DOUBLE)
			ent->color.rgba = RGB(0.9, 0.7 ,0);
		if (fx & RF_SHELL_RED)
			ent->color.c[0] = 255;
		if (fx & RF_SHELL_GREEN)
			ent->color.c[1] = 255;
		if (fx & RF_SHELL_BLUE)
			ent->color.c[2] = 255;
		ent->color.c[3] = 255;
		V_AddEntity2(ent);
	}
}

void AddEntityWithEffects(entity_t *ent, unsigned fx)
{
	ent->pos.axis.FromEuler(ent->angles);
	AddEntityWithEffects2(ent, fx);
}


static void RegisterShaders()
{
	disguiseShader   = RE_RegisterSkin("fx/disguiseShell");
	irShader         = RE_RegisterSkin("fx/irModel");
	shellShader      = RE_RegisterSkin("fx/colorShell");
	railBeamShader   = RE_RegisterSkin("fx/railBeam");
	railSpiralShader = RE_RegisterSkin("fx/railSpiral");
	railRingsShader  = RE_RegisterSkin("fx/railRings");
}


void V_AddLight(const CVec3 &org, float intensity, float r, float g, float b)
{
	if (r_numdlights >= MAX_DLIGHTS) return;
	dlight_t &dl = r_dlights[r_numdlights++];
	dl.origin    = org;
	dl.intensity = intensity;
	dl.color.Set(r, g, b);
}


// If cl_testentities is set, create 32 player models
#if !NO_DEBUG

static void TestEntities()
{
	r_numentities = 32;
	memset(r_entities, 0, sizeof(r_entities));

	for (int i = 0; i < r_numentities; i++)
	{
		entity_t &ent = r_entities[i];

		float r = 64 * (i % 4) - (1*64+32);
		float f = 64 * (i / 4) - (3*64+32);
		VectorMA(cl.refdef.vieworg, 192, cl.v_forward, ent.pos.origin);
		ent.pos.origin[0] += r;
		ent.pos.origin[1] += f;
		ent.pos.axis = identAxis;
		// animate frames
#if 0
#define FIRST_FRAME	0
#define LAST_FRAME	39
#else
#define FIRST_FRAME 40
#define LAST_FRAME	45
#endif
		ent.frame = (cl.time + 99) / 100 % (LAST_FRAME-FIRST_FRAME+1) + FIRST_FRAME;
		ent.oldframe = ent.frame - 1;
		if (ent.oldframe < FIRST_FRAME)
			ent.oldframe = LAST_FRAME;
		ent.backlerp = 1.0f - (cl.time % 100) / 100.0f;
		if (ent.backlerp == 1)
			ent.backlerp = 0;
		ent.model = RE_RegisterModel("players/male/tris.md2");
		ent.skin = NULL;	//??
	}
#undef FIRST_FRAME
#undef LAST_FRAME
}


// If cl_testlights is set, create 32 lights models
static void TestLights()
{
	int		i;
	dlight_t *dl;

	r_numdlights = MAX_DLIGHTS;
	memset(r_dlights, 0, sizeof(r_dlights));

	for (i = 0, dl = r_dlights; i < MAX_DLIGHTS; i++, dl++)
	{
#if 0
		if (!(cl_testlights->integer & (1<<i)))
		{
			dl--;
			r_numdlights--;
			continue;
		}
#endif
		float r = 64 * ((i % 4) - 1.5);
		float f = 64 * (i / 4) + 128;

		for (int j = 0; j < 3; j++)
			dl->origin[j] = cl.refdef.vieworg[j] + cl.v_forward[j] * f + cl.v_right[j] * r;
		dl->color[0] = ((i % 6) + 1) & 1;
		dl->color[1] = (((i % 6) + 1) & 2) >> 1;
		dl->color[2] = (((i % 6) + 1) & 4) >> 2;
		dl->intensity = 200;
	}
}

static void SetLight_f(bool usage, int argc, char **argv)
{
	if (usage || argc != 3)
	{
		appPrintf("Usage: setlight <style> <value=0..2>\n");
		return;
	}
	if (!cls.cheatsEnabled)
		return;
	int style = atoi(argv[1]);
	if (style < 0 || style > 255)
	{
		appWPrintf("bad style: %d\n", style);
		return;
	}
	char str[2];
	str[0] = appRound(atof(argv[2]) * ('m' - 'a')) + 'a';
	str[1] = 0;
	CL_SetLightstyle(style, str);
}

#endif // NO_DEBUG

//===================================================================


// weights of separate loading units
#define W_MAP			10
#define W_TEXTURES		3
#define W_MODEL			1
#define W_CLIENT		3
#define W_SKY			1


void V_InitRenderer()
{
	guard(V_InitRenderer);

	int		i;

	if (!cl.configstrings[CS_MODELS+1][0]) return;			// no map loaded
	SCR_SetLevelshot();

	// wait a small time to let server complete initialization
	// allow map to be changed before initializing renderer, when loading savegames,
	// saved at point of changing map; this situation is possible because:
	//   - server frame#1:
	//      - loading savegame, will create configstrings + may be insert command string "gamemap <newmap>"
	//      - send configstrings#1
	//   - server frame#2:
	//      - exec "gamemap" command from previous frame, change map etc.
	//      - send configstrings#2
	// in some circumstances, client will receive configstrings#1 after server frame#2 - this will load
	// client map#1 after loading server map#2 (out-of-sync client/server maps -- fatal error)
	static int startTime = 0;
	static int serverCount = 0;
	if (serverCount != cl.servercount)
	{
		serverCount = cl.servercount;
		startTime = cls.realtime;
		return;
	}
	if (cls.realtime < startTime + 300)
		return;

	CL_ClearEffects();		// can use shaders ...
	CL_ClearTEnts();		// temp entities linked to models, which are invalid after vid_restart

	// let the render dll load the map
	char mapname[MAX_QPATH];
	strcpy(mapname, cl.configstrings[CS_MODELS+1] + 5);	// skip "maps/"
	mapname[strlen(mapname)-4] = 0;		// cut off ".bsp"

	// compute total loading weight
	float totalWeight = W_MAP + W_TEXTURES + W_SKY;
	for (i = 1; i < MAX_MODELS && cl.configstrings[CS_MODELS+i][0]; i++) // models
		if (cl.configstrings[CS_MODELS+i][0] != '*')
			totalWeight += W_MODEL;
	for (i = 0; i < MAX_CLIENTS; i++)	// clients
		if (cl.configstrings[CS_PLAYERSKINS+i][0])
			totalWeight += W_CLIENT;

	// register models, pics, and skins
	float loadingFrac = 0;
	SCR_LoadingNotify(va("map: %s", mapname), 0);
	loadingFrac += W_MAP / totalWeight;
	RE_LoadNewWorld();

	// precache status bar pics
	SCR_TouchPics();

	CL_RegisterTEntModels();
	RegisterShaders();

	strcpy(cl_weaponmodels[0], "weapon.md2");	// default weapon model
	num_cl_weaponmodels = 1;

	for (i = 1; i < MAX_MODELS && cl.configstrings[CS_MODELS+i][0]; i++)
	{
		const char *name = cl.configstrings[CS_MODELS+i];
		if (name[0] != '*')
		{
			SCR_LoadingNotify(name, loadingFrac);
			loadingFrac += W_MODEL / totalWeight;
		}
		if (name[0] == '#')
		{
			// special player weapon model
			if (num_cl_weaponmodels < MAX_CLIENTWEAPONMODELS)
				strncpy(cl_weaponmodels[num_cl_weaponmodels++], cl.configstrings[CS_MODELS+i]+1,
					sizeof(cl_weaponmodels[0])-1);
		}
		else
		{
			const char *mdl = cl.configstrings[CS_MODELS + i];
			const char *ext = strrchr(name, '.');
			// load model, but do not reload BSP file:
			cl.model_draw[i] = (!ext || stricmp(ext, ".bsp")) ? RE_RegisterModel(mdl) : NULL;
			cl.model_clip[i] = (name[0] == '*') ? CM_InlineModel(mdl) : NULL;
		}
	}

	SCR_LoadingNotify("textures", loadingFrac);
	loadingFrac += W_TEXTURES / totalWeight;
	for (i = 1; i < MAX_IMAGES && cl.configstrings[CS_IMAGES+i][0]; i++)
		cl.image_precache[i] = RE_RegisterPic(va("pics/%s", cl.configstrings[CS_IMAGES+i]));

	for (i = 0; i < MAX_CLIENTS; i++)
	{
		if (!cl.configstrings[CS_PLAYERSKINS+i][0])
			continue;
		SCR_LoadingNotify(va("client %d: %s", i, cl.configstrings[CS_PLAYERSKINS+i]), loadingFrac);
		loadingFrac += W_CLIENT / totalWeight;
		CL_ParseClientinfo(i);
	}
	if (!cl.configstrings[CS_PLAYERSKINS+cl.playernum][0])
	{
		// in a singleplayer mode, server will not send clientinfo - generate it by
		// ourself for correct third-person view
//		Com_DPrintf("Fixing CI[playernum]\n");
		CL_UpdatePlayerClientInfo();
	}

	// setup sky
	SCR_LoadingNotify("sky", loadingFrac);
	float rotate = atof(cl.configstrings[CS_SKYROTATE]);
	CVec3 axis;
	sscanf(cl.configstrings[CS_SKYAXIS], "%f %f %f", &axis[0], &axis[1], &axis[2]);
	RE_SetSky(cl.configstrings[CS_SKY], rotate, axis);

	// the renderer can now free unneeded stuff
	RE_FinishLoadingWorld();

	Con_ClearNotify();
	SCR_EndLoadingPlaque(true);
	cl.rendererReady  = true;
	cl.forceViewFrame = true;

	// start the cd track
	CDAudio_Play(atoi(cl.configstrings[CS_CDTRACK]), true);

	unguard;
}


/*-----------------------------------------------------------------------------
	Support for reducing rendering viewport
-----------------------------------------------------------------------------*/

static struct
{
	int		x, y, width, height;
} scr_vrect;					// position of render window (changed when scr_viewsize cvar modified)
//?? rename to v_vrect ?

static void CalcVrect()
{
	float frac = scr_viewsize->Clamp(40, 100) / 100.0f;

	scr_vrect.width = appRound(viddef.width * frac);
	scr_vrect.height = appRound(viddef.height * frac);

	scr_vrect.width  &= ~1;		// align(2)
	scr_vrect.height &= ~1;		// align(2)

	scr_vrect.x = (viddef.width - scr_vrect.width) / 2;
	scr_vrect.y = (viddef.height - scr_vrect.height) / 2;
}


static void TileClear()
{
	const char *tileName = "pics/backtile";	//?? try "*detail" with a different color
	if (con_height == viddef.height || scr_viewsize->integer == 100) return;

	int y1 = scr_vrect.y;
	int y2 = y1 + scr_vrect.height;
	RE_TileClear(0, 0, viddef.width, y1, tileName);
	RE_TileClear(0, y2, viddef.width, viddef.height - y2, tileName);
	int x1 = scr_vrect.x;
	int x2 = x1 + scr_vrect.width;
	RE_TileClear(0, y1, x1, scr_vrect.height, tileName);
	RE_TileClear(x2, y1, viddef.width - x2, scr_vrect.height, tileName);

	return;
}


float CalcFov(float fov_x, float width, float height)
{
	if (fov_x < 1 || fov_x > 179)
		Com_DropError("Bad fov: %g", fov_x);

	float aspect = width / tan(fov_x / 360 * M_PI);
	return atan(height / aspect) * 360 / M_PI;
}

//============================================================================

#if GUN_DEBUG

// gun frame debugging functions
static void Gun_Next_f()
{
	gun_frame++;
	appPrintf("frame %i\n", gun_frame);
}

static void Gun_Prev_f()
{
	gun_frame--;
	if (gun_frame < 0)
		gun_frame = 0;
	appPrintf("frame %i\n", gun_frame);
}

static void Gun_Model_f(int argc, char **argv)
{
	if (argc != 2)
	{
		gun_model = NULL;
		return;
	}
	gun_model = RE_RegisterModel(va("models/%s/tris.md2", argv[1]));
}

#endif


// Set a specific sky and rotation speed
static void Sky_f(bool usage, int argc, char **argv)
{
	if (argc < 2 || usage)
	{
		appPrintf("Usage: sky <basename> <rotate> <axis x y z>\n");
		return;
	}
	float rotate = (argc > 2) ? atof(argv[2]) : 0;
	CVec3 axis;
	if (argc == 6)
	{
		axis[0] = atof(argv[3]);
		axis[1] = atof(argv[4]);
		axis[2] = atof(argv[5]);
	}
	else
	{
		axis[0] = 0;
		axis[1] = 0;
		axis[2] = 1;
	}

	RE_SetSky(argv[1], rotate, axis);
}

//============================================================================

struct flagInfo_t
{
	unsigned code;
	const char *name;
};

static void DrawFlag(int flag, const flagInfo_t *info, int numFlags, const char *prefix)
{
#if 0
	int		i;

	for (i = 0; i < numFlags; i++, info++)
		if (flag & info->code)
			RE_DrawTextLeft(va("%s%s", prefix, info->name), RGB(0.3, 0.6, 0.4));
#else
	TString<256> Buf;
	Buf[0] = 0;
	for (int i = 0; i < numFlags; i++, info++)
		if (flag & info->code)
		{
			Buf += va(Buf[0] ? " %s%s" : "%s%s", prefix, info->name);
			if (Buf.len() > 40)
			{
				RE_DrawTextLeft(Buf, RGB(0.3, 0.6, 0.4));
				Buf[0] = 0;
			}
			flag &= ~info->code;
		}
	if (Buf[0])
		RE_DrawTextLeft(Buf, RGB(0.3, 0.6, 0.4));
	if (flag)
		RE_DrawTextLeft(va("%sUNK_%X", prefix, flag), RGB(0.6, 0.3, 0.4));
#endif
}


static void DecodeContents(int i)
{
	static const flagInfo_t contentsNames[] = {
#define T(name)		{CONTENTS_##name, #name}
		T(SOLID),	T(WINDOW),		T(AUX),		T(LAVA),	T(SLIME),		T(WATER),
		T(MIST),	T(ALPHA),		T(AREAPORTAL),	T(PLAYERCLIP),	T(MONSTERCLIP),
		T(CURRENT_0),	T(CURRENT_90),	T(CURRENT_180),	T(CURRENT_270),
		T(CURRENT_UP),	T(CURRENT_DOWN),T(ORIGIN),		T(MONSTER),
		T(DEADMONSTER),	T(DETAIL),		T(TRANSLUCENT),	T(LADDER)
#undef T
	};

//	RE_DrawTextLeft("Contents:", RGB(0.4, 0.4, 0.6));
//	RE_DrawTextLeft("---------", RGB(0.4, 0.4, 0.6));
	if (!i)
		RE_DrawTextLeft("CONTENTS_EMPTY", RGB(0.3, 0.6, 0.4));
	else
		DrawFlag(i, ARRAY_ARG(contentsNames), "CONTENTS_");
}


static char *ModelName(int modelIndex)
{
	if (!modelIndex) return NULL;
	if (modelIndex < 255)
		return cl.configstrings[CS_MODELS + modelIndex];
	else
		return "(custom)";
}


#if !NO_DEBUG

static CMemoryChain *debugMem;


static void DrawSurfInfo()
{
	static const flagInfo_t surfNames[] = {
#define T(name)		{SURF_##name, #name}
		T(LIGHT), T(SLICK), T(SKY), T(WARP),
		T(TRANS33), T(TRANS66), T(FLOWING), T(NODRAW),
		// Kingpin flags
		T(ALPHA), T(DIFFUSE), T(SPECULAR),
		// new flags (temp ??)
		T(AUTOFLARE)
#undef T
	};

	static const char *materialNames[MATERIAL_COUNT] = {
		"silent",
		"concrete",
		"fabric",
		"gravel",
		"metal",
		"metal_l",
		"snow",
		"tin",
		"tile",
		"wood",
		"water",
		"glass",
		"dirt"
	};

	CVec3 start, end;
	start = cl.refdef.vieworg;
	Euler2Vecs(cl.refdef.viewangles, &end, NULL, NULL);
	end.Scale(500);
	end.Add(start);

	unsigned cont = r_surfinfo->integer & 4 ? MASK_ALL : MASK_SHOT|MASK_WATER;
	trace_t	trace;
#if TRACE_DEBUG
	cm_showTrace = true;
#endif
//static CBox nullBox = {{-2,-2,-2},{2,2,2}};
//static CBox nullBox = {{-10,-10,-10},{10,10,10}};
//static CBox nullBox = {{-20,-20,-20},{20,20,20}};
	CM_BoxTrace(trace, start, end, nullBox, 0, cont);
	CM_ClipTraceToModels(trace, start, end, nullBox, cont);
#if TRACE_DEBUG
	cm_showTrace = false;
#endif
	if (!(r_surfinfo->integer & 2))
		CL_EntityTrace(trace, start, end, nullBox, cont);
	V_AddLight(trace.endpos, 5, 0.3, 0.6, 0.3);	//?? keep or remove? intens > trace bounds!

	if (trace.fraction < 1.0)
	{
		if (trace.brushNum >= 0)
		{
			// brush
			RE_DrawTextLeft(va("BrushNum: %d", trace.brushNum), RGB(0.2,0.4,0.1));
			if (r_surfinfo->integer & 8)
			{
				// visualize brush
				if (!debugMem) debugMem = new CMemoryChain;
				CBrush *brush = CM_BuildBrush(trace.brushNum, debugMem);
				if (brush)
					RE_DrawBrush(brush, "brush", trace.brushNum);
				else
					RE_DrawTextLeft(va("brush %d: error", trace.brushNum), RGB(1,0,0));
			}
		}
		RE_DrawTextLeft("Surface info:\n-------------", RGB(0.4,0.4,0.6));
		RE_DrawTextLeft(va("Point: %g  %g  %g", VECTOR_ARG(trace.endpos)), RGB(0.2,0.4,0.1));
		csurface_t *surf = trace.surface;
		RE_DrawTextLeft(va("Normal: %g  %g  %g", VECTOR_ARG(trace.plane.normal)), RGB(0.2,0.4,0.1));
		if (surf->fullName[0])		// non-null surface
		{
			RE_DrawTextLeft(va("Texture: %s", surf->fullName), RGB(0.2,0.4,0.1));
			if (surf->value)
				RE_DrawTextLeft(va("Value: %d", surf->value), RGB(0.2,0.4,0.1));
			DrawFlag(surf->flags, ARRAY_ARG(surfNames), "SURF_");
			// material
			const char *s = (surf->material > 0 && surf->material < MATERIAL_COUNT) ?
				materialNames[surf->material] : "?? bad ??";
			RE_DrawTextLeft(va("Material: %s", s), RGB(0.3,0.6,0.4));
		}
		DecodeContents(trace.contents);
		if (trace.ent)
		{
			clEntityState_t *ent = (clEntityState_t*)trace.ent;
			RE_DrawTextLeft("\nEntity:\n-------", RGB(0.4,0.4,0.6));
			RE_DrawTextLeft(va("Origin: %g %g %g", VECTOR_ARG(ent->origin)), RGB(0.2,0.4,0.1));
			RE_DrawTextLeft(va("fx: %X  rfx: %X", ent->effects, ent->renderfx), RGB(0.2,0.4,0.1));
			if (ent->modelindex)	RE_DrawTextLeft(va("model: %s", ModelName(ent->modelindex)), RGB(0.2,0.4,0.1));
			if (ent->modelindex2)	RE_DrawTextLeft(va("model2: %s", ModelName(ent->modelindex2)), RGB(0.2,0.4,0.1));
			if (ent->modelindex3)	RE_DrawTextLeft(va("model3: %s", ModelName(ent->modelindex3)), RGB(0.2,0.4,0.1));
			if (ent->modelindex4)	RE_DrawTextLeft(va("model4: %s", ModelName(ent->modelindex4)), RGB(0.2,0.4,0.1));

			CL_AddEntityBox(ent, RGB(1,0.1,0.1));
		}
		RE_DrawTextLeft("");	// empty line
	}
}

#if 0
//!!!!!!!!!
struct cbrush_t
{
	unsigned contents;
	void	*sides;
	int		numSides;
	int		traceFrame;					// to avoid repeated testings
};
extern cbrush_t	*map_brushes;
//!!!!!!!!!
#endif

static void DrawOriginInfo()
{
	RE_DrawTextLeft("Player position:\n----------------", RGB(0.4,0.4,0.6));
	RE_DrawTextLeft(va("Point: %.0f  %.0f  %.0f", VECTOR_ARG(cl.refdef.vieworg)), RGB(0.2,0.4,0.1));

	CVec3 view;
	Euler2Vecs(cl.refdef.viewangles, &view, NULL, NULL);
	RE_DrawTextLeft(va("View direction: %g  %g  %g", VECTOR_ARG(view)), RGB(0.2,0.4,0.1));

	const CBspLeaf *leaf = CM_FindLeaf(cl.refdef.vieworg);
	RE_DrawTextLeft(va("Leaf: %d, cluster: %d, zone: %d", leaf->num, leaf->cluster, leaf->zone), RGB(0.2,0.4,0.1));
#if 0
//!!!!!!!!!
RE_DrawTextLeft(va("nBrushes: %d", leaf->numBrushes), RGB(1,0.2,0.2));
for (int i = 0; i < leaf->numBrushes; i++)
{
cbrush_t* b = leaf->brushes[i];
RE_DrawTextLeft(va("%d: b=%d", i, b - map_brushes), RGB(1,0,0));
DecodeContents(b->contents);
}
//!!!!!!!!!
#endif
	DecodeContents(leaf->GetContents(cl.refdef.vieworg) | CM_PointModelContents(cl.refdef.vieworg));
	RE_DrawTextLeft("");		// empty line
}

static void DrawBrush()
{
	int brushNum = r_showBrush->integer;
	if (brushNum < 0) return;

	if (!debugMem) debugMem = new CMemoryChain;
	CBrush *brush = CM_BuildBrush(brushNum, debugMem);
	if (brush)
		RE_DrawBrush(brush, "brush", brushNum);
	else
		RE_DrawTextLeft(va("DrawBrush %d: error", brushNum), RGB(1,0,0));
}

#endif // NO_DEBUG

//#define FILTER_FPS_COUNTER		1

static void DrawFpsInfo()
{
	static float avgFps, minFps, maxFps;
	static int frames;
	static int64 startSecTime, lastFrameTime;

	int64 time = appCycles64();

	double timeDelta = appCyclesToMsecf(time - lastFrameTime);
	if (cls.key_dest != key_game || (timeDelta > 2000))
	{	// reinitialize counters
		startSecTime = lastFrameTime = time;
		frames = 0;
		maxFps = avgFps = 0;
		minFps = BIG_NUMBER;
		return;
	}

	// update min/max stats
	float tmpFps = (timeDelta == 0) ? 1000 : 1000.0f / timeDelta;
#if FILTER_FPS_COUNTER
	// median filter
	static float values[3];
	if (minFps > maxFps)
	{
		values[0] = values[1] = values[2] = tmpFps;
	}
	else
	{
		values[0] = values[1];
		values[1] = values[2];
		values[2] = tmpFps;
		if (values[0] < values[1] && values[0] < values[2])
		{
			// [0] is min
			tmpFps = min(values[1], values[2]);
		}
		else if (values[0] > values[1] && values[0] > values[2])
		{
			// [0] is max
			tmpFps = max(values[1], values[2]);
		}
		else
		{
			// [0] is average
			tmpFps = values[0];
		}
	}
#endif
	EXPAND_BOUNDS(tmpFps, minFps, maxFps);

	lastFrameTime = time;

	// update avg stats
	timeDelta = appCyclesToMsecf(time - startSecTime);
	frames++;
	if (timeDelta >= 500)			// update 2 times per second
	{
		avgFps = frames * 1000.0f / timeDelta;
		startSecTime = time;
		frames = 0;
	}

	// draw info
	unsigned color;
	if (avgFps < 15) color = RGB(1,0,0);
	else if (avgFps < 30) color = RGB(1,0,1);
	else if (avgFps < 60) color = RGB(1,1,0);
	else color = RGB(0,1,0);
	RE_DrawTextRight(va("FPS: %.2f  min: %.2f  max: %.2f", avgFps, minFps, maxFps), color);
}


#define MIN_WATER_DISTANCE	4.5

static void FixWaterVis()
{
	CVec3	p;			// point
	bool	w, w1;		// "in water" flag

	if (cl.refdef.rdflags & RDF_THIRD_PERSON)
		w1 = (CM_PointContents(cl.refdef.vieworg, 0) & MASK_WATER) != 0;	// need to recompute UNDERWATER flag
	else
		w1 = (cl.refdef.rdflags & RDF_UNDERWATER) != 0;
	p[0] = cl.refdef.vieworg[0];
	p[1] = cl.refdef.vieworg[1];

	// check point below
	p[2] = cl.refdef.vieworg[2] - MIN_WATER_DISTANCE;
	unsigned cont = CM_PointContents(p, 0);
	w = (cont & MASK_WATER) != 0;

	if (w1 == w || cont & MASK_SOLID)
	{
		// check point above
		p[2] += 2 * MIN_WATER_DISTANCE;
		cont = CM_PointContents(p, 0);
		w = (cont & MASK_WATER) != 0;
	}

	// do not require to fix position
	if (w1 == w || cont & MASK_SOLID)
		return;

	//?? improve by disabling this trick for correct maps

	// trace from air to water
	trace_t trace;
	if (!w1)
		CM_BoxTrace(trace, cl.refdef.vieworg, p, nullBox, 0, MASK_WATER);
	else
		CM_BoxTrace(trace, p, cl.refdef.vieworg, nullBox, 0, MASK_WATER);
	if (trace.fraction < 1.0f && !(trace.surface->flags & (SURF_TRANS33|SURF_TRANS66)))
	{
//		RE_DrawTextLeft("WATER FIX!", 1, 1, 1);
		if (cl.refdef.vieworg[2] < trace.endpos[2])
			cl.refdef.vieworg[2] = trace.endpos[2] - MIN_WATER_DISTANCE;
		else
			cl.refdef.vieworg[2] = trace.endpos[2] + MIN_WATER_DISTANCE;
	}
}


//#define PROFILE_VIEW	1

#if PROFILE_VIEW
#define PRF(x)	x
#else
#define PRF(x)
#endif


bool V_RenderView()
{
	guard(V_RenderView);

	if (cls.state != ca_active)
		return false;
	if (!cl.rendererReady)
		return false;			// still loading
	if (!bspfile.clientLoaded)
		return false;			// map already unloaded (by server), but client is still active (ca_active)

	if (timedemo->integer)
	{
		static unsigned lastTime = 0;

		unsigned time = appMilliseconds();
		if (!cl.timedemoStart)
		{
			cl.timedemoStart        = time;
//			cl.timedemoLongestFrame = 0;	-- cleared anyway within a new server map
//			cl.timedemoFrames       = 0;
		}
		else
		{
			unsigned timeDelta = time - lastTime;
			if (timeDelta > cl.timedemoLongestFrame) //?? && !fileFromPak)
				cl.timedemoLongestFrame = timeDelta;
		}
		lastTime = time;
		cl.timedemoFrames++;
	}

#if PROFILE_VIEW
	unsigned beforePrep = 0, beforeDebug = 0, beforeEffects = 0, afterEffects = 0, beforeRender, afterRender;
#endif

	// an invalid frame will just use the exact previous refdef
	// we can't use the old frame if the video mode has changed, though...
	if (cl.frame.valid && (cl.forceViewFrame || !cl_paused->integer))
	{
		PRF(beforePrep = appCycles());
		cl.forceViewFrame = false;

		CalcVrect();
		TileClear();
		V_ClearScene();

		cl.refdef.rdflags = cl.frame.playerstate.rdflags;

		// build a renderer entity list and calc cl.sim*
		// this also calls CL_CalcViewValues which loads
		// v_forward, etc.
		CL_AddEntities();
		PRF(beforeEffects = appCycles());
		CL_AddEffects();
		PRF(afterEffects = appCycles());
		CL_AddTEnts();

#if !NO_DEBUG
		if (cl_testentities->integer)	TestEntities();
		if (cl_testlights->integer)		TestLights();
		if (cl_testblend->integer)
		{
			r_blend[0] = 1;
			r_blend[1] = 0.5;
			r_blend[2] = 0.25;
			r_blend[3] = 0.5;
		}

		// debug output
		// free debug memory from previous frame
		if (debugMem)
		{
			delete debugMem;
			debugMem = NULL;
		}
		PRF(beforeDebug = appCycles());
		if (r_playerpos->integer)	DrawOriginInfo();
		if (r_surfinfo->integer)	DrawSurfInfo();
		DrawBrush();
#endif // NO_DEBUG

#if 0
		// never let it sit exactly on a node line, because a water plane can
		// dissapear when viewed with the eye exactly on it.
		// the server protocol only specifies to 1/8 pixel, so add 1/16 in each axis
		cl.refdef.vieworg[0] += 1.0f/16;	//??
		cl.refdef.vieworg[1] += 1.0f/16;
		cl.refdef.vieworg[2] += 1.0f/16;
#endif

		cl.refdef.x      = scr_vrect.x;
		cl.refdef.y      = scr_vrect.y;
		cl.refdef.width  = scr_vrect.width;
		cl.refdef.height = scr_vrect.height;
		cl.refdef.fov_y  = CalcFov(cl.refdef.fov_x, cl.refdef.width, cl.refdef.height);
		cl.refdef.time   = cl.ftime;

		cl.refdef.zonebits = cl.frame.zonebits;

		if (!cl_add_entities->integer)	r_numentities = 0;
		if (!cl_add_lights->integer)	r_numdlights = 0;
		if (!cl_add_blend->integer || cl.refdef.rdflags & RDF_THIRD_PERSON)
			r_blend[3] = 0;

		cl.refdef.num_entities = r_numentities;
		cl.refdef.entities     = r_entities;
		cl.refdef.particles    = cl_add_particles->integer ? active_particles : NULL;
		cl.refdef.beams        = active_beams;
		cl.refdef.num_dlights  = r_numdlights;
		cl.refdef.dlights      = r_dlights;
		cl.refdef.lightstyles  = cl_lightstyles;

		// underwater fov warp (taken from Q3 game source)
		if (cl.refdef.rdflags & RDF_UNDERWATER)
		{
			float v = sin(cl.ftime * 0.4f * M_PI * 2);
			cl.refdef.fov_x += v;
			cl.refdef.fov_y -= v;
		}
		FixWaterVis();
	}
	PRF(beforeRender = appCycles());
	// render scene
	RE_RenderFrame(&cl.refdef);
	PRF(afterRender = appCycles());
	// add full-screen blend
	if (r_blend[3])
		RE_Fill(cl.refdef.x, cl.refdef.y, cl.refdef.width, cl.refdef.height,
			RGBAS(r_blend[0], r_blend[1], r_blend[2], r_blend[3]));

#if PROFILE_VIEW
	RE_DrawTextLeft(va("V: prep1: %5.2f fx: %5.2f prep2: %5.2f dbg: %5.2f render: %5.2f",
		appCyclesToMsecf(beforeEffects - beforePrep),
		appCyclesToMsecf(afterEffects - beforeEffects),
		appCyclesToMsecf(beforeDebug - afterEffects),
		appCyclesToMsecf(beforeRender - beforeDebug),
		appCyclesToMsecf(afterRender - beforeRender)),
		RGB(0.2,1,0.2));
#endif

	// stats
	if (r_drawfps->integer)
		DrawFpsInfo();

	return true;
	unguard;
}


void V_Init()
{
CVAR_BEGIN(vars)
#if !NO_DEBUG
	CVAR_VAR(cl_testblend, 0, 0),
	CVAR_VAR(cl_testentities, 0, 0),
	CVAR_VAR(cl_testlights, 0, CVAR_CHEAT),

	CVAR_VAR(r_playerpos, 0, CVAR_CHEAT),
	CVAR_VAR(r_surfinfo, 0, CVAR_CHEAT),
	CVAR_VAR(r_showBrush, -1, CVAR_CHEAT),
#endif // NO_DEBUG

	CVAR_VAR(r_drawfps, 0, 0),
	CVAR_VAR(scr_viewsize, 100, CVAR_ARCHIVE)	//?? should rename: not scr var
CVAR_END

	Cvar_GetVars(ARRAY_ARG(vars));

#if GUN_DEBUG
	RegisterCommand("gun_next",  Gun_Next_f);
	RegisterCommand("gun_prev",  Gun_Prev_f);
	RegisterCommand("gun_model", Gun_Model_f);
#endif
#if !NO_DEBUG
	RegisterCommand("setlight", SetLight_f);
#endif
	RegisterCommand("sky", Sky_f);
}
