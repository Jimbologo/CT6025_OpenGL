#ifndef __FRAME_BUFFER_OBJECT_H__
#define __FRAME_BUFFER_OBJECT_H__


class FrameBufferObject
{
public:
	FrameBufferObject();
	~FrameBufferObject();

	unsigned int m_FBO_ID;
	unsigned int m_FBO_texture;
	unsigned int m_FBO_depth_texture;
	unsigned int m_FBO_Linear_Depth;
private:

};

#endif // !__FRAME_BUFFER_OBJECT_H__
