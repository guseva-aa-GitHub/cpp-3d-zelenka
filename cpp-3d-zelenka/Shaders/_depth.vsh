#version 330 core

in vec3 vertex;

// параметры преобразований
struct Transform {
	mat4 model;
	mat4 viewProjection;
};

uniform Transform transform;

void main(void) {
    // переводим координаты вершины в однородные
    vec4 vert   = transform.model * vec4(vertex, 1.0);
    gl_Position = transform.viewProjection * vert;
}
