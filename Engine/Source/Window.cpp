#include "Window.h"
#include "Windows.h"
#include <windowsx.h>
#include "Core/Types.h"
#include "Core/Assert.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"

namespace LL3D
{

  Window::Window(json11::Json config)
  {
    // Init wnd class
    /*wnd_class_.cbSize = sizeof(WNDCLASSEX);
    wnd_class_.lpfnWndProc = MsgProc;
    wnd_class_.hInstance = GetModuleHandle(NULL);
    wnd_class_.hIcon = nullptr;
    wnd_class_.hCursor = LoadCursor(0, IDC_ARROW);
    wnd_class_.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wnd_class_.lpszClassName = L"LL3DWndClass";*/
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = msgProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = L"LL3DWndClass";
    // Register wnd class
    WA(RegisterClass(&wc));

    //>/ Compute window rectangle dimensions based on requested client area dimensions.
    //RECT R = { 0, 0, mClientWidth, mClientHeight };
    //AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
    //int width = R.right - R.left;
    //int height = R.bottom - R.top;

    // Create window
    handle_ = CreateWindowEx(
      0,
      L"LL3DWndClass",
      L"LL3D",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,  // default x  
      CW_USEDEFAULT,  // default y   
      CW_USEDEFAULT,  // default width                
      CW_USEDEFAULT,  // default height
      0,  // No parent window.
      NULL,  // No menu.
      GetModuleHandle(NULL),
      this);
    /* handle_ = CreateWindow(L"LL3DWndClass", L"aa",
       WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 400, 0, 0, GetModuleHandle(NULL), 0);*/
    WA(handle_);

    // map this to handle
    SetWindowLongPtr(handle_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  }

  Window::~Window()
  {
    DestroyWindow(handle_);
  }

  HWND Window::handle() const
  {
    return handle_;
  }

  void Window::setVisible(bool visible)
  {
    if (visible)
    {
      ShowWindow(handle_, SW_SHOW);
    }
    else
    {
      ShowWindow(handle_, SW_HIDE);
    }
    UpdateWindow(handle_);
  }

  void Window::setClientRect(IntRectangle rect)
  {
    RECT r{ rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetBottom() };
    WA(AdjustWindowRect(&r,
                        WS_OVERLAPPEDWINDOW, false));
  }

  void Window::onResize(std::function<void(void)> callback)
  {
    resizeCallback_ = callback;
  }

  IntRectangle Window::clientRect() const
  {
    RECT r;
    ::GetClientRect(handle(), &r);
    return IntRectangle(r.left, r.top, r.right, r.bottom);
  }

  IntRectangle Window::windowRect() const
  {
    RECT r;
    ::GetWindowRect(handle(), &r);
    return IntRectangle(r.left, r.top, r.right, r.bottom);
  }

  LRESULT Window::msgProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
  {
    Window* self = reinterpret_cast<Window*>(
      GetWindowLongPtr(handle, GWLP_USERDATA));

    switch (msg)
    {
      case WM_CREATE:
      {
        Input::Mouse::initialize(handle);
        return 0;
      }
      case WM_DESTROY:
      {
        // Clear the pointer to stop calling the self once WM_DESTROY is
        // received.
        SetLastError(ERROR_SUCCESS);
        LONG_PTR result = SetWindowLongPtr(handle, GWLP_USERDATA, NULL);
        WA(result);
        PostQuitMessage(0);
        return 0;
      }
      // WM_ACTIVATE is sent when the window is activated or deactivated.  
      // We pause the game when the window is deactivated and unpause it 
      // when it becomes active.  
      case WM_ACTIVATE:
      {
        if (LOWORD(wparam) == WA_INACTIVE)
        {
          self->active_ = false;
        }
        else
        {
          self->active_ = true;
        }
        return 0;
      }
      // WM_SIZE is sent when the user resizes the window.  
      case WM_SIZE:
      {
        if (wparam == SIZE_MINIMIZED)
        {
          self->active_ = false;
          self->showState_ = Minimized;
        }
        else if (wparam == SIZE_MAXIMIZED)
        {
          self->showState_ = Maximized;
          if (self->resizeCallback_)
            self->resizeCallback_();
        }
        else if (wparam == SIZE_RESTORED)
        {

          // Restoring from minimized state
          if (self->showState_ == Minimized)
          {
            self->active_ = true;
            self->showState_ = Normal;
            if (self->resizeCallback_)
              self->resizeCallback_();
          }
          // Restoring from maximized state
          else if (self->showState_ == Maximized)
          {
            self->showState_ = Normal;
            if (self->resizeCallback_)
              self->resizeCallback_();
          }
          else if (self->resizing_)
          {
            // If user is dragging the resize bars, we do not resize 
            // the buffers here because as the user continuously 
            // drags the resize bars, a stream of WM_SIZE messages are
            // sent to the window, and it would be pointless (and slow)
            // to resize for each WM_SIZE message received from dragging
            // the resize bars.  So instead, we reset after the user is 
            // done resizing the window and releases the resize bars, which 
            // sends a WM_EXITSIZEMOVE message.
            if (self->resizeCallback_)
              self->resizeCallback_();
          }
          else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
          {
            if (self->resizeCallback_)
              self->resizeCallback_();
          }
        }
        return 0;
      }
      // WM_EXITSIZEMOVE is sent when the user grabs the resize bars
      case WM_ENTERSIZEMOVE:
      {
        self->resizing_ = true;
      }
      // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
      // Here we reset everything based on the new window dimensions.
      case WM_EXITSIZEMOVE:
      {
        self->resizing_ = false;
        if (self->resizeCallback_)
          self->resizeCallback_();
        return 0;
      }
      // The WM_MENUCHAR message is sent when a menu is active and the user presses 
      // a key that does not correspond to any mnemonic or accelerator key. 
      case WM_MENUCHAR:
      {
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);
      }
      // Catch this message so to prevent the window from becoming too small.
      case WM_GETMINMAXINFO:
      {
        ((MINMAXINFO*)lparam)->ptMinTrackSize.x = 516;
        ((MINMAXINFO*)lparam)->ptMinTrackSize.y = 360;
        return 0;
      }
      default:
      {
        Input::Mouse::processMessage(msg, wparam, lparam);
        Input::Keyboard::processMessage(msg, wparam, lparam);

        return DefWindowProc(handle, msg, wparam, lparam);
      }
    }
  }

}  // namespace LL3D