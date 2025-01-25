#ifndef BUTTONS
#define BUTTONS

#include <Windows.h>
#include <array>

struct ButtonDesc {
	const TCHAR* name{};
};

constexpr std::array buttonsDescs = {
	ButtonDesc{TEXT("1")}, ButtonDesc{TEXT("2")},
	ButtonDesc{TEXT("3")}, ButtonDesc{TEXT("4")},
	ButtonDesc{TEXT("5")}, ButtonDesc{TEXT("6")},

	ButtonDesc{TEXT("7")}, ButtonDesc{TEXT("8")},
	ButtonDesc{TEXT("9")}, ButtonDesc{TEXT("+")},
	ButtonDesc{TEXT("-")}, ButtonDesc{TEXT("=")}
};

#endif