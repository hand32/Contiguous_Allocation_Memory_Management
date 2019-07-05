Contiguous_Allocation_Memory_Management
2019/06 HDH

"How to simulate"
1. Memory size initialize.
 Wrtie memory size in the box and push the button.
 Then initialized memory box will be created.
 You can initialize anytime you want. Just Push the button again.

2. Request input (Input Command [P num, size(KB)])
 Write process number in the first box,
 and write process's size in the second box.
 According to best fit condition, process will be created.

 If you write 0 for the size, target process changes to hole which means empty memory space.

 You can input request as much as you want.

3. Coalsing, Compaction
 Coalsing, Compaction all implemented.
 Using BFS for finding most efficient compaction sequence.

 Only result showed in GUI.
 You can found more specific information at the console window.

Using 'nana library' for GUI.