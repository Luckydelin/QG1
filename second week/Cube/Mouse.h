#pragma once

#include <windows.h>
#include <memory>

namespace DirectX
{
	class Mouse
	{
	public:
		Mouse() noexcept(false);
		Mouse(Mouse&& moveFrom) noexcept;
		Mouse& operator= (Mouse&& moveFrom) noexcept;

		Mouse(Mouse const&) = delete;
		Mouse& operator=(Mouse const&) = delete;

		virtual ~Mouse();

		//鼠标模式
		enum Mode
		{
			MODE_ABSOLUTE = 0,// 绝对坐标模式，每次状态更新xy值为屏幕像素坐标，且鼠标可见    
			MODE_RELATIVE,// 相对运动模式，每次状态更新xy值为每一帧之间的像素位移量，且鼠标不可见
		};

		struct State
		{
			bool    leftButton;//按下左键
			bool    middleButton;//按下中键，即滑轮键
			bool    rightButton;//按下右键
			bool    xButton1;//忽略
			bool    xButton2;//忽略
			int     x;//绝对/相对坐标x
			int     y;//绝对/相对坐标y
			int     scrollWheelValue;//滑轮累积值
			Mode    positionMode;//鼠标模式
		};

		//鼠标追踪类
		class ButtonStateTracker
		{
		public:
			//鼠标状态枚举
			enum ButtonState
			{
				UP = 0,         // 按钮未被按下
				HELD = 1,       // 按钮长按中
				RELEASED = 2,   // 按钮刚被放开
				PRESSED = 3,    // 按钮刚被按下
			};

			ButtonState leftButton;//左键状态
			ButtonState middleButton;//滑轮//中键状态
			ButtonState rightButton;//右键状态
			ButtonState xButton1;//忽略
			ButtonState xButton2;//忽略

#pragma prefast(suppress: 26495, "Reset() performs the initialization")
			ButtonStateTracker() noexcept { Reset(); }

			//更新状态
			void __cdecl Update(const State& state);

			//
			void __cdecl Reset() noexcept;

			//获取上一帧的鼠标状态 
			State __cdecl GetLastState() const { return lastState; }

		private:
			State lastState;
		};

		//获取每帧的鼠标运动状态
		State __cdecl GetState() const;

		// 清空滑轮的累计值
		void __cdecl ResetScrollWheelValue();

		// 设置鼠标模式
		void __cdecl SetMode(Mode mode);

		// 检验鼠标是否连接
		bool __cdecl IsConnected() const;

		// 检验鼠标是否可见
		bool __cdecl IsVisible() const;
		//设置鼠标是否可见
		void __cdecl SetVisible(bool visible);

#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) && defined(WM_USER)
		//绑定窗口
		void __cdecl SetWindow(HWND window);
		//处理鼠标信息
		static void __cdecl ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
#endif
		//获取mouse实例
		static Mouse& __cdecl Get();

	private:
		class Impl;

		std::unique_ptr<Impl> pImpl;
	};
}