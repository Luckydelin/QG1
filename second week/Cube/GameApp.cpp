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

void GameApp::UpdateScene(float dt)//dt为两帧时间间隔
{
	//自动绕x轴或y轴旋转
	/*static float phi = 0.0f, theta = 0.0f;
	phi += 0.0001f, theta += 0.001f;
	m_CBuffer.world = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));*/
	


	static float cubePhi = 0.0f, cubeTheta = 0.0f,cubemid= 1.0f;
	// 获取鼠标状态
	Mouse::State mouseState = m_pMouse->GetState();
	Mouse::State lastMouseState = m_MouseTracker.GetLastState();
	// 获取键盘状态
	Keyboard::State keyState = m_pKeyboard->GetState();
	Keyboard::State lastKeyState = m_KeyboardTracker.GetLastState();

	// 更新鼠标按钮状态跟踪器，仅当鼠标按住的情况下才进行移动
	m_MouseTracker.Update(mouseState);
	m_KeyboardTracker.Update(keyState);

	//当鼠标左键长按时可进行绕Y轴或X轴的旋转
	if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
	{
		cubeTheta -= (mouseState.x - lastMouseState.x) * 0.01f;//鼠标使用像素单位
		cubePhi -= (mouseState.y - lastMouseState.y) * 0.01f;//鼠标使用像素单位
		
	}

	//滑动滚轮进行放大或缩小
	cubemid += (mouseState.scrollWheelValue - lastMouseState.scrollWheelValue) * 0.001f;//鼠标使用像素单位

	//按W/S/A/D四个键可进行绕Y轴或X轴的旋转
	if (keyState.IsKeyDown(Keyboard::W))
		cubePhi += dt * 2;
	if (keyState.IsKeyDown(Keyboard::S))
		cubePhi -= dt * 2;
	if (keyState.IsKeyDown(Keyboard::A))
		cubeTheta += dt * 2;
	if (keyState.IsKeyDown(Keyboard::D))
		cubeTheta -= dt * 2;

	//按Up/Down/Left/Right四个键可进行绕Y轴或X轴的旋转
	if (keyState.IsKeyDown(Keyboard::Up))
		cubePhi += dt * 2;
	if (keyState.IsKeyDown(Keyboard::Down))
		cubePhi -= dt * 2;
	if (keyState.IsKeyDown(Keyboard::Left))
		cubeTheta += dt * 2;
	if (keyState.IsKeyDown(Keyboard::Right))
		cubeTheta -= dt * 2;

	//按I/O键进行放大或缩小，I---IN,O---OUT
	if (keyState.IsKeyDown(Keyboard::I))
		cubemid += dt* 1;
	if (keyState.IsKeyDown(Keyboard::O))
		cubemid -= dt *1;

	//矩阵转置
	//对几何体的变换顺序通常为先缩放，后旋转，再平移
	//XMMatrixTranspose()中参数为变换的复合，比如现在的参数为先旋转Y轴再旋转X轴
	m_CBuffer.world = XMMatrixTranspose( XMMatrixRotationY(cubeTheta)* XMMatrixRotationX(cubePhi)*XMMatrixScaling(cubemid, cubemid, cubemid));
	//XMMatrixRotationX，返回旋转矩阵，参数：围绕x轴旋转的角度，以弧度表示。当沿着旋转轴向原点看时，角度是顺时针测量的
	//XMMatrixRotationY，与XMMatrixRotationX一样，只不过是返回y轴

	// 更新常量缓冲区，让立方体转起来
	D3D11_MAPPED_SUBRESOURCE mappedData;
	//map函数，获取指向缓冲区中数据的指针并拒绝gpu对该缓冲区的访问，参数1：接口资源对象，参数2：缓冲区资源，参数3：枚举值，指定读写操作，参数4：填0，cpu需要等待gpu使用完毕当前缓冲区
	//参数5：获取映射到缓冲区的内存
	HR(m_pContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	//更新//通过映射内存更新
	memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer));

	//unmap函数，让指向资源的指针无效并重新启用gpu对该资源的访问权限，参数1：接口资源对象，参数2：缓冲区资源，填0
	m_pContext->Unmap(m_pConstantBuffer.Get(), 0);
}

