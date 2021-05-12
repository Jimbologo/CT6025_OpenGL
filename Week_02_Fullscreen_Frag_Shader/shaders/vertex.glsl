#version 330

//input parameter - thing that come into the shader from the vertex buffer
in vec4 Position;
in vec4 Colour;
//outputs from the vertex stage of the pipeline
out vec4 vColour;

void main()
{
	vColour = Colour; //pass colour info along to next shader pipeline stage
	gl_Position = Position;
}