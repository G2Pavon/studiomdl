

extern char outname[1024];
extern bool cdset;
extern char cdpartial[256];
extern char cddir[256];
extern int cdtextureset;
extern char cdtexture[16][256];
extern float default_scale;
extern float scale_up;
extern float defaultzrotation;
extern float zrotation;
extern char defaulttexture[16][256];
extern char sourcetexture[16][256];
extern int numrep;
extern int tag_reversed;
extern int tag_normals;
extern int flip_triangles;
extern float normal_blend;
extern int dump_hboxes;
extern int ignore_warnings;
extern vec3_t eyeposition;
extern int gflags;
extern vec3_t bbox[2];
extern vec3_t cbox[2];
extern bool keep_all_bones;

extern void WriteFile(void);
void *kalloc(int num, int size);

struct s_trianglevert_t
{
	int vertindex;
	int normindex; // index into normal array
	int s, t;
	float u, v;
};

struct s_vertex_t
{
	int bone;	// bone transformation index
	vec3_t org; // original position
};

struct s_normal_t
{
	int skinref;
	int bone;	// bone transformation index
	vec3_t org; // original position
};

// dstudiobone_t bone[MAXSTUDIOBONES];
struct s_bonefixup_t
{
	vec3_t worldorg;
	float m[3][4];
	float im[3][4];
	float length;
};
extern s_bonefixup_t bonefixup[MAXSTUDIOSRCBONES];

extern int numbones;
struct s_bonetable_t
{
	char name[32];		// bone name for symbolic links
	int parent;			// parent bone
	int bonecontroller; // -1 == 0
	int flags;			// X, Y, Z, XR, YR, ZR
	// short		value[6];	// default DoF values
	vec3_t pos;		   // default pos
	vec3_t posscale;   // pos values scale
	vec3_t rot;		   // default pos
	vec3_t rotscale;   // rotation values scale
	int group;		   // hitgroup
	vec3_t bmin, bmax; // bounding box
};
extern s_bonetable_t bonetable[MAXSTUDIOSRCBONES];

extern int numrenamedbones;
struct s_renamebone_t
{
	char from[32];
	char to[32];
};
extern s_renamebone_t renamedbone[MAXSTUDIOSRCBONES];

extern int numhitboxes;
struct s_bbox_t
{
	char name[32]; // bone name
	int bone;
	int group; // hitgroup
	int model;
	vec3_t bmin, bmax; // bounding box
};
extern s_bbox_t hitbox[MAXSTUDIOSRCBONES];

extern int numhitgroups;
struct s_hitgroup_t
{
	int models;
	int group;
	char name[32]; // bone name
};
extern s_hitgroup_t hitgroup[MAXSTUDIOSRCBONES];

struct s_bonecontroller_t
{
	char name[32];
	int bone;
	int type;
	int index;
	float start;
	float end;
};

extern s_bonecontroller_t bonecontroller[MAXSTUDIOSRCBONES];
extern int numbonecontrollers;

struct s_attachment_t
{
	char name[32];
	char bonename[32];
	int index;
	int bone;
	int type;
	vec3_t org;
};

extern s_attachment_t attachment[MAXSTUDIOSRCBONES];
extern int numattachments;

struct s_node_t
{
	char name[64];
	int parent;
	int mirrored;
};

extern char mirrored[MAXSTUDIOSRCBONES][64];
extern int nummirrored;

extern int numani;
struct s_animation_t
{
	char name[64];
	int startframe;
	int endframe;
	int flags;
	int numbones;
	s_node_t node[MAXSTUDIOSRCBONES];
	int bonemap[MAXSTUDIOSRCBONES];
	int boneimap[MAXSTUDIOSRCBONES];
	vec3_t *pos[MAXSTUDIOSRCBONES];
	vec3_t *rot[MAXSTUDIOSRCBONES];
	int numanim[MAXSTUDIOSRCBONES][6];
	mstudioanimvalue_t *anim[MAXSTUDIOSRCBONES][6];
};
extern s_animation_t *panimation[MAXSTUDIOSEQUENCES * MAXSTUDIOBLENDS]; // each sequence can have 16 blends

struct s_event_t
{
	int event;
	int frame;
	char options[64];
};

struct s_pivot_t
{
	int index;
	vec3_t org;
	int start;
	int end;
};

