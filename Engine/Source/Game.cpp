#include "Game.h"
#include <fstream>
#include "Core\Assert.h"

namespace LL3D {

Game::Game() {
  window_.reset(new Window(json11::Json{}));
  window_->OnMouseDown(std::bind(&Game::OnMouseDown, this, std::tr1::placeholders::_1));
  window_->OnMouseUp(std::bind(&Game::OnMouseUp, this, std::tr1::placeholders::_1));
  window_->OnMouseMove(std::bind(&Game::OnMouseMove, this, std::tr1::placeholders::_1));
  window_->OnMouseScroll(std::bind(&Game::OnMouseScroll, this, std::tr1::placeholders::_1));
  window_->OnResize(std::bind(&Game::OnResize, this));

  std::ifstream ifs("Config/GraphicsDevice.json");
  std::string content((std::istreambuf_iterator<char>(ifs)),
    (std::istreambuf_iterator<char>()));

  std::string err;
  auto config = json11::Json::parse(content, err);
  ASSERT(err.size() == 0 && "Config error (possibly no config file at all).");
  graphics_device_.reset(new Graphics::GraphicsDevice(config,
    window_->GetClientRect().GetSize(), window_->GetHandle()));
}

void Game::Run() {
  MSG msg = { 0 };

  while (msg.message != WM_QUIT) {
    // If there are Window messages then process them.
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    Sleep(20);  // TODO: remove hardcode.
    Update();
  }
}

}

