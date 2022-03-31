# Directx--初始化操作

## 一.建立 D3DAPP 头文件及cpp文件

![img](https://img-blog.csdnimg.cn/30e0a49aa4fa462dabc8a20700a4aeb4.png)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

文件创建后，构造D3D类并转运原来main中的代码

构造D3D类：

```cpp
#include<windows.h>
class D3DAPP
{
public:
	D3DAPP(HINSTANCE hInst);
	LRESULT MsgProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
	bool InitWnd();
	bool InitD3D();
	bool Init();

protected:
	

protected:
	HINSTANCE m_hAppInst;

};
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)



 将实例句柄赋给成员变量m_hAppInst

```cpp
D3DAPP::D3DAPP(HINSTANCE hInst) :m_hAppInst(hInst){}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

同时构建初始化函数--Init函数：

```cpp
bool D3DAPP::Init()
{
	if (!InitWnd())
		return false;
	if (!InitD3D())
		return false;
	return true;
}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 使用全局指针g_pd3dapp指针建立成员函数--MsgProc回调函数，这样就方便我们在MsgProc函数中使用类内的变量与函数，而原来的WndProc函数难以做到这点

```cpp
#include"D3DAPP.h"
namespace 
{
	D3DAPP* g_pd3dapp = nullptr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	

	return g_pd3dapp->MsgProc(hwnd, msg, wParam, lParam);
}
D3DAPP::D3DAPP(HINSTANCE hInst) :m_hAppInst(hInst) 
{
	g_pd3dapp = this;
}
LRESULT D3DAPP::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)



## 二.初始化窗口



将main中的“窗口创建”代码转至InitWnd函数中：

```cpp
bool D3DAPP::InitWnd()
{
	WNDCLASS wnd;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = WndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = m_hAppInst;
	wnd.hIcon = LoadIcon(0, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.lpszMenuName = 0;
	wnd.lpszClassName = L"MY";

	if (!RegisterClass(&wnd))
	{
		MessageBox(0, L"Register Windows Failed !", L"Warning !", 0);
		return false;
	}
	RECT R = { 0,0,640,480 };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	int width = R.right - R.left;
	int height = R.bottom - R.top;

	HWND hWnd = CreateWindow(L"MY", L"Rendering a hexagon",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0,m_hAppInst, 0);

	if (!hWnd)
	{
		MessageBox(0, L"Create Windows Failed.", L"Warning !", 0);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	return true;
}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)



并将main中的“回调函数”放到D3D.cpp中

示例代码：

​    D3DAPP头文件：

```cpp
#pragma once
#include<windows.h>
class D3DAPP
{
public:
	D3DAPP(HINSTANCE hInst);
	LRESULT MsgProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
	bool InitWnd();
	bool InitD3D();
	bool Init();

protected:
	

protected:
	HINSTANCE m_hAppInst;

};
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

​     D3DAPP.CPP

```cpp
#include"D3DAPP.h"
namespace 
{
	D3DAPP* g_pd3dapp = nullptr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	

	return g_pd3dapp->MsgProc(hwnd, msg, wParam, lParam);
}
D3DAPP::D3DAPP(HINSTANCE hInst) :m_hAppInst(hInst) 
{
	g_pd3dapp = this;
}
LRESULT D3DAPP::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
bool D3DAPP::InitWnd()
{
	WNDCLASS wnd;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = WndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = m_hAppInst;
	wnd.hIcon = LoadIcon(0, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.lpszMenuName = 0;
	wnd.lpszClassName = L"MY";

	if (!RegisterClass(&wnd))
	{
		MessageBox(0, L"Register Windows Failed !", L"Warning !", 0);
		return false;
	}
	RECT R = { 0,0,640,480 };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	int width = R.right - R.left;
	int height = R.bottom - R.top;

	HWND hWnd = CreateWindow(L"MY", L"Rendering a hexagon",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0,m_hAppInst, 0);

	if (!hWnd)
	{
		MessageBox(0, L"Create Windows Failed.", L"Warning !", 0);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	return true;
}
bool D3DAPP::InitD3D()
{
	return true;
}
bool D3DAPP::Init()
{
	if (!InitWnd())
		return false;
	if (!InitD3D())
		return false;
	return true;
}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)



​    main.cpp

```cpp
#include "D3DAPP.h"


int WINAPI WinMain(_In_ HINSTANCE hInst,_In_opt_ HINSTANCE PrehInst,_In_ LPSTR CmdLine,_In_ INT ShowCmd )
{
	
	D3DAPP first(hInst);

	first.Init();

	
	MSG msg={0};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}
	return true;
}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

效果图：

