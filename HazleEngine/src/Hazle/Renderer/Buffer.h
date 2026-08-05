#pragma once


namespace Hazle
{
	// Buffer is a class that represents a buffer in the graphics pipeline. It is used to store data that will be sent to the GPU for rendering.
	// The Buffer class is an abstract base class that defines the interface for different types of buffers, such as vertex buffers and index buffers.

	// ShaderDataType is an enumeration that defines the different types of data that can be stored in a buffer. 
	// It includes types such as Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, and Bool.
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};


	// ShaderDataTypeSize is a function used to map the size of every data type that a buffer element stores.
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}
		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	// BufferElemet is a struct that represents a single element in a buffer. 
	// It contains information about the element's name, type, size, offset, and whether it is normalized.
	struct BufferElement 
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset; // Offset is the position of the element in the buffer, in bytes.
		bool Normalized; // Normalized is a flag that indicates whether the element's data should be normalized when it is sent to the GPU.

		BufferElement() {};

		BufferElement(ShaderDataType type, std::string name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{}

		// GetComponentCount is a function that returns the number of components in the element's data type.
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Bool:		return 1;
			}
			HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

	};

	// BufferLayout is a class that represents the layout of a buffer. 
	// It contains a vector of BufferElements and calculates the stride and offset of each element in the buffer.
	class BufferLayout
	{
	public:

		BufferLayout() {};

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		};
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }		// GetElements() is a function that returns the vector of BufferElements in the layout.
																								// and every element in the layout has a name, type, size, offset, and normalized flag.
																								// BufferLayout is used to define the structure of the data that will be sent to the GPU for rendering.

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }				// begin() returns an iterator to the first element in the vector of BufferElements.
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }					// end() returns an iterator to the last element in the vector of BufferElements.
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); } // const begin() returns a const iterator to the first element in the vector of BufferElements.
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }		// const end() returns a const iterator to the last element in the vector of BufferElements.

		uint32_t GetStride() const { return m_Stride; }											// GetStride() returns the stride of the buffer, which is the total size of all the elements in the buffer.


	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;


		// CalculateOffsetsAndStride() calculates the offset and stride of each element in the buffer. 
		// Stride is the total size of all the elements in the buffer, and offset is the position of each element in the buffer, in bytes.
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	};

	// VertexBuffer is a class that represents a vertex buffer in the graphics pipeline.
	// Vertex buffers are used to store vertex data, such as positions, normals, texture coordinates, and colors.
	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer();

		virtual void Bind() const = 0;										// Bind() binds the vertex buffer to the graphics pipeline, making it the active vertex buffer for rendering.
		virtual void Unbind() const = 0;									// Unbind() unbinds the vertex buffer from the graphics pipeline, making it inactive for rendering.

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);	// Create() is a static function that creates a new vertex buffer with the specified vertex data and size.
		static Ref<VertexBuffer> Create(uint32_t size);						// Create() is a static function that creates a new vertex buffer with the specified size, but without any vertex data.
		virtual const BufferLayout& GetLayout() const = 0;					// GetLayout() returns the layout of the vertex buffer, which describes the structure of the vertex data stored in the buffer.
		virtual void SetLayout(const BufferLayout& layout) = 0;				// SetLayout() sets the layout of the vertex buffer, which describes the structure of the vertex data stored in the buffer.
		virtual void SetData(const void* data, uint32_t size) = 0;			// SetData() sets the vertex data stored in the buffer, replacing any existing data.

	private:
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer();
		virtual void Bind() const = 0;										// Bind() binds the index buffer to the graphics pipeline, making it the active index buffer for rendering.
		virtual void Unbind() const = 0;									// Unbind() unbinds the index buffer from the graphics pipeline, making it inactive for rendering.

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);	// Create() is a static function that creates a new index buffer with the specified index data and count.

		virtual uint32_t GetCount() const = 0;								// GetCount() returns the number of indices in the index buffer.
	};

}