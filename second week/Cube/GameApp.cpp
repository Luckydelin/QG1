#include "GameApp.h"
#include "D3DUtil.h"
#include "DXTrace.h"
using namespace DirectX;
GameApp::GameApp(HINSTANCE hInstance) : D3DApp(hInstance) ,m_CBuffer()
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

void GameApp::UpdateScene(float dt)//dtΪ��֡ʱ����
{
	//�Զ���x���y����ת
	/*static float phi = 0.0f, theta = 0.0f;
	phi += 0.0001f, theta += 0.001f;
	m_CBuffer.world = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));*/
	


	static float cubePhi = 0.0f, cubeTheta = 0.0f,cubemid= 1.0f;
	// ��ȡ���״̬
	Mouse::State mouseState = m_pMouse->GetState();
	Mouse::State lastMouseState = m_MouseTracker.GetLastState();
	// ��ȡ����״̬
	Keyboard::State keyState = m_pKeyboard->GetState();
	Keyboard::State lastKeyState = m_KeyboardTracker.GetLastState();

	// ������갴ť״̬��������������갴ס������²Ž����ƶ�
	m_MouseTracker.Update(mouseState);
	m_KeyboardTracker.Update(keyState);

	//������������ʱ�ɽ�����Y���X�����ת
	if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
	{
		cubeTheta -= (mouseState.x - lastMouseState.x) * 0.01f;//���ʹ�����ص�λ
		cubePhi -= (mouseState.y - lastMouseState.y) * 0.01f;//���ʹ�����ص�λ
		
	}

	//�������ֽ��зŴ����С
	cubemid += (mouseState.scrollWheelValue - lastMouseState.scrollWheelValue) * 0.001f;//���ʹ�����ص�λ

	//��W/S/A/D�ĸ����ɽ�����Y���X�����ת
	if (keyState.IsKeyDown(Keyboard::W))
		cubePhi += dt * 2;
	if (keyState.IsKeyDown(Keyboard::S))
		cubePhi -= dt * 2;
	if (keyState.IsKeyDown(Keyboard::A))
		cubeTheta += dt * 2;
	if (keyState.IsKeyDown(Keyboard::D))
		cubeTheta -= dt * 2;

	//��Up/Down/Left/Right�ĸ����ɽ�����Y���X�����ת
	if (keyState.IsKeyDown(Keyboard::Up))
		cubePhi += dt * 2;
	if (keyState.IsKeyDown(Keyboard::Down))
		cubePhi -= dt * 2;
	if (keyState.IsKeyDown(Keyboard::Left))
		cubeTheta += dt * 2;
	if (keyState.IsKeyDown(Keyboard::Right))
		cubeTheta -= dt * 2;

	//��I/O�����зŴ����С��I---IN,O---OUT
	if (keyState.IsKeyDown(Keyboard::I))
		cubemid += dt* 1;
	if (keyState.IsKeyDown(Keyboard::O))
		cubemid -= dt *1;

	//����ת��
	//�Լ�����ı任˳��ͨ��Ϊ�����ţ�����ת����ƽ��
	//XMMatrixTranspose()�в���Ϊ�任�ĸ��ϣ��������ڵĲ���Ϊ����תY������תX��
	m_CBuffer.world = XMMatrixTranspose( XMMatrixRotationY(cubeTheta)* XMMatrixRotationX(cubePhi)*XMMatrixScaling(cubemid, cubemid, cubemid));
	//XMMatrixRotationX��������ת���󣬲�����Χ��x����ת�ĽǶȣ��Ի��ȱ�ʾ����������ת����ԭ�㿴ʱ���Ƕ���˳ʱ�������
	//XMMatrixRotationY����XMMatrixRotationXһ����ֻ�����Ƿ���y��

	// ���³�������������������ת����
	D3D11_MAPPED_SUBRESOURCE mappedData;
	//map��������ȡָ�򻺳��������ݵ�ָ�벢�ܾ�gpu�Ըû������ķ��ʣ�����1���ӿ���Դ���󣬲���2����������Դ������3��ö��ֵ��ָ����д����������4����0��cpu��Ҫ�ȴ�gpuʹ����ϵ�ǰ������
	//����5����ȡӳ�䵽���������ڴ�
	HR(m_pContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	//����//ͨ��ӳ���ڴ����
	memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer));

	//unmap��������ָ����Դ��ָ����Ч����������gpu�Ը���Դ�ķ���Ȩ�ޣ�����1���ӿ���Դ���󣬲���2����������Դ����0
	m_pContext->Unmap(m_pConstantBuffer.Get(), 0);
}

