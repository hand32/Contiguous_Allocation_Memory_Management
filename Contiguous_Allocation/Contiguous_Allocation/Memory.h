#pragma once
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui.hpp>

struct Process {
	//�⺻������ process�� id�� id��, �� process�� ũ�⸦ size��, ������ġ�� pos�� ������.
	//Hole�� ��� id�� -1�� �ϰ� �ش� Ȧ�� ũ�⸦ size�� ������.
	int id;
	int size;
	int pos;
	Process* next;

	nana::textbox* box;
};

class Memory {
public:
	//size �����ϰ�, memory�� �ʱ�ȭ ����.
	Memory(int size, nana::form* _fm);
	Memory(const Memory& other);
	~Memory();

	void Allocate(int id, int size, bool print); //Process�� Memory�� �Ҵ��ϰ�, size=0�̸� �ش� process�� Free ������.
	void Coalescing(bool print); //������ Hole���� �ϳ��� ������.
	void Compaction(bool print); //Compaction ����. ������ ��츦 BFS�� Ž���ؼ� ������.
	void Allocate(int pos[2]); //process �̵��� �� �̿��ϴ� �Լ�.
	int GetFreeSize(); //�� ������ ��ü ũ�⸦ ��ȯ.
	void PrintMemory(); //Memory ���¸� �ַܼ� ���.
	int GetProcessNum(); //Process�� ������ ��ȯ.

	int max_num; //���μ����� �ִ� ����.
	int max_size; //���μ����� �ִ� ũ��.
	Process* processes; // ���μ��� LinkedList�� ���� ���.
	//���μ������� ���� ũ�⸦ ������ �ʰ�,
	//LinkedList ���·� �� ���μ����� Hole���� ������ ��ü �������� �����ؼ� �ؼ��ؼ� ��.

	//GUI ����.
	nana::form* fm;
};