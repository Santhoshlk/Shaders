#ifdef GL_ES
precision lowp float;
#endif

uniform float u_time;

const vec3 color = vec3(1.0,0.0,0.0);
vec3 redcolor()
{
    return vec3(1.0,0.0,0.0);
}

void main() {
    // here wrote on own the frag color
    //aldready predefine variable in global
	// gl_FragColor = vec4(redcolor(),1.0);
    //u can also use like this
    gl_FragColor = vec4(vec3(0.0,0.0,1.0),1.0);
}