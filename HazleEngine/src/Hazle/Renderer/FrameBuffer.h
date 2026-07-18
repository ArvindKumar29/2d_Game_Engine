#pragma once
#include "Renderer.h"

namespace Hazle
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		
		//Color
		RGBA8,
		RED_INTEGER,

		//Depth/Stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8,
	};

	struct FramebufferTextureSpecifications
	{
		FramebufferTextureSpecifications() = default;
		FramebufferTextureSpecifications(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		//TODO: filtering/wrap

	};

	struct FramebufferAttachementSpecifications
	{
		FramebufferAttachementSpecifications() = default;
		FramebufferAttachementSpecifications(const std::initializer_list<FramebufferTextureSpecifications> attachements)
			: Attachements(attachements) {}


		std::vector<FramebufferTextureSpecifications> Attachements;

	};

	struct FrameBufferSpecifications
	{
		uint32_t Width, Height;
		FramebufferAttachementSpecifications Attachements;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;

	};

	class FrameBuffer
	{
	public:
		FrameBuffer();
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecifications& spec);

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearColorAttachments(uint32_t attachmentIndex, const int value) = 0;

		virtual const FrameBufferSpecifications GetSpecifications() const = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
	};
}