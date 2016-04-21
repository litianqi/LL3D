#pragma once

#include <iterator>
#include <stack>
#include <list>
#include <memory>

namespace LL3D {
class Scene;
class GameObject;
}

namespace LL3D {
class RecursiveSceneIterator
  : public std::iterator<std::forward_iterator_tag, GameObject>
{
public:
  // Constructs an end iterator.
  RecursiveSceneIterator() = default;
  // Constructs an begin iterator.
  RecursiveSceneIterator(Scene& scene) noexcept;
  // Increments iterator. Increments end iterator have no effects.
  RecursiveSceneIterator& operator++() noexcept;
  RecursiveSceneIterator& operator++(int) noexcept;
  // Compares iterator.
  bool operator==(const RecursiveSceneIterator& rhs) const noexcept;
  bool operator!=(const RecursiveSceneIterator& rhs) const noexcept;

  // Returns value this iterator points to.
  // Exception: Throw a std::out_of_range exception if dereferenced end.
  GameObject& operator*();
  GameObject* operator->();

private:
  // Returns if this is an end iterator.
  bool isEnd() const noexcept;

  Scene* scene_ = nullptr;
  // Iterator points to value.
  std::list<std::unique_ptr<GameObject>>::iterator iter_;
  // Parents. If parents is empty, then parent is Scene.
  std::stack<std::list<std::unique_ptr<GameObject>>::iterator> parents_;
};

// Range-based Loop

// Returns iter unchanged.
RecursiveSceneIterator begin(RecursiveSceneIterator& iter) noexcept;
// Returns a default-constructed RecursiveSceneIterator, which serves as the
// end iterator. The argument is ignored.
RecursiveSceneIterator end(const RecursiveSceneIterator&) noexcept;

} // namespace LL3D
