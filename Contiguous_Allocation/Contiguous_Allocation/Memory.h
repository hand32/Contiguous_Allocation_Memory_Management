#pragma once
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui.hpp>

struct Process {
	//기본적으로 process의 id를 id에, 그 process의 크기를 size에, 저장위치를 pos에 저장함.
	//Hole의 경우 id를 -1로 하고 해당 홀의 크기를 size에 저장함.
	int id;
	int size;
	int pos;
	Process* next;

	nana::textbox* box;
};

class Memory {
public:
	//size 설정하고, memory를 초기화 해줌.
	Memory(int size, nana::form* _fm);
	Memory(const Memory& other);
	~Memory();

	void Allocate(int id, int size, bool print); //Process를 Memory에 할당하고, size=0이면 해당 process를 Free 시켜줌.
	void Coalescing(bool print); //근접한 Hole들을 하나로 합쳐줌.
	void Compaction(bool print); //Compaction 진행. 최적의 경우를 BFS로 탐색해서 적용함.
	void Allocate(int pos[2]); //process 이동할 때 이용하는 함수.
	int GetFreeSize(); //빈 공간의 전체 크기를 반환.
	void PrintMemory(); //Memory 상태를 콘솔로 출력.
	int GetProcessNum(); //Process의 개수를 반환.

	int max_num; //프로세스의 최대 개수.
	int max_size; //프로세스의 최대 크기.
	Process* processes; // 프로세스 LinkedList의 시작 노드.
	//프로세스들이 실제 크기를 가지지 않고,
	//LinkedList 형태로 각 프로세스와 Hole들의 정보를 객체 형식으로 저장해서 해석해서 씀.

	//GUI 관련.
	nana::form* fm;
};