#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		
		default:
			break;
		}
		ASSERT(false);
		return 0;
	}
};


class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> _elements;
	unsigned int _stride;
public:
	VertexBufferLayout(): _stride(0){}
	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count)
	{
		(void)count;
		static_assert(false, "NOP only uchar, uint and float");
	}

	inline std::vector<VertexBufferElement> GetElements() const {return _elements;}
	inline unsigned int GetStride() const {return _stride;}
};

template<>
void VertexBufferLayout::Push<float>(unsigned int count)
{
	_elements.push_back((VertexBufferElement){GL_FLOAT, count, GL_FALSE});
	_stride+= VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
	_elements.push_back((VertexBufferElement){GL_UNSIGNED_INT, count, GL_FALSE});
	_stride+= VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
	_elements.push_back((VertexBufferElement){GL_UNSIGNED_BYTE, count, GL_TRUE});
	_stride+= VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
