#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;



GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance),
	m_IndexCount(),	
	m_CurrFrame(),
	m_CurrMode(ShowMode::Test1),
	m_VSConstantBuffer3(),
	m_PSConstantBuffer(),
	m_VSConstantBuffer1(),
	m_DirLight(),
	m_PointLight(),
	m_SpotLight(),
	AutoRotation(false)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	// 初始化鼠标，键盘
	m_pMouse->SetWindow(m_hMainWnd);
	m_pMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{

	Keyboard::State state = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(state);

	// 键盘切换模式
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::D1) && m_CurrMode != ShowMode::Test1)
	{
		// 播放木箱动画
		m_CurrMode = ShowMode::Test1;
		m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
		auto meshData = Geometry::CreateBox();
		ResetMesh(meshData);
		m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
		
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureOne.GetAddressOf());
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureTwo.GetAddressOf());
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureThree.GetAddressOf());
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureFour.GetAddressOf());
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureFive.GetAddressOf());
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureSix.GetAddressOf());
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::D2) && m_CurrMode != ShowMode::Test2)
	{
		m_CurrMode = ShowMode::Test2;
		m_CurrFrame = 0;
		m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout2D.Get());
		auto meshData = Geometry::Create2DShow();
		ResetMesh(meshData);
		m_pd3dImmediateContext->VSSetShader(m_pVertexShader2D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShader(m_pPixelShader2D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pFireAnims[0].GetAddressOf());

	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::D3) && m_CurrMode != ShowMode::Test3)
	{
		// 播放木箱动画
		m_CurrMode = ShowMode::Test3;
		m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
		auto meshData = Geometry::CreateBox();
		ResetMesh(meshData);
		m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureInputA.GetAddressOf());
	}

	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Q))
	{
		m_PSConstantBuffer.dirLight[0] = m_DirLight;
		m_PSConstantBuffer.pointLight[0] = PointLight();
		m_PSConstantBuffer.spotLight[0] = SpotLight();
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::W))
	{
		m_PSConstantBuffer.dirLight[0] = DirectionalLight();
		m_PSConstantBuffer.pointLight[0] = m_PointLight;
		m_PSConstantBuffer.spotLight[0] = SpotLight();
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::E))
	{
		m_PSConstantBuffer.dirLight[0] = DirectionalLight();
		m_PSConstantBuffer.pointLight[0] = PointLight();
		m_PSConstantBuffer.spotLight[0] = m_SpotLight;
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::R))
	{
		m_PSConstantBuffer.dirLight[0] = DirectionalLight();
		m_PSConstantBuffer.pointLight[0] = PointLight();
		m_PSConstantBuffer.spotLight[0] = SpotLight();
	}

	if (m_CurrMode == ShowMode::Test1)
	{

		static float cubePhi = 0.0f, cubeTheta = 0.0f, cubemid = 1.0f;
		// 获取鼠标状态
		Mouse::State mouseState = m_pMouse->GetState();
		Mouse::State lastMouseState = m_MouseTracker.GetLastState();
		// 获取键盘状态
		Keyboard::State keyState = m_pKeyboard->GetState();
		Keyboard::State lastKeyState = m_KeyboardTracker.GetLastState();

		// 更新鼠标按钮状态跟踪器，仅当鼠标按住的情况下才进行移动
		m_MouseTracker.Update(mouseState);
		m_KeyboardTracker.Update(keyState);


		if (mouseState.rightButton == true && m_MouseTracker.rightButton == m_MouseTracker.PRESSED) //&& AutoRotation==falseRELEASED
		{
			if (AutoRotation == true)
				AutoRotation = false;
			else
				AutoRotation = true;
		}//当鼠标左键长按时可进行绕Y轴或X轴的旋转
		if (AutoRotation == false)
		{
			cubePhi += 0.0005f, cubeTheta += 0.00075f;
			XMMATRIX W = XMMatrixRotationX(cubePhi) * XMMatrixRotationY(cubeTheta);
			m_VSConstantBuffer1.world = XMMatrixTranspose(W);
			m_VSConstantBuffer1.worldInvTranspose = XMMatrixTranspose(InverseTranspose(W));
			//AutoRotation = true;
		}
		else
		{
			cubePhi += 0.0f, cubeTheta += 0.00f;
			XMMATRIX W = XMMatrixRotationX(cubePhi) * XMMatrixRotationY(cubeTheta);
			m_VSConstantBuffer1.world = XMMatrixTranspose(W);
			m_VSConstantBuffer1.worldInvTranspose = XMMatrixTranspose(InverseTranspose(W));
		}

		if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
		{
			cubeTheta -= (mouseState.x - lastMouseState.x) * 0.01f;//鼠标使用像素单位
			cubePhi -= (mouseState.y - lastMouseState.y) * 0.01f;//鼠标使用像素单位

		}

		//滑动滚轮进行放大或缩小
		cubemid += (mouseState.scrollWheelValue - lastMouseState.scrollWheelValue) * 0.001f;//鼠标使用像素单位

		////按W/S/A/D四个键可进行绕Y轴或X轴的旋转
		//if (keyState.IsKeyDown(Keyboard::W))
		//	cubePhi += dt * 2;
		//if (keyState.IsKeyDown(Keyboard::S))
		//	cubePhi -= dt * 2;
		//if (keyState.IsKeyDown(Keyboard::A))
		//	cubeTheta += dt * 2;
		//if (keyState.IsKeyDown(Keyboard::D))
		//	cubeTheta -= dt * 2;

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
			cubemid += dt * 1;
		if (keyState.IsKeyDown(Keyboard::O))
			cubemid -= dt * 1;

		//矩阵转置
		//对几何体的变换顺序通常为先缩放，后旋转，再平移
		//XMMatrixTranspose()中参数为变换的复合，比如现在的参数为先旋转Y轴再旋转X轴
		m_VSConstantBuffer1.world = XMMatrixTranspose(XMMatrixRotationY(cubeTheta) * XMMatrixRotationX(cubePhi) * XMMatrixScaling(cubemid, cubemid, cubemid));
		//XMMatrixRotationX，返回旋转矩阵，参数：围绕x轴旋转的角度，以弧度表示。当沿着旋转轴向原点看时，角度是顺时针测量的
		//XMMatrixRotationY，与XMMatrixRotationX一样，只不过是返回y轴

		m_VSConstantBuffer1.worldInvTranspose = XMMatrixTranspose(InverseTranspose(XMMatrixRotationY(cubeTheta) * XMMatrixRotationX(cubePhi) * XMMatrixScaling(cubemid, cubemid, cubemid)));

		m_VSConstantBuffer1.RotationMatrix = XMMatrixIdentity();

		// 更新常量缓冲区，让立方体转起来
		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer1, sizeof(VSConstantBuffer));
		m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

		HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
		m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);

	}
	else if (m_CurrMode == ShowMode::Test2)
	{
		// 用于限制在1秒60帧
		static float totDeltaTime = 0;

		totDeltaTime += dt;
		if (totDeltaTime > 1.0f / 60)
		{
			totDeltaTime -= 1.0f / 60;
			m_CurrFrame = (m_CurrFrame + 1) % 120;
			m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pFireAnims[m_CurrFrame].GetAddressOf());
		}
	}
	else if (m_CurrMode == ShowMode::Test3)
	{

		static float cubePhi = 0.0f, cubeTheta = 0.0f, cubemid = 1.0f;
		// 获取鼠标状态
		Mouse::State mouseState = m_pMouse->GetState();
		Mouse::State lastMouseState = m_MouseTracker.GetLastState();
		// 获取键盘状态
		Keyboard::State keyState = m_pKeyboard->GetState();
		Keyboard::State lastKeyState = m_KeyboardTracker.GetLastState();

		// 更新鼠标按钮状态跟踪器，仅当鼠标按住的情况下才进行移动
		m_MouseTracker.Update(mouseState);
		m_KeyboardTracker.Update(keyState);


		if (mouseState.rightButton == true && m_MouseTracker.rightButton == m_MouseTracker.PRESSED) //&& AutoRotation==falseRELEASED
		{
			if (AutoRotation == true)
				AutoRotation = false;
			else
				AutoRotation = true;
		}//当鼠标左键长按时可进行绕Y轴或X轴的旋转
		if (AutoRotation == false)
		{
			cubePhi += 0.0005f, cubeTheta += 0.00075f;
			XMMATRIX W = XMMatrixRotationX(cubePhi) * XMMatrixRotationY(cubeTheta);
			m_VSConstantBuffer1.world = XMMatrixTranspose(W);
			m_VSConstantBuffer1.worldInvTranspose = XMMatrixTranspose(InverseTranspose(W));
			//AutoRotation = true;
		}
		else
		{
			cubePhi += 0.0f, cubeTheta += 0.00f;
			XMMATRIX W = XMMatrixRotationX(cubePhi) * XMMatrixRotationY(cubeTheta);
			m_VSConstantBuffer1.world = XMMatrixTranspose(W);
			m_VSConstantBuffer1.worldInvTranspose = XMMatrixTranspose(InverseTranspose(W));
		}

		if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
		{
			cubeTheta -= (mouseState.x - lastMouseState.x) * 0.01f;//鼠标使用像素单位
			cubePhi -= (mouseState.y - lastMouseState.y) * 0.01f;//鼠标使用像素单位

		}

		//滑动滚轮进行放大或缩小
		cubemid += (mouseState.scrollWheelValue - lastMouseState.scrollWheelValue) * 0.001f;//鼠标使用像素单位

		////按W/S/A/D四个键可进行绕Y轴或X轴的旋转
		//if (keyState.IsKeyDown(Keyboard::W))
		//	cubePhi += dt * 2;
		//if (keyState.IsKeyDown(Keyboard::S))
		//	cubePhi -= dt * 2;
		//if (keyState.IsKeyDown(Keyboard::A))
		//	cubeTheta += dt * 2;
		//if (keyState.IsKeyDown(Keyboard::D))
		//	cubeTheta -= dt * 2;

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
			cubemid += dt * 1;
		if (keyState.IsKeyDown(Keyboard::O))
			cubemid -= dt * 1;

		//矩阵转置
		//对几何体的变换顺序通常为先缩放，后旋转，再平移
		//XMMatrixTranspose()中参数为变换的复合，比如现在的参数为先旋转Y轴再旋转X轴
		m_VSConstantBuffer3.world = XMMatrixTranspose(XMMatrixRotationY(cubeTheta) * XMMatrixRotationX(cubePhi) * XMMatrixScaling(cubemid, cubemid, cubemid));
		//XMMatrixRotationX，返回旋转矩阵，参数：围绕x轴旋转的角度，以弧度表示。当沿着旋转轴向原点看时，角度是顺时针测量的
		//XMMatrixRotationY，与XMMatrixRotationX一样，只不过是返回y轴
		m_VSConstantBuffer3.worldInvTranspose = XMMatrixTranspose(InverseTranspose(XMMatrixRotationY(cubeTheta) * XMMatrixRotationX(cubePhi) * XMMatrixScaling(cubemid, cubemid, cubemid)));

		static float phi2 = 0.0f;
		phi2 -= 0.01f;
		XMMATRIX texMat = XMMatrixTranslation(-0.5f, -0.5f, 0.0f) * XMMatrixRotationZ(phi2) * XMMatrixTranslation(0.5f, 0.5f, 0.0f);
		m_VSConstantBuffer3.RotationMatrix = XMMatrixTranspose(texMat);





		// 更新常量缓冲区，让立方体转起来
		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer3, sizeof(VSConstantBuffer));
		m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

		HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
		m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);
	}

	float fontsize = 20.0f;//字体大小
	//左上角文字
	ImGui::GetForegroundDrawList()->AddText(m_font, fontsize, ImVec2(m_ClientWidth - m_ClientWidth, m_ClientHeight - m_ClientHeight), ImColor(255, 255, 255, 255), "切换显示：1-六个不同纹理 2-火焰 3-旋转纹理");
	ImGui::GetForegroundDrawList()->AddText(m_font, fontsize, ImVec2(0, m_ClientWidth - m_ClientWidth + 1 * fontsize), ImColor(255, 255, 255, 255), "切换灯光：Q:方向光 W:点光 E:聚光灯 R:关灯");
	ImGui::GetForegroundDrawList()->AddText(m_font, fontsize, ImVec2(0, m_ClientWidth - m_ClientWidth + 2 * fontsize), ImColor(255, 255, 255, 255), "移动：UP/DOWN/LEFT/RIGHT/长按鼠标左键");
	ImGui::GetForegroundDrawList()->AddText(m_font, fontsize, ImVec2(0, m_ClientWidth - m_ClientWidth + 3 * fontsize), ImColor(255, 255, 255, 255), "放缩：I/U/滑轮");
	ImGui::GetForegroundDrawList()->AddText(m_font, fontsize, ImVec2(0, m_ClientWidth - m_ClientWidth + 4 * fontsize), ImColor(255, 255, 255, 255), "是否自动旋转-按击鼠标右键");
	//右上角文字
	ImGui::GetForegroundDrawList()->AddText(m_font, fontsize, ImVec2(m_ClientWidth-15*fontsize, m_ClientHeight - m_ClientHeight), ImColor(255, 255, 255, 255), "2022QG工作室图形组第3次小组培训");
	ImGui::GetForegroundDrawList()->AddText(m_font, fontsize, ImVec2(m_ClientWidth - 10 * fontsize, m_ClientWidth - m_ClientWidth + 1 * fontsize), ImColor(255, 255, 255, 255), "梁德麟 2022年4月7日");
}
void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&Colors::Pink));

	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// 绘制几何模型
	if (m_CurrMode == ShowMode::Test1)
	{
		for (int i = 0; i < m_IndexCount; i += 3)
		{
			if (i == 0)
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureOne.GetAddressOf());
			else if (i == 6)
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureTwo.GetAddressOf());
			else if (i == 12)
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureThree.GetAddressOf());
			else if (i == 18)
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureFour.GetAddressOf());
			else if (i == 24)
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureFive.GetAddressOf());
			else if (i == 30)
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureSix.GetAddressOf());
			m_pd3dImmediateContext->DrawIndexed(3, i, 0);
		}
	}
	else //if(m_CurrMode == ShowMode::Test3)
		m_pd3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	HR(m_pSwapChain->Present(0, 0));
}


