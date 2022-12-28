#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main() {
    gl_Position = position;
};

#shader fragment
#version 330

out vec4 outputColor;

void main() {
    float lerpValue = gl_FragCoord.y / 500.0f;
    
    outputColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f),
        vec4(0.2f, 0.2f, 0.2f, 1.0f), lerpValue);
};