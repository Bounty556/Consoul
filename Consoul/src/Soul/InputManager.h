#pragma once

namespace Soul {

	enum Keys : unsigned char
	{
		LeftMouse		= 0x01,
		RightMouse		= 0x02,
		MiddleMouse		= 0x04,
		BackSpace		= 0x08,
		Tab				= 0x09,
		Clear			= 0x0C,
		Enter			= 0x0D,
		Alt				= 0x12,
		Escape			= 0x1B,
		Space			= 0x20,
		PageUp			= 0x21,
		PageDown		= 0x22,
		End				= 0x23,
		Home			= 0x24,
		Left			= 0x25,
		Up				= 0x26,
		Right			= 0x27,
		Down			= 0x28,
		Insert			= 0x2D,
		Delete			= 0x2E,
		Zero			= 0x30,
		One				= 0x31,		
		Two				= 0x32,
		Three			= 0x33,
		Four			= 0x34,
		Five			= 0x35,
		Six				= 0x36,
		Seven			= 0x37,
		Eight			= 0x38,
		Nine			= 0x39,
		A				= 0x41,
		B				= 0x42,
		C				= 0x43,
		D				= 0x44,
		E				= 0x45,
		F				= 0x46,
		G				= 0x47,
		H				= 0x48,
		I				= 0x49,
		J				= 0x4A,
		K				= 0x4B,
		L				= 0x4C,
		M				= 0x4D,
		N				= 0x4E,
		O				= 0x4F,
		P				= 0x50,
		Q				= 0x51,
		R				= 0x52,
		S				= 0x53,
		T				= 0x54,
		U				= 0x55,
		V				= 0x56,
		W				= 0x57,
		X				= 0x58,
		Y				= 0x59,
		Z				= 0x5A,
		Numpad0			= 0x60,
		Numpad1			= 0x61,
		Numpad2			= 0x62,
		Numpad3			= 0x63,
		Numpad4			= 0x64,
		Numpad5			= 0x65,
		Numpad6			= 0x66,
		Numpad7			= 0x67,
		Numpad8			= 0x68,
		Numpad9			= 0x69,
		Multiply		= 0x6A,
		Add				= 0x6B,
		Subtract		= 0x6D,
		Decimal			= 0x6E,
		Divide			= 0x6F,
		F1				= 0x70,
		F2				= 0x71,
		F3				= 0x72,
		F4				= 0x73,
		F5				= 0x74,
		F6				= 0x75,
		F7				= 0x76,
		F8				= 0x77,
		F9				= 0x78,
		F10				= 0x79,
		F11				= 0x7A,
		F12				= 0x7B,
		F13				= 0x7C,
		F14				= 0x7D,
		F15				= 0x7E,
		F16				= 0x7F,
		F17				= 0x80,
		F18				= 0x81,
		F19				= 0x82,
		F20				= 0x83,
		F21				= 0x84,
		F22				= 0x85,
		F23				= 0x86,
		F24				= 0x87,
		LeftShift		= 0xA0,
		RightShift		= 0xA1,
		LeftControl		= 0xA2,
		RightControl	= 0xA3,
		Colon			= 0xBA,
		Plus			= 0xBB,
		Comma			= 0xBC,
		Minus			= 0xBD,
		Period			= 0xBE,
		ForwardSlash	= 0xBF,
		Tilde			= 0xC0,
		OpenBracket		= 0xDB,
		Pipe			= 0xDC,
		CloseBracket	= 0xDD,
		Quote			= 0xDE,
		BackSlash		= 0xE2
	};

	class InputManager
	{
		enum State : unsigned char
		{
			None = 0,
			Released = 1 << 0,
			Pressed = 1 << 1,
			Down = 1 << 2
		};

		InputManager() = delete;
		~InputManager() = delete;
	public:
		// Keys that 
		static void AddKey(Keys key);
		static void AddNoteKey(Keys key);
		static bool IsKeyDown(Keys key);
		static bool WasKeyPressed(Keys key);
		static bool WasKeyReleased(Keys key);
		static bool IsInputChangeDetected();
		static void UpdateStates();
		static short GetKeysDown();

	private:
		static std::map<Keys, State> m_KeyStates; // All keys used
		static std::vector<Keys> m_NoteKeys; // All note Keys
		static bool m_InputDetected;
	};
}