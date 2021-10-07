#ifndef MATH_3D_H
#define	MATH_3D_H

#include <stdio.h>
#include <math.h>

#define M_PI 3.14

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

struct Vector2i
{
    int x;
    int y;
};

struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f()
    {
    }

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f& operator+=(const Vector3f& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;

        return *this;
    }

    Vector3f& operator-=(const Vector3f& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;

        return *this;
    }

    Vector3f& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

	
	Vector3f& operator-()
    {
        x = -x;
        y = -y;
		z = -z;
    

        return *this;
    }

	




    Vector3f Cross(const Vector3f& v) const;

    Vector3f& Normalize();

    void Rotate(float Angle, const Vector3f& Axis);

	float Length();

    void Print() const
    {
        printf("(%.02f, %.02f, %.02f", x, y, z);
    }
};


inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x + r.x,
                 l.y + r.y,
                 l.z + r.z);

    return Ret;
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x - r.x,
                 l.y - r.y,
                 l.z - r.z);

    return Ret;
}

inline Vector3f operator*(const Vector3f& l, float f)
{
    Vector3f Ret(l.x * f,
                 l.y * f,
                 l.z * f);

    return Ret;
}




inline Vector3f operator*(float f, const Vector3f& l)
{
    Vector3f Ret(l.x * f,
                 l.y * f,
                 l.z * f);

    return Ret;
}


inline Vector3f operator/(const Vector3f& l, float f)
{
    Vector3f Ret(l.x / f,
                 l.y / f,
				 l.z / f
                 );

    return Ret;
}

struct Vector2f
{
	float x;
	float y;

	
	Vector2f() {};
	
	Vector2f(float _x, float _y)
	{
		x = _x;
		y = _y;
	
	}

Vector2f& operator*=(float f)
    {
        x *= f;
        y *= f;
    

        return *this;
    }

Vector2f& operator-()
    {
        x = -x;
        y = -y;
    

        return *this;
    }



float Length();

};

inline Vector2f operator*(const Vector2f& l, float f)
{
    Vector2f Ret(l.x * f,
                 l.y * f
                 );

    return Ret;
}

inline Vector2f operator*(float f, const Vector2f& l)
{
    Vector2f Ret(l.x * f,
                 l.y * f
                 );

    return Ret;
}

inline Vector2f operator-(const Vector2f& l, const Vector2f& r)
{
    Vector2f Ret(l.x - r.x,
                 l.y - r.y
                 );

    return Ret;
}

inline Vector2f operator/(const Vector2f& l, float f)
{
    Vector2f Ret(l.x / f,
                 l.y / f
                 );

    return Ret;
}


inline Vector2f operator+(const Vector2f& l, const Vector2f& r)
{
    Vector2f Ret(l.x + r.x,
                 l.y + r.y
                 );

    return Ret;
}



class Matrix4f
{
public:
    float m[4][4];

    Matrix4f()
    {        
    }


    inline void InitIdentity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    inline Matrix4f operator*(const Matrix4f& Right) const
    {
        Matrix4f Ret;

        for (unsigned int i = 0 ; i < 4 ; i++) {
            for (unsigned int j = 0 ; j < 4 ; j++) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                              m[i][1] * Right.m[1][j] +
                              m[i][2] * Right.m[2][j] +
                              m[i][3] * Right.m[3][j];
            }
        }

        return Ret;
    }

    void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
    void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
    void InitTranslationTransform(float x, float y, float z);
    void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
    void InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar);
	void InitOrthoProjTransform(float left, float right, float bottom, float top, float zNear, float zFar);
};


struct Quaternion
{
    float x, y, z, w;

    Quaternion(float _x, float _y, float _z, float _w);

    void Normalize();

    Quaternion Conjugate();  
 };

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);




struct Vector4f
{
	float x;
	float y;
	float z;
	float w;

	Vector4f()
    {
    }

    Vector4f(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
		w = _w;
    }



};

inline Vector4f operator*(const Matrix4f& Matrix, const Vector4f& Vector)
{
    Vector4f Ret;
	Ret.x = Matrix.m[0][0] * Vector.x + Matrix.m[0][1] * Vector.y + Matrix.m[0][2] * Vector.z + Matrix.m[0][3] * Vector.w;
	Ret.y = Matrix.m[1][0] * Vector.x + Matrix.m[1][1] * Vector.y + Matrix.m[1][2] * Vector.z + Matrix.m[1][3] * Vector.w;
	Ret.z = Matrix.m[2][0] * Vector.x + Matrix.m[2][1] * Vector.y + Matrix.m[2][2] * Vector.z + Matrix.m[2][3] * Vector.w;
	Ret.w =	Matrix.m[3][0] * Vector.x + Matrix.m[3][1] * Vector.y + Matrix.m[3][2] * Vector.z + Matrix.m[3][3] * Vector.w;

    return Ret;
}

#endif	/* MATH_3D_H */

