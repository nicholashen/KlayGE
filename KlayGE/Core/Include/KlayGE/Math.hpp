// Math.hpp
// KlayGE 数学函数库 头文件
// Ver 3.12.0
// 版权所有(C) 龚敏敏, 2003-2011
// Homepage: http://www.klayge.org
//
// 3.12.0
// Implement moved to Math.cpp (2011.2.15)
//
// 3.8.0
// 增加了decompose (2009.1.24)
//
// 3.6.0
// 重写了intersect_ray (2007.5.11)
// 来自Quake III的recip_sqrt (2007.6.22)
//
// 3.4.0
// 增加了refract和fresnel_term (2006.8.22)
//
// 2.6.0
// 改进了ComputeBoundingSphere (2005.5.23)
//
// 2.5.0
// 改为通过返回值返回结果 (2005.4.12)
//
// 2.1.1
// 修改了自定义类型 (2004.4.22)
// 增加了网格函数 (2004.5.18)
//
// 2.0.4
// 修改了Random的接口 (2004.3.29)
//
// 2.0.0
// 初次建立 (2003.9.18)
//
// 修改记录
///////////////////////////////////////////////////////////////////////////////

#ifndef _MATH_HPP
#define _MATH_HPP

#pragma once

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <KlayGE/PreDeclare.hpp>

#include <limits>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iterator>

#include <boost/static_assert.hpp>

namespace KlayGE
{
	// 常量定义
	/////////////////////////////////////////////////////////////////////////////////
	float const PI		= 3.141592f;			// PI
	float const PI2		= 6.283185f;			// PI * 2
	float const PIdiv2	= 1.570796f;			// PI / 2

	float const DEG90	= 1.570796f;			// 90 度
	float const DEG270	= -1.570796f;			// 270 度
	float const DEG45	= 0.7853981f;			// 45 度
	float const DEG5	= 0.0872664f;			// 5 度
	float const DEG10	= 0.1745329f;			// 10 度
	float const DEG20	= 0.3490658f;			// 20 度
	float const DEG30	= 0.5235987f;			// 30 度
	float const DEG60	= 1.047197f;			// 60 度
	float const DEG120	= 2.094395f;			// 120 度

	float const DEG40	= 0.6981317f;			// 40 度
	float const DEG80	= 1.396263f;			// 80 度
	float const DEG140	= 2.443460f;			// 140 度
	float const DEG160	= 2.792526f;			// 160 度

	float const SQRT2	= 1.414213f;			// 根2
	float const SQRT_2	= 0.7071068f;			// 1 / SQRT2
	float const SQRT3	= 1.732050f;			// 根3

	float const DEG2RAD	= 0.01745329f;			// 角度化弧度因数
	float const RAD2DEG	= 57.29577f;			// 弧度化角度因数

	enum BoundOverlap
	{
		BO_Yes,
		BO_No,
		BO_Partial
	};

	namespace MathLib
	{
		// 求绝对值
		template <typename T>
		inline T
		abs(T const & x)
		{
			return x < T(0) ? -x : x;
		}

		// 取符号
		template <typename T>
		inline T
		sgn(T const & x)
		{
			return x < T(0) ? T(-1) : (x > T(0) ? T(1) : T(0));
		}

		// 平方
		template <typename T>
		inline T
		sqr(T const & x)
		{
			return x * x;
		}
		// 立方
		template <typename T>
		inline T
		cube(T const & x)
		{
			return sqr(x) * x;
		}

		// 角度化弧度
		template <typename T>
		inline T
		deg2rad(T const & x)
		{
			return static_cast<T>(x * DEG2RAD);
		}
		// 弧度化角度
		template <typename T>
		inline T
		rad2deg(T const & x)
		{
			return static_cast<T>(x * RAD2DEG);
		}

		// 取小于等于x的最大整数
		template <typename T>
		inline T
		floor(T const & x)
		{
			return static_cast<T>(static_cast<int>(x > 0 ? x : (x - 1)));
		}

		// 取x的小数部分
		template <typename T>
		inline T
		frac(T const & x)
		{
			return x - static_cast<int>(x);
		}

		// 四舍五入
		template <typename T>
		inline T
		round(T const & x)
		{
			return (x > 0) ? static_cast<T>(static_cast<int>(T(0.5) + x)) :
					-static_cast<T>(static_cast<int>(T(0.5) - x));
		}
		// 取整
		template <typename T>
		inline T
		trunc(T const & x)
		{
			return static_cast<T>(static_cast<int>(x));
		}

