// tristrip - convert triangle list into tristrips and fans
#include <ctime>
#include <cstring>

#include "cmdlib.hpp"
#include "qctokenizer.hpp"
#include "mathlib.hpp"
#include "studio.hpp"
#include "studiomdl.hpp"

int g_used[MAXSTUDIOTRIANGLES];
// the command list holds counts and s/t values that are valid for
// every frame
short g_commands[MAXSTUDIOTRIANGLES * 13];
// all frames will have their vertexes rearranged and expanded
// so they are in the order expected by the command list
int g_stripverts[MAXSTUDIOTRIANGLES + 2];
int g_striptris[MAXSTUDIOTRIANGLES + 2];
int g_stripcount;
int g_neighbortri[MAXSTUDIOTRIANGLES][3];
int g_neighboredge[MAXSTUDIOTRIANGLES][3];

s_trianglevert_t (*g_triangles)[3];
s_mesh_t *g_pmesh;

void FindNeighbor(int starttri, int startv)
{
	s_trianglevert_t m1, m2;
	int j, k;
	s_trianglevert_t *last, *check;

	// used[starttri] |= (1 << startv);
	last = &g_triangles[starttri][0];

	m1 = last[(startv + 1) % 3];
	m2 = last[(startv + 0) % 3];

	for (j = starttri + 1, check = &g_triangles[starttri + 1][0]; j < g_pmesh->numtris; j++, check += 3)
	{
		if (g_used[j] == 7)
			continue;
		for (k = 0; k < 3; k++)
		{
			if (memcmp(&check[k], &m1, sizeof(m1)))
				continue;
			if (memcmp(&check[(k + 1) % 3], &m2, sizeof(m2)))
				continue;

			g_neighbortri[starttri][startv] = j;
			g_neighboredge[starttri][startv] = k;

			g_neighbortri[j][k] = starttri;
			g_neighboredge[j][k] = startv;

			g_used[starttri] |= (1 << startv);
			g_used[j] |= (1 << k);
			return;
		}
	}
}

int StripLength(int starttri, int startv)
{
	int j;
	int k;

	g_used[starttri] = 2;

	g_stripverts[0] = (startv) % 3;
	g_stripverts[1] = (startv + 1) % 3;
	g_stripverts[2] = (startv + 2) % 3;

	g_striptris[0] = starttri;
	g_striptris[1] = starttri;
	g_striptris[2] = starttri;
	g_stripcount = 3;

	while (true)
	{
		if (g_stripcount & 1)
		{
			j = g_neighbortri[starttri][(startv + 1) % 3];
			k = g_neighboredge[starttri][(startv + 1) % 3];
		}
		else
		{
			j = g_neighbortri[starttri][(startv + 2) % 3];
			k = g_neighboredge[starttri][(startv + 2) % 3];
		}
		if (j == -1 || g_used[j])
			break;

		g_stripverts[g_stripcount] = (k + 2) % 3;
		g_striptris[g_stripcount] = j;
		g_stripcount++;

		g_used[j] = 2;

		starttri = j;
		startv = k;
	}

	// clear the temp used flags
	for (j = 0; j < g_pmesh->numtris; j++)
		if (g_used[j] == 2)
			g_used[j] = 0;

	return g_stripcount;
}

int FanLength(int starttri, int startv)
{
	int j;
	int k;

	g_used[starttri] = 2;

	g_stripverts[0] = (startv) % 3;
	g_stripverts[1] = (startv + 1) % 3;
	g_stripverts[2] = (startv + 2) % 3;

	g_striptris[0] = starttri;
	g_striptris[1] = starttri;
	g_striptris[2] = starttri;
	g_stripcount = 3;

	while (true)
	{
		j = g_neighbortri[starttri][(startv + 2) % 3];
		k = g_neighboredge[starttri][(startv + 2) % 3];

		if (j == -1 || g_used[j])
			break;

		g_stripverts[g_stripcount] = (k + 2) % 3;
		g_striptris[g_stripcount] = j;
		g_stripcount++;

		g_used[j] = 2;

		starttri = j;
		startv = k;
	}
	// clear the temp used flags
	for (j = 0; j < g_pmesh->numtris; j++)
		if (g_used[j] == 2)
			g_used[j] = 0;

	return g_stripcount;
}

