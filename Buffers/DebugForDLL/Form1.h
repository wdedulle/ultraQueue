#pragma once

#include <Windows.h>
#include "UltraQueue.h"
#include <string.h>

unsigned int buffernrshared=0;

namespace DebugForDLL {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	delegate void SetTextDelegate(String^ text);


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

	protected: 

	protected: 





	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  StartThreaded;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker2;
	private: System::Windows::Forms::Button^  button1;





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
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->StartThreaded = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker2 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->AcceptsReturn = true;
			this->textBox1->Location = System::Drawing::Point(25, 60);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(700, 680);
			this->textBox1->TabIndex = 5;
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker1_DoWork);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(25, 12);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Start Stress";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// StartThreaded
			// 
			this->StartThreaded->Location = System::Drawing::Point(107, 11);
			this->StartThreaded->Name = L"StartThreaded";
			this->StartThreaded->Size = System::Drawing::Size(125, 23);
			this->StartThreaded->TabIndex = 7;
			this->StartThreaded->Text = L"Start Threaded Stress";
			this->StartThreaded->UseVisualStyleBackColor = true;
			this->StartThreaded->Click += gcnew System::EventHandler(this, &Form1::StartThreaded_Click);
			// 
			// backgroundWorker2
			// 
			this->backgroundWorker2->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker2_DoWork);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(239, 10);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 8;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(753, 770);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->StartThreaded);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->textBox1);
			this->Name = L"Form1";
			this->Text = L"UltraQueue 0.7.0 Stress";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

    private:
        void SetText(String^ text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this->textBox1->InvokeRequired)
            {
                SetTextDelegate^ d = 
                    gcnew SetTextDelegate(this, &Form1::SetText);
                this->Invoke(d, gcnew array<Object^> { text });
            }
            else
            {
                this->textBox1->Text = text;
            }
        }

		    private:
        void AppendText(String^ text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this->textBox1->InvokeRequired)
            {
                SetTextDelegate^ d = 
                    gcnew SetTextDelegate(this, &Form1::AppendText);
                this->Invoke(d, gcnew array<Object^> { text });
            }
            else
            {
                this->textBox1->AppendText(text);
            }
        }

private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
		 {

		unsigned int v =0;
		unsigned char a[262144];
		unsigned char b = 0x41;
		unsigned char aa[24] = {0};

		double results[24] = {0};
		int test=0;
		DWORD tstart, tend, tdif;

		unsigned int writesizesmall=256;
		unsigned int start_blocksizesmall=1;
		unsigned int stop_blocksizesmall=32;

		unsigned int writesizelarge=16384;
		unsigned int start_blocksizelarge=64;
		unsigned int stop_blocksizelarge=262144;

		int CreateStatus=0;

bool bd = false;
unsigned int resultptr =0;
unsigned int converter=0;

bool dowrite = true;
bool dosmall = true;
bool dolarge = true;
unsigned int BenchNrOfReadChan = 32;

bool doread = false;

this->SetText("");
this->SetText("WRITE benchmarks\n");
this->AppendText("\n");

if (dowrite)
{
	if (dosmall)
	{
		v = BufferCreate(64*1048576,FIFO,BenchNrOfReadChan,&CreateStatus);	// function(size,buffertype,ReadChannels)

		//if (!v)
		//{
		//	this->AppendText("Not enough memory could be allocated to start the small block tests");
		//	break;
		//}
		this->AppendText("Writing ");
		this->AppendText(writesizesmall.ToString());
		this->AppendText("MB to a 64MB FIFO for each blocksize");
		this->AppendText("\n");
	
		for (unsigned int loops=start_blocksizesmall;loops<=stop_blocksizesmall;loops *= 2)
		{
			this->AppendText(loops.ToString());
			this->AppendText(" Bytes : ");
				tstart = GetTickCount();	// Get begintime

				for (unsigned int i=0;i<(1048576*writesizesmall)/loops;++i)
				{
					test = BufferWrite(v,loops,a);
				}

				tend = GetTickCount();	// Get end time

				tdif = tend - tstart; //will now have the time elapsed since the start of the call	(performance measurement

				results[resultptr] = ((double)writesizesmall/(double)tdif)*1000;
				converter = results[resultptr];
				this->AppendText(converter.ToString());
				this->AppendText(" MB/s\n");
				resultptr++;

				BufferFlush(v,-1);
		}

		BufferRelease(v);
	}

	if (dolarge)
	{
		v = BufferCreate(512*1048576,FIFO,BenchNrOfReadChan,&CreateStatus);	// function(size,buffertype,ReadChannels,status)

		//if (!v)
		//{
		//	this->AppendText("Not enough memory could be allocated to start the Large block tests");
		//	break;
		//}
		this->AppendText("\n");
		this->AppendText("Writing ");
		this->AppendText((writesizelarge/1024).ToString());
		this->AppendText("GB to a 512MB FIFO for each blocksize");
		this->AppendText("\n");

		for (unsigned int loops=start_blocksizelarge;loops<=stop_blocksizelarge;loops *= 2)
		{
			this->AppendText(loops.ToString());
			this->AppendText(" Bytes : ");
				tstart = GetTickCount();	// Get begintime

				for (unsigned int i=0;i<((1024*writesizelarge)/loops)*1024;++i)
				{
					test = BufferWrite(v,loops,a);
				}

				tend = GetTickCount();	// Get end time

				tdif = tend - tstart; //will now have the time elapsed since the start of the call	(performance measurement

				results[resultptr] = ((double)writesizelarge/(double)tdif)*1000;
				converter = results[resultptr];
				this->AppendText(converter.ToString());
				this->AppendText(" MB/s\n");
				resultptr++;

				BufferFlush(v,-1);
		}

		BufferRelease(v);
	}

	resultptr	=	0;
}


//Read benchmarks

if (doread)
{
	this->AppendText(" \n");
	this->AppendText(" \n");
	this->AppendText("READ benchmarks\n");
	this->AppendText(" \n");

	double ReadResults[24] = {0};
	unsigned int ReadResultsPtr=0;
	unsigned int ReadPtr=0;
	int stat=0;

	v = BufferCreate(512*1048576,FIFO,32,&CreateStatus);	// function(size,buffertype,ReadChannels)

	//if (!v) break;

	// else{
	unsigned char bz = 0x30;

	for (unsigned int i=0;i<sizeof a;++i)
	{
		a[i]=bz;
		bz++;
		if (bz == 0x3A) bz = 0x30;
	}

	for (unsigned int i=0;i<4096;++i)
		{
			test = BufferWrite(v,sizeof a,a);
		}

	memset (a,0,sizeof a);

	this->AppendText("Reading 512MB from a FIFO for each blocksize .. :\n");

	for (unsigned int i=1;i<=262144;i *= 2)
		{
			this->AppendText(i.ToString());
			this->AppendText(" Bytes : ");
			tstart = GetTickCount();	// Get begintime

			for (unsigned int j=0;j<((512*1024)/i)*1024;++j)
			{
				stat = BufferRead (v, ReadPtr, i, a);
			}
		
			tend = GetTickCount();	// Get end time
			tdif = tend - tstart; //will now have the time elapsed since the start of the call	(performance measurement

			ReadResults[ReadResultsPtr] = ((double)512/(double)tdif)*1000;
			converter = ReadResults[ReadResultsPtr];
			this->AppendText(converter.ToString());
			this->AppendText(" MB/s\n");
			ReadResultsPtr++;
			ReadPtr++;
		}

	results[23]=0;
	ReadResults[23]=0;

	BufferRelease(v);

	//}	//end else
	this->AppendText("\n");
	this->AppendText("Finished !");



}
}

