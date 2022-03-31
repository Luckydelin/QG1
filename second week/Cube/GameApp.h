#include"D3DAPP.h"
#include "DXTrace.h"
#include"Timer.h"

class GameApp : public D3DApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();

	void OnResize();

	void UpdateScene(float dt);
	void DrawScene();
	bool InitShader();
	bool InitResource();
public:

	//在C++应用层，添加对应的顶点缓冲区结构体：
	struct VertexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	};

	//在C++应用层，添加对应的常量缓冲区结构体：
	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

private:
	ComPtr<ID3D11InputLayout> m_pVertexLayout;	// 顶点输入布局
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// 顶点缓冲区
	ComPtr<ID3D11VertexShader> m_pVertexShader;	// 顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader;	// 像素着色器

	ComPtr<ID3D11Buffer> m_pIndexBuffer;			// 索引缓冲区
	ComPtr<ID3D11Buffer> m_pConstantBuffer;		    // 常量缓冲区
	ConstantBuffer m_CBuffer;	                    // 用于修改GPU常量缓冲区的变量
};


