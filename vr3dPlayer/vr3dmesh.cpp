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
	// ��������VAO����
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	// ����EBO����
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
		// ����VBO����
		glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);

		GLint attrib_location = shader->vr_3d_get_attribute_location(buf->name);

		if (attrib_location != -1) {
			// �趨��������ָ�� ����OpenGL����ν�����������
			glVertexAttribPointer(attrib_location, //index: ��ɫ����������ɫ���е�����λ��ֵ
				buf->vector_length, //size: ������ÿ��������Ҫ���µķ�����Ŀ
				GL_FLOAT,           //type: ���������ݵ����� ��GL_FLOAT��GL_BYTE��GL_INT ��
				GL_FALSE,           //normalized: ���ö��������ڴ洢ǰ�Ƿ���Ҫ��һ��   GL_FALSE:����Ҫ��һ��
				0, 0);              //stride:����Ԫ��֮��Ĵ�Сƫ��ֵ(byte)      pointer: ����Ӧ�ôӻ�������ĸ���ַ��ʼ��ȡ
			glEnableVertexAttribArray(attrib_location);//����������index����������Ķ�������
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
	// ָ�������������� ����λ�� ��ɫ ����
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,	// 0
		 0.5f, -0.5f, 0.0f,	// 1
		 0.5f,  0.5f, 0.0f,	// 2
		-0.5f,  0.5f, 0.0f,	// 3
	};
	//��������
	GLfloat uvs[] = {
	   0.0f, 0.0f,	// 0
	   1.0f, 0.0f,	// 1
	   1.0f, 1.0f,	// 2
	   0.0f, 1.0f	// 3
	};
	// ��������
	const GLushort indices[] = {
		0, 1, 2,  // ��һ��������
		0, 2, 3   // �ڶ���������
	};

	this->vertex_count = 4;	//����ĸ���
	this->draw_mode = GL_TRIANGLES;

	// ���������� ���������ύGPU
	vr_3d_mesh_append_attribute_buffer("position", sizeof(GLfloat), 3, vertices);
	vr_3d_mesh_append_attribute_buffer("uv", sizeof(GLfloat), 2, uvs);

	this->index_size = sizeof(indices);
	// ��EBO����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	// ����ռ䣬������������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index_size, indices, GL_STATIC_DRAW);
}

// name��glsl�н������ݱ���������
// element_size��ÿ��Ԫ�صĴ�С
// vector_length��ÿ�����ݸ���
// vertices�����ݵĵ�ַ
void vr3dmesh::vr_3d_mesh_append_attribute_buffer(const char* name, size_t element_size, int vector_length, GLfloat* vertices)
{
	struct vr3dattributebuffer* attrib_buffer = (struct vr3dattributebuffer*)malloc(sizeof(struct vr3dattributebuffer));

	attrib_buffer->name = name;
	attrib_buffer->element_size = element_size;
	attrib_buffer->vector_length = vector_length;
	// ����VBO����
	glGenBuffers(1, &attrib_buffer->vbo);
	// ��VBO����
	glBindBuffer(GL_ARRAY_BUFFER, attrib_buffer->vbo);
	// ����ռ䣬���Ͷ�������
	glBufferData(GL_ARRAY_BUFFER, this->vertex_count * attrib_buffer->vector_length * attrib_buffer->element_size, vertices, GL_STATIC_DRAW);

	this->attribute_buffers.push_back(attrib_buffer);//�� attrib_buffer ��ӵ� attribute_buffers(List)��

	printf("generated %s buffer #%d", attrib_buffer->name, attrib_buffer->vbo);
}
