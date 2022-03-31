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

		//���ģʽ
		enum Mode
		{
			MODE_ABSOLUTE = 0,// ��������ģʽ��ÿ��״̬����xyֵΪ��Ļ�������꣬�����ɼ�    
			MODE_RELATIVE,// ����˶�ģʽ��ÿ��״̬����xyֵΪÿһ֮֡�������λ����������겻�ɼ�
		};

		struct State
		{
			bool    leftButton;//�������
			bool    middleButton;//�����м��������ּ�
			bool    rightButton;//�����Ҽ�
			bool    xButton1;//����
			bool    xButton2;//����
			int     x;//����/�������x
			int     y;//����/�������y
			int     scrollWheelValue;//�����ۻ�ֵ
			Mode    positionMode;//���ģʽ
		};

		//���׷����
		class ButtonStateTracker
		{
		public:
			//���״̬ö��
			enum ButtonState
			{
				UP = 0,         // ��ťδ������
				HELD = 1,       // ��ť������
				RELEASED = 2,   // ��ť�ձ��ſ�
				PRESSED = 3,    // ��ť�ձ�����
			};

			ButtonState leftButton;//���״̬
			ButtonState middleButton;//����//�м�״̬
			ButtonState rightButton;//�Ҽ�״̬
			ButtonState xButton1;//����
			ButtonState xButton2;//����

#pragma prefast(suppress: 26495, "Reset() performs the initialization")
			ButtonStateTracker() noexcept { Reset(); }

			//����״̬
			void __cdecl Update(const State& state);

			//
			void __cdecl Reset() noexcept;

			//��ȡ��һ֡�����״̬ 
			State __cdecl GetLastState() const { return lastState; }

		private:
			State lastState;
		};

		//��ȡÿ֡������˶�״̬
		State __cdecl GetState() const;

		// ��ջ��ֵ��ۼ�ֵ
		void __cdecl ResetScrollWheelValue();

		// �������ģʽ
		void __cdecl SetMode(Mode mode);

		// ��������Ƿ�����
		bool __cdecl IsConnected() const;

		// ��������Ƿ�ɼ�
		bool __cdecl IsVisible() const;
		//��������Ƿ�ɼ�
		void __cdecl SetVisible(bool visible);

#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) && defined(WM_USER)
		//�󶨴���
		void __cdecl SetWindow(HWND window);
		//���������Ϣ
		static void __cdecl ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
#endif
		//��ȡmouseʵ��
		static Mouse& __cdecl Get();

	private:
		class Impl;

		std::unique_ptr<Impl> pImpl;
	};
}