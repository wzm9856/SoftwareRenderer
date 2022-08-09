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
	double div = a[3];
	if (abs(div) < 1e-6) div += div > 0 ? 1e-6 : -1e-6;
	ans[0] = a[0] / div;
	ans[1] = a[1] / div;
	ans[2] = a[2] / div;
	return ans;
}

inline Vector<3> homotoscreen(const Vector<4>& a, size_t halfWidth, size_t halfHeight) {
	Vector<3> ans;
	double div = a[3];
	if (abs(div) < 1e-6) div += div > 0 ? 1e-6 : -1e-6;
	ans[0] = (a[1] / div + 1) * halfHeight;
	ans[1] = (a[0] / div + 1) * halfWidth;
	ans[2] = a[2] / div;
	return ans;
}

inline Vector<4> tohomo(const Vector<3>& a) {
	return Vector<4>(a[0], a[1], a[2], 1);
}

struct Vertex {
	Vertex(double x, double y, double z, double r, double g, double b, double u, double v):
		pos(x,y,z),color(r,g,b),tex(u,v) {}
	Vertex() {}
	vec3 pos;
	vec3 color;
	vec2 tex;
	double invz;
	vec3 camPos;
	vec3 scrPos;
};

inline double threeInter(double a, double b, double c, double coea, double coeb, double coec) {
	return coea * a + coeb * b + coec * c;
}

Vertex interpolateVertex(const Vertex& a, const Vertex& b, const Vertex& c, double sa, double sb, double sc) {
	double worldz = 1 / (a.invz * sa + b.invz * sb + c.invz * sc);
	double coea = sa * a.invz * worldz;
	double coeb = sb * b.invz * worldz;
	double coec = sc * c.invz * worldz;
	//double x = (sa * a.pos.x() * a.invz + sb * b.pos.x() * b.invz + sc * c.pos.x() * c.invz) / worldz;
	double r = threeInter(a.color.x(), b.color.x(), c.color.x(), coea, coeb, coec);
	double g = threeInter(a.color.y(), b.color.y(), c.color.y(), coea, coeb, coec);
	double bb= threeInter(a.color.z(), b.color.z(), c.color.z(), coea, coeb, coec);
	double u = threeInter(a.tex.x(), b.tex.x(), c.tex.x(), coea, coeb, coec);
	double v = threeInter(a.tex.y(), b.tex.y(), c.tex.y(), coea, coeb, coec);
	double wx = threeInter(a.camPos.x(), b.camPos.x(), c.camPos.x(), coea, coeb, coec);
	double wy = threeInter(a.camPos.y(), b.camPos.y(), c.camPos.y(), coea, coeb, coec);
	Vertex ans;
	ans.color = vec3(r, g, bb);
	ans.tex = vec2(u, v);
	ans.camPos = vec3(wx, wy, worldz);
	return ans;
}