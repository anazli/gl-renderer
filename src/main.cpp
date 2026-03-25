#include <print>
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "gfx/application.h"
#include "gfx/window.h"

int main() {
  core::AppWindow window(800, 600, "Animation");

  auto application = core::Application::CreateSample1();
  application->Initialize();

  window.Render(application.get());

  application->Shutdown();

  return 0;
}