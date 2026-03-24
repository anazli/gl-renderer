#include <print>
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "application.h"
#include "window.h"

int main() {
  auto application = core::Application::CreateSample1();
  auto vao = application->GetVertexArrayObject();
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  application->Initialize();

  core::AppWindow window(800, 600, "Animation");
  window.Render(application.get());

  application->Shutdown();
  glDeleteVertexArrays(1, &vao);

  return 0;
}