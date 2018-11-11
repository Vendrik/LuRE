#pragma once


namespace lumi {
	namespace graphics {

		class ShaderStorageBuffer
		{
		private:

			unsigned int m_id;
			unsigned int m_bindingPoint;
			size_t m_size;

		public:

			ShaderStorageBuffer(unsigned int bindingPoint);
			ShaderStorageBuffer(const ShaderStorageBuffer& other) = delete;
			ShaderStorageBuffer(ShaderStorageBuffer&& other);
			~ShaderStorageBuffer();

			ShaderStorageBuffer& operator=(const ShaderStorageBuffer& other) = delete;
			ShaderStorageBuffer& operator=(ShaderStorageBuffer&& other);

			void* mapData(unsigned int count, size_t elem_size);
			void  unmapData();

			void bind();
			void unbind();

			inline const unsigned int getBindingPoint() const { return m_bindingPoint; };
			inline const size_t getSize() const { return m_size; };


		};

	}
}