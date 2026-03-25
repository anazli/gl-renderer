#include "gfx/window.h"

#include <stdexcept>

#include "gfx/application.h"

core::AppWindow::AppWindow(int width, int height, const std::string& title)
    : m_window(nullptr), m_width(width), m_height(height), m_title(title) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window =
      glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
  if (!m_window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  if (!gladLoadGL()) {
    throw std::runtime_error("Failed to load OpenGL");
  }
}

core::AppWindow::~AppWindow() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void core::AppWindow::Render(core::Application* application) const {
  double last_time = glfwGetTime();
  while (!glfwWindowShouldClose(m_window)) {
    double current_time = glfwGetTime();
    float delta_time = static_cast<float>(current_time - last_time);
    last_time = current_time;

    application->Update(delta_time);

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    application->Render(aspect);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}