#version 450

precision mediump float;       
                              
varying vec4 v_Color;          
                               
 
// The entry point for our fragment shader.
void main()
{
    gl_FragColor = v_Color;    
}