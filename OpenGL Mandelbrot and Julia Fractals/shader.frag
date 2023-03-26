#version 330 core
in vec4 gl_FragCoord;

out vec4 frag_color;

uniform vec4 bounds;
uniform vec2 point;
uniform vec4 color[16];
uniform vec2 resolution;

#define MAX_ITER 500

void main() {
	vec2 pos = vec2(
		((gl_FragCoord.x / resolution.x) * (bounds.y - bounds.x)) + bounds.x,
		((gl_FragCoord.y / resolution.y) * (bounds.w - bounds.z)) + bounds.z
	);

	vec2 toCenter = point - pos;
	float dist = length(toCenter);
	
	if (dist < 0.03) {
		frag_color = vec4 (1.0f, 0.0f, 1.0f, 1.0f);
		return;
	}

	int iterations = 0;

	vec2 z = pos;
	vec2 c = point;

	while (iterations < MAX_ITER) {
	
		z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
		
		if (dot(z, z) > 4.0) {
			break;
		}

		++iterations;
	}

	if (iterations == MAX_ITER || iterations == 0) {
		frag_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		return;
	}

	frag_color = color[iterations % 16];	
}