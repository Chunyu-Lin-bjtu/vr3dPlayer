#include <string.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "vr3dmesh.h"

vr3dmesh::vr3dmesh()
{
}

vr3dmesh::~vr3dmesh()
{
}

// 平面
void vr3dmesh::vr_3d_mesh_new_plane()
{
	vr_3d_mesh_init_buffers();
	vr_3d_mesh_upload_plane();
}

// 立方体
void vr3dmesh::vr_3d_mesh_new_cube()
{
	vr_3d_mesh_init_buffers();      //创建顶点缓存对象 索引缓存对象(顶点坐标的索引)
	vr_3d_mesh_upload_cube();
}

// 球体
void vr3dmesh::vr_3d_mesh_new_sphere(float radius, unsigned stacks, unsigned slices)
{
	vr_3d_mesh_init_buffers();       //创建顶点缓存对象 索引缓存对象(顶点坐标的索引)
	vr_3d_mesh_upload_sphere(radius, stacks, slices);//球体？将顶点坐标数据，纹理坐标数据，索引数据提交GPU
}

// 线
void vr3dmesh::vr_3d_mesh_new_line(glm::vec3* from, glm::vec3* to, glm::vec3* color)
{
	vr_3d_mesh_init_buffers();      //创建顶点缓存对象 索引缓存对象(顶点坐标的索引)
	vr_3d_mesh_upload_line(from, to, color);
}

void vr3dmesh::vr_3d_mesh_init_buffers()
{
	// 创建并绑定VAO对象
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	// 创建EBO对象
	glGenBuffers(1, &this->ebo);
}

void vr3dmesh::vr_3d_mesh_bind_shader(vr3dshader* shader)
{
	shader->vr_3d_shader_bind();
	list<vr3dattributebuffer*>::iterator it;
	for (it = this->attribute_buffers.begin(); it != this->attribute_buffers.end(); ++it)
	{
		struct vr3dattributebuffer* buf = *it;
		printf("%s: location: %d length: %d size: %zu", buf->name, buf->vbo, buf->vector_length, buf->element_size);
		// 创建VBO对象
		glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);

		GLint attrib_location = shader->vr_3d_get_attribute_location(buf->name);

		if (attrib_location != -1) {
			// 设定顶点属性指针 告诉OpenGL该如何解析顶点数据
			glVertexAttribPointer(attrib_location, //index: 着色器变量在着色器中的属性位置值
				buf->vector_length, //size: 数组中每个顶点需要更新的分量数目
				GL_FLOAT,           //type: 数组中数据的类型 有GL_FLOAT、GL_BYTE、GL_INT 等
				GL_FALSE,           //normalized: 设置顶点数据在存储前是否需要归一化   GL_FALSE:不需要归一化
				0, 0);              //stride:两个元素之间的大小偏移值(byte)      pointer: 数据应该从缓存对象哪个地址开始获取
			glEnableVertexAttribArray(attrib_location);//设置启用于index索引相关联的顶点数组
		}
		else {
			printf("could not find attribute %s in shader.", buf->name);
		}
	}
}

void vr3dmesh::vr_3d_mesh_bind()
{
	glBindVertexArray(this->vao);
}

void vr3dmesh::vr_3d_mesh_draw()
{
	glDrawElements(this->draw_mode, this->index_size, GL_UNSIGNED_SHORT, 0);
}

void vr3dmesh::vr_3d_mesh_draw_mode(GLenum draw_mode)
{
	glDrawElements(draw_mode, this->index_size, GL_UNSIGNED_SHORT, 0);
}

