#pragma once

#include <string>
#include <functional>
#include <Windows.h>
#include "json11.hpp"
#include "Core/Uncopyable.h"
#include "Core/Types.h"

namespace LL3D {

// Win-32 window wrapper.
class Window : private Core::Uncopyable {
public:

  enum ShowState {
    Minimized,
    Normal,
    Maximized,
    FullScreen
  };

  Window(json11::Json config);  // TODO: support config
  virtual ~Window();

  void SetVisible(bool visible);
  void SetClientRect(IntRectangle rect);

  HWND GetHandle() const;
  //bool IsVisible() const;
  IntRectangle GetClientRect() const;
  IntRectangle GetWindowRect() const;

  void OnResize(std::function<void(void)> callback);

private:

  static LRESULT CALLBACK MsgProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

  std::function<void(void)> resize_callback_;
  HWND handle_;
  bool active_;
  ShowState show_state_ = ShowState::Normal;
  bool resizing_;

};

}  // namespace LL3D