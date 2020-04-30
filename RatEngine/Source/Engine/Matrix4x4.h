#pragma once
#include <memory>
#include "Vector3.h"

class Matrix4x4
{
public:
	Matrix4x4()
	{
	}

	~Matrix4x4()
	{
	}

	void setIdentity()
	{
		memset(mat, 0, sizeof(float) * 16);
		mat[0][0] = mat[1][1] = mat[2][2] = mat[3][3] = 1;
	}

	void setTranslation(const Vector3& translation)
	{
		setIdentity();
		mat[3][0] = translation.x;
		mat[3][1] = translation.y;
		mat[3][2] = translation.z;
	}

	void setScale(const Vector3& scale)
	{
		setIdentity();
		mat[0][0] = scale.x;
		mat[1][1] = scale.y;
		mat[2][2] = scale.z;
	}

	void setRotationX(float theta)
	{
		mat[1][1] = cos(theta);
		mat[1][2] = sin(theta);
		mat[2][1] = -sin(theta);
		mat[2][2] = cos(theta);
	}

	void setRotationY(float theta)
	{
		mat[0][0] = cos(theta);
		mat[0][2] = -sin(theta);
		mat[2][0] = sin(theta);
		mat[2][2] = cos(theta);
	}

	void setRotationZ(float theta)
	{
		mat[0][0] = cos(theta);
		mat[0][1] = sin(theta);
		mat[1][0] = -sin(theta);
		mat[1][1] = cos(theta);
	}

	void operator *=(const Matrix4x4& matrix)
	{
		Matrix4x4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.mat[i][j] = mat[i][0] * matrix.mat[0][j] +
							mat[i][1] * matrix.mat[1][j] +
							mat[i][2] * matrix.mat[2][j] +
							mat[i][3] * matrix.mat[3][j];
			}
		}

		memcpy(mat, out.mat, sizeof(float) * 16);
	}

	void setOrthoLH(float width, float height, float nearPlane, float farPlane)
	{
		setIdentity();
		mat[0][0] = 2.0f / width;
		mat[1][1] = 2.0f / height;
		mat[2][2] = 1.0f / (farPlane - nearPlane);
		mat[3][2] = -(nearPlane / (farPlane - nearPlane));
	}

public:

	float mat[4][4] = {};
};