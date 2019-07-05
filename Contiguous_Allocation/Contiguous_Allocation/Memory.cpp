#include "Memory.h"
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <nana/gui/widgets/textbox.hpp>
using namespace std;
using namespace nana;

//GUI
void UpdateBox(Process* process, form* fm);
//
Memory::Memory(int size, form* _fm)
{
	max_num = 100;
	max_size = size;
	fm = _fm;

	processes = new Process[100];

	Process* empty = new Process();
	empty->id = -1;
	empty->size = size;
	empty->pos = 0;
	if (size != 0)
	{
		empty->box = new textbox(*fm);
		empty->box->size(nana::size(400, 20));
		empty->box->move(100, 50);
		empty->box->editable(false);
		empty->box->caption(to_string(empty->pos) + "KB\t" + "Hole:\t" + to_string(size) + "KB");
		empty->box->text_align(nana::align::center);
	}
	processes = empty;
}
Memory::Memory(const Memory& other)
{
	fm = other.fm;
	max_num = other.max_num;
	max_size = other.max_size;
	Process* otherLocation = other.processes;
	Process* location;
	Process* newProc;
	newProc = new Process();
	newProc->id = otherLocation->id;
	newProc->size = otherLocation->size;
	newProc->pos = otherLocation->pos;

	processes = newProc;
	location = processes;
	otherLocation = otherLocation->next;
	while (otherLocation != NULL)
	{
		newProc = new Process();
		newProc->id = otherLocation->id;
		newProc->size = otherLocation->size;
		newProc->pos = otherLocation->pos;

		location -> next = newProc;
		location = location->next;
		otherLocation = otherLocation->next;
	}
}
Memory::~Memory()
{
	while (processes != NULL)
	{
		Process* tmp = processes;
		processes = processes->next;
		if(tmp->box != NULL)
			tmp->box->close();
		delete tmp;
	}
}

bool IsFragmented(Process* processes);
void Memory::Allocate(int id, int size, bool print)
{
	//Free 시켜주기.
	if (size == 0)
	{
		Process* location = processes;
		while(location != NULL)
		{
			if (location->id == id)
			{
				location->id = -1;
				if (print)
					cout << "\nFREE REQUEST " << id << ": " << location->size << "KB\n";
				location->box->caption(to_string(location->pos) + "KB\t" + "Hole:\t" + to_string(location->size) + "KB");
				Coalescing(print);
				break;
			}
			location = location->next;
		}
	}
	//

	//Bestfit 찾기.
	else if (size > 0)
	{
		if(print)
			cout << "\nREQUEST " << id << ": " << size << "KB\n";
		Process* min_location = NULL;
		int min_size = max_size + 1;
		Process* location = processes;
		int cnt = -1;
		//Bestfit 찾는 loop구문.
		while (location != NULL)
		{
			if (location->id == -1 && location->size >= size && location->size < min_size)
			{
				min_location = location;
				min_size = location->size;
			}
			cnt++;
			location = location->next;
		}
		//프로세스 Allocation.
		if (min_location != NULL)
		{
			Process* hole = new Process();
			hole->id = -1;
			hole->size = min_location->size - size;
			hole->pos = min_location->pos + size;
			hole->next = min_location->next;
			//GUI
			hole->box = new textbox(*fm);
			hole->box->size(nana::size(400, 20));
			hole->box->move(100, 50 + 20 * (cnt + 1));
			hole->box->editable(false);
			hole->box->caption(to_string(hole->pos) + "KB\t" + "Hole:\t" + to_string(hole->size) + "KB");
			hole->box->text_align(nana::align::center);
			//

			min_location->id = id;
			min_location->size = size;
			min_location->next = hole;
			//GUI
			min_location->box->size(nana::size(400, 20));
			min_location->box->move(100, 50 + 20 * cnt);
			min_location->box->editable(false);
			min_location->box->caption(to_string(min_location->pos) + "KB\t" + "P" + to_string(id) + ":\t" + to_string(size) + "KB");
			min_location->box->text_align(nana::align::center);
			//
			if (hole->size == 0)
			{
				min_location->next = hole->next;
				if (hole->box != NULL)
					hole->box->close();
				delete hole;
			}
				cout << "  Best Fit : Allocated at  " << min_location->pos << "KB\n";
			if (print)
				PrintMemory();

		}
		else if (GetFreeSize() >= size)
		{
			if (IsFragmented(processes))
			{
				Compaction(print);
				Allocate(id, size, false);
				if (print)
					PrintMemory();
			}
		}
		else
		{
			cout << "  Error: Process is to big to Allocate.\n";
			cout << "  Process " << id << " " << size << "KB" << '\n';
			cout << GetFreeSize() << "KB free.\n";
		}
	}
	UpdateBox(processes, fm);
	//
}