![img](https://img-blog.csdnimg.cn/3bc59eab952a4372b99ce2a3f3c8b0c3.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

## 三.初始化D3D

主流程如下：

![img](https://img-blog.csdnimg.cn/20d4bb6282c34567b1a3dcfc67e2dd48.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_13,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 创建设备及上下文：

​    根据特征等级及驱动类型创建相应的设备及上下文

```cpp
//特征等级
	D3D_FEATURE_LEVEL FeatureLevel;
	D3D_FEATURE_LEVEL FeatureLevelArray[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT FeatureLevelLength = ARRAYSIZE(FeatureLevelArray);

	//驱动类型
	D3D_DRIVER_TYPE DriverType;
	D3D_DRIVER_TYPE DriverTypeArray[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT DriverTypeLength = ARRAYSIZE(DriverTypeArray);

	//根据特征等级及驱动类型创建设备及上下文
	for (UINT DriverTypeIndex = 0; DriverTypeIndex < DriverTypeLength; DriverTypeIndex++)
	{
		DriverType = DriverTypeArray[DriverTypeIndex];
		hr = D3D11CreateDevice(nullptr, DriverType, nullptr, createDeviceFlags, FeatureLevelArray, FeatureLevelLength,
			D3D11_SDK_VERSION, m_pDevice.GetAddressOf(), &FeatureLevel, m_pContext.GetAddressOf());

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(nullptr, DriverType, nullptr, createDeviceFlags, &FeatureLevelArray[1], FeatureLevelLength - 1,
				D3D11_SDK_VERSION, m_pDevice.GetAddressOf(), &FeatureLevel, m_pContext.GetAddressOf());
		}

		if (SUCCEEDED(hr))
			break;
	}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

检查4xMASS质量：

```cpp
//检查4倍反采样质量
	m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
	assert(m_4xMsaaQuality > 0);
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

获取适配器：

```cpp
ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	
	ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	

	//取设备接口
	m_pDevice.As(&dxgiDevice);
	//获取适配器
	dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

取factory接口：

```cpp
dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf()));

	hr = dxgiFactory1.As(&dxgiFactory2);
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

factory2交换链结构的描述：

```cpp
		m_pDevice.As(&m_pDevice1);
		m_pContext.As(&m_pContext1);
		//交换链描述
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = m_ClientWidth;
		sd.Height = m_ClientHeight;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;
		if (m_Enable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}
		//交换链全屏描述
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd;
		fd.RefreshRate.Numerator = 60;
		fd.RefreshRate.Denominator = 1;
		fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fd.Windowed = TRUE;
		//结合两个结构体描述来创建交换链
		dxgiFactory2->CreateSwapChainForHwnd(m_pDevice.Get(), m_hMainWnd, &sd, &fd, nullptr, m_pSwapChain1.GetAddressOf());
		m_pSwapChain1.As(&m_pSwapChain);
	
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

factory1交换链结构的描述：

```cpp
        //描述交换链
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = m_ClientWidth;
		sd.BufferDesc.Height = m_ClientHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_hMainWnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;
		if (m_Enable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}
		//创建交换链
		dxgiFactory1->CreateSwapChain(m_pDevice.Get(), &sd, m_pSwapChain.GetAddressOf());
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

最后与主窗口句柄拉取联系，交换链创建成功：

```cpp
//与主窗口句柄建立联系
	dxgiFactory1->MakeWindowAssociation(m_hMainWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

### 三.创建视图和设置视口

主要流程如下：

![img](https://img-blog.csdnimg.cn/779a6a24251c4804bbbbb1b2d118ff52.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 ![img](https://img-blog.csdnimg.cn/ac22a661b62142fabef689fc7fdd9d44.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 获取渲染目标视图：

```cpp
ComPtr<ID3D11Texture2D> backBuffer;
	//描述2D纹理
	m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	//获取2D纹理
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	//创建渲染目标视图
	m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf());
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

描述深度模板纹理：

```cpp
//描述深度模板2D纹理
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_ClientWidth;
	depthStencilDesc.Height = m_ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	if (m_Enable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 获取深度模板视图：

```cpp
//创建深度模板2D纹理
	m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf());
	//创建深度模板视图
	m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf());
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

描述并设置视口：

```cpp
//视口的描述
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
	m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;
	//设置视口
	m_pContext->RSSetViewports(1, &m_ScreenViewport);
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 绑定设备上下文：

```cpp
m_pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

测试代码 ：

![img](https://img-blog.csdnimg.cn/32a08e818e354353936958484c66672b.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 效果图：



![img](https://img-blog.csdnimg.cn/f4a64315d9d34c3e92d962668c2baf3a.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)




 