#include <print>
// clang-format off
#include "glad/glad.h"
#include <GLFW/glfw3.h>
// clang-format on

#include "application.h"

int main() {
  if (!glfwInit()) {
    std::println(stderr, "Failed to initialize GLFW");
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window =
      glfwCreateWindow(800, 600, "Animation", nullptr, nullptr);
  if (!window) {
    std::println(stderr, "Failed to create GLFW window");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (!gladLoadGL()) {
    std::println(stderr, "Failed to load OpenGL");
    return EXIT_FAILURE;
  }

  auto application = core::Application::CreateSample1();
  auto vao = application->GetVertexArrayObject();
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  application->Initialize();

  double last_time = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    double current_time = glfwGetTime();
    float delta_time = static_cast<float>(current_time - last_time);
    last_time = current_time;

    application->Update(delta_time);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    application->Render(aspect);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  application->Shutdown();
  glDeleteVertexArrays(1, &vao);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}