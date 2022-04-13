#include "pch.h"
#include "NV12ToRgbShader.h"
#include "PixelShader.h"
using namespace DirectX;
#define NUMVERTICES 6
typedef struct _VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT2 TexCoord;
} VERTEX;

// Vertices for drawing whole texture
VERTEX Vertices[NUMVERTICES] =
{
	{XMFLOAT3(-1.0f, -1.0f, 0), XMFLOAT2(0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, 0), XMFLOAT2(0.0f, 0.0f)},
	{XMFLOAT3(1.0f, -1.0f, 0), XMFLOAT2(1.0f, 1.0f)},
	{XMFLOAT3(1.0f, -1.0f, 0), XMFLOAT2(1.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, 0), XMFLOAT2(0.0f, 0.0f)},
	{XMFLOAT3(1.0f, 1.0f, 0), XMFLOAT2(1.0f, 0.0f)},
};

NV12ToRgbShader::NV12ToRgbShader(AVHWDeviceContext* deviceContext) {
	this->_avhw_deviceCtx = deviceContext;
}

NV12ToRgbShader::~NV12ToRgbShader() {
	if (this->_d3d11_pixelShader != nullptr)
		this->_d3d11_pixelShader->Release();
}

bool NV12ToRgbShader::Init() {
	if (this->_avhw_deviceCtx == nullptr ||
		this->_avhw_deviceCtx->type != AVHWDeviceType::AV_HWDEVICE_TYPE_D3D11VA)
		return false;

	this->_av_d3d11_vaDeviceCtx = reinterpret_cast<AVD3D11VADeviceContext*>(this->_avhw_deviceCtx->hwctx);
	this->_d3d11_deviceCtx = this->_av_d3d11_vaDeviceCtx->device_context;
	this->_d3d11_device = _av_d3d11_vaDeviceCtx->device;

	if (!InitShader())
		return false;

	return true;
}

bool NV12ToRgbShader::InitShader() {
	if (this->_d3d11_device == nullptr)
		return false;
	HRESULT hr = 0;

	UINT Size = ARRAYSIZE(g_PS);
	hr = this->_d3d11_device->CreatePixelShader(
		g_PS,
		Size,
		NULL,
		&this->_d3d11_pixelShader);

	if (FAILED(hr))
		return false;

	//_d3d11_deviceCtx->PSSetShader(this->_d3d11_pixelShader, nullptr, 0);

	return true;
}

//https://medium.com/swlh/streaming-video-with-ffmpeg-and-directx-11-7395fcb372c4
bool NV12ToRgbShader::Convert(const AVFrame* source, AVFrame** received) {
	if (source == NULL) return false;
	if (source->format != AV_PIX_FMT_D3D11) return false;
	if (!source->hw_frames_ctx) return false;
	if (this->_avhw_deviceCtx == nullptr) return false;
	HRESULT hr{ 0 };
	//For hwaccel-format frames, this should be a reference to the AVHWFramesContext describing the frame.
	AVHWFramesContext* hw_frames_ctx = reinterpret_cast<AVHWFramesContext*>(source->hw_frames_ctx->data);
	AVHWDeviceContext* hw_device_ctx = hw_frames_ctx->device_ctx;
	if (hw_device_ctx != _avhw_deviceCtx)
		return false;

	AVD3D11VADeviceContext* d3d11_device_ctx = reinterpret_cast<AVD3D11VADeviceContext*>(hw_device_ctx->hwctx);
	AVD3D11VAFramesContext* d3d11_frame_ctx = reinterpret_cast<AVD3D11VAFramesContext*>(hw_frames_ctx->hwctx);

	ID3D11Device* d3d11_device = d3d11_device_ctx->device;
	ID3D11Texture2D* d3d11_texture_2d = d3d11_frame_ctx->texture;

	D3D11_TEXTURE2D_DESC textureDesc{ 0 };
	d3d11_texture_2d->GetDesc(&textureDesc);
	

	
	ComPtr<ID3D11ShaderResourceView> luminanceView{ NULL };
	D3D11_SHADER_RESOURCE_VIEW_DESC const luminancePlaneDesc
		= CD3D11_SHADER_RESOURCE_VIEW_DESC(d3d11_texture_2d, D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R8_UNORM);
	hr = this->_d3d11_device->CreateShaderResourceView(d3d11_texture_2d, &luminancePlaneDesc, &luminanceView);
	if (FAILED(hr))
		return false;

	ComPtr<ID3D11ShaderResourceView> chrominanceView{ NULL };
	D3D11_SHADER_RESOURCE_VIEW_DESC const chrominancePlaneDesc
		= CD3D11_SHADER_RESOURCE_VIEW_DESC(d3d11_texture_2d, D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R8G8_UNORM);
	hr = this->_d3d11_device->CreateShaderResourceView(d3d11_texture_2d, &chrominancePlaneDesc, &chrominanceView);
	if (FAILED(hr))
		return false;
	
	// Rendering NV12 requires two resource views, which represent the luminance and chrominance channels of the YUV formatted texture.
	std::array<ID3D11ShaderResourceView*, 2> const textureViews =
	{
		luminanceView.Get(),
		chrominanceView.Get()
	};
	// Bind the NV12 channels to the shader.
	_d3d11_deviceCtx->PSSetShaderResources(
		0,
		textureViews.size(),
		textureViews.data()
	);
	_d3d11_deviceCtx->PSSetShader(this->_d3d11_pixelShader, nullptr, 0);

	D3D11_BUFFER_DESC BufferDesc;
	RtlZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth = sizeof(VERTEX) * NUMVERTICES;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	RtlZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = Vertices;

	ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
	hr = d3d11_device->CreateBuffer(&BufferDesc, &InitData, &VertexBuffer);
	if (FAILED(hr))
		return false;

	UINT Stride = sizeof(VERTEX);
	UINT Offset = 0;
	_d3d11_deviceCtx->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	// Draw textured quad onto render target
	_d3d11_deviceCtx->Draw(NUMVERTICES, 0);

	D3D11_MAPPED_SUBRESOURCE ms;
	_d3d11_deviceCtx->Map(d3d11_texture_2d, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	
	AVFrame* frame = av_frame_alloc();
	av_image_fill_arrays(frame->data, frame->linesize, nullptr, AVPixelFormat::AV_PIX_FMT_BGRA, source->width, source->height, source->width * 4);
	memcpy(frame->data[0], ms.pData, frame->linesize[0]);
	*received = frame;
	
	_d3d11_deviceCtx->Unmap(d3d11_texture_2d, 0);
	return true;
}