private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {

			 backgroundWorker1->RunWorkerAsync();
		 }


ref class ThreadBench
{
public:
   static void DoWork()
   {
	unsigned int buffernr=0;
	unsigned char a[262144];
	int test=0;
	int CreateStatus=0;

	
	if (!buffernrshared) buffernr = BufferCreate(32*1048576,FIFO,1,&CreateStatus);	// function(size,buffertype,ReadChannels)

	if (buffernr)
	{
		for (unsigned int i=0;i<65536;i++)
		{
		test = BufferWrite(buffernr,sizeof a,a);
		}
	}

	else
	{
		for (unsigned int i=0;i<65536;i++)
		{
			test = BufferWrite(buffernrshared,sizeof a,a);
		}
	}


	if (buffernr) BufferRelease(buffernr);
   }

};



private: System::Void StartThreaded_Click(System::Object^  sender, System::EventArgs^  e) {

		backgroundWorker2->RunWorkerAsync();
}

private: System::Void backgroundWorker2_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {

		DWORD tstart, tend, tdif;
		double result;
		unsigned int convert;
		int CreateStatus=0;

		tstart = GetTickCount();
							
		this->SetText("");
		this->AppendText("Starting benchmark with 8 Threads.");
		this->AppendText("\n");
		this->AppendText("Each Thread writes 16GB to it's own 32MB FIFO buffer in blocks of 256kB");
		this->AppendText("\n");

		buffernrshared = BufferCreate(128*1048576,FIFO,1,&CreateStatus);	// function(size,buffertype,ReadChannels)

		ThreadStart^ myThreadDelegate = gcnew ThreadStart( &ThreadBench::DoWork );
		
		Thread^ Thread1 = gcnew Thread( myThreadDelegate );
		Thread^ Thread2 = gcnew Thread( myThreadDelegate );
		Thread^ Thread3 = gcnew Thread( myThreadDelegate );
		Thread^ Thread4 = gcnew Thread( myThreadDelegate );
		Thread^ Thread5 = gcnew Thread( myThreadDelegate );
		Thread^ Thread6 = gcnew Thread( myThreadDelegate );
		Thread^ Thread7 = gcnew Thread( myThreadDelegate );
		Thread^ Thread8 = gcnew Thread( myThreadDelegate );

		Thread1->Start();	// Start all Threads
		Thread2->Start();
		Thread3->Start();
		Thread4->Start();
		Thread5->Start();
		Thread6->Start();
		Thread7->Start();
		Thread8->Start();

		Thread1->Join();	// Wait for all threads to finish
		Thread2->Join();
		Thread3->Join();
		Thread4->Join();
		Thread5->Join();
		Thread6->Join();
		Thread7->Join();
		Thread8->Join();

		tend = GetTickCount();	// Get end time
		tdif = tend - tstart; //will now have the time elapsed since the start of the call	(performance measurement

		result = ((double)131072/(double)tdif)*1000;

		convert = result;
				
		this->AppendText("All Threads Finished");
		this->AppendText(" \n");
		this->AppendText(" \n");
		this->AppendText("Total Write Speed : ");

		this->AppendText(convert.ToString());
		this->AppendText(" MB/s\n");

		if (buffernrshared) BufferRelease(buffernrshared);	// function(size,buffertype,ReadChannels)




		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
		 {

			 unsigned int v;
			 int stat = 0;

			 unsigned char a[256];

			 bool test = false;

			 v = BufferCreate (20,FIFO, 1, &stat);

			 stat = BufferWrite (v, 16, a);
				stat = BufferRead (v, 0, 16, a);
			 stat = BufferWrite (v, 20, a);
				test = BufferGetOverflow_Wait (v);
		 }
};
}