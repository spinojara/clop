/////////////////////////////////////////////////////////////////////////////
//
// lutest.cpp
//
// January, 2003
//
// Rémi Coulom
//
/////////////////////////////////////////////////////////////////////////////
#include "CLUTestCUI.h"
#include <iostream>

int main() {
	CLUTestCUI lutestcui;
	lutestcui.MainLoop(std::cin, std::cout);
	return 0;
}
