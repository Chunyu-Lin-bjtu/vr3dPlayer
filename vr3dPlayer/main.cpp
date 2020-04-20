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
	
	// 开始游戏主循环
	GLFWwindow* window = cp.vr_3d_compositor_get_window();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // 处理例如鼠标 键盘等事件

		cp.vr_3d_compositor_draw();

	}

	return 0;
}