		// 取三个中小的
		template <typename T>
		inline T const &
		min3(T const & a, T const & b, T const & c)
		{
			return std::min(std::min(a, b), c);
		}
		// 取三个中大的
		template <typename T>
		inline T const &
		max3(T const & a, T const & b, T const & c)
		{
			return std::max(std::max(a, b), c);
		}

		// 余数
		template <typename T>
		inline T
		mod(T const & x, T const & y)
		{
			return x % y;
		}
		// 浮点版本
		template<>
		inline float
		mod<float>(float const & x, float const & y)
		{
			return std::fmod(x, y);
		}
		template <>
		inline double
		mod<double>(double const & x, double const & y)
		{
			return std::fmod(x, y);
		}

		// 限制 val 在 low 和 high 之间
		template <typename T>
		inline T const &
		clamp(T const & val, T const & low, T const & high)
		{
			return std::max(low, std::min(high, val));
		}

		// 环绕处理
		template <typename T>
		inline T
		wrap(T const & val, T const & low, T const & high)
		{
			T ret(val);
			T rang(high - low);

			while (ret >= high)
			{
				ret -= rang;
			}
			while (ret < low)
			{
				ret += rang;
			}

			return ret;
		}

		// 镜像处理
		template <typename T>
		inline T
		mirror(T const & val, T const & low, T const & high)
		{
			T ret(val);
			T rang(high - low);

			while ((ret > high) || (ret < low))
			{
				if (ret > high)
				{
					ret = 2 * high - val;
				}
				else
				{
					if (ret < low)
					{
						ret = 2 * low - val;
					}
				}
			}

			return ret;
		}

		// 奇数则返回true
		template <typename T>
		inline bool
		is_odd(T const & x)
		{
			return mod(x, 2) != 0;
		}
		// 偶数则返回true
		template <typename T>
		inline bool
		is_even(T const & x)
		{
			return !is_odd(x);
		}

		// 判断 val 是否在 low 和 high 之间
		template <typename T>
		inline bool
		in_bound(T const & val, T const & low, T const & high)
		{
			return ((val >= low) && (val <= high));
		}

		// 判断两个数是否相等
		template <typename T>
		inline bool
		equal(T const & lhs, T const & rhs)
		{
			return (lhs == rhs);
		}
		// 浮点版本
		template <>
		inline bool
		equal<float>(float const & lhs, float const & rhs)
		{
			return (abs<float>(lhs - rhs)
				<= std::numeric_limits<float>::epsilon());
		}
		template <>
		inline bool
		equal<double>(double const & lhs, double const & rhs)
		{
			return (abs<double>(lhs - rhs)
				<= std::numeric_limits<double>::epsilon());
		}


		// 基本数学运算
		///////////////////////////////////////////////////////////////////////////////
		KLAYGE_CORE_API float abs(float x);
		KLAYGE_CORE_API float sqrt(float x);
		KLAYGE_CORE_API float recip_sqrt(float number);

		KLAYGE_CORE_API float pow(float x, float y);
		KLAYGE_CORE_API float exp(float x);

		KLAYGE_CORE_API float log(float x);
		KLAYGE_CORE_API float log10(float x);

		KLAYGE_CORE_API float sin(float x);
		KLAYGE_CORE_API float cos(float x);
		KLAYGE_CORE_API void sincos(float x, float& s, float& c);
		KLAYGE_CORE_API float tan(float x);

		KLAYGE_CORE_API float asin(float x);
		KLAYGE_CORE_API float acos(float x);
		KLAYGE_CORE_API float atan(float x);

		KLAYGE_CORE_API float sinh(float x);
		KLAYGE_CORE_API float cosh(float x);
		KLAYGE_CORE_API float tanh(float x);


		// 几种类型的Dot
		template <typename T>
		typename T::value_type dot(T const & lhs, T const & rhs);

		// Length的平方
		template <typename T>
		typename T::value_type length_sq(T const & rhs);

		// 几种类型的Length
		template <typename T>
		typename T::value_type length(T const & rhs);

		// 几种类型的Lerp
		template <typename T>
		T lerp(T const & lhs, T const & rhs, float s);

		template <typename T>
		T maximize(T const & lhs, T const & rhs);

		template <typename T>
		T minimize(T const & lhs, T const & rhs);

