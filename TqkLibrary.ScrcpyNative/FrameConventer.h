#ifndef FrameConventer_H
#define FrameConventer_H
class FrameConventer
{
public:
	FrameConventer();
	~FrameConventer();
	bool Convert(AVFrame* frame, BYTE* buff, const int sizeInByte, const int w, const int h, const int lineSize);

private:
};

#endif // !FrameConventer_H


