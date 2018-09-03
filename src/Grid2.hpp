
#ifndef GRID_2_HPP
#define GRID_2_HPP

#include <cassert>

struct Pos {
  long x;
  long y;
};

template <typename T>
class Grid2
{
public:
  Grid2(Pos size, T fillValue);
  Grid2(const Grid2<T>& other) = default;
  Grid2(Grid2<T>&& other);
  Grid2<T>& operator=(const Grid2<T>& other) = default;
  Grid2<T>& operator=(Grid2<T>&& other);

  T get(Pos pos) const;
  T get(long x, long y) const;
  void set(Pos pos, T newValue);
  void set(long x, long y, T newValue);

  Pos getSize() const;

  using Ptr = std::unique_ptr<Grid2<T>>;

private:
  std::vector<T> m_values;
  Pos m_size;
};

template <typename T>
Grid2<T>::Grid2(Pos size, T fillValue) :
  m_values(size.x * size.y, fillValue),
  m_size(size)
{

}

template <typename T>
Grid2<T>::Grid2(Grid2<T>&& other) :
  m_values(std::move(other.m_values)),
  m_size(other.m_size)
{
  other.m_size = {0, 0};
}

template <typename T>
Grid2<T>& Grid2<T>::operator=(Grid2<T>&& other)
{
  m_values = std::move(other.m_values);
  m_size = other.m_size;
  other.m_size = {0, 0};

  return *this;
}

template <typename T>
T Grid2<T>::get(Pos pos) const
{
  return get(pos.x, pos.y);
}

template <typename T>
T Grid2<T>::get(long x, long y) const
{
  assert(x < m_size.x && y < m_size.y);
  return m_values[x + y * m_size.y];
}

template <typename T>
void Grid2<T>::set(Pos pos, T newValue)
{
  set(pos.x, pos.y, newValue);
}

template <typename T>
void Grid2<T>::set(long x, long y, T newValue)
{
  assert(x < m_size.x && y < m_size.y);
  m_values[x + y * m_size.y] = newValue;
}

template <typename T>
Pos Grid2<T>::getSize() const
{
  return m_size;
}

#endif // !GRID_2_HPP