bool GameApp::InitEffect()
{
	ComPtr<ID3DBlob> blob;

	// 创建顶点着色器(2D)
	HR(CreateShaderFromFile(L"HLSL\\Third_2D_VS.cso", L"HLSL\\Third_2D_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader2D.GetAddressOf()));
	// 创建顶点布局(2D)
	HR(m_pd3dDevice->CreateInputLayout(VertexPosTex::inputLayout, ARRAYSIZE(VertexPosTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout2D.GetAddressOf()));

	// 创建像素着色器(2D)
	HR(CreateShaderFromFile(L"HLSL\\Third_2D_PS.cso", L"HLSL\\Third_2D_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader2D.GetAddressOf()));

	// 创建顶点着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Third_3D_VS.cso", L"HLSL\\Third_3D_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader3D.GetAddressOf()));
	// 创建顶点布局(3D)
	HR(m_pd3dDevice->CreateInputLayout(VertexPosNormalTex::inputLayout, ARRAYSIZE(VertexPosNormalTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout3D.GetAddressOf()));

	// 创建像素着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Third_3D_PS.cso", L"HLSL\\Third_3D_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader3D.GetAddressOf()));

	return true;
}

bool GameApp::InitResource()
{
	// 初始化网格模型并设置到输入装配阶段
	auto meshData = Geometry::CreateBox();
	ResetMesh(meshData);



	// ******************
	// 初始化纹理和采样器状态
	//

	// 初始化旋转纹理
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\flareoutputB.dds", nullptr, m_pTextureInputA.GetAddressOf()));
	//获取六个不同的纹理
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\brick.dds", nullptr, m_pTextureOne.GetAddressOf()));
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\checkboard.dds", nullptr, m_pTextureTwo.GetAddressOf()));
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\WireFence.dds", nullptr, m_pTextureThree.GetAddressOf()));
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\floor.dds", nullptr, m_pTextureFour.GetAddressOf()));
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\ice.dds", nullptr, m_pTextureFive.GetAddressOf()));
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\water.dds", nullptr, m_pTextureSix.GetAddressOf()));


	// 初始化火焰纹理
	WCHAR strFile[40];
	m_pFireAnims.resize(120);
	for (int i = 1; i <= 120; ++i)
	{
		wsprintf(strFile, L"..\\Texture\\FireAnim\\Fire%03d.bmp", i);
		HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), strFile, nullptr, m_pFireAnims[static_cast<size_t>(i) - 1].GetAddressOf()));
	}


	// 
	// ******************
	// 初始化默认光照
	// 方向光
	m_DirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_DirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_DirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLight.direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
	// 点光
	m_PointLight.position = XMFLOAT3(0.0f, 0.0f, -10.0f);
	m_PointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_PointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_PointLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_PointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	m_PointLight.range = 25.0f;
	// 聚光灯
	m_SpotLight.position = XMFLOAT3(0.0f, 0.0f, -5.0f);
	m_SpotLight.direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_SpotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_SpotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_SpotLight.spot = 12.0f;
	m_SpotLight.range = 10000.0f;


	// 注意不要忘记设置此处的观察位置，否则高亮部分会有问题
	m_PSConstantBuffer.eyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);
	// 使用默认平行光
	m_PSConstantBuffer.dirLight[0] = m_DirLight;

	m_PSConstantBuffer.numDirLight = 1;
	m_PSConstantBuffer.numPointLight = 1;
	m_PSConstantBuffer.numSpotLight = 1;


	// 初始化材质
	m_PSConstantBuffer.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_PSConstantBuffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PSConstantBuffer.material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 5.0f);


	// 初始化采样器状态
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(m_pd3dDevice->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf()));

	

	// ******************
// 设置常量缓冲区描述

	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(VSConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 新建用于VS和PS的常量缓冲区
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[0].GetAddressOf()));
	cbd.ByteWidth = sizeof(PSConstantBuffer);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[1].GetAddressOf()));

	// ******************
	// 初始化常量缓冲区的值
	//

	// 初始化用于VS的常量缓冲区的值
	m_VSConstantBuffer3.world = XMMatrixIdentity();			
	m_VSConstantBuffer3.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_VSConstantBuffer3.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	m_VSConstantBuffer3.worldInvTranspose = XMMatrixIdentity();
	m_VSConstantBuffer3.RotationMatrix = XMMatrixIdentity();

	m_VSConstantBuffer1.world = XMMatrixIdentity();
	m_VSConstantBuffer1.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_VSConstantBuffer1.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	m_VSConstantBuffer1.worldInvTranspose = XMMatrixIdentity();
	m_VSConstantBuffer1.RotationMatrix = XMMatrixIdentity();
	

	// 更新PS常量缓冲区资源
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);

	// ******************
	// 给渲染管线各个阶段绑定好所需资源
	// 设置图元类型，设定输入布局
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
	// 默认绑定3D着色器
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
	// VS常量缓冲区对应HLSL寄存于b0的常量缓冲区
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffers[0].GetAddressOf());
	// PS常量缓冲区对应HLSL寄存于b1的常量缓冲区
	m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
	// 像素着色阶段设置好采样器
	m_pd3dImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	//设置纹理资源
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureInputA.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureOne.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureTwo.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureThree.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureFour.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureFive.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTextureSix.GetAddressOf());

	m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
	
	// ******************
	// 设置调试对象名
	//
	D3D11SetDebugObjectName(m_pVertexLayout2D.Get(), "VertexPosTexLayout");
	D3D11SetDebugObjectName(m_pVertexLayout3D.Get(), "VertexPosNormalTexLayout");
	D3D11SetDebugObjectName(m_pConstantBuffers[0].Get(), "VSConstantBuffer");
	D3D11SetDebugObjectName(m_pConstantBuffers[1].Get(), "PSConstantBuffer");
	D3D11SetDebugObjectName(m_pVertexShader2D.Get(), "Basic_2D_VS");
	D3D11SetDebugObjectName(m_pVertexShader3D.Get(), "Basic_3D_VS");
	D3D11SetDebugObjectName(m_pPixelShader2D.Get(), "Basic_2D_PS");
	D3D11SetDebugObjectName(m_pPixelShader3D.Get(), "Basic_3D_PS");
	D3D11SetDebugObjectName(m_pSamplerState.Get(), "SSLinearWrap");

	return true;
}
template<class VertexType>
bool GameApp::ResetMesh(const Geometry::MeshData<VertexType>& meshData)
{
	// 释放旧资源
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();

	// 设置顶点缓冲区描述
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * sizeof(VertexType);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));

	// 输入装配阶段的顶点缓冲区设置
	UINT stride = sizeof(VertexType);			// 跨越字节数
	UINT offset = 0;							// 起始偏移量

	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);



	// 设置索引缓冲区描述
	m_IndexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * m_IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = meshData.indexVec.data();
	HR(m_pd3dDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
	// 输入装配阶段的索引缓冲区设置
	m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// 设置调试对象名
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
	D3D11SetDebugObjectName(m_pIndexBuffer.Get(), "IndexBuffer");

	return true;
}
