#include <iostream>
// 包含纹理加载辅助类
#include "vr3dtexture.h"

#include "vr3devent.h"
#include "glfwcallback.h"
#include "vr3dcamera_arcball.h"
#include "vr3dcompositor.h"

#ifdef HAVE_OPENHMD
#include "vr3dcamera_hmd.h"
#endif

static void _init_scene(vr3dscene* scene)
{
	vr3dmesh* mesh = new vr3dmesh();
	vr3dnode* node;
	vr3dshader* shader = new vr3dshader("shader/mvp_uv.vert", "shader/texture_uv.frag");

	// 平面
	//mesh->vr_3d_mesh_new_plane();
	// 立方体
	//mesh->vr_3d_mesh_new_cube();
	// 球体
	mesh->vr_3d_mesh_new_sphere(0.8, 100, 100);
	
	node = vr3dnode::vr_3d_node_new_from_mesh_shader(mesh, shader);
	scene->vr_3d_scene_append_node(node);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	//绑定纹理对象
	glActiveTexture(GL_TEXTURE0);

	shader->vr_3d_shader_bind();
	shader->vr_3d_shader_update_uniform_1i("texture", 0);// 设置纹理单元为0号
}
vr3dcompositor::vr3dcompositor()
{
	this->screen_w = 960;
	this->screen_h = 540;

	// 初始化 opengl，创建渲染窗口
	vr_3d_compositor_init_gl_context();
#ifdef HAVE_OPENHMD
	vr3dcamera* cam = new vr3dcamera_hmd();
#else
	vr3dcamera* cam = new vr3dcamera_arcball();
#endif
	this->scene = new vr3dscene(cam, &_init_scene);
#ifdef HAVE_OPENHMD
	this->scene->vr_3d_scene_init_hmd();
#endif
}

vr3dcompositor::~vr3dcompositor()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

bool vr3dcompositor::vr_3d_compositor_init_scene()
{
#ifdef HAVE_OPENHMD
	vr3dhmd* hmd = ((vr3dcamera_hmd*)this->scene->camera)->hmd;
	if (!hmd->device)
		return false;

	/*screen_w = hmd->vr_3d_hmd_get_eye_width();
	screen_h = hmd->vr_3d_hmd_get_eye_height();*/
	screen_w = 960;
	screen_h = 540;
#endif // !HAVE_OPENHMD

	this->scene->vr_3d_scene_init_gl();	// invoke "_init_scene" in this function

	return true;
}

bool vr3dcompositor::vr_3d_compositor_update_texture(/*AVFrame* pictureYUV*/)
{
	textureId1 = TextureHelper::load2DTexture("resources/textures/cat.png");
	glBindTexture(GL_TEXTURE_2D, textureId1);

	return true;
}

int vr3dcompositor::vr_3d_compositor_init_gl_context()
{
	// Init GLFW
	if (!glfwInit())	// 初始化glfw库
	{
		std::cout << "Error::GLFW could not initialize GLFW!" << std::endl;
		return -1;
	}
	// 开启OpenGL 3.3 core profile
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	glfwWindowHint(GLFW_SAMPLES, 4);				// 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//We don't want the old OpenGL
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(screen_w, screen_h, "vr 3d player", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 创建的窗口的context指定为当前context
	glfwMakeContextCurrent(window);

	//初始化事件
	vr3devent::vr_3d_event_create(window);

	// 让glew获取所有拓展函数
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
			<< " error string:" << glewGetErrorString(status) << std::endl;
		glfwTerminate();
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	// 设置视口参数
	glViewport(0, 0, width, height);

	return 0;
}

GLFWwindow* vr3dcompositor::vr_3d_compositor_get_window()
{
	return window;
}

bool vr3dcompositor::vr_3d_compositor_draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureId1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->scene->vr_3d_scene_draw();

	glfwSwapBuffers(window); // 交换缓存
	return true;
}
