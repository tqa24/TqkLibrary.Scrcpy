#ifndef _H_AudioDecoder_H_
#define _H_AudioDecoder_H_
class AudioDecoder
{
public:
	AudioDecoder(const AVCodec* codec, const ScrcpyNativeConfig& nativeConfig);
	~AudioDecoder();
	bool Init();

	bool Decode(const AVPacket* packet);
private:
	//const
	const AVCodec* _codec = nullptr;
	ScrcpyNativeConfig _nativeConfig{};
	std::mutex _mtx_frame;



	//need delete
	AVFrame* _decoding_frame = nullptr;
	AVCodecContext* _codec_ctx = nullptr;



};
#endif
