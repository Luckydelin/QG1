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
	struct VertexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	};

private:
	ComPtr<ID3D11InputLayout> m_pVertexLayout;	// �������벼��
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// ���㻺����
	ComPtr<ID3D11VertexShader> m_pVertexShader;	// ������ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader;	// ������ɫ��

	
};


