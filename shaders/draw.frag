uniform float w;
uniform float h;
uniform float arr[10000];

void main(){
    int x = int(gl_TexCoord[0].x*w);
    int y = int(gl_TexCoord[0].y*h);
    if (arr[y*int(w)+x]==0.0){
        gl_FragColor = vec4(1.0);
    } else {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

}
