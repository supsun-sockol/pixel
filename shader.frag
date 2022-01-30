uniform float b_w;
uniform float b_c;
uniform float w;
uniform float h;

void main() {
    if (mod(gl_TexCoord[0].x*w/b_w, b_c+1.0) < 1.0){
        gl_FragColor = vec4(vec3(0.5), 1.0);
    }
    else if (mod(gl_TexCoord[0].y*h/b_w, b_c+1.0) <= 1.0){
        gl_FragColor = vec4(vec3(0.5), 1.0);
    } else {

        gl_FragColor = vec4(1.0);
    }
}
