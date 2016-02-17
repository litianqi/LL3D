#pragma once

#include <string>
#include <boost\noncopyable.hpp>
#include <Windows.h>
#include "Type.h"

namespace LL3D {

struct MouseButtonEvent;
struct MouseScrollEvent;

// Win-32 window wrapper.
class Window: private boost::noncopyable {
public:
  enum ShowState {
    Minimized,
    Normal,
    Maximized,
    FullScreen
  };

  Window();
  ~Window();

  HWND GetHandle() const;

  //void SetVisible(bool enable);
  void SetClientRect(IntRectangle rect);

  //bool IsVisible() const;
  IntRectangle GetClientRect() const;
  IntRectangle GetWindowRect() const;

protected:
  // Handle mouse inputs:
  virtual void OnMouseDown(const MouseButtonEvent& event) {}
  virtual void OnMouseUp(const MouseButtonEvent& event) {}
  virtual void OnMouseMove(const MouseButtonEvent& event) {}
  virtual void OnMouseScroll(const MouseScrollEvent& event) {}
  
  virtual void OnResize() {}
  
private:
  static LRESULT CALLBACK MsgProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

  HWND handle_;
  bool active_;
  ShowState show_state_;
  bool resizing_;
};

}