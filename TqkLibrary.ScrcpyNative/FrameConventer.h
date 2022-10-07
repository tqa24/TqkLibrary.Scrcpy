#ifndef _H_FrameConventer_H_
#define _H_FrameConventer_H_
class FrameConventer
{
public:
	static bool Convert(AVFrame* frame, BYTE* buff, const int sizeInByte, const int w, const int h, const int lineSize);

private:
};

#endif // !FrameConventer_H


