#include "Memory.h"
#include <iostream>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui.hpp>

using namespace std;
using namespace nana;

void MemorySizeInput(int memory_size, form* fm);
void CommandInput(int processNum, int size);

//핵심 기능은 Memory.cpp 에 있음.
Memory* memory = new Memory(0, NULL);
int main()
{
	form fm = form();
	//GUI

	//Init
	//label
	label input_size_label{ fm };
	label input_command_label{ fm };

	fm.size(nana::size(600, 400));
	fm.caption("Contiguous_Allocation_Memory_Management");
	fm.show();

	input_size_label.caption("Memory Size(KB)");
	input_size_label.size(nana::size(100, 20));
	input_size_label.move(5, 8);

	input_command_label.caption("Input Command [P num, size(KB)]");
	input_command_label.size(nana::size(200, 20));
	input_command_label.move(235, 8);
	//

	//textbox
	textbox input_size_textbox{ fm };
	textbox input_procnum_textbox{ fm };
	textbox input_procsize_textbox{ fm };

	input_size_textbox.size(nana::size(50, 20));
	input_size_textbox.move(100, 5);

	input_procnum_textbox.size(nana::size(23, 20));
	input_procnum_textbox.move(425, 5);

	input_procsize_textbox.size(nana::size(35, 20));
	input_procsize_textbox.move(450, 5);
	//

	//button
	button size_button{ fm };
	button command_button{ fm };

	size_button.caption("Init!");
	size_button.size(nana::size(55, 20));
	size_button.move(154, 5);
	size_button.events().click([&input_size_textbox, &fm]
	{ MemorySizeInput(atoi((input_size_textbox.caption()).c_str()), &fm); }
	);

	command_button.caption("Request!");
	command_button.size(nana::size(70, 20));
	command_button.move(488, 5);
	command_button.events().click([&input_procnum_textbox, &input_procsize_textbox]
	{ CommandInput(atoi((input_procnum_textbox.caption()).c_str()),
		atoi((input_procsize_textbox.caption()).c_str())); }
	);

	//
	//
	exec();
	//
	return 0;
}

//버튼으로 input 함.
void MemorySizeInput(int memory_size, form* fm)
{
	if (memory != NULL)
		delete memory;
	cout << "\n--Memory Initialize\t" << memory_size << "KB--\n";
	memory = new Memory(memory_size, fm);
}

//버튼으로 input 함.
void CommandInput(int processNum, int size)
{
	memory->Allocate(processNum, size, true);
}