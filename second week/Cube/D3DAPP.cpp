#include"D3DAPP.h"


namespace
{
	D3DApp* g_pd3dApp = nullptr;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_pd3dApp->MsgProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance) : m_hAppInst(hInstance), m_MainWndCaption(L"DirectX11 Initialization"), m_ClientWidth(1280), m_ClientHeight(1280), m_hMainWnd(nullptr), m_AppPaused(false),
m_Minimized(false),
m_Maximized(false),
m_Resizing(false),
m_Enable4xMsaa(true),
m_4xMsaaQuality(0),
m_pDevice(nullptr),
m_pContext(nullptr),
m_pSwapChain(nullptr),
m_pDepthStencilBuffer(nullptr),
m_pRenderTargetView(nullptr),
m_pDepthStencilView(nullptr)
{
	ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));
	g_pd3dApp = this;
}

D3DApp::~D3DApp()
{
	
	if (m_pContext)
		m_pContext->ClearState();
}

HINSTANCE D3DApp::AppInst()const
{
	return m_hAppInst;
}

HWND D3DApp::MainWnd()const
{
	return m_hMainWnd;
}

float D3DApp::AspectRatio()const
{
	return (float)(m_ClientWidth) / m_ClientHeight;
}

int D3DApp::Run()
{
	MSG msg = { 0 };

	m_Timer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//������Ϣ
			TranslateMessage(&msg);
		
			//������Ϣ
			DispatchMessage(&msg);
		}
		else
		{
			//����ʱ����
			m_Timer.Tick();

			if (!m_AppPaused)
			{
				//�ڷǿͻ�������֡�ʼ�������ʱ
				CalculateFrameStats();

				//���»���
				UpdateScene(m_Timer.DeltaTime());

				//���ƻ���
				DrawScene();
			}
			else
			{
				//�ȴ�һ��ʱ�䣬��λΪ���룬��ǰΪ100����
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}

void D3DApp::OnResize()
{
	//-------------�����󻺳���-->��ȡ2D��ͼ-->������ȾĿ����ͼ
	//-------------�������ģ������-->��ȡ����-->�������ģ����ͼ
	//-------------��ȾĿ����ͼ+���ģ����ͼ+�ӿ�����->�豸������
	assert(m_pContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	if (m_pDevice1 != nullptr)
	{
		assert(m_pContext1);
		assert(m_pDevice1);
		assert(m_pSwapChain1);
	}

	//��2D��ͼ����ȾĿ����ͼ����
	m_pRenderTargetView.Reset();
	m_pDepthStencilView.Reset();
	m_pDepthStencilBuffer.Reset();

	ComPtr<ID3D11Texture2D> backBuffer;
	//����2D����
	m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	//��ȡ2D����
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	//������ȾĿ����ͼ
	m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf());

	//����backbuffer�������������ģ��2D����
	backBuffer.Reset();

	//�������ģ��2D����
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

	//�������ģ��2D����
	m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf());
	//�������ģ����ͼ
	m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf());
	
	//�ӿڵ�����
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
	m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;
	//�����ӿ�
	m_pContext->RSSetViewports(1, &m_ScreenViewport);
	//������ȾĿ����ͼ
	m_pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

}

LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)//wParamΪ���°�����������룬lParam�洢�������״̬��Ϣ���������Ϣ����ʱ��lParam�洢�������꣬���ֽ�HIWORD����y���꣬���ֽ� LOWORD����x����
	{
		
		//�����ڱ������ʧȥ����״̬
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)// WA_INACTIVE:ȡ�����ڼ���
		{
			m_AppPaused = true;
			m_Timer.Stop();//��ͣ�������ۼ�ʱ��
		}
		else
		{
			m_AppPaused = false;
			m_Timer.Start();//��ʼ������ʼ�ۼ�ʱ��
		}
		return 0;

		// �ı䴰�ڵĴ�С
	case WM_SIZE:
		m_ClientWidth = LOWORD(lParam);//����x����
		m_ClientHeight = HIWORD(lParam);//����y����
		if (m_pDevice)
		{
			if (wParam == SIZE_MINIMIZED)//���㵽��С��
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)//���㵽���
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_Maximized = true;
				OnResize();//������С
			}
			else if (wParam == SIZE_RESTORED)//���㵽��ԭ�������С�����ڵ��Ǹ����
			{

				if (m_Minimized)//���ԭ���Ĵ���״̬����С��
				{
					m_AppPaused = false;
					m_Minimized = false;
					OnResize();//������С
				}

				else if (m_Maximized)//���ԭ���Ĵ���״̬�����
				{
					m_AppPaused = false;
					m_Maximized = false;
					OnResize();//������С
				}
				else if (m_Resizing)
				{
					
				}
				else 
				{
					OnResize();//������С
				}
			}
		}
		return 0;

		//���ڽ��� �ƶ�/�ı��С ģʽѭ��
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing = true;
		m_Timer.Stop();//��ͣ�������ۼ�ʱ��
		return 0;

		//�����˳� �ƶ�/�ı��С ģʽѭ��
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing = false;
		m_Timer.Start();//��ʼ������ʼ�ۼ�ʱ��
		OnResize();//������С
		return 0;

		//��������
	case WM_DESTROY:
		PostQuitMessage(0);//�˳������ٵ�ǰ����
		return 0;
		//�˵��������ұ�����ĳ����
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

		//���ڸı� ��С��λ��
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_INPUT:

		//�������
	case WM_LBUTTONDOWN:
		//���»��ּ�
	case WM_MBUTTONDOWN:
		//�����Ҽ�
	case WM_RBUTTONDOWN:

	case WM_XBUTTONDOWN:

		//�ͷ����
	case WM_LBUTTONUP:
		//�ͷŻ��ּ�
	case WM_MBUTTONUP:
		//�ͷ��Ҽ�
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:
		//�����ֹ���ʱ
	case WM_MOUSEWHEEL:
		
	case WM_MOUSEHOVER:
		//������ƶ�ʱ
	case WM_MOUSEMOVE:
		m_pMouse->ProcessMessage(msg, wParam, lParam);//��������Ϣ�Ĵ���
		return 0;
		//���·�ϵͳ��
	case WM_KEYDOWN:
		//����alt+��������
	case WM_SYSKEYDOWN:
		//�ͷŷ�ϵͳ��
	case WM_KEYUP:
		//�ͷ�alt+��������
	case WM_SYSKEYUP:
		m_pKeyboard->ProcessMessage(msg, wParam, lParam);//��������Ϣ�Ĵ���
		return 0;
		//���ͼ���򱻼�����Ϣ
	case WM_ACTIVATEAPP:
		m_pMouse->ProcessMessage(msg, wParam, lParam);//��������Ϣ�Ĵ���
		m_pKeyboard->ProcessMessage(msg, wParam, lParam);//��������Ϣ�Ĵ���
		return 0;
	}

	//����Ĭ�ϴ�����
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


