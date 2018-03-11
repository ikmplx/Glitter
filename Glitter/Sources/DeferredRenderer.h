// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class DeferredRenderer
	{
	public:
		DeferredRenderer(int width, int height);
		~DeferredRenderer();

		void BeginPass1();
		void EndPass1();

		void BindColorAttachments(ShaderPtr shader);

		const FramebufferPtr& GetFramebufferPass1();
		const AttachmentPtr& GetAlbedoSpecular();
		const AttachmentPtr& GetDepth();

	private:
		void BindColorAttachment(ShaderPtr shader, const char* name, AttachmentPtr attachment, int index);

	private:
		int _width, _height;

		AttachmentPtr _gAlbedoSpecular;
		AttachmentPtr _gNormal;
		AttachmentPtr _gPosition;
		AttachmentPtr _gDepth;

		FramebufferPtr _framebufferPass1;
	};


}
