#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include "SOIL/src/SOIL.h"
#include "common.h"

class TextureHelper
{
public:
	/*
	/* 成功加载2D纹理则返回纹理对象Id 否则返回0
	*/
	static  GLint load2DTexture(const char* filename, GLint internalFormat = GL_RGB,
		GLenum picFormat = GL_RGB, int loadChannels = SOIL_LOAD_RGB)
	{
		// Step1 创建并绑定纹理对象
		GLuint textureId = 0;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		// Step2 设定wrap参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Step3 设定filter参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR); // 为MipMap设定filter方法
		// Step4 加载纹理
		GLubyte* imageData = NULL;
		int picWidth, picHeight;
		imageData = SOIL_load_image(filename, &picWidth, &picHeight, 0, loadChannels);
		if (imageData == NULL)
		{
			std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
			return 0;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, picWidth, picHeight,
			0, picFormat, GL_UNSIGNED_BYTE, imageData);

		//为当前绑定的纹理自动生成所有需要的多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
		// Step5 释放纹理图片资源
		SOIL_free_image_data(imageData);
		glBindTexture(GL_TEXTURE_2D, 0);
		return textureId;
	}

	/*
	* 创建 framebuffer-attachable texture （纹理附加图像）
	*/
	static GLuint makeAttachmentTexture(GLint level = 0, GLint internalFormat = GL_DEPTH24_STENCIL8,
		GLsizei width = 800, GLsizei height = 600, GLenum picFormat = GL_DEPTH_STENCIL,
		GLenum picDataType = GL_UNSIGNED_INT_24_8)
	{
		GLuint textId;
		glGenTextures(1, &textId);
		glBindTexture(GL_TEXTURE_2D, textId);
		glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, picFormat, picDataType, NULL); // 预分配空间
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return textId;
	}
};

#endif
