// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Framebuffer.h"
#include "Utils.h"

namespace MyGL
{
	// Attachment
	Attachment::Attachment(int width, int height, Type type, bool highPrecision)
	{
		GLint internalFormat;
		GLenum dataFormat;
		GLenum dataType;
		GLint filter;

		if (type == Type::Depth) {
			internalFormat = dataFormat = GL_DEPTH_COMPONENT;
			dataType = GL_FLOAT;
			filter = GL_NEAREST;
		}
		else if (type == Type::RGB) {
			filter = GL_NEAREST;
			dataFormat = GL_RGB;

			if (highPrecision) {
				internalFormat = GL_RGB16F;
				dataType = GL_FLOAT;
			}
			else {
				internalFormat = GL_RGB;
				dataType = GL_UNSIGNED_BYTE;
			}
		}
		else if (type == Type::RGBA) {
			filter = GL_NEAREST;
			dataFormat = GL_RGBA;

			if (highPrecision) {
				internalFormat = GL_RGBA16F;
				dataType = GL_FLOAT;
			}
			else {
				internalFormat = GL_RGBA;
				dataType = GL_UNSIGNED_BYTE;
			}
		}

		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, dataType, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Attachment::~Attachment()
	{
		glDeleteTextures(1, &_texture);
	}

	void Attachment::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, _texture);
	}

	GLuint Attachment::GetTexture() const
	{
		return _texture;
	}



	// Framebuffer
	Framebuffer::Framebuffer()
	{
		glGenFramebuffers(1, &_framebuffer);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &_framebuffer);
	}

	void Framebuffer::BeginRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

		if (_colorAttachments.size() > 1) {
			glDrawBuffers(_attachmentsArg.size(), _attachmentsArg.data());
		}
	}

	void Framebuffer::EndRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::AttachColor(AttachmentPtr colorAttachment)
	{
		_colorAttachments.push_back(colorAttachment);

		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _colorAttachments.size() - 1, GL_TEXTURE_2D, colorAttachment->GetTexture(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		_attachmentsArg.push_back(GL_COLOR_ATTACHMENT0 + _colorAttachments.size() - 1);
	}

	void Framebuffer::AttachDepth(AttachmentPtr depthAttachment)
	{
		_depthAttachments = depthAttachment;

		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment->GetTexture(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::BlitDepthToMainFramebuffer(int width, int height)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint Framebuffer::GetId() const
	{
		return _framebuffer;
	}

}
