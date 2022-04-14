#include "pch.h"
#include "PixelShader.h"
#include "Utils.h"
#include "NV12ToRgbShader.h"

using namespace Microsoft::WRL;
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

NV12ToRgbShader::NV12ToRgbShader() {
}

NV12ToRgbShader::~NV12ToRgbShader() {
	this->ReleaseSharedSurf();
	
	if (this->_d3d11_pixelShader != nullptr)
		this->_d3d11_pixelShader->Release();

	if (this->_d3d11_device != nullptr)
		this->_d3d11_device->Release();

	if (this->_d3d11_deviceCtx != nullptr)
		this->_d3d11_deviceCtx->Release();
}

bool NV12ToRgbShader::Init() {
	/*if (this->_avhw_deviceCtx == nullptr ||
		this->_avhw_deviceCtx->type != AVHWDeviceType::AV_HWDEVICE_TYPE_D3D11VA)
		return false;

	this->_av_d3d11_vaDeviceCtx = reinterpret_cast<AVD3D11VADeviceContext*>(this->_avhw_deviceCtx->hwctx);
	this->_d3d11_deviceCtx = this->_av_d3d11_vaDeviceCtx->device_context;
	this->_d3d11_device = _av_d3d11_vaDeviceCtx->device;*/

	HRESULT hr;

	// Driver types supported
	D3D_DRIVER_TYPE DriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT NumDriverTypes = ARRAYSIZE(DriverTypes);

	// Feature levels supported
	D3D_FEATURE_LEVEL FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1
	};
	UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);
	D3D_FEATURE_LEVEL FeatureLevel;
	// This flag adds support for surfaces with a different color channel ordering
	// than the default. It is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	for (UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes; ++DriverTypeIndex)
	{
		hr = D3D11CreateDevice(nullptr, DriverTypes[DriverTypeIndex], nullptr, creationFlags, FeatureLevels, NumFeatureLevels,
			D3D11_SDK_VERSION, &this->_d3d11_device, &FeatureLevel, &this->_d3d11_deviceCtx);
		if (SUCCEEDED(hr))
		{
			// Device creation succeeded, no need to loop anymore
			break;
		}
	}
	if (FAILED(hr))
		return false;

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

	return true;
}

bool NV12ToRgbShader::CreateSharedSurf(int width, int height) {
	//
	D3D11_TEXTURE2D_DESC const texDesc = CD3D11_TEXTURE2D_DESC(
		DXGI_FORMAT_NV12,           // HoloLens PV camera format, common for video sources
		width,					// Width of the video frames
		height,					// Height of the video frames
		1,                          // Number of textures in the array
		1,                          // Number of miplevels in each texture
		D3D11_BIND_SHADER_RESOURCE, // We read from this texture in the shader
		D3D11_USAGE_DYNAMIC,        // Because we'll be copying from CPU memory
		D3D11_CPU_ACCESS_WRITE      // We only need to write into the texture
	);
	HRESULT hr = this->_d3d11_device->CreateTexture2D(&texDesc, nullptr, &this->_texture);
	if (FAILED(hr))
		return false;

	//
	D3D11_SHADER_RESOURCE_VIEW_DESC const luminancePlaneDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(
		this->_texture,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_R8_UNORM
	);
	hr = this->_d3d11_device->CreateShaderResourceView(
		this->_texture,
		&luminancePlaneDesc,
		&this->_luminanceView
	);
	if (FAILED(hr))
		return false;

	//
	D3D11_SHADER_RESOURCE_VIEW_DESC const chrominancePlaneDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(
		this->_texture,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_R8G8_UNORM
	);
	hr = this->_d3d11_device->CreateShaderResourceView(
		this->_texture,
		&chrominancePlaneDesc,
		&this->_chrominanceView
	);
	if (FAILED(hr))
		return false;

	this->_width = width;
	this->_height = height;

	return true;
}
void NV12ToRgbShader::ReleaseSharedSurf() {
	if (this->_luminanceView != nullptr) {
		this->_luminanceView->Release();
		this->_luminanceView = nullptr;
	}
	if (this->_chrominanceView != nullptr) {
		this->_chrominanceView->Release();
		this->_chrominanceView = nullptr;
	}
	if (this->_texture != nullptr) {
		this->_texture->Release();
		this->_texture = nullptr;
	}
	this->_width = 0;
	this->_height = 0;
}




//https://medium.com/swlh/streaming-video-with-ffmpeg-and-directx-11-7395fcb372c4
bool NV12ToRgbShader::Convert(const AVFrame* source, AVFrame** received) {
	HRESULT hr{ 0 };
	if (source == NULL)
		return false;
	if (source->format != AV_PIX_FMT_D3D11)
		return false;
	if (!source->hw_frames_ctx)
		return false;

	if (this->_d3d11_device == nullptr) {
		if (!Init())
			return false;
	}
	//init/reinit shader surface
	if (this->_width != source->width || this->_height != source->height) {
		this->ReleaseSharedSurf();
		if (!this->CreateSharedSurf(source->width, source->height))
			return false;
	}


	//bind/copy ffmpeg hw texture -> local d3d11 texture
	ComPtr<ID3D11Texture2D> new_texture = (ID3D11Texture2D*)source->data[0];
	const int texture_index = (int)source->data[1];
	this->_d3d11_deviceCtx->CopySubresourceRegion(
		this->_texture, 0, 0, 0, 0,
		new_texture.Get(), texture_index, nullptr
	);

	// Rendering NV12 requires two resource views, which represent the luminance and chrominance channels of the YUV formatted texture.
	std::array<ID3D11ShaderResourceView*, 2> const textureViews =
	{
		this->_luminanceView,
		this->_chrominanceView
	};

	// Bind the NV12 channels to the shader.
	this->_d3d11_deviceCtx->PSSetShaderResources(
		0,
		textureViews.size(),
		textureViews.data()
	);

	this->_d3d11_deviceCtx->PSSetShader(this->_d3d11_pixelShader, nullptr, 0);
	this->_d3d11_deviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
	hr = this->_d3d11_device->CreateBuffer(&BufferDesc, &InitData, &VertexBuffer);
	if (FAILED(hr))
		return false;

	UINT Stride = sizeof(VERTEX);
	UINT Offset = 0;
	_d3d11_deviceCtx->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	// Draw textured quad onto render target
	_d3d11_deviceCtx->Draw(NUMVERTICES, 0);

	D3D11_MAPPED_SUBRESOURCE ms;
	_d3d11_deviceCtx->Map(this->_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

	AVFrame* frame = av_frame_alloc();
	if (frame != nullptr) {
		if (avcheck(av_image_fill_arrays(frame->data, frame->linesize, nullptr, AVPixelFormat::AV_PIX_FMT_BGRA, source->width, source->height, source->width * 4)))
		{
			memcpy(frame->data[0], ms.pData, frame->linesize[0]);
		}
		else
		{
			av_frame_free(&frame);
		}
	}
	*received = frame;

	_d3d11_deviceCtx->Unmap(this->_texture, 0);

	return *received != nullptr;
}

