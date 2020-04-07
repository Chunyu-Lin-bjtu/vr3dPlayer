#include "vr3dmesh.h"

vr3dmesh::vr3dmesh()
{
}

vr3dmesh::~vr3dmesh()
{
}

void vr3dmesh::vr_3d_mesh_new_plane(float aspect)
{
	vr_3d_mesh_init_buffers();
	vr_3d_mesh_upload_plane(aspect);
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

void vr3dmesh::vr_3d_mesh_upload_plane(float aspect)
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
