#include "application.h"

#include "sample1/sample1.h"

GLuint& core::Application::GetVertexArrayObject() { return m_vertex_array_obj; }

const GLuint& core::Application::GetVertexArrayObject() const {
  return m_vertex_array_obj;
}

void core::Application::Initialize() {}

void core::Application::Update(float delta_time) {}

void core::Application::Render(float aspect_ratio) {}

void core::Application::Shutdown() {
  glDeleteVertexArrays(1, &m_vertex_array_obj);
}

std::unique_ptr<core::Application> core::Application::CreateSample1() {
  return std::make_unique<Sample1>();
}

std::filesystem::path core::Application::GetAssetsPath() { return ASSETS_DIR; }

std::filesystem::path core::Application::GetShadersPath() {
  return SHADERS_DIR;
}

core::Application::Application() : m_vertex_array_obj(0) {
  glGenVertexArrays(1, &m_vertex_array_obj);
  glBindVertexArray(m_vertex_array_obj);
}
