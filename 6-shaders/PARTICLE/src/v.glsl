uniform float time;
attribute  float vx, vy, vz;
const float a = -0.000001;

void main() {

    vec4 t = gl_Vertex;
    t.y = gl_Vertex.y + (vy * time) + (0.5 * a * time * time);
    t.x = gl_Vertex.x + (vx * 2.5 * time);
    t.z = gl_Vertex.z + (vz * 9.0 * time);

	gl_PointSize = 2.0 * (5.0 + t.y);

    gl_Position = gl_ModelViewProjectionMatrix * t;
    gl_FrontColor =  gl_Color * t;
}