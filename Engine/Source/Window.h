#pragma once

#include <string>
#include <functional>
#include <Windows.h>
#include "json11.hpp"
#include "Core/Uncopyable.h"
#include "Core/Types.h"

namespace LL3D
{
  // Win-32 window wrapper.
  class Window : private Core::Uncopyable
  {
  public:

    enum ShowState
    {
      Minimized,
      Normal,
      Maximized,
      FullScreen
    };

    Window(json11::Json config);  // TODO: support config
    virtual ~Window();

    void setVisible(bool visible);
    void setClientRect(IntRectangle rect);

    HWND handle() const;
    //bool IsVisible() const;
    IntRectangle clientRect() const;
    IntRectangle windowRect() const;

    void onResize(std::function<void(void)> callback);

  private:

    static LRESULT CALLBACK msgProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

    std::function<void(void)> resizeCallback_;
    HWND handle_;
    bool active_;
    ShowState showState_ = ShowState::Normal;
    bool resizing_;
  };

}  // namespace LL3D