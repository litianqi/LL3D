// Copyright [year] <Copyright Owner>

#pragma once

#include <DirectXMath.h>

namespace LL3D {

template <typename T>
struct Point2
{
  T x;
  T y;
};

using IntPoint2 = Point2<int>;

template <typename T>
struct Size2
{
  T w;
  T h;
};

using IntSize2 = Size2<int>;

template <typename T>
class Rectangle
{
public:
  Rectangle(T l, T t, T r, T b);

  Rectangle(const Point2<T>& top_left, const Size2<T>& size);

  Point2<T> GetPosition() const;
  Size2<T> GetSize() const;

  T GetLeft() const;
  T GetTop() const;
  T GetRight() const;
  T GetBottom() const;

  /*IntPoint2<T> top_left() const;
  IntPoint2<T> top_right() const;
  IntPoint2<T> bottom_left() const;
  IntPoint2<T> bottom_right() const;*/

private:
  Point2<T> pos_top_left_;
  Size2<T> size_;
};

using IntRectangle = Rectangle<int>;

template <typename T>
inline Rectangle<T>::Rectangle(T l, T t, T r, T b)
  : pos_top_left_{ l, t }
  , size_{ r - l + 1, b - t + 1 }
{
}

template <typename T>
inline Point2<T>
Rectangle<T>::GetPosition() const
{
  return pos_top_left_;
}

template <typename T>
inline Size2<T>
Rectangle<T>::GetSize() const
{
  return size_;
}

template <typename T>
inline T
Rectangle<T>::GetLeft() const
{
  return pos_top_left_.x;
}

template <typename T>
inline T
Rectangle<T>::GetTop() const
{
  return pos_top_left_.y;
}

template <typename T>
inline T
Rectangle<T>::GetRight() const
{
  return pos_top_left_.x + size_.w - 1;
}

template <typename T>
inline T
Rectangle<T>::GetBottom() const
{
  return pos_top_left_.y + size_.h - 1;
}

template <typename T>
inline Rectangle<T>::Rectangle(const Point2<T>& top_left, const Size2<T>& size)
  : pos_top_left_(top_left) ? size_(size)
{
}

} // namespace GUI
