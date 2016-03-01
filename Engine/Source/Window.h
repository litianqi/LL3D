#pragma once

#include <string>
#include <Windows.h>
#include "Core\Uncopyable.h"
#include "Core/Types.h"

namespace LL3D {

struct MouseButtonEvent;
struct MouseScrollEvent;

// Win-32 window wrapper.
class Window : private Uncopyable {
public:
  enum ShowState {
    Minimized,
    Normal,
    Maximized,
    FullScreen
  };

  Window();
  virtual ~Window();

  HWND GetHandle() const;

  void SetVisible(bool visible);
  void SetClientRect(IntRectangle rect);

  //bool IsVisible() const;
  IntRectangle GetClientRect() const;
  IntRectangle GetWindowRect() const;

protected:
  // Handle mouse inputs:
  virtual void OnMouseDown(const MouseButtonEvent& event) = 0;
  virtual void OnMouseUp(const MouseButtonEvent& event) = 0;
  virtual void OnMouseMove(const MouseButtonEvent& event) = 0;
  virtual void OnMouseScroll(const MouseScrollEvent& event) = 0;
  
  virtual void OnResize() = 0;
  
private:
  static LRESULT CALLBACK MsgProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

  HWND handle_;
  bool active_;
  ShowState show_state_ = ShowState::Normal;
  bool resizing_;
};

}