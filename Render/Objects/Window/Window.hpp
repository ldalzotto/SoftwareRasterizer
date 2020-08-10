#pragma once

#include "Functional/Callback/ObserverT.hpp"
#include "Functional/Optional/OptionalT.hpp"

#include "v2/Matrix/Matrix.hpp"

#include <vector>
#include <string>

#ifdef _WIN32
#include "Include/PlatformInclude.hpp"
namespace _RenderV2
{
	struct WindowHandle
	{
		HWND Window;
	};

	template <int C, typename T>
	struct Texture;
}
#endif

namespace _RenderV2
{
	struct WindowSize
	{
		uint32_t Width;
		uint32_t Height;
	};

	struct WindowState
	{
		bool AskedForClosed;
		_Core::OptionalT<WindowSize> HasResizedThisFrame;
		Texture<3, char>* PendingPresentingTexture;
	};

	extern const uint32_t WINDOW_WIDTH;
	extern const uint32_t WINDOW_HEIGHT;

	struct Window
	{
		WindowHandle Handle;
		WindowSize WindowSize;

		WindowState WindowState;

		_MathV2::Matrix3x3<float> WindowToGraphicsAPIPixelCoordinates;
		_MathV2::Matrix4x4<float> GraphicsAPIToWindowPixelCoordinates;

		_Core::ObserverT<void> OnWindowSizeChanged;
	};

	void Window_init(Window* p_window);
	void Window_closeWindow(Window* p_window);
	bool Window_askedForClose(Window* p_window);
	bool Window_consumeSizeChangeEvent(Window* p_window);

	WindowSize Window_getSize(Window* p_window);
	void Window_presentTexture(Window* p_window, Texture<3, char>* p_texture);
}