uniform float time;
attribute  float vx, vy, vz;
const float a = -0.000001;

void main() {
    
    vec4 t = gl_Vertex;
    t.y = gl_Vertex.y + vy*time + 0.5*a*time*time;
    t.x = gl_Vertex.x + vx*2.5*time;
    t.z = gl_Vertex.z + vz*2.5*time;
	// gl_PointSize = gl_Vertex.y * 10.0;
	//TODO varia dimensione in base alla y

    gl_Position = gl_ModelViewProjectionMatrix*t;
    gl_FrontColor =  gl_Color;
}