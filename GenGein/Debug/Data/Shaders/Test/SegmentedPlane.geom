#version 430

layout(points) in;
layout(triangles, max_vertices = 400) out;

// Input data from Vert shader
in vec4 vPosition[][];

// Out data to Frag shader
out vec4 vColour;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
	int r,c;
	int maxR = 10, maxC = 10;
	
	for(r = 0; r < maxR; r+=1)
	{
		for(c = 0; c < maxC; c+=1)
		{
			int nxtRwIncr = 1, nxtClIncr = 1;
	
			if (r == maxR - 1) nextRowInc = -1;
			else 
				nextRowInc = 1;
				
			if (c == maxC - 1) nxtClIncr = -1;
			else 
				nxtClIncr = 1;
			
			//current | across | across + above
			int a = c*maxC + r;
			int b = a + (maxC * nxtClIncr);
			int c = a + nxtClIncr;
			int d = c + nxtRwIncr;
			
			gl_Position = Projection * View * vPosition[a];
			EmitVertex();
			gl_Position = Projection * View * vPosition[b];
			EmitVertex();
			gl_Position = Projection * View * vPosition[c];
			EmitVertex();
			gl_Position = Projection * View * vPosition[d];
			EmitVertex();
		}
	}
	vColour = vec4(0,0,0,1);
}