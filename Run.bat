g++ PROBABLY.cpp -DGLEW_STATIC -I\GameEngine\ -I\GameEngine\GLEW\include\ -I\GameEngine\GLFW\include\ -I\GameEngine\FREETYPE\include\ -I\GameEngine\PORTAUDIO\include\ -L\GameEngine\FREETYPE\lib -l:libfreetype.a -L\GameEngine\GLEW\lib -l:libglew32.a -L\GameEngine\GLFW\lib -l:libglfw3.a -l:libopengl32.a -l:libuser32.a -l:libgdi32.a -l:libshell32.a -L\GameEngine\PORTAUDIO\lib -l:libportaudiocpp.a