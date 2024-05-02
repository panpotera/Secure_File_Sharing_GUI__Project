#include "iostream"
#include "UserGUI.h"



[System::STAThreadAttribute]
int main(array<System::String^>^ args)
{
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	
	//std::cout << "helo WRLD!" << std::endl;

	FileSharingGUI::UserGUI form(args);
	System::Windows::Forms::Application::Run(% form);

	return 0;
}
