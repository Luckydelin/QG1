



#  Directx11--窗口初始化（win32）

# 一.入口函数: int WINAPI WinMain

```cpp
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevhInstance,
 LPSTR cmdLine, int showCmd)
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

1.第一个参数 hinstance：实例句柄

2.第二个参数 prevhInstance:先前实例句柄

3.第三个参数 cmdLine:命令行（为远指针）

4.第四个参数 showCmd:指定窗口如何显示

| 参数               | 值   | 意义                                                         |
| ------------------ | ---- | ------------------------------------------------------------ |
| SW_HIDE            | 0    | 隐藏此窗口并激活另一个窗口                                   |
| SW_MAXIMIZE        | 3    | 最大化指定的窗口                                             |
| SW_MINIMIZE        | 6    | 最小化指定的窗口并激活当前次序中顶部的窗口                   |
| SW_RESTORE         | 9    | 激活并显示此窗口，如果窗口已经最小化或最大化，系统会恢复它到原始的尺寸和位置，一个应用程序应该在恢复最小化的窗口时指定此SW_RESTORE标识 |
| SW_SHOW            | 5    | 以当前的尺寸和位置激活与显示指定窗口                         |
| SW_SHOWMAXIMIZED   | 3    | 最大化激活并显示这个窗口                                     |
| SW_SHOWMINIMIZED   | 2    | 最小化激活并显示这个窗口                                     |
| SW_SHOWMINNOACTIVE | 7    | 最小化显示这个窗口，与SW_SHOWMINIMIZED唯一的区别是不会去激活指定的窗口 |
| SW_SHOWNA          | 8    | 以当前的尺寸和位置激活与显示指定窗口，与SW_SHOW的唯一区别是不会去激活指定的窗口 |
| SW_SHOWNOACTIVATE  | 4    | 显示一个窗口，若指定的窗口是最小化或者最大化的，系统将恢复其到原始尺寸和位置，与SW_SHOWNORMAL的唯一区别是不会去激活指定的窗口 |
| SW_SHOWNORMAL      | 1    | 激活并显示一个窗口，若指定的窗口是最小化或者最大化的，系统将恢复其到原始尺寸和位置。一个应用程序应该在第一次显示窗口的时候指定这个标识 |







































# 二.初始化窗口类

```cpp
    WNDCLASS wnd;
	wnd.style = CS_HREDRAW | CS_VREDRAW;//窗口风格
	wnd.lpfnWndProc = WndProc;//窗口回调
	wnd.cbClsExtra = 0;//窗口类附加内存
	wnd.cbWndExtra = 0;//窗口附加内存
	wnd.hInstance = m_hAppInst;//实例句柄
	wnd.hIcon = LoadIcon(0, IDI_APPLICATION);//图标句柄
	wnd.hCursor = LoadCursor(0, IDC_ARROW);//光标句柄--此时为默认箭头光标
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景画刷句柄--（HBRUSH）为强制类型转换
	wnd.lpszMenuName = 0;//菜单名字
	wnd.lpszClassName = L"MY";//窗口类名
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

1.LoadIcon函数：

​    第一个参数：设置为null时则默认使用系统图标

备注：若参数不为空可查看msdn官方函数文档使用其他定义https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadacceleratorsa

​    第二个参数：图标的名字（为远指针）

# 三.注册窗口类

```cpp
    if (!RegisterClass(&wc))
	{
		MessageBox(0, L"Register Windows Failed !", L"Warning !", 0);
		return false;
	}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

​    传入窗口类地址即可 

1.MessageBox函数：

函数原型：

```cpp
int WINAPI MessageBox(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCTSTR lpText,
  _In_opt_  LPCTSTR lpCaption,
  _In_      UINT uType
);
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

​    第一个参数 hwnd：父窗口句柄

​    第二个参数 lptext：框内文本

​    第三个参数 lpcaption:消息框标题

​    第四个参数 uType:按钮及图标

按钮：

| **按钮**            | **含义**                         |
| ------------------- | -------------------------------- |
| MB_OK               | 默认值，有一个“确认”按钮         |
| MB_YESNO            | 有“是”和“否”两个按钮             |
| MB_ABORTRETRYIGNORE | 有“中止”，“重试”和“跳过”三个按钮 |
| MB_YESNOCANCEL      | 有“是”，“否”和“取消”三个按钮     |
| MB_RETRYCANCEL      | 有“重试”和“取消”两个按钮         |
| MB_OKCANCEL         | 有“确定”和“取消”两个按钮         |

