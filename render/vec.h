#pragma once
#include<cmath>
#include<iostream>
#include<assert.h>

template <size_t N> 
class Vector {
public:
	double e[N];
	Vector(double e0 = 0) { for (size_t i = 0; i < N; i++) e[i] = e0; }
	Vector(double e0, double e1) { assert(N == 2); e[0] = e0; e[1] = e1; }
	Vector(double e0, double e1, double e2) { assert(N == 3); e[0] = e0; e[1] = e1; e[2] = e2;}
	Vector(double e0, double e1, double e2, double e3) { assert(N == 4); e[0] = e0; e[1] = e1; e[2] = e2; e[3] = e3; }
	Vector(const Vector& v) { for (size_t i = 0; i < N; i++) e[i] = v[i]; }
	template<size_t N1> operator Vector<N1>() const {
		Vector<N1> b;
		for (size_t i = 0; i < N1; i++)
			b[i] = (i < N) ? e[i] : 0;
		return b;
	}
	double& operator[] (size_t i) { assert(i < N); return e[i]; }
	const double& operator[] (size_t i) const { assert(i < N); return e[i]; }
	double& x() { assert(N > 0); return e[0]; }
	double& y() { assert(N > 1); return e[1]; }
	double& z() { assert(N > 2); return e[2]; }
	double& w() { assert(N > 3); return e[3]; }
	const double& x() const{ assert(N > 0); return e[0]; }
	const double& y() const{ assert(N > 1); return e[1]; }
	const double& z() const{ assert(N > 2); return e[2]; }
	const double& w() const{ assert(N > 3); return e[3]; }

	double length_squared() const { double ans = 0; for (size_t i = 0; i < N; i++) ans += e[i] * e[i]; return ans; }
	double length() const { return std::sqrt(this->length_squared()); }
};

typedef Vector<2> vec2;
typedef Vector<3> vec3;
typedef Vector<4> vec4;

template <size_t N>
inline Vector<N> operator + (const Vector<N>& a, const Vector<N>& b) {
	Vector<N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] + b[i];
	return c;
};

template <size_t N>
inline Vector<N> operator + (const Vector<N>& a, double b) {
	Vector<N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] + b;
	return c;
};

template <size_t N>
inline Vector<N>& operator += (Vector<N>& a, const Vector<N>& b) {
	for (size_t i = 0; i < N; i++) a[i] += b[i];
	return a;
};

template <size_t N>
inline Vector<N>& operator += (Vector<N>& a, double b) {
	for (size_t i = 0; i < N; i++) a[i] += b;
	return a;
};

template <size_t N>
inline Vector<N> operator - (const Vector<N>& a, const Vector<N>& b) {
	Vector<N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] - b[i];
	return c;
};

template <size_t N>
inline Vector<N> operator - (const Vector<N>& a, double b) {
	Vector<N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] - b;
	return c;
};

template <size_t N>
inline Vector<N>& operator -= (Vector<N>& a, const Vector<N>& b) {
	for (size_t i = 0; i < N; i++) a[i] -= b[i];
	return a;
};

template <size_t N>
inline Vector<N>& operator -= (Vector<N>& a, double b) {
	for (size_t i = 0; i < N; i++) a[i] -= b;
	return a;
};

template <size_t N>
inline Vector<N> operator * (const Vector<N>& a, const Vector<N>& b) {
	Vector<N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] * b[i];
	return c;
};

template <size_t N>
inline Vector<N> operator * (const Vector<N>& a, double b) {
	Vector<N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] * b;
	return c;
};

template <size_t N>
inline Vector<N>& operator *= (Vector<N>& a, const Vector<N>& b) {
	for (size_t i = 0; i < N; i++) a[i] *= b[i];
	return a;
};

template <size_t N>
inline Vector<N>& operator *= (Vector<N>& a, double b) {
	for (size_t i = 0; i < N; i++) a[i] *= b;
	return a;
};

template <size_t N>
inline Vector<N> operator / (const Vector<N>& a, const Vector<N>& b) {
	Vector<N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] / b[i];
	return c;
};

