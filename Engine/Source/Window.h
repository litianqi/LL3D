#pragma once

#include <string>
#include <functional>
#include <Windows.h>
#include "json11.hpp"
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

  Window(json11::Json config);  // TODO: support config
  virtual ~Window();

  HWND GetHandle() const;

  void SetVisible(bool visible);
  void SetClientRect(IntRectangle rect);

  void OnResize(std::function<void(void)> callback);

  //bool IsVisible() const;
  IntRectangle GetClientRect() const;
  IntRectangle GetWindowRect() const;

private:
  static LRESULT CALLBACK MsgProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

  std::function<void(void)> resize_callback_;

  HWND handle_;
  bool active_;
  ShowState show_state_ = ShowState::Normal;
  bool resizing_;
};

}