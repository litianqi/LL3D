#include "RecursiveSceneIterator.h"
#include "Scene.h"
#include "Core\Assert.h"

namespace LL3D {

RecursiveSceneIterator::RecursiveSceneIterator(Scene & scene) noexcept
  : scene_(&scene),
  iter_(scene.objects_.begin())
{
}

RecursiveSceneIterator& RecursiveSceneIterator::operator++()
{
  if (IsEnd())
    throw std::out_of_range("Incremented(++) an end RecursiveSceneIterator.");

  // If has child
  if (!iter_->children_.empty()) {
    // References to first child
    parents_.push(iter_);
    iter_ = iter_->children_.begin();
  }
  // Else if has GameObject as parent
  else if (!parents_.empty()) {
    ASSERT(scene_);
    // If has brother, references to brother
    // Else, references to parent
    if (++iter_ == parents_.top()->children_.end()){
      iter_ = parents_.top();
      parents_.pop();
    }
  }
  // Else (has Scene as parent)
  else {
    // If has brother, references to brother.
    // Else (no brother), references to end.
    ++iter_;
  }
  
  return *this;
}

RecursiveSceneIterator& RecursiveSceneIterator::operator++(int)
{
  this->operator++();
  return *this;
}

bool RecursiveSceneIterator::operator==(const RecursiveSceneIterator& rhs) 
const noexcept
{
  if (IsEnd() || rhs.IsEnd()) {
    return IsEnd() == rhs.IsEnd();
  }
  return &(*iter_) == &(*rhs.iter_);
}
bool RecursiveSceneIterator::operator!=(const RecursiveSceneIterator& rhs) 
const noexcept
{
  return !operator==(rhs);
}

GameObject& RecursiveSceneIterator::operator*()
{
  if (IsEnd())
    throw std::out_of_range("Dereferenced(*) an end RecursiveSceneIterator.");
  return *iter_;
}

GameObject* RecursiveSceneIterator::operator->()
{
  if (IsEnd())
    throw std::out_of_range("Dereferenced(->) an end RecursiveSceneIterator.");
  return &(*iter_);
}

bool RecursiveSceneIterator::IsEnd() const noexcept
{
  auto is_end = true;
    if (!parents_.empty()) {
      ASSERT(scene_);
      if (iter_ != parents_.top()->children_.end())
        is_end = false;
    }
    else if (scene_) {
      if (iter_ != scene_->objects_.end())
        is_end = false;
    }

  return is_end;
}

RecursiveSceneIterator begin(RecursiveSceneIterator & iter) noexcept
{
  return iter;
}

RecursiveSceneIterator end(const RecursiveSceneIterator &) noexcept
{
  return RecursiveSceneIterator();
}

}  // namespace LL3D
