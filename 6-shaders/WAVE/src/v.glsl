//#version 110

uniform float time;
uniform float A;
uniform float omega;

void main()
{
    // ogni volta che chiamo init? viene eseguito questo per ogni vertex
    // alziamo la y dei punti
    // vec4 v = vec4(gl_Vertex);
    // v.y = A * sin(omega*time + 5.0*gl_Vertex.x) * sin(omega*time _ 5.0*glVertex.z )
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	gl_FrontColor = gl_Color;
}