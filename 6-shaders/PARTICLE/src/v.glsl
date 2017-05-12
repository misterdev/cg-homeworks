uniform float time;
attribute  float vx, vy, vz; //add velocity in z-direction
const float a = -0.000001;

void main()
{
    
    vec4 t = gl_Vertex;
    t.y = gl_Vertex.y + vy*time + 0.5*a*time*time;
    t.x = gl_Vertex.x + vx*time;
	// utilize t.z and gl_PointSize
	//FATTO t.z = gl_Vertex.x + vz*time

	//FATTO gl_PointSize = max(t.y * 50.0, 1.0)

    gl_Position = gl_ModelViewProjectionMatrix*t;
    gl_FrontColor =  gl_Color;
}