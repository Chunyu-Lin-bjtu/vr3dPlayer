// OpenGL-Graphics.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include "common.h"


// 包含mesh类
#include "vr3dmesh.h"
#include "vr3dnode.h"
#include "vr3dscene.h"
#include "vr3dcompositor.h"

// Window dimensions
const GLuint WIDTH = 960;
const GLuint HEIGHT = 540;


int main()
{
	vr3dcompositor cp;
	cp.vr_3d_compositor_init_scene();
	cp.vr_3d_compositor_update_texture();
#if 0
	// Section1 准备顶点数据
	// 指定顶点属性数据 顶点位置 颜色 纹理
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f,  // 0
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,1.0f, 0.0f,  // 1
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,1.0f, 1.0f,  // 2
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,0.0f, 1.0f   // 3
	};

	// 索引数据
	GLshort indices[] = {
		0, 1, 2,  // 第一个三角形
		0, 2, 3   // 第二个三角形
	};

	// 创建缓存对象
	GLuint VAOId, VBOId, EBOId;
	// 1.创建并绑定VAO对象
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glGenVertexArrays(1, &VAOId);
	glBindVertexArray(VAOId);

	// 2.创建并绑定VBO 对象
	glGenBuffers(1, &VBOId);
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	// 分配空间，传送顶点数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3.创建并绑定EBO对象
	glGenBuffers(1, &EBOId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOId);
	// 分配空间，传送索引数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 4.指定解析方式，并启用顶点属性
	// 顶点位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 顶点颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	// 顶点纹理坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	// 解除绑定，防止后续操作干扰到了当前VAO和VBO
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // 注意不要解除EBO绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0);			  // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	//第二部分：准备着色器程序
	vr3dshader shader("shader/04-gltexture-v3.0.vertex", "shader/04-gltexture-v3.0.frag");
#else
	
#endif // 0

	// Uncommenting this call will result in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//填充绘制
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//用线来绘制

	// Section3 准备纹理对象
	//GLuint textureId1 = TextureHelper::load2DTexture("resources/textures/cat.png");
	
	//shader.vr_3d_shader_bind();
	// 启用多个纹理单元 绑定纹理对象
	//glBindTexture(GL_TEXTURE_2D, textureId1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, textureId2);
	//shader.updateUniform1i("tex2", 1);// 设置纹理单元为1号
	
	// 开始游戏主循环
	GLFWwindow* window = cp.vr_3d_compositor_get_window();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // 处理例如鼠标 键盘等事件

		
#if 0
		// 清除颜色缓冲区 重置为指定颜色
		glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// 这里填写场景绘制代码
		glBindVertexArray(VAOId);
		shader.vr_3d_shader_bind();

		// 使用索引绘制
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window); // 交换缓存
#else
		cp.vr_3d_compositor_draw();
#endif // 0

	}
#if 0
	// 释放资源
	glDeleteVertexArrays(1, &VAOId);
	glDeleteBuffers(1, &VBOId);
#else

#endif // 0

	return 0;
}
