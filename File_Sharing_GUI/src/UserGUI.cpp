#include "UserGUI.h"
#pragma managed(push, off)
#include "CryptingLibraries.h"
#pragma managed(pop)



FileSharingGUI::UserGUI::UserGUI(array<System::String^>^ args)
{
	InitializeComponent();
	//
	//TODO: Add the constructor code here
	//
}


FileSharingGUI::UserGUI::~UserGUI()
{
	if (components)
	{
		delete components;
	}
}