template <size_t N>
inline Vector<N> operator / (const Vector<N>& a, double b) {
	Vector<N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] / b;
	return c;
};

template <size_t N>
inline Vector<N>& operator /= (Vector<N>& a, const Vector<N>& b) {
	for (size_t i = 0; i < N; i++) a[i] /= b[i];
	return a;
};

template <size_t N>
inline Vector<N>& operator /= (Vector<N>& a, double b) {
	for (size_t i = 0; i < N; i++) a[i] /= b;
	return a;
};

template <size_t N>
inline Vector<N> operator == (const Vector<N>& a, const Vector<N>& b) {
	for (size_t i = 0; i < N; i++) {
		if (abs(a[i] - b[i]) > 1e-6)
			return false;
	}
	return true;
};

template<size_t N>
inline std::ostream& operator << (std::ostream& out, const Vector<N>& v) {
	for (size_t i = 0; i < N; i++)
		out << v[i] << "\t";
	out << std::endl;
	return out;
};

template<size_t N>
inline double dot(const Vector<N>& a, const Vector<N>& b) {
	double ans = 0;
	for (size_t i = 0; i < N; i++)
		ans += a[i] * b[i];
	return ans;
}

inline double cross(const Vector<2>& a, const Vector<2>& b) {
	return a[0] * b[1] - a[1] * b[0];
}

inline Vector<3> cross(const Vector<3>& a, const Vector<3>& b) {
	return Vector<3>(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

inline Vector<4> cross(const Vector<4>& a, const Vector<4>& b) {
	return Vector<4>(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0], a[3]);
}

template<size_t N>
inline Vector<N> normalize(const Vector<N>& a) {
	return a / a.length();
}

template<size_t N>
inline double dist(const Vector<N>& a, const Vector<N>& b) {
	double ans = 0;
	for (size_t i = 0; i < N; i++)
		ans += pow(a[i] - b[i], 2);
	return sqrt(ans);
}

template<size_t N1, size_t N2>
inline Vector<N1> vector_convert(const Vector<N2>& a) {
	Vector<N1> b;
	for (size_t i = 0; i < N1; i++)
		b[i] = (i < N2) ? a[i] : 0;
	return b;
}

template<size_t N>
inline Vector<N> lerp(const Vector<N>& a, const Vector<N>& b, double ratio) {
	Vector<N> ans;
	for (size_t i = 0; i < N; i++)
		ans[i] = a[i] * ratio + b[i] * (1 - ratio);
	return ans;
}

inline Vector<3> homoto3(const Vector<4>& a) {
	Vector<3> ans;
	assert(abs(a[3]) > 1e-6);
	ans[0] = a[0] / a[3];
	ans[1] = a[1] / a[3];
	ans[2] = a[2] / a[3];
	return ans;
}

inline Vector<3> homotoscreen(const Vector<4>& a, size_t halfWidth, size_t halfHeight) {
	Vector<3> ans;
	assert(abs(a[3]) > 1e-6);
	ans[0] = (a[0] / a[3] + 1) * halfWidth;
	ans[1] = (a[1] / a[3] + 1) * halfHeight;
	ans[2] = a[2] / a[3];
	return ans;
}

inline Vector<4> tohomo(const Vector<3>& a) {
	return Vector<4>(a[0], a[1], a[2], 1);
}
//vec reflect(const vec& v, const vec& n) {
//	return v - 2 * dot(v, n) * n;
//}
//vec refract(const vec& uv, const vec& n, double etai_over_etat) {
//	auto cos_theta = fmin(dot(-uv, n), 1.0);
//	vec r_out_perp = etai_over_etat * (uv + cos_theta * n);
//	vec r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
//	return r_out_perp + r_out_parallel;
//}

struct Vertex {
	Vertex(double x, double y, double z, double r, double g, double b, double u, double v):
		pos(x,y,z),color(r,g,b),tex(u,v){}
	vec3 pos;
	vec3 color;
	vec2 tex;
};