#include "Game.h"
#include <fstream>
#include <plog\Log.h>
#include "Window.h"
#include "Scene.h"
#include "Component.h"
#include "Core\Assert.h"
#include "Graphics\Device.h"
#include "Graphics\Base.h"
#include "Input\Mouse.h"
#include "Input\Keyboard.h"

namespace LL3D {

Game::Game() {
  // Initialize the logger.
  plog::init(plog::debug, "Engine.log");

  window_.reset(new Window(json11::Json{}));
  window_->onResize(std::bind(&Game::onResize, this));

  std::ifstream ifs("_Config/GraphicsDevice.json");
  std::string content((std::istreambuf_iterator<char>(ifs)),
    (std::istreambuf_iterator<char>()));

  std::string err;
  auto config = json11::Json::parse(content, err);
  ASSERT(err.size() == 0 && "Config error (possibly no config file at all).");
  graphicsDevice_.reset(new Graphics::Device(config,
    window_->clientRect().GetSize(), window_->handle()));

  Graphics::Base::initialize(graphicsDevice_.get());

  scene_.reset(new Scene(window_.get()));
}

void Game::run() {
  MSG msg = { 0 };

  while (msg.message != WM_QUIT) {
    // If there are Window messages then process them.
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else {
      Sleep(5);
      update();
      scene_->update();
      scene_->render();
      Input::Mouse::update();
      Input::Keyboard::update();
    }
  }
}

}

