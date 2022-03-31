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

	//��C++Ӧ�ò㣬��Ӷ�Ӧ�Ķ��㻺�����ṹ�壺
	struct VertexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	};

	//��C++Ӧ�ò㣬��Ӷ�Ӧ�ĳ����������ṹ�壺
	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

private:
	ComPtr<ID3D11InputLayout> m_pVertexLayout;	// �������벼��
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// ���㻺����
	ComPtr<ID3D11VertexShader> m_pVertexShader;	// ������ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader;	// ������ɫ��

	ComPtr<ID3D11Buffer> m_pIndexBuffer;			// ����������
	ComPtr<ID3D11Buffer> m_pConstantBuffer;		    // ����������
	ConstantBuffer m_CBuffer;	                    // �����޸�GPU�����������ı���
};