void GameApp::DrawScene()
{

	assert(m_pContext);
	assert(m_pSwapChain);

	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255) R--red  G--green B--blue A-Alphaɫ�ʿռ�
	m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ������̨
	m_pContext->DrawIndexed(36,0,0);
	//����1�����Ƶ�������
	//����2�����Ƶ��������
	//����3����ÿ����������ĳһ��ֵ

	HR(m_pSwapChain->Present(0, 0));

	
}
bool GameApp::InitShader()
{
	ComPtr<ID3DBlob> blob;
	//����������ɫ��
	CreateShaderFromFile(L"HLSL\\PS_Cube.cso", L"HLSL\\PS_Cube.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf());
	m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf());
	//����������ɫ��
	HR(CreateShaderFromFile(L"HLSL\\VS_Cube.cso", L"HLSL\\VS_Cube.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf());
	//�������벼��
	m_pDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf());
	

	return true;
}
const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
bool GameApp::InitResource()
{
	//================================���㻺����������========================================
	
	// ------���涥�������
	/*VertexPosColor  vertices[] =
	{
		{ XMFLOAT3(-0.5f,0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.25f,sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(-0.25f,-sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f,0.2f,1.0f, 1.0f) },
		{ XMFLOAT3(0.25f, sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(1.0f, 0.2f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.25f,-sqrtf(3.0) / 4.0f ,0.0f),XMFLOAT4(0.5f, 1.0f,0.0f, 1.0f) },
		{ XMFLOAT3(0.5f,0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	};*/
	//��һ��ͼԪ���͵��������ζ��������
		
	VertexPosColor vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },//0
		{ XMFLOAT3(-0.4f, 0.7f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },//1
		{ XMFLOAT3(0.4f, 0.7f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },//2
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },//3
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },//4
		{ XMFLOAT3(-0.4f, 0.7f, 0.8f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },//5
		{ XMFLOAT3(0.4f, 0.7f, 0.8f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },//6
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }//7
	};

	// ------�������㻺����
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	//-------�������㻺����
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	m_pDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf());
	//================================���㻺����������========================================




	//=======================================�������������==============================================

	// --------��������
	DWORD indices[] =
	{
		// ����
		0, 1, 2,
		2, 3, 0,
		// ����
		4, 5, 1,
		1, 0, 4,
		// ����
		1, 5, 6,
		6, 2, 1,
		// ����
		7, 6, 5,
		5, 4, 7,
		// ����
		3, 2, 6,
		6, 7, 3,
		// ����
		4, 0, 3,
		3, 7, 4
	};

	// -------��������������
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;


	// -------��������������
	InitData.pSysMem = indices;
	HR(m_pDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
	//=======================================�������������==============================================


	//======================================����������������=====================================================

	// --------��������������
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// --------�½�����������
	HR(m_pDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffer.GetAddressOf()));


	//--------- ��ʼ������������
	// 
	//���ص�λ����
	m_CBuffer.world = XMMatrixIdentity();

	//��ΪHLSL�еľ����У�������worldΪ��λ���������ǰ�view��proj����ת��
	//����ת��
	m_CBuffer.view = XMMatrixTranspose

	//XMMatrixLookAtLH������������������ϵ�����ؽ��������ռ�ת��Ϊ��ͼ�ռ����ͼ���󣬲�1:�������λ��
							//	��2��Э�����ĵ�λ��
							//����3��������ϵ���������ʾ3D����ϵ�����ϵ����������������ʾ��Ļ�ɵ����ϵ��������ϣ�����һ���淶��������ϵ
	(XMMatrixLookAtLH
	(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)//������ռ��y����Ϊ����������ϡ��ķ������(0, 1, 0)��ƽ��������ռ���y���һ����λ����
	));

	m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	//XMMatrixPerspectiveFovLH����������͸��ͶӰ���󣬲���1���Ի���Ϊ��λ���Զ������ӳ��Ƕȣ�����2���ӿ�ȣ�����3��������зɻ��ľ��룬�����㡣����4������ңԶ�ļ��÷ɻ���������
	


	//======================================����������������=====================================================



	//========================================��װ��׶�======================================================

	UINT stride = sizeof(VertexPosColor);	// ��Խ�ֽ���
	UINT offset = 0;						// ��ʼƫ����

	//����ɫ��
	//1.������ɫ��
	m_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	//2.������ɫ��
	m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	//��ͼԪ����
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�����벼��
	m_pContext->IASetInputLayout(m_pVertexLayout.Get());

	//�󶨶��㻺����
	m_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	//�󶨳���������
	m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	//������������
	m_pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//=======================================��װ��׶�========================================================
	return true;
}






