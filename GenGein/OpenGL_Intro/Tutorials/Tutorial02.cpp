
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Engine\Objects\ObjectLoader.h"
#include "Cameras\FlyCamera.h"

#include "Tutorial02.h"

using glm::vec3;
using glm::mat4;

// Default constructor
Tutorial02::Tutorial02() :
	m_VAO(0),
	m_VBO(0),
	m_IBO(0),
	m_indexCount(0)
{}

// Construct with specified window attributes
Tutorial02::Tutorial02( const int a_width, const int a_height, const char* a_title ) 
	: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial02::~Tutorial02()
{}

// Initialise things
void Tutorial02::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();

	//Initialise camera
	InitialiseFlyCamera(20.f, 40.0f, 1.0f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	// Generate model in pre-process
	GenerateModel("sphere_hires", "sphere");
}

// Destroy things 
void Tutorial02::ShutDown()
{}

// Update loop
void Tutorial02::Update( const double a_dt )
{
	// Apply dt to camera for movement
	m_pBaseCamera->Update(a_dt);
}

// Render elements to GL
void Tutorial02::Render()
{
	GLApplication::Render();

	//Drawing Objects
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

// Generate model and populate buffers with model data
void Tutorial02::GenerateModel( const char* folderPath, const char* filePath )
{
	// Create a new object with model data from filepath
	ObjectLoader* m_object = new ObjectLoader( folderPath, filePath);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray( m_VAO );
	
	// Create and bind buffers to an vertex array object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_object->GetMesh()->vertices.size() * sizeof(ObjVertex), &m_object->GetMesh()->vertices[0], GL_STATIC_DRAW);
	
	// (0) - (vec3)Positon | (1) - (vec3)Colour | (2) - (vec3)Normal
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), (void*)(sizeof(vec3)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), (void*)(sizeof(vec3)*2));
	
	// Create and bind buffers to an index buffer object
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	m_indexCount = m_object->GetMesh()->indices.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(GLuint),
		&m_object->GetMesh()->indices[0], GL_STATIC_DRAW);
	
	// Unbind Vert Array Objs
	glBindVertexArray(0);
	// Delete m_obj pointer (memsave)
	delete m_object;
}

// Generate grid and populate buffers with grid data
void Tutorial02::GenerateGrid( const uint rows, const uint cols )
{
	// Create a center-point based on input
	float center = ((rows+cols)/2.0f);

	// Populate verts with row + cols input
	
	ObjVertex* aoVertices = new ObjVertex[rows * cols];
	
	for ( GLuint r = 0 ; r < rows ; ++r )
	{
		for ( GLuint c = 0 ; c < cols ; ++c )
		{
			aoVertices[ r * cols + c ].position = glm::vec4(-center + (float)c, 0, -center + (float)r, 1);
			aoVertices[ r * cols + c ].colour = glm::vec4(1,0,0,1);
			aoVertices[ r * cols + c ].normal = glm::vec4(0,1,0,1);
		}
	}
	
	// defining index count based off quad count (2 triangles per quad)
	GLuint* auiIndices = new GLuint[ ((rows - 1) * (cols - 1) * 6) ];
	m_indexCount = 0;

	int iLength =  (rows - 1) * (cols - 1) * 6;
	
	for ( GLuint r = 0 ; r < (rows - 1) ; ++r )
	{
		for ( GLuint c = 0 ; c < (cols - 1) ; ++c ) 
		{
			// triangle 1
			auiIndices[ m_indexCount++ ] = r * cols + c;
			auiIndices[ m_indexCount++ ] = (r + 1) * cols + c;
			auiIndices[ m_indexCount++ ] = (r + 1) * cols + (c + 1);
			// triangle 2
			auiIndices[ m_indexCount++ ] = r * cols + c;
			auiIndices[ m_indexCount++ ] = (r + 1) * cols + (c + 1);
			auiIndices[ m_indexCount++ ] = r * cols + (c + 1);
		}
	} 

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray( m_VAO );
	
	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(glm::vec4)*3, aoVertices, GL_STATIC_DRAW);
	
	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4)*3, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4)*3, (void*) sizeof(glm::vec4));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4)*3, (void*)(sizeof(glm::vec4) * 2));
	
	// Gen and bind data to Index buffer object
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(GLuint), auiIndices, GL_STATIC_DRAW);
	// Unbind VAO (memsave)
	glBindVertexArray(0);
	// Remove pointers (memsave)
	delete[] aoVertices;
	delete[] auiIndices;
}