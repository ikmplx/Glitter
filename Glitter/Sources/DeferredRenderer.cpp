// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "DeferredRenderer.h"

#include "Framebuffer.h"
#include "Res/Shader.h"
#include "Res/Texture.h"

namespace MyGL
{
	DeferredRenderer::DeferredRenderer(int width, int height)
		: _width(width)
		, _height(height)
	{
		_gAlbedoSpecular = std::make_shared<Attachment>(width, height, Attachment::Type::RGBA, true);
		_gNormal = std::make_shared<Attachment>(width, height, Attachment::Type::RGB, true);
		_gDepth = std::make_shared<Attachment>(width, height, Attachment::Type::Depth, false);

		_framebufferPass1 = std::make_shared<Framebuffer>();

		_framebufferPass1->AttachColor(_gAlbedoSpecular);
		_framebufferPass1->AttachColor(_gNormal);
		_framebufferPass1->AttachDepth(_gDepth);

		glGenVertexArrays(1, &_emptyVao);
	}

	DeferredRenderer::~DeferredRenderer()
	{
		glDeleteVertexArrays(1, &_emptyVao);
	}

	void DeferredRenderer::DrawDebug()
	{
		ShaderPtr shader = ResourceManager::Instance()->GetShader("vpquad");

		shader->Bind();
		shader->SetFloat("quadScale", 2/3.f);

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(_emptyVao);

		auto DrawDebugAttachment = [this, &shader](const AttachmentPtr& att, int slot) {
			att->Bind();

			shader->SetFloat2("quadShift", 1/3.f, -1.f + 2/3.f * slot);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		};

		DrawDebugAttachment(_gAlbedoSpecular, 0);
		DrawDebugAttachment(_gNormal, 1);
		DrawDebugAttachment(_gDepth, 2);

		glBindVertexArray(0);

	}

	void DeferredRenderer::BeginPass1()
	{
		_framebufferPass1->BeginRender();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);

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
		BindColorAttachment(shader, "gDepth", _gDepth, 2);

		glActiveTexture(GL_TEXTURE0);
	}

	const FramebufferPtr & DeferredRenderer::GetFramebufferPass1()
	{
		return _framebufferPass1;
	}

	const AttachmentPtr & DeferredRenderer::GetAlbedoSpecular()
	{
		return _gAlbedoSpecular;
	}

	const AttachmentPtr & DeferredRenderer::GetDepth()
	{
		return _gDepth;
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
