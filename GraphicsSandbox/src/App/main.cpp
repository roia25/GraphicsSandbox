#include <Graphics/GraphicsEngine.h>

int main(void)
{  
  GraphicsEngine *gEngine = new GraphicsEngine;

  float oldtime = 0.0f;
  float time = (float)glfwGetTime();

  while (!glfwWindowShouldClose(gGraphics->window))
  {
    glfwPollEvents();
    gGraphics->update(time - oldtime);
    glfwSwapBuffers(gGraphics->window);
    oldtime = time;
  }

  return 0;
}