bool D3DApp::InitMainWindow()
{
	//����������
	WNDCLASS wnd;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = MainWndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = m_hAppInst;
	wnd.hIcon = LoadIcon(0, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wnd.lpszMenuName = 0;
	wnd.lpszClassName = L"MY";

	//ע�ᴰ��
	if (!RegisterClass(&wnd))
	{
		MessageBox(0, L"RegisterClass Failed.", L"WARNING !", 0);
		return false;
	}

	//�����ӿ�
	RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	//��������
	m_hMainWnd = CreateWindow(L"MY", m_MainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, 0);
	if (!m_hMainWnd)
	{
		MessageBox(0, L"Create Window Failed !", L"WARNING !", 0);
		return false;
	}

	//չʾ�͸��´���
	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);

	return true;
}


bool D3DApp::InitDirect3D()
{
	//----------�����ȼ�+��������-->�����豸��������-->ȡ������-->ȡ�����ӿڣ�factory��-->����������������
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//�����ȼ�
	D3D_FEATURE_LEVEL FeatureLevel;
	D3D_FEATURE_LEVEL FeatureLevelArray[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT FeatureLevelLength = ARRAYSIZE(FeatureLevelArray);

	//��������
	D3D_DRIVER_TYPE DriverType;
	D3D_DRIVER_TYPE DriverTypeArray[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT DriverTypeLength = ARRAYSIZE(DriverTypeArray);

	//���������ȼ����������ʹ����豸��������
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

	//�����豸��������ʧ��
	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	//�����ȼ���֧��
	if (FeatureLevel != D3D_FEATURE_LEVEL_11_0 && FeatureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	//���4������������
	m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
	assert(m_4xMsaaQuality > 0);

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	
	ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	

	//ȡ�豸�ӿ�
	m_pDevice.As(&dxgiDevice);
	//��ȡ������
	dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
	//��ȡfactory1��factory2�ӿ�----���Դ���������
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf()));

	hr = dxgiFactory1.As(&dxgiFactory2);
	//factory2�ӿ�Ҫ���������ṹ��������������
	if (dxgiFactory2 != nullptr)
	{
		m_pDevice.As(&m_pDevice1);
		m_pContext.As(&m_pContext1);
		//����������
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
		//������ȫ������
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd;
		fd.RefreshRate.Numerator = 60;
		fd.RefreshRate.Denominator = 1;
		fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fd.Windowed = TRUE;
		//��������ṹ������������������
		dxgiFactory2->CreateSwapChainForHwnd(m_pDevice.Get(), m_hMainWnd, &sd, &fd, nullptr, m_pSwapChain1.GetAddressOf());
		m_pSwapChain1.As(&m_pSwapChain);
	}
	//factory1�ӿ�Ҫ����һ���ṹ��������������
	else
	{
		//����������
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
		//����������
		dxgiFactory1->CreateSwapChain(m_pDevice.Get(), &sd, m_pSwapChain.GetAddressOf());
	}
	//�������ھ��������ϵ
	dxgiFactory1->MakeWindowAssociation(m_hMainWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

	OnResize();

	return true;
}
bool D3DApp::Init()
{
	m_pMouse = std::make_unique<DirectX::Mouse>();

	m_pKeyboard = std::make_unique<DirectX::Keyboard>();

	if (!InitMainWindow())
		return false;

	if (!InitDirect3D())
		return false;

	return true;
}
void D3DApp::CalculateFrameStats()
{
	
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((m_Timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; 
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << m_MainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(m_hMainWnd, outs.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}