		template <typename T>
		Vector_T<typename T::value_type, 4> transform(T const & v, Matrix4_T<typename T::value_type> const & mat);

		template <typename T>
		T transform_coord(T const & v, Matrix4_T<typename T::value_type> const & mat);

		template <typename T>
		T transform_normal(T const & v, Matrix4_T<typename T::value_type> const & mat);

		template <typename T>
		T bary_centric(T const & v1, T const & v2, T const & v3,
			typename T::value_type const & f, typename T::value_type const & g);

		template <typename T>
		T normalize(T const & rhs);

		template <typename T>
		Plane_T<T> normalize(Plane_T<T> const & rhs);

		template <typename T>
		T reflect(T const & incident, T const & normal);

		template <typename T>
		T refract(T const & incident, T const & normal, typename T::value_type const & refraction_index);

		template <typename T>
		T fresnel_term(T const & cos_theta, T const & refraction_index);


		// 2D 向量
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		T cross(Vector_T<T, 2> const & lhs, Vector_T<T, 2> const & rhs);


		// 3D 向量
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		T angle(Vector_T<T, 3> const & lhs, Vector_T<T, 3> const & rhs);

		template <typename T>
		Vector_T<T, 3> cross(Vector_T<T, 3> const & lhs, Vector_T<T, 3> const & rhs);

		template <typename T>
		Vector_T<T, 3> transform_quat(Vector_T<T, 3> const & v, Quaternion_T<T> const & quat);

		template <typename T>
		Vector_T<T, 3> project(Vector_T<T, 3> const & vec,
			Matrix4_T<T> const & world, Matrix4_T<T> const & view, Matrix4_T<T> const & proj,
			int const viewport[4], T const & nearPlane, T const & farPlane);

		template <typename T>
		Vector_T<T, 3> unproject(Vector_T<T, 3> const & winVec, T const & clipW,
			Matrix4_T<T> const & world, Matrix4_T<T> const & view, Matrix4_T<T> const & proj,
			int const viewport[4], T const & nearPlane, T const & farPlane);


		// 4D 向量
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Vector_T<T, 4> cross(Vector_T<T, 4> const & v1, Vector_T<T, 4> const & v2, Vector_T<T, 4> const & v3);


		// 4D 矩阵
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Matrix4_T<T> mul(Matrix4_T<T> const & lhs, Matrix4_T<T> const & rhs);

		template <typename T>
		T determinant(Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> inverse(Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> look_at_lh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt);

		template <typename T>
		Matrix4_T<T> look_at_lh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt,
			Vector_T<T, 3> const & vUp);

		template <typename T>
		Matrix4_T<T> look_at_rh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt);