// 平面顶点数据
void vr3dmesh::vr_3d_mesh_upload_plane()
{
	// 指定顶点属性数据 顶点位置 颜色 纹理
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,	// 0
		 0.5f, -0.5f, 0.0f,	// 1
		 0.5f,  0.5f, 0.0f,	// 2
		-0.5f,  0.5f, 0.0f,	// 3
	};
	//纹理坐标
	GLfloat uvs[] = {
	   0.0f, 0.0f,	// 0
	   1.0f, 0.0f,	// 1
	   1.0f, 1.0f,	// 2
	   0.0f, 1.0f	// 3
	};
	// 索引数据
	const GLushort indices[] = {
		0, 1, 2,  // 第一个三角形
		0, 2, 3   // 第二个三角形
	};

	this->vertex_count = 4;	//顶点的个数
	this->draw_mode = GL_TRIANGLES;

	// 将顶点坐标 纹理坐标提交GPU
	vr_3d_mesh_append_attribute_buffer("position", sizeof(GLfloat), 3, vertices);
	vr_3d_mesh_append_attribute_buffer("uv", sizeof(GLfloat), 2, uvs);

	this->index_size = sizeof(indices);
	// 绑定EBO对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	// 分配空间，传送索引数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index_size, indices, GL_STATIC_DRAW);
}

// 立方体顶点数据
void vr3dmesh::vr_3d_mesh_upload_cube()
{
	/* *INDENT-OFF* */
	GLfloat positions[] = {
		/* front face */
		 1.0,  1.0, -1.0,
		 1.0, -1.0, -1.0,
		-1.0, -1.0, -1.0,
		-1.0,  1.0, -1.0,
		/* back face */
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		/* right face */
		 1.0,  1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		/* left face */
		-1.0,  1.0,  1.0,
		-1.0,  1.0, -1.0,
		-1.0, -1.0, -1.0,
		-1.0, -1.0,  1.0,
		/* top face */
		 1.0, -1.0,  1.0,
		-1.0, -1.0,  1.0,
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		/* bottom face */
		 1.0,  1.0,  1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
		-1.0,  1.0,  1.0
	};

	/* *INDENT-OFF* */
	GLfloat uvs[] = {
#if 0	// youtu video
		/* front face */
		0.667, 0.5,
		0.667, 0.0,
		0.333, 0.0,
		0.333, 0.5,
		/* back face */
		0.333, 0.5,
		0.333, 1.0,
		0.667, 1.0,
		0.667, 0.5,
		/* right face */
		1.0, 0.5,
		1.0, 0.0,
		0.667, 0.0,
		0.667, 0.5,
		/* left face */
		0.0, 0.5,
		0.333, 0.5,
		0.333, 0.0,
		0.0, 0.0,
		/* top face */
		0.667, 0.5,
		0.667, 1.0,
		1.0, 1.0,
		1.0, 0.5,
		/* bottom face */
		0.333, 0.5,
		0.0, 0.5,
		0.0, 1.0,
		0.333, 1.0,
#endif // 0
		/* front face */
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
		/* back face */
		1.0, 0.0,
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		/* right face */
		1.0, 0.0,
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		/* left face */
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
		/* top face */
		1.0, 0.0,
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		/* bottom face */
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0
	};

	GLushort indices[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	};
	/* *INDENT-ON* */

	this->vertex_count = 4 * 6;	//顶点的个数
	this->draw_mode = GL_TRIANGLES;

	vr_3d_mesh_append_attribute_buffer("position", sizeof(GLfloat), 3, positions);
	vr_3d_mesh_append_attribute_buffer("uv", sizeof(GLfloat), 2, uvs);

	// index
	this->index_size = sizeof(indices);// / sizeof(indices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index_size, indices, GL_STATIC_DRAW);
}

