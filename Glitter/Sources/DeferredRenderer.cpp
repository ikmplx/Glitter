// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "DeferredRenderer.h"

#include "Framebuffer.h"
#include "Res/Shader.h"

namespace MyGL
{
	DeferredRenderer::DeferredRenderer(int width, int height)
		: _width(width)
		, _height(height)
	{
		_gAlbedoSpecular = std::make_shared<Attachment>(width, height, Attachment::Type::RGBA, false);
		_gNormal = std::make_shared<Attachment>(width, height, Attachment::Type::RGB, true);
		_gPosition = std::make_shared<Attachment>(width, height, Attachment::Type::RGB, true);
		_gDepth = std::make_shared<Attachment>(width, height, Attachment::Type::Depth, false);

		_framebufferPass1 = std::make_shared<Framebuffer>();

		_framebufferPass1->AttachColor(_gAlbedoSpecular);
		_framebufferPass1->AttachColor(_gNormal);
		_framebufferPass1->AttachColor(_gPosition);
		_framebufferPass1->AttachDepth(_gDepth);
	}

	DeferredRenderer::~DeferredRenderer()
	{
	}

	void DeferredRenderer::BeginPass1()
	{
		_framebufferPass1->BeginRender();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void DeferredRenderer::EndPass1()
	{
		_framebufferPass1->EndRender();
	}

	void DeferredRenderer::BindColorAttachments(ShaderPtr shader)
	{
		BindColorAttachment(shader, "gAlbedoSpecular", _gAlbedoSpecular, 0);
		BindColorAttachment(shader, "gNormal", _gNormal, 1);
		BindColorAttachment(shader, "gPosition", _gPosition, 2);

		glActiveTexture(GL_TEXTURE0);
	}

	const FramebufferPtr & DeferredRenderer::GetFramebufferPass1()
	{
		return _framebufferPass1;
	}

	void DeferredRenderer::BindColorAttachment(ShaderPtr shader, const char* name, AttachmentPtr attachment, int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, attachment->GetTexture());

		auto uniformLocation = glGetUniformLocation(shader->ID, name);
		if (uniformLocation >= 0) {
			glUniform1i(uniformLocation, index);
		}
	}
}
