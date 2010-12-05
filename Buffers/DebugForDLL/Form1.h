#pragma once

#include <Windows.h>
#include "Buffers.h"
#include <string.h>


namespace DebugForDLL {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected: 
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button5;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 12);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Create";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(145, 72);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Flush Buffer";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(12, 72);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(104, 23);
			this->button3->TabIndex = 2;
			this->button3->Text = L"Read From Buffer";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 4800;
			this->serialPort1->PortName = L"COM4";
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(154, 12);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(648, 23);
			this->progressBar1->TabIndex = 3;
			this->progressBar1->Click += gcnew System::EventHandler(this, &Form1::progressBar1_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(12, 41);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(104, 23);
			this->button4->TabIndex = 4;
			this->button4->Text = L"Stress";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// textBox1
			// 
			this->textBox1->AcceptsReturn = true;
			this->textBox1->Location = System::Drawing::Point(35, 117);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(1017, 459);
			this->textBox1->TabIndex = 5;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(411, 41);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(79, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Buffer Capacity";
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(414, 88);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(100, 23);
			this->button5->TabIndex = 7;
			this->button5->Text = L"Clear Window";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1064, 588);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	unsigned int t;

	int test=0;
	unsigned char aa[24];
	unsigned char bb[24];
	double space_used_channel_1 = 0;
	double space_used_channel_2 = 0;
	double space_free_channel_1 = 0;
	double space_free_channel_2 = 0;
	
	
	t= BufferCreate(16,1,2);	// function(size,buffertype,ReadChannels)
	if (t)
	{
		memset (aa,0,sizeof aa);
		memset (bb,0,sizeof bb);
		//strcpy(aa,"abcdefghijklmnopqrs");

		test = BufferWrite(t,10,aa);	//BufferWrite (unsigned int buffer_nr, unsigned int NrOfBytes, char * Data);
		test = BufferRead(t,0,5,bb);		//BUFFERS_API int CALLING_CONVENTION BufferRead (unsigned int buffer_nr, unsigned short ReadChannel, unsigned int NrOfBytes, char * Data);
		
		space_used_channel_1 = BufferSpaceUsed_Percentage (t, 0);
		space_free_channel_1 = BufferSpaceAvailable_Percentage (t, 0);

		space_used_channel_2 = BufferSpaceUsed_Percentage (t, 1);
		space_free_channel_2 = BufferSpaceAvailable_Percentage (t, 1);


		textBox1->Text = "done";
	}

	
}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {


				 				 
			 }
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e)
			 {

			 }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {

		unsigned int v =0;
		unsigned char a[65536];
		unsigned char b = 0x41;


		int test=0;
		DWORD tstart, tend, tdif;
		
		for (unsigned int i1=0;i1<sizeof a;i1++)
		{
			a[i1] = b;
			b++;
			if (b == 0x5b) b=0x41;
		}

		v = BufferCreate(512*1048576,FIFO,1);	// function(size,buffertype,ReadChannels)


tstart = GetTickCount();	// Get begintime

	for (unsigned int i=0;i<32768*16;i++)
	{
		test = BufferWrite(v,sizeof a,a);	//BufferWrite (unsigned int buffer_nr, unsigned int NrOfBytes, char * Data);
		if (test !=0)
		{
			break;
		}
	}

tend = GetTickCount();	// Get end time

tdif = tend - tstart; //will now have the time elapsed since the start of the call	(performance measurement

double gh = ((double)32768/(double)tdif)*1000;

tstart = GetTickCount();	// Get begintime

// Original Writespeeds	(single ptr)
//64k : 341	-> 12073 (4 ptrs : 107)
//32k : 341 -> 11409
//16k : 338 -> 10502
// 8k : 339
// 4k : 336
// 2k : 325
// 1k : 313	(4 ptrs : 104)
// 512 : 288 -> 1381
// 256 : 248
// 128 : 213
// 64 : 124 -> 238
// 32 : 90
// 16 : 53
// 8 : 34->33 (4 ptrs : 24)
// 4 : 12->14
// 2 :7,5 	(4 ptrs : 7,8)
// 1 :4,7

//Read



//1st = 5
// last in row = N

unsigned char a1[20] = {0};

unsigned int count = 0;

test = BufferSpaceUsed(v,0);
test /= 1048576;
//for (unsigned int t=0;t<1024*524288;t++)
//{
		test = BufferRead(v,0,16,a1);		//BUFFERS_API int CALLING_CONVENTION BufferRead (unsigned int buffer_nr, unsigned short ReadChannel, unsigned int NrOfBytes, char * Data);

//}

test = BufferSpaceUsed(v,0);

tend = GetTickCount();	// Get end time

tdif = tend - tstart; //will now have the time elapsed since the start of the call	(performance measurement

double ij = ((double)1024/(double)tdif)*1000;

// DEBUG
//126080

// RELEASE	(speed in MB/s)
//Write
//156	original code
//178
//189
//266
//295

//Read : 605

double WriteSpeed = gh;
double ReadSpeed = ij;

// Byte-Byte
// 7,3
// BlockWrite
// 8,41
BufferRelease(v);

textBox1->Text = "done";


		 }
private: System::Void progressBar1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {

			 textBox1->Text = "";
		 }
};

}