void GameApp::DrawScene()
{

	assert(m_pContext);
	assert(m_pSwapChain);

	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255) R--red  G--green B--blue A-Alpha色彩空间
	m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 绘制棱台
	m_pContext->DrawIndexed(36,0,0);
	//参数1：绘制的索引数
	//参数2：绘制的索引起点
	//参数3：对每个索引加上某一数值

	HR(m_pSwapChain->Present(0, 0));

	
}
bool GameApp::InitShader()
{
	ComPtr<ID3DBlob> blob;
	//创建像素着色器
	CreateShaderFromFile(L"HLSL\\PS_Cube.cso", L"HLSL\\PS_Cube.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf());
	m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf());
	//创建顶点着色器
	HR(CreateShaderFromFile(L"HLSL\\VS_Cube.cso", L"HLSL\\VS_Cube.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf());
	//创建输入布局
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
	//================================顶点缓冲区的设置========================================
	
	// ------保存顶点的数组
	/*VertexPosColor  vertices[] =
	{
		{ XMFLOAT3(-0.5f,0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.25f,sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(-0.25f,-sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(0.0f,0.2f,1.0f, 1.0f) },
		{ XMFLOAT3(0.25f, sqrtf(3.0) / 4.0f, 0.0f), XMFLOAT4(1.0f, 0.2f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.25f,-sqrtf(3.0) / 4.0f ,0.0f),XMFLOAT4(0.5f, 1.0f,0.0f, 1.0f) },
		{ XMFLOAT3(0.5f,0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	};*/
	//另一种图元类型的正六边形顶点的设置
		
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

	// ------描述顶点缓冲区
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	//-------创建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	m_pDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf());
	//================================顶点缓冲区的设置========================================




	//=======================================索引缓存的设置==============================================

	// --------索引数组
	DWORD indices[] =
	{
		// 正面
		0, 1, 2,
		2, 3, 0,
		// 左面
		4, 5, 1,
		1, 0, 4,
		// 顶面
		1, 5, 6,
		6, 2, 1,
		// 背面
		7, 6, 5,
		5, 4, 7,
		// 右面
		3, 2, 6,
		6, 7, 3,
		// 底面
		4, 0, 3,
		3, 7, 4
	};

	// -------描述索引缓冲区
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;


	// -------创建索引缓冲区
	InitData.pSysMem = indices;
	HR(m_pDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
	//=======================================索引缓存的设置==============================================


	//======================================常量缓冲区的设置=====================================================

	// --------描述常量缓冲区
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// --------新建常量缓冲区
	HR(m_pDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffer.GetAddressOf()));


	//--------- 初始化常量缓冲区
	// 
	//返回单位矩阵
	m_CBuffer.world = XMMatrixIdentity();

	//因为HLSL中的矩阵按列，又由于world为单位阵，所以我们把view和proj进行转置
	//矩阵转置
	m_CBuffer.view = XMMatrixTranspose

	//XMMatrixLookAtLH函数，基于左手坐标系，返回将点从世界空间转换为视图空间的视图矩阵，参1:摄像机的位置
							//	参2：协调中心的位置
							//参数3：相机向上的向量，表示3D坐标系中向上的坐标向量，这里表示屏幕由底向上递增的向上，这是一个规范化的坐标系
	(XMMatrixLookAtLH
	(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)//以世界空间的y轴作为摄像机“向上”的方向。因此(0, 1, 0)是平行于世界空间中y轴的一个单位向量
	));

	m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	//XMMatrixPerspectiveFovLH函数，返回透视投影矩阵，参数1：以弧度为单位的自顶向下视场角度，参数2：视宽比，参数3：距离近切飞机的距离，大于零。参数4：距离遥远的剪裁飞机，大于零
	


	//======================================常量缓冲区的设置=====================================================



	//========================================绑定装配阶段======================================================

	UINT stride = sizeof(VertexPosColor);	// 跨越字节数
	UINT offset = 0;						// 起始偏移量

	//绑定着色器
	//1.顶点着色器
	m_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	//2.像素着色器
	m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	//绑定图元类型
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//绑定输入布局
	m_pContext->IASetInputLayout(m_pVertexLayout.Get());

	//绑定顶点缓冲区
	m_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	//绑定常量缓冲区
	m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	//绑定索引缓冲区
	m_pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//=======================================绑定装配阶段========================================================
	return true;
}