void Memory::Allocate(int pos[2])
{
	if (pos[0] == pos[1])
		return;

	Process* location = processes;
	int cnt = 0;
	while (location != NULL)
	{
		if (cnt == pos[0])
			break;
		location = location->next;
		cnt++;
	}
	Process* target = processes;
	cnt = 0;
	while (target != NULL)
	{
		if (cnt == pos[1])
			break;
		target = target->next;
		cnt++;
	}

	Process* hole = new Process();
	hole->id = -1;
	hole->size = target->size - location->size;
	hole->pos = target->pos + location->size;
	hole->next = target->next;

	target->id = location->id;
	target->size = location->size;
	target->next = hole;
	location->id = -1;

	hole->box = new textbox(*fm);
	hole->box->size(nana::size(400, 20));
	hole->box->editable(false);
	hole->box->text_align(nana::align::center);
	if (hole->size == 0)
	{
		target->next = hole->next;
		if (hole->box != NULL)
			hole->box->close();
		delete hole;
	}
	Coalescing(false);
}

void Memory::Coalescing(bool print)
{
	UpdateBox(processes, fm);
	Process* location = processes;
	if(print)
		cout << "  Coalescing: \n\t";
	while (location != NULL)
	{
		if (location->id == -1 && location->next != NULL && location->next->id == -1)
		{
			Process* tmp = location->next;
			if(print)
				cout << location->pos << "\t & \t" << location->next->pos << '\n';
			location->size += location->next->size;
			location->next = location->next->next;
			location->box->caption(to_string(location->pos) + "KB\t" + "P" + to_string(location->id) + ":\t" + to_string(location->size) + "KB");
			if (tmp->box != NULL)
				tmp->box->close();
			delete tmp;
		}
		else if (location->id == -1 && location->size == 0)
		{
			Process* tmp;
			if (location->next != NULL)
			{
				tmp = location->next;
				location->id = location->next->id;
				location->size = location->next->size;
				location->next = location->next->next;
			}
			else
			{
				tmp = processes;
				while (tmp->next != location)
					tmp = tmp->next;
				location = tmp;
				tmp = location->next;
				location->next = NULL;
			}
			if(tmp->box != NULL)
				tmp->box->close();
			delete tmp;
		}
		else
			location = location->next;
	}
	if (print)
		PrintMemory();
	UpdateBox(processes, fm);
}

bool IsFragmented(Process* processes);
void BFSCompaction(const Memory* memory, int& min_move_size, queue<vector<int>>& minPath);
void Memory::Compaction(bool print)
{
	int min_move_size = max_size + 1;
	queue<vector<int>> minPath;
	BFSCompaction(this, min_move_size, minPath);

	cout << "  Compaction:";
	int totalmove = minPath.back()[2];
	minPath.pop();
	while (!minPath.empty())
	{
		int allocateInfo[2];
		vector<int> path = minPath.front();
		minPath.pop();

		Process* location = processes;
		int cnt = 0;
		while (location != NULL)
		{
			if (cnt == path[0])
				break;
			location = location->next;
			cnt++;
		}
		cnt = location->id;

		allocateInfo[0] = path[0];
		allocateInfo[1] = path[1];
		Allocate(allocateInfo);

		location = processes;
		while (location != NULL)
		{
			if (location->id == cnt)
				break;
			location = location->next;
		}

		if(print)
			cout << "\tP" << location->id << " " << location->size << "KB" << " moves to " << location->pos << "KB\n";
	}
	cout << "    Total Move:\t" << totalmove << "KB\n";
	if(print)
		PrintMemory();
}