图标： 

| **图标**           | **含义**                              |
| ------------------ | ------------------------------------- |
| MB_ICONEXCLAMATION | 一个惊叹号                            |
| MB_ICONWARNING     | 一个惊叹号（同上）                    |
| MB_ICONINFORMATION | 一个圆圈中小写字母i组成的图标         |
| MB_ICONASTERISK    | 一个圆圈中小写字母i组成的图标（同上） |
| MB_ICONQUESTION    | 一个问题标记图标                      |
| MB_ICONSTOP        | 一个停止消息图标                      |
| MB_ICONERROR       | 一个停止消息图标（同上）              |
| MB_ICONHAND        | 一个停止消息图标（同上）              |

# 四.初始化视口大小

```cpp
    RECT R = { 0, 0, Width,Height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 1.RECT类：

RECT类原型：

```cpp
typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT, NEAR *NPRECT, FAR *LPRECT;
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

​    参数1：设置窗口左边位置

​    参数2：设置窗口顶部位置

​    参数3：设置窗口右边位置

​    参数4：设置窗口底部位置

2.AdjustWindowRect函数：

​    作用：调整窗口客户区的大小

​    参数1：传入RECT类变量，设置视口大小

​    参数2：调整窗口风格

​    参数3：该窗口是否有菜单



# 五.创建窗口

```cpp
  HWND hWnd = CreateWindow(L"MY", L"Rendering a hexagon",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,width,height, 0, 0, hInst, 0);

	if (!hWnd)
	{
		MessageBox(0, L"Create Windows Failed.", L"Warning !", 0);
		return false;
	}
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 CreateWindow函数：

作用：创建窗口，并返回窗口句柄

​    参数1：需要创建的窗口类的名字

​    参数2：窗口标题

​    参数3：窗口风格

​    参数4：窗口的水平位置

​    参数5：窗口的竖直位置

​    参数6：窗口宽度

​    参数7：窗口高度

​    参数8：父类句柄

​    参数9：菜单句柄

​    参数10：实例句柄

​    参数11：WM_CREATE的附加信息，一般为空

# 六.显示和更新窗口

```cpp
ShowWindow(hWnd, SW_SHOW);//参数2为显示状态cmdshow
UpdateWindow(hWnd);
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

传入窗口句柄即可

# 七.消息机制

当我们在窗口上操作时，调用peekmessage函数后窗口将获取消息传递给系统，形成消息队列，将其消息调用translatemessage函数后可获得对应的翻译，最后通过dispatchmessage函数传给窗口，让窗口执行下一步的操作

getmessage函数与peekmessage函数的区别：

getmessage函数的参数与peekmessage函数的前四个是一致的，而peekmessage函数比前者多了一个参数:UINT类型的wRemoveMsg.当该参数取PM_REMOVE的时候，同时窗口没有消息的话，那么就会执行else内的内容，这就适合我们没有接受到消息的时候可以对窗口画面的绘制。换作getmessage函数的话，如果一直没有接收到消息，那么它就会一直等到有消息为止，那么我们没有对窗口操作的时候，也就是说没有消息传到窗口，那么我们的窗口画面就一直没有更新

代码示例：

```bash
MSG msg = { 0 };

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
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 DefWindowProc函数是默认的窗口处理函数，我们可以将不重要不关心的消交给它处理

 [window部分消息](https://blog.csdn.net/weixin_62773441/article/details/123724612)



# 八.程序运行

示例代码：

```cpp
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	}
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
int WINAPI WinMain(_In_ HINSTANCE hInst,_In_opt_ HINSTANCE PrehInst,_In_ LPSTR CmdLine,_In_ INT ShowCmd )
{
	WNDCLASS wnd;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = WndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = PrehInst;
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
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,width,height, 0, 0, hInst, 0);

	if (!hWnd)
	{
		MessageBox(0, L"Create Windows Failed.", L"Warning !", 0);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	MSG msg = { 0 };

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

运行结果：

 ![img](https://img-blog.csdnimg.cn/cee21a05961d4579ab7b1da2cb7655a1.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)




 