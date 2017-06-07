#ifndef ALIGNEDVECTOR_H
#define ALIGNEDVECTOR_H

#include <cstring>
#ifdef __linux
#include <cstdlib>
#endif

//Really inefficient vector container that guarantees the data is 32-byte aligned.
class AlignedVector
{
	public:
		using iterator = float*;
		using const_iterator = const float*;

		AlignedVector()
			: data(nullptr),
			  dataSize(0)
		{
		}
		AlignedVector(const unsigned int size,const float value)
			: data(nullptr),
			  dataSize(0)
		{
			resize(size);
			for(unsigned int x = 0;x < size;x++)
			{
				data[x] = value;
			}
		}
		AlignedVector(const AlignedVector& other)
			: data(nullptr),
			  dataSize(0)
		{
			copy(other);
		}
		AlignedVector(AlignedVector&& other)
			: data(nullptr),
			  dataSize(0)
		{
			std::swap(data,other.data);
			std::swap(dataSize,other.dataSize);
		}
		~AlignedVector()
		{
			free(data);
		}
		AlignedVector& operator=(const AlignedVector& other)
		{
			if(this == &other)
				return *this;

			copy(other);
			return *this;
		}
		float& operator[](const unsigned int index)
		{
			return data[index];
		}
		const float& operator[](const unsigned int index) const
		{
			return data[index];
		}
		void resize(const unsigned int newSize)
		{
			if(newSize == dataSize)
				return;

			float* newData = nullptr;
#ifdef __linux
			posix_memalign(reinterpret_cast<void**>(&newData),32,newSize * sizeof(float));
#else
#error Platform not supported.
#endif
			memcpy(newData,data,std::min(dataSize,newSize) * sizeof(float));
			free(data);

			data = newData;
			dataSize = newSize;
		}
		void push_back(const float value)
		{
			resize(dataSize + 1);
			data[dataSize - 1] = value;
		}
		unsigned int size() const
		{
			return dataSize;
		}
		float* begin() const
		{
			return data;
		}
		float* end() const
		{
			return &data[dataSize];
		}
		const float* cbegin() const
		{
			return data;
		}
		const float* cend() const
		{
			return &data[dataSize];
		}
	private:
		float* data;
		unsigned int dataSize;

		void copy(const AlignedVector& other)
		{
			resize(other.dataSize);
			memcpy(data,other.data,dataSize * sizeof(float));
		}
};

#endif
