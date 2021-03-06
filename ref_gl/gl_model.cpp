#include "OpenGLDrv.h"
#include "gl_model.h"

//#define TEST_LOAD		1	// will add command "loadmodel <filename>"; may implement this in client (not renderer)

namespace OpenGLDrv {


model_t *modelsArray[MAX_GLMODELS];
int	modelCount;


model_t	*FindModel(const char *name)
{
	guard(R_FindModel);

	TString<64> Name2;
	Name2.filename(name);

#if 0
	// try to load md3 instead of md2
	//!! disable later
	char *ext = strrchr(Name2, '.');
	if (ext && !strcmp(ext, ".md2"))
	{
		ext[3] = '3';		// ".md2" -> ".md3"
		if (GFileSystem->FileExists(Name2))
			return FindModel(Name2);
		// md3 model with the same name is not found -- load md2
		ext[3] = '2';		// ".md3" -> ".md2"
	}
#endif

	model_t *m;
	// search already loaded models
	for (int i = 0; i < modelCount; i++)
	{
		m = modelsArray[i];
		if (!m) continue;				//?? should not happens
		if (Name2 == m->Name)
		{
			// found
			if (m->type == MODEL_UNKNOWN)
				return NULL;			// model name was cached to avoid file system lookup again
			return m;
		}
	}
	if (Name2[0] == '*')
		Com_DropError("R_FindModel: inline model \"%s\" was not found", *Name2);

	if (modelCount == MAX_GLMODELS)
	{
		appWPrintf("R_FindModel: MAX_GLMODELS\n");
		return NULL;
	}

	/*----- not found -- load model ------*/
	unsigned len;
	unsigned *file;
	if (!(file = (unsigned*) GFileSystem->LoadFile(Name2, &len)))
	{
		m = new model_t;
		m->Name = Name2;
		modelsArray[modelCount++] = m;
		Com_DPrintf("R_FindModel: not found: %s\n", *Name2);
		return NULL;	// file not found
	}

	switch (LittleLong(*file))
	{
	case MD2_IDENT:
		m = LoadMd2(Name2, (byte*)file, len);
		break;
	case MD3_IDENT:
		m = LoadMd3(Name2, (byte*)file, len);
		break;
	case SP2_IDENT:
		m = LoadSp2(Name2, (byte*)file, len);
		break;
	case BYTES4('/','/','S','P'):
		m = LoadSpr(Name2, (byte*)file, len);
		break;
	default:
		// no error here: simply ignore unknown model formats
		appWPrintf("R_FindModel: unknown ID 0x%X in %s", (unsigned)LittleLong(*file), name);
		m = NULL;
	}
	if (m) modelsArray[modelCount++] = m;

	delete file;
	return m;

	unguardf(("mdl=%s", name));
}


void FreeModels()
{
	// free non-inline models
	for (int i = 0; i < modelCount; i++)
	{
		model_t *m = modelsArray[i];
		if (m && m->size >= 0) delete m;		// when size < 0 -- not allocated directly, used from different place
	}

	memset(modelsArray, 0, sizeof(modelsArray));
	modelCount = 0;

	// free map data
	if (map.dataChain) delete map.dataChain;
	if (map.lightGridChain) delete map.lightGridChain;
	memset(&map, 0, sizeof(map));
}


static void Modellist_f(bool usage, int argc, char **argv)
{
	static const char *types[] = {"unk",	"inl",	"spr",		"md3"};		// see modelType_t
	static const char *colors[] = {S_RED,	"",		S_MAGENTA, S_GREEN};	// ...

	if (argc > 2 || usage)
	{
		appPrintf("Usage: modellist [mask]\n");
		return;
	}
	const char *mask = (argc == 2) ? argv[1] : NULL;

	int totalSize = 0;
	int totalCount = 0;
	appPrintf("-----type-size----name---------\n");
	for (int i = 0; i < modelCount; i++)
	{
		//?? skip inline model from list, or compact (print somethink like this: "0-96 inl *1..*97")
		model_t *m = modelsArray[i];
		if (mask && !appMatchWildcard(m->Name, mask, true)) continue;
		totalCount++;
		appPrintf("%-3d  %3s  %-7d %s%s\n", i, types[m->type], m->size, colors[m->type], *m->Name);
		totalSize += m->size;
	}
	appPrintf("Displayed %d/%d models, used %d bytes\n", totalCount, modelCount, totalSize);
}


#if TEST_LOAD
static void LoadModel_f(bool usage, int argc, char **argv)
{
	if (argc != 2 || usage)
	{
		appPrintf("Usage: loadmodel <filename>\n");
		return;
	}
	FindModel(argv[1]);
}
#endif


void InitModels()
{
	memset(&map, 0, sizeof(map));

	RegisterCommand("modellist", Modellist_f);
#if TEST_LOAD
	RegisterCommand("loadmodel", LoadModel_f);
#endif
}


void ShutdownModels()
{
	FreeModels();

	UnregisterCommand("modellist");
#if TEST_LOAD
	UnregisterCommand("loadmodel");
#endif
}


} // namespace
