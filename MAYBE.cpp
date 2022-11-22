#include "glad\glad.h"
#include <glfw3.h>

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

static unsigned int CompileShader(const std::string& source, unsigned int type) { //makes the shader doohicky
  unsigned int id = glCreateShader(type); 
  const char* scr = source.c_str();                 
  glShaderSource(id, 1, &scr, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile shader" << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) { //slaps shader to the program
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
  unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

  glAttachShader(program, vs); //do the shader stuff
  glAttachShader(program, fs);
  glLinkProgram(program);  
  glValidateProgram(program);  

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(void) {

    FT_Library freetype;
    FT_Face face;

// start freetype
    if (FT_Init_FreeType(&freetype)) {
       std::cout << "Failed to initialize FREETYPE" << std::endl;
       return 1;
    }

    if(FT_New_Face(freetype, "/GameEngine/Office Code Pro/officecodeprod-regular.otf", 0, &face)) {
    std::cout << "Could not open font" << std::endl;
      if (FT_Err_Unknown_File_Format) {
      std::cout << "Unsupported font file" << std::endl;
      }
    return 1;
}

    FT_GlyphSlot F_glyph = face->glyph;



// start glfw
    GLFWwindow* window;

    if (!glfwInit()) {
       std::cout << "Failed to initialize GLFW" << std::endl;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL) ;
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

// start glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    float postitons[6] = { //hd rendition of your mom
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), postitons, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader = //the shaders
      "#version 330 core\n"
      "\n"
      "layout(location = 0) in vec4 position;"
      "\n"
      "void main() {\n"
      " gl_Position = postion;\n"
      "}\n";

    std::string fragmentShader = //the shaders
      R"glsl(
        #version 330 core

        layout(location = 0) in vec4 position;

        void main(){
          color = vec4(1.0, 0.0, 0.0, 1.0);
        }
      )glsl";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader); //hey thats pretty cool

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
