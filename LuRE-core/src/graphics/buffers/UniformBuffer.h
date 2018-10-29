#pragma once


namespace lumi {
	namespace graphics {

		class UniformBuffer
		{

		private:

			unsigned int m_id;
			unsigned int m_bindingPoint;
			size_t m_size;

		public:

			UniformBuffer(unsigned int bindingPoint);
			UniformBuffer(const UniformBuffer& other) = delete;
			UniformBuffer(UniformBuffer&& other);
			~UniformBuffer();

			UniformBuffer& operator=(const UniformBuffer& other) = delete;
			UniformBuffer& operator=(UniformBuffer&& other);

			void* mapData(unsigned int count, size_t elem_size);
			void  unmapData();

			void bind();
			void unbind();

			inline const unsigned int getBindingPoint() const { return m_bindingPoint; };
			inline const size_t getSize() const { return m_size; };

		};


	}
}