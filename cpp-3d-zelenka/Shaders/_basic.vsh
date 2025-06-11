#version 330 core

in vec3 vertex, normal;
in vec2 texel;

out vec3 v_normal;
out vec2 v_texel;
out vec3 v_light_dir;
out vec3 v_view_dir;
out vec4 v_smcoord;

struct Transform {
    mat4 model;
    mat4 viewProjection;
    mat3 normal;
    vec3 viewPosition;
    mat4 light;
};

struct DirectLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform Transform transform;
uniform DirectLight direct_light;

void main() {
    // переведем координаты вершины в мировую систему координат
    vec4 vert   = transform.model *  vec4(vertex, 1.0);

    // вычисляем текстурные координаты вершины на карте глубины
    v_smcoord  = transform.light * vert;

    // передаем текстурные координаты
    v_texel = texel;

    // передаем нормаль в мировой системе координат
    v_normal   = transform.normal * normalize(normal);

    // передаем направление на источник освещения
    v_light_dir = vec3(direct_light.position);

    // передаем направление от вершины к наблюдателю в мировой системе координат
    v_view_dir  = transform.viewPosition - vec3(vert);

    // переводим координаты вершины в однородные
    gl_Position = transform.viewProjection * vert;

}
