#include "GameApp.h"
#include "D3DUtil.h"

using namespace DirectX;
GameApp::GameApp(HINSTANCE hInstance) : D3DApp(hInstance)
{

}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;
	if(!InitShader())
		return false;
	if(!InitResource())
		return false;
	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{

}

void GameApp::DrawScene()
{
	assert(m_pContext);
	assert(m_pSwapChain);
	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//��ɫRGBA,�ֱ��Ӧ��������͸����
	m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(),black);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_pContext->Draw(6, 0);
	HR(m_pSwapChain->Present(0, 0));//����ǰ�󻺳���
}
bool GameApp::InitShader()
{
	ComPtr<ID3DBlob> blob;
	//����������ɫ��
	HR(CreateShaderFromFile(L"HLSL\\VS_Hexagon.cso", L"HLSL\\VS_Hexagon.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf());
	//�������벼��
	m_pDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf());
	//����������ɫ��
	CreateShaderFromFile(L"HLSL\\PS_Hexagon.cso", L"HLSL\\PS_Hexagon.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf());
	m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf());

	return true;
}
const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
bool GameApp::InitResource()
{
	// �����������ζ���
	VertexPosColor  vertices[] =
	{
		{ XMFLOAT3(-0.5f,0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.25f,sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(-0.25f,-sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f,0.2f,1.0f, 1.0f) },
		{ XMFLOAT3(0.25f, sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(1.0f, 0.2f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.25f,-sqrtf(3.0) / 4.0f ,0.0f),XMFLOAT4(0.5f, 1.0f,0.0f, 1.0f) },
		{ XMFLOAT3(0.5f,0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

	};
	//��һ��ͼԪ���͵��������ζ��������
	/*VertexPosColor vertices[] =
	{
		{ XMFLOAT3(-0.5f,0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.25f,sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(-0.25f,-sqrtf(3.0)/ 4.0f, 0.0f), XMFLOAT4(0.0f,0.2f,1.0f, 1.0f) },

		{ XMFLOAT3(-0.25f,sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.25f, sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(1.0f, 0.2f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.25f,-sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f,0.2f,1.0f, 1.0f) },

		{ XMFLOAT3(-0.25f,-sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f,0.2f,1.0f, 1.0f) },
		{ XMFLOAT3(0.25f, sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(1.0f, 0.2f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.25f,-sqrtf(3.0) / 4.0f ,0.0f), XMFLOAT4(0.5f, 1.0f,0.0f, 1.0f) },

		{ XMFLOAT3(0.25f, sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(1.0f, 0.2f, 0.0f, 1.0f) },
		 {XMFLOAT3(0.5f,0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.25f,-sqrtf(3.0) / 4.0f ,0.0f), XMFLOAT4(0.5f, 1.0f,0.0f, 1.0f) },


	};*/
	// �������㻺����
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	//���㻺����������Դ
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	m_pDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf());



	UINT stride = sizeof(VertexPosColor);	// ��Խ�ֽ���
	UINT offset = 0;						// ��ʼƫ����

	//����ɫ��
	m_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	//����ͼԪ����
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�������벼��
	m_pContext->IASetInputLayout(m_pVertexLayout.Get());
	//�󶨶��㻺����
	m_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	return true;
}






