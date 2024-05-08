//#pragma once

namespace FileSharingGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for UserGUI
	/// </summary>
	public ref class UserGUI : public System::Windows::Forms::Form
	{
	public:
		UserGUI(array<System::String^>^ args);

	private:
		int lastComboBoxOption;
		OpenFileDialog^ openFileDialog1;
		FolderBrowserDialog^ fbd;
	private: System::Windows::Forms::TabControl^ tabControl1;
		   int idle;
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~UserGUI();

	private: System::Windows::Forms::TabPage^ Sending;
	private: System::Windows::Forms::TabPage^ Receiving;
	private: System::Windows::Forms::ComboBox^ sndOrRcvCombBox;
	private: System::Windows::Forms::Label^ textForSRcmbBx;
	private: System::Windows::Forms::Button^ refreshButton;
	private: System::Windows::Forms::TextBox^ sendIPaddressTxtBox;

	private: System::Windows::Forms::Button^ browseSendFileBox;
	private: System::Windows::Forms::TextBox^ BrowseSndFileTxtBox;
	private: System::Windows::Forms::Label^ label_sendPort;
	private: System::Windows::Forms::Label^ label_sendIPaddr;
	private: System::Windows::Forms::Button^ sendFileButton;

	private: System::Windows::Forms::TextBox^ sendPortTxtBox;
	private: System::Windows::Forms::Label^ sendFileStatus_label;
	private: System::Windows::Forms::TextBox^ recvFilePortTxtBox;
	private: System::Windows::Forms::Button^ RcvFileButton;
	private: System::Windows::Forms::Label^ label_sendFilePath;
	private: System::Windows::Forms::Label^ RecvFileSatusLabel;
	private: System::Windows::Forms::Button^ recvFile_DirectoryBrowse_button;
	private: System::Windows::Forms::TextBox^ recvFileDir_txtBox;
	private: System::Windows::Forms::Label^ label_fileToBeReceivedName;
	private: System::Windows::Forms::TextBox^ RecvFileNameTxtBox;
	private: System::Windows::Forms::Label^ label_RecvFileDir;
	private: System::Windows::Forms::Label^ label_recvPort;





	protected:



	protected:



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->Sending = (gcnew System::Windows::Forms::TabPage());
			this->label_sendFilePath = (gcnew System::Windows::Forms::Label());
			this->sendFileStatus_label = (gcnew System::Windows::Forms::Label());
			this->label_sendPort = (gcnew System::Windows::Forms::Label());
			this->label_sendIPaddr = (gcnew System::Windows::Forms::Label());
			this->sendFileButton = (gcnew System::Windows::Forms::Button());
			this->sendPortTxtBox = (gcnew System::Windows::Forms::TextBox());
			this->sendIPaddressTxtBox = (gcnew System::Windows::Forms::TextBox());
			this->browseSendFileBox = (gcnew System::Windows::Forms::Button());
			this->BrowseSndFileTxtBox = (gcnew System::Windows::Forms::TextBox());
			this->Receiving = (gcnew System::Windows::Forms::TabPage());
			this->label_fileToBeReceivedName = (gcnew System::Windows::Forms::Label());
			this->RecvFileNameTxtBox = (gcnew System::Windows::Forms::TextBox());
			this->label_RecvFileDir = (gcnew System::Windows::Forms::Label());
			this->label_recvPort = (gcnew System::Windows::Forms::Label());
			this->RecvFileSatusLabel = (gcnew System::Windows::Forms::Label());
			this->recvFile_DirectoryBrowse_button = (gcnew System::Windows::Forms::Button());
			this->recvFileDir_txtBox = (gcnew System::Windows::Forms::TextBox());
			this->RcvFileButton = (gcnew System::Windows::Forms::Button());
			this->recvFilePortTxtBox = (gcnew System::Windows::Forms::TextBox());
			this->sndOrRcvCombBox = (gcnew System::Windows::Forms::ComboBox());
			this->textForSRcmbBx = (gcnew System::Windows::Forms::Label());
			this->refreshButton = (gcnew System::Windows::Forms::Button());
			this->tabControl1->SuspendLayout();
			this->Sending->SuspendLayout();
			this->Receiving->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->Sending);
			this->tabControl1->Controls->Add(this->Receiving);
			this->tabControl1->Enabled = false;
			this->tabControl1->Location = System::Drawing::Point(12, 102);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(822, 419);
			this->tabControl1->TabIndex = 0;
			this->tabControl1->TabStop = false;
			// 
			// Sending
			// 
			this->Sending->Controls->Add(this->label_sendFilePath);
			this->Sending->Controls->Add(this->sendFileStatus_label);
			this->Sending->Controls->Add(this->label_sendPort);
			this->Sending->Controls->Add(this->label_sendIPaddr);
			this->Sending->Controls->Add(this->sendFileButton);
			this->Sending->Controls->Add(this->sendPortTxtBox);
			this->Sending->Controls->Add(this->sendIPaddressTxtBox);
			this->Sending->Controls->Add(this->browseSendFileBox);
			this->Sending->Controls->Add(this->BrowseSndFileTxtBox);
			this->Sending->Location = System::Drawing::Point(4, 22);
			this->Sending->Name = L"Sending";
			this->Sending->Padding = System::Windows::Forms::Padding(3);
			this->Sending->Size = System::Drawing::Size(814, 393);
			this->Sending->TabIndex = 0;
			this->Sending->Text = L"Sending";
			this->Sending->UseVisualStyleBackColor = true;
			// 
			// label_sendFilePath
			// 
			this->label_sendFilePath->AutoSize = true;
			this->label_sendFilePath->Location = System::Drawing::Point(6, 17);
			this->label_sendFilePath->Name = L"label_sendFilePath";
			this->label_sendFilePath->Size = System::Drawing::Size(113, 13);
			this->label_sendFilePath->TabIndex = 8;
			this->label_sendFilePath->Text = L"Path of File to Be Sent";
			// 
			// sendFileStatus_label
			// 
			this->sendFileStatus_label->AutoSize = true;
			this->sendFileStatus_label->Location = System::Drawing::Point(141, 217);
			this->sendFileStatus_label->Name = L"sendFileStatus_label";
			this->sendFileStatus_label->Size = System::Drawing::Size(89, 13);
			this->sendFileStatus_label->TabIndex = 7;
			this->sendFileStatus_label->Text = L"Status:    notSent";
			// 
			// label_sendPort
			// 
			this->label_sendPort->AutoSize = true;
			this->label_sendPort->Location = System::Drawing::Point(141, 90);
			this->label_sendPort->Name = L"label_sendPort";
			this->label_sendPort->Size = System::Drawing::Size(60, 13);
			this->label_sendPort->TabIndex = 6;
			this->label_sendPort->Text = L"Target Port";
			// 
			// label_sendIPaddr
			// 
			this->label_sendIPaddr->AutoSize = true;
			this->label_sendIPaddr->Location = System::Drawing::Point(6, 90);
			this->label_sendIPaddr->Name = L"label_sendIPaddr";
			this->label_sendIPaddr->Size = System::Drawing::Size(92, 13);
			this->label_sendIPaddr->TabIndex = 5;
			this->label_sendIPaddr->Text = L"Target IP Address";
			// 
			// sendFileButton
			// 
			this->sendFileButton->Location = System::Drawing::Point(60, 212);
			this->sendFileButton->Name = L"sendFileButton";
			this->sendFileButton->Size = System::Drawing::Size(75, 23);
			this->sendFileButton->TabIndex = 4;
			this->sendFileButton->Text = L"SendFile";
			this->sendFileButton->UseVisualStyleBackColor = true;
			this->sendFileButton->Click += gcnew System::EventHandler(this, &UserGUI::sendFileButton_Click);
			// 
			// sendPortTxtBox
			// 
			this->sendPortTxtBox->Enabled = false;
			this->sendPortTxtBox->Location = System::Drawing::Point(144, 106);
			this->sendPortTxtBox->Name = L"sendPortTxtBox";
			this->sendPortTxtBox->Size = System::Drawing::Size(53, 20);
			this->sendPortTxtBox->TabIndex = 3;
			this->sendPortTxtBox->Text = L"32755";
			// 
			// sendIPaddressTxtBox
			// 
			this->sendIPaddressTxtBox->Enabled = false;
			this->sendIPaddressTxtBox->Location = System::Drawing::Point(7, 106);
			this->sendIPaddressTxtBox->Name = L"sendIPaddressTxtBox";
			this->sendIPaddressTxtBox->Size = System::Drawing::Size(114, 20);
			this->sendIPaddressTxtBox->TabIndex = 2;
			// 
			// browseSendFileBox
			// 
			this->browseSendFileBox->Location = System::Drawing::Point(468, 31);
			this->browseSendFileBox->Name = L"browseSendFileBox";
			this->browseSendFileBox->Size = System::Drawing::Size(75, 23);
			this->browseSendFileBox->TabIndex = 1;
			this->browseSendFileBox->Text = L"Browse";
			this->browseSendFileBox->UseVisualStyleBackColor = true;
			this->browseSendFileBox->Click += gcnew System::EventHandler(this, &UserGUI::browseSendFileBox_Click);
			// 
			// BrowseSndFileTxtBox
			// 
			this->BrowseSndFileTxtBox->Enabled = false;
			this->BrowseSndFileTxtBox->Location = System::Drawing::Point(7, 33);
			this->BrowseSndFileTxtBox->Name = L"BrowseSndFileTxtBox";
			this->BrowseSndFileTxtBox->ReadOnly = true;
			this->BrowseSndFileTxtBox->Size = System::Drawing::Size(455, 20);
			this->BrowseSndFileTxtBox->TabIndex = 0;
			// 
			// Receiving
			// 
			this->Receiving->Controls->Add(this->label_fileToBeReceivedName);
			this->Receiving->Controls->Add(this->RecvFileNameTxtBox);
			this->Receiving->Controls->Add(this->label_RecvFileDir);
			this->Receiving->Controls->Add(this->label_recvPort);
			this->Receiving->Controls->Add(this->RecvFileSatusLabel);
			this->Receiving->Controls->Add(this->recvFile_DirectoryBrowse_button);
			this->Receiving->Controls->Add(this->recvFileDir_txtBox);
			this->Receiving->Controls->Add(this->RcvFileButton);
			this->Receiving->Controls->Add(this->recvFilePortTxtBox);
			this->Receiving->Location = System::Drawing::Point(4, 22);
			this->Receiving->Name = L"Receiving";
			this->Receiving->Padding = System::Windows::Forms::Padding(3);
			this->Receiving->Size = System::Drawing::Size(814, 393);
			this->Receiving->TabIndex = 1;
			this->Receiving->Text = L"Receiving";
			this->Receiving->UseVisualStyleBackColor = true;
			// 
			// label_fileToBeReceivedName
			// 
			this->label_fileToBeReceivedName->AutoSize = true;
			this->label_fileToBeReceivedName->Enabled = false;
			this->label_fileToBeReceivedName->Location = System::Drawing::Point(13, 151);
			this->label_fileToBeReceivedName->Name = L"label_fileToBeReceivedName";
			this->label_fileToBeReceivedName->Size = System::Drawing::Size(146, 13);
			this->label_fileToBeReceivedName->TabIndex = 8;
			this->label_fileToBeReceivedName->Text = L"Name of File to Be  Received";
			// 
			// RecvFileNameTxtBox
			// 
			this->RecvFileNameTxtBox->Location = System::Drawing::Point(10, 170);
			this->RecvFileNameTxtBox->Name = L"RecvFileNameTxtBox";
			this->RecvFileNameTxtBox->Size = System::Drawing::Size(100, 20);
			this->RecvFileNameTxtBox->TabIndex = 7;
			// 
			// label_RecvFileDir
			// 
			this->label_RecvFileDir->AutoSize = true;
			this->label_RecvFileDir->Location = System::Drawing::Point(10, 84);
			this->label_RecvFileDir->Name = L"label_RecvFileDir";
			this->label_RecvFileDir->Size = System::Drawing::Size(154, 13);
			this->label_RecvFileDir->TabIndex = 6;
			this->label_RecvFileDir->Text = L"Directory to Hold Received File";
			// 
			// label_recvPort
			// 
			this->label_recvPort->AutoSize = true;
			this->label_recvPort->Enabled = false;
			this->label_recvPort->Location = System::Drawing::Point(7, 18);
			this->label_recvPort->Name = L"label_recvPort";
			this->label_recvPort->Size = System::Drawing::Size(126, 13);
			this->label_recvPort->TabIndex = 5;
			this->label_recvPort->Text = L"Port to Receive File From";
			// 
			// RecvFileSatusLabel
			// 
			this->RecvFileSatusLabel->AutoSize = true;
			this->RecvFileSatusLabel->Enabled = false;
			this->RecvFileSatusLabel->Location = System::Drawing::Point(398, 248);
			this->RecvFileSatusLabel->Name = L"RecvFileSatusLabel";
			this->RecvFileSatusLabel->Size = System::Drawing::Size(92, 13);
			this->RecvFileSatusLabel->TabIndex = 4;
			this->RecvFileSatusLabel->Text = L"Status:    Not sent";
			// 
			// recvFile_DirectoryBrowse_button
			// 
			this->recvFile_DirectoryBrowse_button->Location = System::Drawing::Point(467, 103);
			this->recvFile_DirectoryBrowse_button->Name = L"recvFile_DirectoryBrowse_button";
			this->recvFile_DirectoryBrowse_button->Size = System::Drawing::Size(75, 23);
			this->recvFile_DirectoryBrowse_button->TabIndex = 3;
			this->recvFile_DirectoryBrowse_button->Text = L"Browse";
			this->recvFile_DirectoryBrowse_button->UseVisualStyleBackColor = true;
			this->recvFile_DirectoryBrowse_button->Click += gcnew System::EventHandler(this, &UserGUI::recvFile_DirectoryBrowse_button_Click);
			// 
			// recvFileDir_txtBox
			// 
			this->recvFileDir_txtBox->Enabled = false;
			this->recvFileDir_txtBox->Location = System::Drawing::Point(6, 103);
			this->recvFileDir_txtBox->Name = L"recvFileDir_txtBox";
			this->recvFileDir_txtBox->ReadOnly = true;
			this->recvFileDir_txtBox->Size = System::Drawing::Size(455, 20);
			this->recvFileDir_txtBox->TabIndex = 2;
			// 
			// RcvFileButton
			// 
			this->RcvFileButton->Location = System::Drawing::Point(317, 243);
			this->RcvFileButton->Name = L"RcvFileButton";
			this->RcvFileButton->Size = System::Drawing::Size(75, 23);
			this->RcvFileButton->TabIndex = 1;
			this->RcvFileButton->Text = L"Receive File ";
			this->RcvFileButton->UseVisualStyleBackColor = true;
			this->RcvFileButton->Click += gcnew System::EventHandler(this, &UserGUI::RcvFileButton_Click);
			// 
			// recvFilePortTxtBox
			// 
			this->recvFilePortTxtBox->Location = System::Drawing::Point(6, 37);
			this->recvFilePortTxtBox->Name = L"recvFilePortTxtBox";
			this->recvFilePortTxtBox->Size = System::Drawing::Size(100, 20);
			this->recvFilePortTxtBox->TabIndex = 0;
			this->recvFilePortTxtBox->Text = L"32755";
			// 
			// sndOrRcvCombBox
			// 
			this->sndOrRcvCombBox->FormattingEnabled = true;
			this->sndOrRcvCombBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Send File", L"Receive File" });
			this->sndOrRcvCombBox->Location = System::Drawing::Point(16, 50);
			this->sndOrRcvCombBox->Name = L"sndOrRcvCombBox";
			this->sndOrRcvCombBox->Size = System::Drawing::Size(121, 21);
			this->sndOrRcvCombBox->TabIndex = 1;
			this->sndOrRcvCombBox->SelectedIndexChanged += gcnew System::EventHandler(this, &UserGUI::sndOrRcvCombBox_SelectedIndexChanged);
			// 
			// textForSRcmbBx
			// 
			this->textForSRcmbBx->AutoSize = true;
			this->textForSRcmbBx->Enabled = false;
			this->textForSRcmbBx->Location = System::Drawing::Point(13, 34);
			this->textForSRcmbBx->Name = L"textForSRcmbBx";
			this->textForSRcmbBx->Size = System::Drawing::Size(255, 13);
			this->textForSRcmbBx->TabIndex = 2;
			this->textForSRcmbBx->Text = L"Use menu to select whether to send or receive a file.";
			// 
			// refreshButton
			// 
			this->refreshButton->Location = System::Drawing::Point(755, 34);
			this->refreshButton->Name = L"refreshButton";
			this->refreshButton->Size = System::Drawing::Size(75, 23);
			this->refreshButton->TabIndex = 3;
			this->refreshButton->Text = L"REFRESH";
			this->refreshButton->UseVisualStyleBackColor = true;
			this->refreshButton->Click += gcnew System::EventHandler(this, &UserGUI::refreshButton_Click);
			// 
			// UserGUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(846, 533);
			this->Controls->Add(this->refreshButton);
			this->Controls->Add(this->textForSRcmbBx);
			this->Controls->Add(this->sndOrRcvCombBox);
			this->Controls->Add(this->tabControl1);
			this->Name = L"UserGUI";
			this->Text = L"Encrypted_File Sharing GUI";
			this->tabControl1->ResumeLayout(false);
			this->Sending->ResumeLayout(false);
			this->Sending->PerformLayout();
			this->Receiving->ResumeLayout(false);
			this->Receiving->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void sndOrRcvCombBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
	{
		ComboBox^ comboBox = (ComboBox^)(sender);

		//String^ selectedMode = (String^)(sndOrRcvCombBox->SelectedItem);
		int currentComboBoxOption = 0;
		if (sndOrRcvCombBox->SelectedIndex == 0)
		{
			currentComboBoxOption = 1;
		}
		else if (sndOrRcvCombBox->SelectedIndex == 1)
		{
			currentComboBoxOption = 2;
		}
		else if (sndOrRcvCombBox->SelectedIndex == -1)
		{//no reason to do this function
			return;
		}
		else
		{
			MessageBox::Show(this, "error1", nullptr, MessageBoxButtons::OK);
			RefreshApp();
			return;
		}



		if (currentComboBoxOption == lastComboBoxOption)
		{
			//do nothing, chill
			return;
		}
		else if (lastComboBoxOption == 0 && currentComboBoxOption == 1)
		{//first change to send
			tabControl1->Visible = true;
			tabControl1->SelectedIndex = 0;
			lastComboBoxOption = 1;
			tabControl1->Enabled = true;

			//enable page 1 stuff.
			sendIPaddressTxtBox->Enabled = true;
			sendPortTxtBox->Enabled = true;
			sendFileButton->Enabled = true;
			browseSendFileBox->Enabled = true;

		}
		else if (lastComboBoxOption == 0 && currentComboBoxOption == 2)
		{//first change to recv
			tabControl1->Visible = true;
			tabControl1->SelectedIndex = 1;
			lastComboBoxOption = 2;
			tabControl1->Enabled = true;

			//enable page 2 stuff.
			recvFilePortTxtBox->Enabled = true;
			RecvFileNameTxtBox->Enabled = true;
			RcvFileButton->Enabled = true;
			recvFile_DirectoryBrowse_button->Enabled = true;
		}
		else if (lastComboBoxOption == 1 && currentComboBoxOption == 2)
		{
			RefreshApp();
			idle = 0;
			tabControl1->Visible = true;
			lastComboBoxOption = 2;//currentComboBoxOption
			tabControl1->SelectedIndex = 1;//currentComboBoxOption-1
			sndOrRcvCombBox->SelectedIndex = 1;//currentComboBoxOption-1
			tabControl1->Enabled = true;

			//enable page 2 stuff.
			recvFilePortTxtBox->Enabled = true;
			RecvFileNameTxtBox->Enabled = true;
			RcvFileButton->Enabled = true;
			recvFile_DirectoryBrowse_button->Enabled = true;
		}
		else if (lastComboBoxOption == 2 && currentComboBoxOption == 1)
		{
			RefreshApp();
			idle = 0;
			tabControl1->Visible = true;
			lastComboBoxOption = 1;//currentComboBoxOption
			tabControl1->SelectedIndex = 0;//currentComboBoxOption-1
			sndOrRcvCombBox->SelectedIndex = 0;//currentComboBoxOption-1
			tabControl1->Enabled = true;

			//enable page 1 stuff.
			sendIPaddressTxtBox->Enabled = true;
			sendPortTxtBox->Enabled = true;
			sendFileButton->Enabled = true;
			browseSendFileBox->Enabled = true;
		}
		else
		{

			MessageBox::Show(this, "error2", nullptr, MessageBoxButtons::OK);
			RefreshApp();
			return;
		}

		return;
	}





	private: System::Void refreshButton_Click(System::Object^ sender, System::EventArgs^ e)
	{
		RefreshApp();
	}
	private: System::Void RefreshApp()
	{
		tabControl1->Visible = false;
		tabControl1->SelectedIndex = -1;
		lastComboBoxOption = 0;
		sndOrRcvCombBox->SelectedIndex = -1;
		sndOrRcvCombBox->Text = "";
		tabControl1->Enabled = false;
		BrowseSndFileTxtBox->Clear();
		//TODO: why does tabcontrol->enabled give user ability to switch tabs... we dont want that
		//		either way, we can jsut disable the features on the opposite page


		//default all component values and disable them
		//	pg1
		sendIPaddressTxtBox->Enabled = false;
		sendPortTxtBox->Enabled = false;
		sendFileButton->Enabled = false;
		browseSendFileBox->Enabled = false;
		sendFileStatus_label->Text = "Satus: NotSent";
		//	pg2
		recvFilePortTxtBox->Enabled = false;
		RecvFileNameTxtBox->Enabled = false;
		RcvFileButton->Enabled = false;
		recvFile_DirectoryBrowse_button->Enabled = false;
		RecvFileSatusLabel->Text = "Satus: NotRecv";
	}

	private: System::Void sendFileButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void browseSendFileBox_Click(System::Object^ sender, System::EventArgs^ e);

private: System::Void RcvFileButton_Click(System::Object^ sender, System::EventArgs^ e);

private: System::Void recvFile_DirectoryBrowse_button_Click(System::Object^ sender, System::EventArgs^ e) {
	fbd = gcnew FolderBrowserDialog;

	fbd->Description = "Custom Description";
	
	if (fbd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		System::String^ sSelectedPath = fbd->SelectedPath;
		recvFileDir_txtBox->Text = sSelectedPath;
	}
	else
	{
		MessageBox::Show(this, "error8 getting folder", nullptr, MessageBoxButtons::OK);
		RefreshApp();
	}

}
};


}