// 球体数据
void vr3dmesh::vr_3d_mesh_upload_sphere(float radius, unsigned stacks, unsigned slices)
{
	printf("gst_3d_mesh_upload_sphere.radius：%f, stacks:%d, slices:%d.\n", radius, stacks, slices);
	GLfloat* positions;
	GLfloat* uvs;
	GLushort* indices;

	this->vertex_count = (slices + 1) * stacks;	//顶点个数
	const int component_size = sizeof(GLfloat) * this->vertex_count;//保存数据的数组空间的大小

	positions = (GLfloat*)malloc(component_size * 3);//分配对应大小的内存空间(存储顶点坐标)
	uvs = (GLfloat*)malloc(component_size * 2);//分配对应大小的内存空间(存储纹理坐标)

	GLfloat* v = positions;
	GLfloat* t = uvs;

	float const J = 1. / (float)(stacks - 1);//在标准化设备坐标(NDC)中每个格子的间隙？
	float const I = 1. / (float)(slices - 1);//在标准化设备坐标(NDC)中每个格子的间隙？

	for (int i = 0; i < slices; i++) {
		float const theta = M_PI * i * I;
		for (int j = 0; j < stacks; j++) {
			float const phi = 2 * M_PI * j * J + M_PI / 2.0;

			float const x = sin(theta) * cos(phi);
			float const y = -cos(theta);
			float const z = sin(phi) * sin(theta);

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			*t++ = j * J;//texture x 轴坐标
			*t++ = i * I;//texture y 轴坐标
		}
	}

	vr_3d_mesh_append_attribute_buffer("position", sizeof(GLfloat), 3, positions);
	vr_3d_mesh_append_attribute_buffer("uv", sizeof(GLfloat), 2, uvs);

	/* index */
	this->index_size = (slices - 1) * stacks * 2;

	indices = (GLushort*)malloc(sizeof(GLushort) * this->index_size);
	GLushort* indextemp = indices;

	// -3 = minus caps slices - one to iterate over strips
	for (int i = 0; i < slices - 1; i++) {
		for (int j = 0; j < stacks; j++) {
			*indextemp++ = i * stacks + j;
			*indextemp++ = (i + 1) * stacks + j;
		}
	}

	/* linear index */
	/*
	   self->index_size = (slices - 2) * stacks;
	   for (int i = 0; i < self->index_size; i++)
	   *indextemp++ = i;
	 */

	 // upload index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * this->index_size, indices, GL_STATIC_DRAW);

	this->draw_mode = GL_TRIANGLE_STRIP;
}

void vr3dmesh::vr_3d_mesh_upload_line(glm::vec3* from, glm::vec3* to, glm::vec3* color)
{
	GLfloat vertices[] = {
		from->x, from->y, from->z, 1.0,
		to->x, to->y, to->z, 1.0,
	};
	GLfloat colors[] = {
		color->x, color->y, color->z,
		color->x, color->y, color->z,
	};

	this->vertex_count = 2;
	this->draw_mode = GL_LINES;

	vr_3d_mesh_append_attribute_buffer("position", sizeof(GLfloat), 4, vertices);
	vr_3d_mesh_append_attribute_buffer("color", sizeof(GLfloat), 3, colors);

	const GLushort indices[] = { 0, 1 };

	// index
	this->index_size = sizeof(indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index_size, indices, GL_STATIC_DRAW);
}

// name：glsl中接收数据变量的名称
// element_size：每个元素的大小
// vector_length：每行数据个数
// vertices：数据的地址
void vr3dmesh::vr_3d_mesh_append_attribute_buffer(const char* name, size_t element_size, int vector_length, GLfloat* vertices)
{
	struct vr3dattributebuffer* attrib_buffer = (struct vr3dattributebuffer*)malloc(sizeof(struct vr3dattributebuffer));

	attrib_buffer->name = name;
	attrib_buffer->element_size = element_size;
	attrib_buffer->vector_length = vector_length;
	// 创建VBO对象
	glGenBuffers(1, &attrib_buffer->vbo);
	// 绑定VBO对象
	glBindBuffer(GL_ARRAY_BUFFER, attrib_buffer->vbo);
	// 分配空间，传送顶点数据
	glBufferData(GL_ARRAY_BUFFER, this->vertex_count * attrib_buffer->vector_length * attrib_buffer->element_size, vertices, GL_STATIC_DRAW);

	this->attribute_buffers.push_back(attrib_buffer);//将 attrib_buffer 添加到 attribute_buffers(List)中

	printf("generated %s buffer #%d", attrib_buffer->name, attrib_buffer->vbo);
}