// Generate a list of trifans or strips for the model, which holds for all frames
int g_numcommandnodes;

int BuildTris(s_trianglevert_t (*x)[3], s_mesh_t *y, std::uint8_t **ppdata)
{
	int i, j, k, m;
	int len, bestlen, besttype;
	int bestverts[MAXSTUDIOTRIANGLES];
	int besttris[MAXSTUDIOTRIANGLES];
	int peak[MAXSTUDIOTRIANGLES];
	int total = 0;
	long t;
	int maxlen;

	g_triangles = x;
	g_pmesh = y;

	t = time(NULL);

	for (i = 0; i < g_pmesh->numtris; i++)
	{
		g_neighbortri[i][0] = g_neighbortri[i][1] = g_neighbortri[i][2] = -1;
		g_used[i] = 0;
		peak[i] = g_pmesh->numtris;
	}

	// printf("finding neighbors\n");
	for (i = 0; i < g_pmesh->numtris; i++)
	{
		for (k = 0; k < 3; k++)
		{
			if (g_used[i] & (1 << k))
				continue;

			FindNeighbor(i, k);
		}
		// printf("%d", g_used[i] );
	}
	// printf("\n");

	//
	// build tristrips
	//
	g_numcommandnodes = 0;
	int numcommands = 0;
	std::memset(g_used, 0, sizeof(g_used));

	for (i = 0; i < g_pmesh->numtris;)
	{
		// pick an unused triangle and start the trifan
		if (g_used[i])
		{
			i++;
			continue;
		}

		maxlen = 9999;
		bestlen = 0;
		m = 0;
		for (k = i; k < g_pmesh->numtris && bestlen < 127; k++)
		{
			int localpeak = 0;

			if (g_used[k])
				continue;

			if (peak[k] <= bestlen)
				continue;

			m++;
			for (int type = 0; type < 2; type++)
			{
				for (int startv = 0; startv < 3; startv++)
				{
					if (type == 1)
						len = FanLength(k, startv);
					else
						len = StripLength(k, startv);
					if (len > 127)
					{
						// skip these, they are too long to encode
					}
					else if (len > bestlen)
					{
						besttype = type;
						bestlen = len;
						for (j = 0; j < bestlen; j++)
						{
							besttris[j] = g_striptris[j];
							bestverts[j] = g_stripverts[j];
						}
						// printf("%d %d\n", k, bestlen );
					}
					if (len > localpeak)
						localpeak = len;
				}
			}
			peak[k] = localpeak;
			if (localpeak == maxlen)
				break;
		}
		total += (bestlen - 2);

		// printf("%d (%d) %d\n", bestlen, pmesh->numtris - total, i );

		maxlen = bestlen;

		// mark the tris on the best strip as used
		for (j = 0; j < bestlen; j++)
			g_used[besttris[j]] = 1;

		if (besttype == 1)
			g_commands[numcommands++] = -bestlen;
		else
			g_commands[numcommands++] = bestlen;

		for (j = 0; j < bestlen; j++)
		{
			s_trianglevert_t *tri;

			tri = &g_triangles[besttris[j]][bestverts[j]];

			g_commands[numcommands++] = tri->vertindex;
			g_commands[numcommands++] = tri->normindex;
			g_commands[numcommands++] = tri->s;
			g_commands[numcommands++] = tri->t;
		}
		// printf("%d ", bestlen - 2 );
		g_numcommandnodes++;

		if (t != time(NULL))
		{
			printf("%2d%%\r", (total * 100) / g_pmesh->numtris);
			t = time(NULL);
		}
	}

	g_commands[numcommands++] = 0; // end of list marker

	*ppdata = (std::uint8_t *)g_commands;

	// printf("%d %d %d\n", g_numcommandnodes, numcommands, pmesh->numtris  );
	return numcommands * sizeof(short);
}
