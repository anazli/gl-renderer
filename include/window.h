#pragma once

#include <string>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace core {

class Application;

class AppWindow {
 public:
  AppWindow(int width, int height, const std::string& title);

  AppWindow(const AppWindow&) = delete;
  AppWindow& operator=(const AppWindow&) = delete;

  ~AppWindow();

  void Render(Application* application) const;

 private:
  GLFWwindow* m_window;
  std::string m_title;
  int m_width;
  int m_height;
};
}  // namespace core