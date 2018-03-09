// ¯\_(ツ)_/¯

#pragma once

namespace MyGL
{
	class Attachment
	{
	public:
		enum class Type {
			RGB,
			RGBA,
			Depth
		};

		Attachment(int width, int height, Type type, bool highPrecision = false);
		~Attachment();

		void Bind();

		GLuint GetTexture() const;

	private:
		GLuint _texture;
	};

	using AttachmentPtr = std::shared_ptr<Attachment>;

	class Framebuffer
	{
	public:
		Framebuffer();
		~Framebuffer();

		void BeginRender();
		void EndRender();

		void AttachColor(AttachmentPtr colorAttachment);
		void AttachDepth(AttachmentPtr depthAttachment);

		void BlitDepthToMainFramebuffer(int width, int height);

	private:
		GLuint _framebuffer;

		std::vector<AttachmentPtr> _colorAttachments;
		AttachmentPtr _depthAttachments;

		std::vector<GLenum> _attachmentsArg;
	};

	using FramebufferPtr = std::shared_ptr<Framebuffer>;
}
