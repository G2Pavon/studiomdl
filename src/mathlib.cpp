// mathlib.c -- math primitives

#include <cmath>

#include "cmdlib.hpp"
#include "mathlib.hpp"

void MatrixCopy(float in[3][4], float out[3][4])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			out[i][j] = in[i][j];
		}
	}
}

void AngleMatrix(const vec3_t &angles, float matrix[3][4])
{
	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle = angles.z * (Q_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles.y * (Q_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles.x * (Q_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;
	matrix[0][1] = sr * sp * cy + cr * -sy;
	matrix[1][1] = sr * sp * sy + cr * cy;
	matrix[2][1] = sr * cp;
	matrix[0][2] = cr * sp * cy + -sr * -sy;
	matrix[1][2] = cr * sp * sy + -sr * cy;
	matrix[2][2] = cr * cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}

void AngleIMatrix(const vec3_t &angles, float matrix[3][4])
{
	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle = angles.z * (Q_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles.y * (Q_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles.x * (Q_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	matrix[0][0] = cp * cy;
	matrix[0][1] = cp * sy;
	matrix[0][2] = -sp;
	matrix[1][0] = sr * sp * cy + cr * -sy;
	matrix[1][1] = sr * sp * sy + cr * cy;
	matrix[1][2] = sr * cp;
	matrix[2][0] = cr * sp * cy + -sr * -sy;
	matrix[2][1] = cr * sp * sy + -sr * cy;
	matrix[2][2] = cr * cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}

void R_ConcatTransforms(const float in1[3][4], const float in2[3][4], float out[3][4])
{
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
				in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
				in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
				in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
				in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
				in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
				in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
				in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
				in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
				in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
				in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
				in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
				in1[2][2] * in2[2][3] + in1[2][3];
}

void VectorTransform(const vec3_t &in1, const float in2[3][4], vec3_t &out)
{
	vec3_t row0(in2[0][0], in2[0][1], in2[0][2]);
	vec3_t row1(in2[1][0], in2[1][1], in2[1][2]);
	vec3_t row2(in2[2][0], in2[2][1], in2[2][2]);

	out.x = in1.dot(row0) + in2[0][3];
	out.y = in1.dot(row1) + in2[1][3];
	out.z = in1.dot(row2) + in2[2][3];
}