extern int numseq;
struct s_sequence_t
{
	int motiontype;
	vec3_t linearmovement;

	char name[64];
	int flags;
	float fps;
	int numframes;

	int activity;
	int actweight;

	int frameoffset; // used to adjust frame numbers

	int numevents;
	s_event_t event[MAXSTUDIOEVENTS];

	int numpivots;
	s_pivot_t pivot[MAXSTUDIOPIVOTS];

	int numblends;
	s_animation_t *panim[MAXSTUDIOGROUPS];
	float blendtype[2];
	float blendstart[2];
	float blendend[2];

	vec3_t automovepos[MAXSTUDIOANIMATIONS];
	vec3_t automoveangle[MAXSTUDIOANIMATIONS];

	int seqgroup;
	int animindex;

	vec3_t bmin;
	vec3_t bmax;
	int entrynode;
	int exitnode;
	int nodeflags;
};

extern s_sequence_t sequence[MAXSTUDIOSEQUENCES];
extern int numseqgroups;

struct s_sequencegroup_t
{
	char label[32];
	char name[64];
};

extern s_sequencegroup_t sequencegroup[MAXSTUDIOSEQUENCES];
extern int numxnodes;
extern int xnode[100][100];

struct rgb_t
{
	byte r, g, b;
};
struct rgb2_t
{
	byte b, g, r, x;
};

// FIXME: what about texture overrides inline with loading models

struct s_texture_t
{
	char name[64];
	int flags;
	int srcwidth;
	int srcheight;
	byte *ppicture;
	rgb_t *ppal;
	float max_s;
	float min_s;
	float max_t;
	float min_t;
	int skintop;
	int skinleft;
	int skinwidth;
	int skinheight;
	float fskintop;
	float fskinleft;
	float fskinwidth;
	float fskinheight;
	int size;
	void *pdata;
	int parent;
};

extern s_texture_t texture[MAXSTUDIOSKINS];
extern int numtextures;
// extern  float gamma; // old; clashes with deprecated gamma
extern float texgamma;
extern int numskinref;
extern int numskinfamilies;
extern int skinref[256][MAXSTUDIOSKINS]; // [skin][skinref], returns texture index
extern int numtexturegroups;
extern int numtexturelayers[32];
extern int numtexturereps[32];
extern int texturegroup[32][32][32];

struct s_mesh_t
{
	int alloctris;
	int numtris;
	s_trianglevert_t (*triangle)[3];

	int skinref;
	int numnorms;
};

struct s_bone_t
{
	vec3_t pos;
	vec3_t rot;
};

struct s_model_t
{
	char name[64];

	int numbones;
	s_node_t node[MAXSTUDIOSRCBONES];
	s_bone_t skeleton[MAXSTUDIOSRCBONES];
	int boneref[MAXSTUDIOSRCBONES];	 // is local bone (or child) referenced with a vertex
	int bonemap[MAXSTUDIOSRCBONES];	 // local bone to world bone mapping
	int boneimap[MAXSTUDIOSRCBONES]; // world bone to local bone mapping

	vec3_t boundingbox[MAXSTUDIOSRCBONES][2];

	s_mesh_t *trimesh[MAXSTUDIOTRIANGLES];
	int trimap[MAXSTUDIOTRIANGLES];

	int numverts;
	s_vertex_t vert[MAXSTUDIOVERTS];

	int numnorms;
	s_normal_t normal[MAXSTUDIOVERTS];

	int nummesh;
	s_mesh_t *pmesh[MAXSTUDIOMESHES];

	float boundingradius;

	int numframes;
	float interval;
	struct s_model_s *next;
};

extern int nummodels;
extern s_model_t *model[MAXSTUDIOMODELS];
extern vec3_t adjust;
extern vec3_t defaultadjust;

struct s_bodypart_t
{
	char name[32];
	int nummodels;
	int base;
	s_model_t *pmodel[MAXSTUDIOMODELS];
};

extern int numbodyparts;
extern s_bodypart_t bodypart[MAXSTUDIOBODYPARTS];

extern int BuildTris(s_trianglevert_t (*x)[3], s_mesh_t *y, byte **ppdata);
int LoadBMP(const char *szFile, byte **ppbBits, byte **ppbPalette, int *width, int *height);
