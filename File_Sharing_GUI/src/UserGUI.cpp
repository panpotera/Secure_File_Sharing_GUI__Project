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
	FileSharingGUI::UserGUI::tabControl1->Visible = false;
	FileSharingGUI::UserGUI::lastComboBoxOption = 0;
	FileSharingGUI::UserGUI::idle = 1;
	FileSharingGUI::UserGUI::RefreshApp();
	//once text changes,

}


FileSharingGUI::UserGUI::~UserGUI()
{
	if (components)
	{
		delete components;
	}
}

System::Void FileSharingGUI::UserGUI::browseSendFileBox_Click(System::Object^ sender, System::EventArgs^ e)
{
	System::IO::Stream^ myStream;
	openFileDialog1 = gcnew OpenFileDialog;

	openFileDialog1->InitialDirectory = "c:\\";
	openFileDialog1->Filter = "All files (*.*)|*.*";
	openFileDialog1->FilterIndex = 1;
	openFileDialog1->RestoreDirectory = true;
	openFileDialog1->CheckPathExists = true;
	openFileDialog1->CheckFileExists = true;
	openFileDialog1->Multiselect = false;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		if ((myStream = openFileDialog1->OpenFile()) != nullptr)
		{
			BrowseSndFileTxtBox->Clear();
			myStream->Close();
			//save file Path to the textbox
			BrowseSndFileTxtBox->Text = System::String::Copy(openFileDialog1->FileName);
		}
		else
		{
			MessageBox::Show(this, "error4 opening file", nullptr, MessageBoxButtons::OK);
			RefreshApp();
		}
	}
	else if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::Cancel)
	{
		//do nothing, safe exit.
	}
	else
	{
		MessageBox::Show(this, "error3 opening file", nullptr, MessageBoxButtons::OK);
		RefreshApp();
	}
}


System::Void	FileSharingGUI::UserGUI::sendFileButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	//get a good IP
	System::Net::IPAddress^ tmpIP;
	try
	{
		tmpIP = System::Net::IPAddress::Parse(sendIPaddressTxtBox->Text);
	}
	catch (System::ArgumentNullException^ e1)
	{
		MessageBox::Show(this, "error4 empty IP addr", nullptr, MessageBoxButtons::OK);
		RefreshApp();
		return;
	}
	catch (System::FormatException^ e2)
	{
		MessageBox::Show(this, "error5 invalid IP addr", nullptr, MessageBoxButtons::OK);
		RefreshApp();
		return;
	}
	System::String^ ipStr = tmpIP->ToString();
	System::IntPtr ptrToNativeString = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(ipStr);
	char* nativeIPString = static_cast<char*>(ptrToNativeString.ToPointer());

	//get a good port
	int porti = Convert::ToInt32(sendPortTxtBox->Text);
	if (porti < 1)
	{
		MessageBox::Show(this, "error6 port <0", nullptr, MessageBoxButtons::OK);
		RefreshApp();
		return;
	}
	else if (porti > 32766)
	{
		MessageBox::Show(this, "error6 port > 32766", nullptr, MessageBoxButtons::OK);
		RefreshApp();
		return;
	}
	short port = Convert::ToInt16(porti);

	//File is good if its not empty, because it must have been entered usign the safe function^
	if (BrowseSndFileTxtBox->TextLength > 0)
	{
		//we good, make object of native Send class to send the message
		AliceFileCrypt alcObj(static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(BrowseSndFileTxtBox->Text).ToPointer()), nativeIPString, port);
		int FileSent = alcObj.FileSent;
		if (FileSent == 1)
		{
			sendFileStatus_label->Text = "Status:    Successfully Sent File";

		}
		else
		{
			MessageBox::Show(this, FileSent.ToString(), nullptr, MessageBoxButtons::OK);
			sendFileStatus_label->Text = "Status:    Failed to sec File";
		}
	}
	else
	{
		MessageBox::Show(this, "error 7No File to Send", nullptr, MessageBoxButtons::OK);
		RefreshApp();
		return;
	}

}

System::Void FileSharingGUI::UserGUI::RcvFileButton_Click(System::Object^ sender, System::EventArgs^ e) {
	RecvFileSatusLabel->Text = "Status:    Receiving File.";
	//get a good port
	int porti = Convert::ToInt32(recvFilePortTxtBox->Text);
	if (porti < 1)
	{
		MessageBox::Show(this, "error9 port <1", nullptr, MessageBoxButtons::OK);
		RefreshApp();
		return;
	}
	else if (porti > 32766)
	{
		MessageBox::Show(this, "error10 port > 32766", nullptr, MessageBoxButtons::OK);
		RefreshApp();
		return;
	}
	short port = Convert::ToInt16(porti);


	//make sure dir string not empty
	if (recvFileDir_txtBox->TextLength < 2)
	{
		MessageBox::Show(this, "error11 receive file path empty", nullptr, MessageBoxButtons::OK);
		RefreshApp();
		return;
	}


	//make sure file name is valid
	if (!System::String::IsNullOrEmpty(RecvFileNameTxtBox->Text) &&
		RecvFileNameTxtBox->Text->IndexOfAny(System::IO::Path::GetInvalidFileNameChars()) < 0 &&
		!System::IO::File::Exists(System::IO::Path::Combine(recvFileDir_txtBox->Text, RecvFileNameTxtBox->Text)))
	{
		//valid file name, mak the object to ty and receive the file
		BobFileCrypt bobObj(static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(recvFileDir_txtBox->Text + "/" + RecvFileNameTxtBox->Text).ToPointer()),
			nullptr,
			port);
		int FileRx = bobObj.FileReceived;
		if (FileRx == 1)
		{
			RecvFileSatusLabel->Text = "Status:    Successfully Received File";
		}
		else
		{
			MessageBox::Show(this, FileRx.ToString(), nullptr, MessageBoxButtons::OK);
			RecvFileSatusLabel->Text = "Status:    Failed to receiv File";
		}
	}
	else
	{//invalid file name
		MessageBox::Show(this, "error12 to-be-received file name invalid", nullptr, MessageBoxButtons::OK);
		RefreshApp();
		return;
	}

}


