#pragma once

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

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~UserGUI();

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// UserGUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(621, 503);
			this->Name = L"UserGUI";
			this->Text = L"UserGUI";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
