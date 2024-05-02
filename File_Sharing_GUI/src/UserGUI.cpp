#include "UserGUI.h"

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




