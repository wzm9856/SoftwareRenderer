#pragma once
#include "vec.h"
#include <assert.h>

template<size_t M, size_t N>
class Matrix {
public:
	double e[M][N];
	Matrix() {}
	Matrix(double e0) {
		for (size_t i = 0; i < M; i++) {
			for (size_t j = 0; j < N; j++) {
				e[i][j] = e0;
			}
		}
	}
	Matrix(const Matrix<M,N>& a) {
		for (size_t i = 0; i < M; i++) {
			for (size_t j = 0; j < N; j++) {
				e[i][j] = a[i][j];
			}
		}
	}
	const double* operator[] (size_t i) const { assert(i < M); return e[i]; }
	double* operator[] (size_t i) { assert(i < M); return e[i]; }
};

typedef Matrix<3, 3> mat3;
typedef Matrix<4, 4> mat4;

template<size_t M, size_t N>
inline Matrix<N, M> transpose(const Matrix<M, N>& a) {
	Matrix<N, M> b;
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			b[j][i] = a[i][j];
		}
	}
	return b;
}

template<size_t M, size_t N>
inline Matrix<M, N> ones() {
	Matrix<M, N> a(0);
	for (size_t i = 0; i < M && i < N; i++) {
		a[i][i] = 1;
	}
	return a;
}

template<size_t M, size_t N>
inline bool operator==(const Matrix<M, N>& a, const Matrix<M, N>& b) {
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			if (abs(a[i][j] - b[i][j]) > 1e-6)
				return false;
		}
	}
	return true;
}

template<size_t M, size_t N>
inline Matrix<M, N> operator +(const Matrix<M, N>& a, const Matrix<M, N>& b) {
	Matrix<M, N> ans;
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			ans[i][j] = a[i][j] + b[i][j];
		}
	}
	return ans;
}
template<size_t M, size_t N>
inline Matrix<M, N> operator +(const Matrix<M, N>& a, double b) {
	Matrix<M, N> ans;
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			ans[i][j] = a[i][j] + b;
		}
	}
	return ans;
}
template<size_t M, size_t N>
inline Matrix<M, N>& operator +=(Matrix<M, N>& a, const Matrix<M, N>& b) {
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			a[i][j] += b[i][j];
		}
	}
	return a;
}
template<size_t M, size_t N>
inline Matrix<M, N>& operator +=(Matrix<M, N>& a, double b) {
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			a[i][j] += b;
		}
	}
	return a;
}

template<size_t M, size_t N>
inline Matrix<M, N> operator -(const Matrix<M, N>& a, const Matrix<M, N>& b) {
	Matrix<M, N> ans;
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			ans[i][j] = a[i][j] - b[i][j];
		}
	}
	return ans;
}
template<size_t M, size_t N>
inline Matrix<M, N> operator -(const Matrix<M, N>& a, double b) {
	Matrix<M, N> ans;
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			ans[i][j] = a[i][j] - b;
		}
	}
	return ans;
}
template<size_t M, size_t N>
inline Matrix<M, N>& operator -=(Matrix<M, N>& a, const Matrix<M, N>& b) {
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			a[i][j] -= b[i][j];
		}
	}
	return a;
}
template<size_t M, size_t N>
inline Matrix<M, N>& operator -=(Matrix<M, N>& a, double b) {
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			a[i][j] -= b;
		}
	}
	return a;
}
template<size_t M, size_t N, size_t P>
inline Matrix<M, N> operator *(const Matrix<M, N>& a, const Matrix<N, P>& b) {
	Matrix<M, P> ans(0);
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < P; j++) {
			for (size_t t = 0; t < N; t++) {
				ans[i][j] += a[i][t] * b[t][j];
			}
		}
	}
	return ans;
}
template<size_t M, size_t N>
inline Vector<M> operator *(const Matrix<M, N>& a, const Vector<N>& b) {
	Vector<M> ans(0);
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			ans[i] += a[i][j] * b[j];
		}
	}
	return ans;
}
template<size_t M, size_t N>
inline Matrix<M, N> operator *(const Matrix<M, N>& a, double b) {
	Matrix<M, N> ans;
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			ans[i][j] = a[i][j] * b;
		}
	}
	return ans;
}
template<size_t M, size_t N>
inline Matrix<M, N>& operator *=(Matrix<M, N>& a, double b) {
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			a[i][j] *= b;
		}
	}
	return a;
}
template<size_t M, size_t N>
inline Matrix<M, N> operator /(const Matrix<M, N>& a, double b) {
	Matrix<M, N> ans;
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			ans[i][j] = a[i][j] / b;
		}
	}
	return ans;
}
template<size_t M, size_t N>
inline Matrix<M, N>& operator /=(Matrix<M, N>& a, double b) {
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			a[i][j] /= b;
		}
	}
	return a;
}

template<size_t M, size_t N>
inline std::ostream& operator << (std::ostream& out, const Matrix<M,N>& a) {
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < N; j++) {
			out << a[i][j] << '\t';
		}
		out << std::endl;
	}
	return out;
};

mat4 rotateXY(float x, float y)
{
	float sx = sin(x);
	float cx = cos(x);
	float sy = sin(y);
	float cy = cos(y);

	mat4 ans(0);
	ans[0][0] = cy;
	ans[0][2] = sy;
	ans[1][0] = sx * sy;
	ans[1][1] = cx;
	ans[1][2] = -sx * cy;
	ans[2][0] = -cx * sy;
	ans[2][1] = sx;
	ans[2][2] = cx * cy;
	ans[3][3] = 1.0f;
	return ans;
}

mat4 lookAt(const vec3& eye, const vec3& at, const vec3& u=vec3(0,1,0))
{
	//vec3 view = normalize(at - eye);
	vec3 view = normalize(eye-at);
	vec3 right = normalize(cross(u, view));
	vec3 up = cross(view, right);

	mat4 ans(0);
	ans[0][0] = right.x();
	ans[0][1] = right.y();
	ans[0][2] = right.z();
	
	ans[1][0] = up.x();
	ans[1][1] = up.y();
	ans[1][2] = up.z();
	
	ans[2][0] = view.x();
	ans[2][1] = view.y();
	ans[2][2] = view.z();
	
	ans[0][3] = -dot(right, eye);
	ans[1][3] = -dot(up, eye);
	ans[2][3] = -dot(view, eye);
	ans[3][3] = 1.0f;
	return ans;
}

mat4 perspective(double fovy, double aspect, double zn, double zf) {
	mat4 ans(0);
	fovy = fovy / 180 * 3.1415926;
	double height = 1.0f / tan(fovy * 0.5f);
	ans[0][0] = height / aspect;
	ans[1][1] = height;
	ans[2][2] = (zf + zn) / (zn - zf);
	ans[2][3] = 2 * zn * zf / (zn - zf);
	ans[3][2] = -1;
	return ans;
}