varying vec3 N;
varying vec3 L;
varying vec3 E;

void main() {
    vec4 a = vec4(0.0,0.5,0.8,1.0);
    vec4 b = vec4(0.0,0.3,0.6,1.0);
    vec4 c = vec4(0.0,0.2,0.5,1.0);
    vec4 d = vec4(0.0,0.3,0.6,1.0);
    vec4 e = vec4(0.0,0.0,0.1,1.0);
    vec4 red = vec4(1.0, 0.0, 0.0, 1.0);

    vec4 color;
    float intensity = dot(normalize(L), normalize(N));

    if (intensity > 0.95)
        color = a;
    else if (intensity > 0.5)
        color = b;
    else if (intensity > 0.25)
        color = c;
    else if (intensity > 0.15)
        color = d;
    else
        color = e;

	// adjust the color to produce an outline/silhouette
	float angle = dot(normalize(N), normalize(E));
    if (angle < 0.20) {
        color = red;
    }

    gl_FragColor = color;
}