//Hole 들이 분산되어 있는지 체크해서 반환.
bool IsFragmented(Process* processes)
{
	Process* location = processes;
	int cnt = 0;
	while (location != NULL)
	{
		if (location->id == -1)
		{
			cnt++;
			if (cnt >= 2)
				return true;
		}
		location = location->next;
	}
	return false;
}
//Compaction하는 최적의 경로를 찾는다.
void BFSCompaction(const Memory* memory, int& min_move_size, queue<vector<int>>& minPath)
{
	int processNum;
	queue<Memory> bfsQueue;
	queue<queue<vector<int>>> bfsPath;

	Memory initMemory = Memory(*memory);
	processNum = initMemory.GetProcessNum();
	bfsQueue.push(initMemory);
	queue<vector<int>> init;
	init.push(vector<int>({ 0, 0, 0 }));
	bfsPath.push(init);

	while (!bfsQueue.empty())
	{
		Memory r = bfsQueue.front();
		bfsQueue.pop();
		queue<vector<int>> pathNode = bfsPath.front();
		bfsPath.pop();
		int adjustPath[2] = { pathNode.back()[0], pathNode.back()[1] };
		r.Allocate(adjustPath);

		if (!IsFragmented(r.processes))
		{
			if (min_move_size > pathNode.back()[2])
			{
				min_move_size = pathNode.back()[2];
				minPath = pathNode;
			}
		}
		else if(min_move_size > pathNode.back()[2])
		{
			if (pathNode.size() < processNum)
			{
				int swapPos1 = 0;
				Process* location1 = r.processes;
				while (location1 != NULL)
				{
					if (location1->id != -1)
					{
						int swapPos2 = 0;
						Process* location2 = r.processes;
						while (location2 != NULL)
						{
							if (location2->id == -1 && location2->size >= location1->size)
							{
								if (location1->size + pathNode.back()[2] < min_move_size)
								{
									Memory _memory = Memory(r);
									bfsQueue.push(_memory);

									queue<vector<int>> _path = queue<vector<int>>(pathNode);
									_path.push({ swapPos1, swapPos2, location1->size + pathNode.back()[2] });
									bfsPath.push(_path);
								}
							}
							location2 = location2->next;
							swapPos2++;
						}
					}
					location1 = location1->next;
					swapPos1++;
				}
			}
		}
	}
}

int Memory::GetFreeSize()
{
	Process* location = processes;
	int result = 0;
	while (location != NULL)
	{
		if (location->id == -1)
		{
			result += location->size;
		}
		location = location->next;
	}
	return result;
}

int Memory::GetProcessNum()
{
	Process* location = processes;
	int cnt = 0;
	while (location != NULL)
	{
		if (location->id != -1)
		{
			cnt++;
		}
		location = location->next;
	}
	return cnt;
}

void Memory::PrintMemory()
{
	Process* location = processes;

	cout << "\tMemory\t" << max_size << "KB\n";
	while (location != NULL)
	{
		cout << "\t\t" << location->pos << "KB\t\t";
		if (location->id == -1)
			cout << "Hole";
		else
			cout << 'P' << location->id;

		cout << ":\t" << location->size << "KB\n";
		location = location->next;
	}
}

//GUI 관련
void UpdateBox(Process* process, form* fm)
{
	int cnt = 0;
	while (process != NULL)
	{
		if (process->box == NULL)
		{
			process->box = new textbox(*fm);
			process->box->editable(false);
			process->box->size(nana::size(400, 20));
			process->box->text_align(nana::align::center);
		}
		process->box->move(100, 50 + 20 * cnt);
		if (process->id == -1)
			process->box->caption(to_string(process->pos) + "KB\t" + "Hole:\t" + to_string(process->size) + "KB");
		else
			process->box->caption(to_string(process->pos) + "KB\t" + "P" + to_string(process->id) + ":\t"
				+ to_string(process->size) + "KB");
		cnt++;
		process = process->next;
	}
}
//