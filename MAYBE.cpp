#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ft2build.h>
#include FT_FREETYPE_H

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragementSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
  std::ifstream stream(filepath);

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT =  1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    }
    else {
      ss[(int)type] << line  << '\n';
    }
  }

  return { ss[0].str(), ss[1].str() };
}

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

    // start glew
    GLenum err = glewInit();
    if (GLEW_OK != err) 
    std::cout << glewGetErrorString(err) << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float Positions[] = {
     0.0f,    0.5f, 0.0f, 1.0f,
     0.5f, -0.366f, 0.0f, 1.0f,
    -0.5f, -0.366f, 0.0f, 1.0f,
     1.0f,    0.0f, 0.0f, 1.0f,
     0.0f,    1.0f, 0.0f, 1.0f,
     0.0f,    0.0f, 1.0f, 1.0f,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions), Positions, GL_STATIC_DRAW);

    ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
    std::cout << "Vertex source code" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment source code" << std::endl; 
    std::cout << source.FragementSource << std::endl;

    unsigned int shader = CreateShader(source.VertexSource, source.FragementSource);
    glUseProgram(shader); //hey thats pretty cool

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(shader);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glUseProgram(0);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
