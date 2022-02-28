#version 450

precision mediump float;       
                              
in vec4 v_Color;          
                   
out vec4 frag_colour;
 
// The entry point for our fragment shader.
void main()
{
    frag_colour = v_Color;    
}