#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

void runGameLoop(GLFWwindow* window);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
