attribute vec4 vertices2;
attribute vec4 color2;
uniform float time;

void main() {
    float s =  0.5 * (1.0 + sin(0.001 * time));
    vec4 t = mix(gl_Vertex, vertices2, s);
    vec4 c = mix(gl_Color, vertices2, s);
    gl_Position = gl_ModelViewProjectionMatrix * t;
	gl_FrontColor = c;
}