		template <typename T>
		Matrix4_T<T> look_at_rh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt,
			Vector_T<T, 3> const & vUp);

		template <typename T>
		Matrix4_T<T> ortho_lh(T const & w, T const & h, T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> ortho_off_center_lh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane);

		template <typename T>
		Matrix4_T<T> perspective_lh(T const & width, T const & height, T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_fov_lh(T const & fov, T const & aspect, T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_off_center_lh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane);

		template <typename T>
		Matrix4_T<T> reflect(Plane_T<T> const & p);

		template <typename T>
		Matrix4_T<T> rotation_x(T const & x);
		template <typename T>
		Matrix4_T<T> rotation_y(T const & y);
		template <typename T>
		Matrix4_T<T> rotation_z(T const & z);
		template <typename T>
		Matrix4_T<T> rotation(T const & angle, T const & x, T const & y, T const & z);
		template <typename T>
		Matrix4_T<T> rotation_matrix_yaw_pitch_roll(T const & yaw, T const & pitch, T const & roll);

		template <typename T>
		Matrix4_T<T> scaling(T const & sx, T const & sy, T const & sz);
		template <typename T>
		Matrix4_T<T> scaling(Vector_T<T, 3> const & s);

		template <typename T>
		Matrix4_T<T> shadow(Vector_T<T, 4> const & l, Plane_T<T> const & p);

		template <typename T>
		Matrix4_T<T> to_matrix(Quaternion_T<T> const & quat);

		template <typename T>
		Matrix4_T<T> translation(T const & x, T const & y, T const & z);
		template <typename T>
		Matrix4_T<T> translation(Vector_T<T, 3> const & pos);

		template <typename T>
		Matrix4_T<T> transpose(Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> lh_to_rh(Matrix4_T<T> const & rhs);

		template <typename T>
		void decompose(Vector_T<T, 3>& scale, Quaternion_T<T>& rot, Vector_T<T, 3>& trans, Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> transformation(Vector_T<T, 3> const * scaling_center, Quaternion_T<T> const * scaling_rotation, Vector_T<T, 3> const * scale,
			Vector_T<T, 3> const * rotation_center, Quaternion_T<T> const * rotation, Vector_T<T, 3> const * trans);


		template <typename T>
		Matrix4_T<T> ortho_rh(T const & width, T const & height, T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> ortho_off_center_rh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_rh(T const & width, T const & height,
			T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_fov_rh(T const & fov, T const & aspect,
			T const & nearPlane, T const & farPlane);
		template <typename T>
		Matrix4_T<T> perspective_off_center_rh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane);

		template <typename T>
		Matrix4_T<T> rh_to_lh(Matrix4_T<T> const & rhs);

		template <typename T>
		Matrix4_T<T> rotation_matrix_yaw_pitch_roll(Vector_T<T, 3> const & ang);


		// 四元数
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Quaternion_T<T> conjugate(Quaternion_T<T> const & rhs);

		template <typename T>
		Quaternion_T<T> axis_to_axis(Vector_T<T, 3> const & from, Vector_T<T, 3> const & to);
		template <typename T>
		Quaternion_T<T> unit_axis_to_unit_axis(Vector_T<T, 3> const & from, Vector_T<T, 3> const & to);

		template <typename T>
		Quaternion_T<T> bary_centric(Quaternion_T<T> const & q1, Quaternion_T<T> const & q2,
			Quaternion_T<T> const & q3, typename Quaternion_T<T>::value_type const & f, typename Quaternion_T<T>::value_type const & g);

		template <typename T>
		Quaternion_T<T> exp(Quaternion_T<T> const & rhs);
		template <typename T>
		Quaternion_T<T> ln(Quaternion_T<T> const & rhs);

		template <typename T>
		Quaternion_T<T> inverse(Quaternion_T<T> const & rhs);

		template <typename T>
		Quaternion_T<T> mul(Quaternion_T<T> const & lhs, Quaternion_T<T> const & rhs);

		template <typename T>
		Quaternion_T<T> rotation_quat_yaw_pitch_roll(T const & yaw, T const & pitch, T const & roll);

		template <typename T>
		void to_yaw_pitch_roll(T& yaw, T& pitch, T& roll, Quaternion_T<T> const & quat);

		template <typename T>
		void to_axis_angle(Vector_T<T, 3>& vec, T& ang, Quaternion_T<T> const & quat);

		template <typename T>
		Quaternion_T<T> to_quaternion(Matrix4_T<T> const & mat);

		template <typename T>
		Quaternion_T<T> rotation_axis(Vector_T<T, 3> const & v, T const & angle);

		template <typename T>
		Quaternion_T<T> slerp(Quaternion_T<T> const & lhs, Quaternion_T<T> const & rhs, T const & slerp);

		template <typename T>
		Quaternion_T<T> rotation_quat_yaw_pitch_roll(Vector_T<T, 3> const & ang);


		// 平面
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		T dot(Plane_T<T> const & lhs, Vector_T<T, 4> const & rhs);
		template <typename T>
		T dot_coord(Plane_T<T> const & lhs, Vector_T<T, 3> const & rhs);
		template <typename T>
		T dot_normal(Plane_T<T> const & lhs, Vector_T<T, 3> const & rhs);

		template <typename T>
		Plane_T<T> from_point_normal(Vector_T<T, 3> const & point, Vector_T<T, 3> const & normal);
		template <typename T>
		Plane_T<T> from_points(Vector_T<T, 3> const & v0, Vector_T<T, 3> const & v1, Vector_T<T, 3> const & v2);
		template <typename T>
		Plane_T<T> mul(Plane_T<T> const & p, Matrix4_T<T> const & mat);

		// 求直线和平面的交点，直线orig + t * dir
		template <typename T>
		T intersect_ray(Plane_T<T> const & p, Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir);
		
		// From Game Programming Gems 5, Section 2.6.
		template <typename T>
		void oblique_clipping(Matrix4_T<T>& proj, Plane_T<T> const & clip_plane);


		// 颜色
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Color_T<T> negative(Color_T<T> const & rhs);
		template <typename T>
		Color_T<T> modulate(Color_T<T> const & lhs, Color_T<T> const & rhs);


		// 范围
		///////////////////////////////////////////////////////////////////////////////
		template <typename Iterator>
		AABBox_T<typename std::iterator_traits<Iterator>::value_type::value_type> compute_aabbox(Iterator first, Iterator last);
		template <typename Iterator>
		OBBox_T<typename std::iterator_traits<Iterator>::value_type::value_type> compute_obbox(Iterator first, Iterator last);
		template <typename Iterator>
		Sphere_T<typename std::iterator_traits<Iterator>::value_type::value_type> compute_sphere(Iterator first, Iterator last);

		template <typename T>
		AABBox_T<T> transform_aabb(AABBox_T<T> const & aabb, Matrix4_T<T> const & mat);
		template <typename T>
		AABBox_T<T> transform_aabb(AABBox_T<T> const & aabb, T scale, Quaternion_T<T> const & rot, Vector_T<T, 3> const & trans);
		template <typename T>
		OBBox_T<T> transform_obb(OBBox_T<T> const & obb, Matrix4_T<T> const & mat);
		template <typename T>
		OBBox_T<T> transform_obb(OBBox_T<T> const & obb, T scale, Quaternion_T<T> const & rot, Vector_T<T, 3> const & trans);
		template <typename T>
		Sphere_T<T> transform_sphere(Sphere_T<T> const & sphere, Matrix4_T<T> const & mat);
		template <typename T>
		Sphere_T<T> transform_sphere(Sphere_T<T> const & sphere, T scale, Quaternion_T<T> const & rot, Vector_T<T, 3> const & trans);
		template <typename T>
		Frustum_T<T> transform_frustum(Frustum_T<T> const & frustum, Matrix4_T<T> const & mat);
		template <typename T>
		Frustum_T<T> transform_frustum(Frustum_T<T> const & frustum, T scale, Quaternion_T<T> const & rot, Vector_T<T, 3> const & trans);

		template <typename T>
		bool intersect_point_aabb(Vector_T<T, 3> const & v, AABBox_T<T> const & aabb);
		template <typename T>
		bool intersect_point_obb(Vector_T<T, 3> const & v, OBBox_T<T> const & obb);
		template <typename T>
		bool intersect_point_sphere(Vector_T<T, 3> const & v, Sphere_T<T> const & sphere);
		template <typename T>
		bool intersect_point_frustum(Vector_T<T, 3> const & v, Frustum_T<T> const & frustum);

		template <typename T>
		bool intersect_ray_aabb(Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir, AABBox_T<T> const & aabb);
		template <typename T>
		bool intersect_ray_obb(Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir, OBBox_T<T> const & obb);
		template <typename T>
		bool intersect_ray_sphere(Vector_T<T, 3> const & orig, Vector_T<T, 3> const & dir, Sphere_T<T> const & sphere);

		template <typename T>
		bool intersect_aabb_aabb(AABBox_T<T> const & lhs, AABBox_T<T> const & aabb);
		template <typename T>
		bool intersect_aabb_obb(AABBox_T<T> const & lhs, OBBox_T<T> const & obb);
		template <typename T>
		bool intersect_aabb_sphere(AABBox_T<T> const & lhs, Sphere_T<T> const & sphere);
		template <typename T>
		bool intersect_obb_obb(OBBox_T<T> const & lhs, OBBox_T<T> const & obb);
		template <typename T>
		bool intersect_obb_sphere(OBBox_T<T> const & lhs, Sphere_T<T> const & sphere);
		template <typename T>
		bool intersect_sphere_sphere(Sphere_T<T> const & lhs, Sphere_T<T> const & sphere);

		template <typename T>
		BoundOverlap intersect_aabb_frustum(AABBox_T<T> const & lhs, Frustum_T<T> const & frustum);
		template <typename T>
		BoundOverlap intersect_obb_frustum(OBBox_T<T> const & lhs, Frustum_T<T> const & frustum);
		template <typename T>
		BoundOverlap intersect_sphere_frustum(Sphere_T<T> const & lhs, Frustum_T<T> const & frustum);
		template <typename T>
		BoundOverlap intersect_frustum_frustum(Frustum_T<T> const & lhs, Frustum_T<T> const & frustum);


		// 网格
		///////////////////////////////////////////////////////////////////////////////

		// 计算TBN基
		template <typename TangentIterator, typename BinormIterator,
			typename IndexIterator, typename PositionIterator, typename TexCoordIterator, typename NormalIterator>
		inline void
		compute_tangent(TangentIterator targentsBegin, BinormIterator binormsBegin,
								IndexIterator indicesBegin, IndexIterator indicesEnd,
								PositionIterator xyzsBegin, PositionIterator xyzsEnd,
								TexCoordIterator texsBegin, NormalIterator normalsBegin)
		{
			typedef typename std::iterator_traits<PositionIterator>::value_type position_type;
			typedef typename std::iterator_traits<TexCoordIterator>::value_type texcoord_type;
			typedef typename std::iterator_traits<TangentIterator>::value_type tangent_type;
			typedef typename std::iterator_traits<BinormIterator>::value_type binormal_type;
			typedef typename std::iterator_traits<NormalIterator>::value_type normal_type;
			typedef typename position_type::value_type value_type;

			int const num = static_cast<int>(std::distance(xyzsBegin, xyzsEnd));

			for (int i = 0; i < num; ++ i)
			{
				*(targentsBegin + i) = tangent_type::Zero();
				*(binormsBegin + i) = binormal_type::Zero();
			}

			for (IndexIterator iter = indicesBegin; iter != indicesEnd; iter += 3)
			{
				uint16_t const v0Index = *(iter + 0);
				uint16_t const v1Index = *(iter + 1);
				uint16_t const v2Index = *(iter + 2);

				position_type const & v0XYZ(*(xyzsBegin + v0Index));
				position_type const & v1XYZ(*(xyzsBegin + v1Index));
				position_type const & v2XYZ(*(xyzsBegin + v2Index));

				Vector_T<value_type, 3> v1v0 = v1XYZ - v0XYZ;
				Vector_T<value_type, 3> v2v0 = v2XYZ - v0XYZ;

				texcoord_type const & v0Tex(*(texsBegin + v0Index));
				texcoord_type const & v1Tex(*(texsBegin + v1Index));
				texcoord_type const & v2Tex(*(texsBegin + v2Index));

				value_type s1 = v1Tex.x() - v0Tex.x();
				value_type t1 = v1Tex.y() - v0Tex.y();

				value_type s2 = v2Tex.x() - v0Tex.x();
				value_type t2 = v2Tex.y() - v0Tex.y();

				value_type denominator = s1 * t2 - s2 * t1;
				Vector_T<value_type, 3> tangent, binormal;
				if (MathLib::abs(denominator) < std::numeric_limits<value_type>::epsilon())
				{
					tangent = Vector_T<value_type, 3>(1, 0, 0);
					binormal = Vector_T<value_type, 3>(0, 1, 0);
				}
				else
				{
					tangent = (t2 * v1v0 - t1 * v2v0) / denominator;
					binormal = (s1 * v2v0 - s2 * v1v0) / denominator;
				}

				tangent_type t = Vector_T<value_type, 4>(tangent.x(), tangent.y(), tangent.z(), value_type(1));

				*(targentsBegin + v0Index) += t;
				*(binormsBegin + v0Index) += binormal;

				*(targentsBegin + v1Index) += t;
				*(binormsBegin + v1Index) += binormal;

				*(targentsBegin + v2Index) += t;
				*(binormsBegin + v2Index) += binormal;
			}

			for (int i = 0; i < num; ++ i)
			{
				tangent_type t(*(targentsBegin + i));
				Vector_T<value_type, 3> tangent(t.x(), t.y(), t.z());
				binormal_type binormal(*(binormsBegin + i));
				normal_type normal(*(normalsBegin + i));

				// Gram-Schmidt orthogonalize
				tangent = normalize(tangent - normal * dot(tangent, normal));
				// Calculate handedness
				value_type k = 1;
				if (dot(cross(normal, tangent), binormal) < 0)
				{
					k = -1;
				}

				*(targentsBegin + i) = Vector_T<value_type, 4>(tangent.x(), tangent.y(), tangent.z(), k);
				*(binormsBegin + i) = cross(normal, tangent);
			}
		}

		template <typename NormalIterator, typename IndexIterator, typename PositionIterator>
		inline void
		compute_normal(NormalIterator normalBegin,
								IndexIterator indicesBegin, IndexIterator indicesEnd,
								PositionIterator xyzsBegin, PositionIterator xyzsEnd)
		{
			typedef typename std::iterator_traits<PositionIterator>::value_type position_type;
			typedef typename std::iterator_traits<NormalIterator>::value_type normal_type;
			typedef typename position_type::value_type value_type;

			NormalIterator normalEnd = normalBegin;
			std::advance(normalEnd, std::distance(xyzsBegin, xyzsEnd));
			std::fill(normalBegin, normalEnd, normal_type::Zero());

			for (IndexIterator iter = indicesBegin; iter != indicesEnd; iter += 3)
			{
				uint16_t const v0Index = *(iter + 0);
				uint16_t const v1Index = *(iter + 1);
				uint16_t const v2Index = *(iter + 2);

				position_type const & v0(*(xyzsBegin + v0Index));
				position_type const & v1(*(xyzsBegin + v1Index));
				position_type const & v2(*(xyzsBegin + v2Index));

				Vector_T<value_type, 3> v03(v0.x(), v0.y(), v0.z());
				Vector_T<value_type, 3> v13(v1.x(), v1.y(), v1.z());
				Vector_T<value_type, 3> v23(v2.x(), v2.y(), v2.z());

				Vector_T<value_type, 3> vec(cross(v13 - v03, v23 - v03));

				*(normalBegin + v0Index) += vec;
				*(normalBegin + v1Index) += vec;
				*(normalBegin + v2Index) += vec;
			}

			for (NormalIterator iter = normalBegin; iter != normalEnd; ++ iter)
			{
				*iter = normalize(*iter);
			}
		}

		template <typename T>
		void intersect(Vector_T<T, 3> const & v0, Vector_T<T, 3> const & v1, Vector_T<T, 3> const & v2,
						Vector_T<T, 3> const & ray_orig, Vector_T<T, 3> const & ray_dir,
						T& t, T& u, T& v);

		template <typename T>
		bool bary_centric_in_triangle(T const & u, T const & v);


		// Color space
		///////////////////////////////////////////////////////////////////////////////
		KLAYGE_CORE_API float linear_to_srgb(float linear);
		KLAYGE_CORE_API float srgb_to_linear(float srgb);

		
		// Dual quaternion
		///////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Quaternion_T<T> quat_trans_to_udq(Quaternion_T<T> const & q, Vector_T<T, 3> const & t);

		template <typename T>
		Vector_T<T, 3> udq_to_trans(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		Vector_T<T, 3> dq_to_trans(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		Matrix4_T<T> udq_to_matrix(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		std::pair<Quaternion_T<T>, Quaternion_T<T> > conjugate(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		std::pair<Quaternion_T<T>, Quaternion_T<T> > inverse(Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		Quaternion_T<T> mul_real(Quaternion_T<T> const & lhs_real, Quaternion_T<T> const & rhs_real);

		template <typename T>
		Quaternion_T<T> mul_dual(Quaternion_T<T> const & lhs_real, Quaternion_T<T> const & lhs_dual,
			Quaternion_T<T> const & rhs_real, Quaternion_T<T> const & rhs_dual);

		template <typename T>
		void udq_to_screw(T& angle, T& pitch, Vector_T<T, 3>& dir, Vector_T<T, 3>& moment,
			Quaternion_T<T> const & real, Quaternion_T<T> const & dual);

		template <typename T>
		std::pair<Quaternion_T<T>, Quaternion_T<T> > udq_from_screw(T const & angle, T const & pitch,
			Vector_T<T, 3> const & dir, Vector_T<T, 3> const & moment);

		template <typename T>
		std::pair<Quaternion_T<T>, Quaternion_T<T> > sclerp(Quaternion_T<T> const & lhs_real, Quaternion_T<T> const & lhs_dual,
			Quaternion_T<T> const & rhs_real, Quaternion_T<T> const & rhs_dual, float const & slerp);
	}
}

#include <KlayGE/Vector.hpp>
#include <KlayGE/Rect.hpp>
#include <KlayGE/Size.hpp>
#include <KlayGE/Matrix.hpp>
#include <KlayGE/Quaternion.hpp>
#include <KlayGE/Plane.hpp>
#include <KlayGE/Color.hpp>
#include <KlayGE/Bound.hpp>
#include <KlayGE/Sphere.hpp>
#include <KlayGE/AABBox.hpp>
#include <KlayGE/Frustum.hpp>
#include <KlayGE/OBBox.hpp>

#endif		// _MATH_HPP
