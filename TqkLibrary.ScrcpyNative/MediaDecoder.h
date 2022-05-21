#ifndef MediaDecoder_H
#define MediaDecoder_H
class MediaDecoder
{
public:
	MediaDecoder(const AVCodec* codec, const ScrcpyNativeConfig& nativeConfig);
	~MediaDecoder();
	bool Init();


	bool Decode(const AVPacket* packet, AVFrame* received);
private:
	bool TransferNoHw(AVFrame* frame);
	bool FFmpegTransfer(AVFrame* frame);


	ScrcpyNativeConfig _nativeConfig{};
	AVFrame* _decoding_frame = nullptr;
	AVFrame* _transfer_frame = nullptr;
	AVCodecContext* _codec_ctx = nullptr;
	const AVCodec* _codec = nullptr;
	AVHWDeviceType _hwType;
	NV12ToRgbShader* _d3d11_shader = nullptr;
};
#endif // !MediaDecoder_H