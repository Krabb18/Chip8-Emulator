#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

typedef unsigned char BYTE;
typedef unsigned short WORD;

const int ROMSIZE = 0xFFF;

class Chip8CPU
{
private:

	//Opcode functions
	void		DecodeOpcode0(WORD opcode);
	void		DecodeOpcode8(WORD opcode);
	void		DecodeOpcodeE(WORD opcode);
	void		DecodeOpcodeF(WORD opcode);

	void		Opcode00EE();
	void		Opcode1NNN(WORD opcode);
	void		Opcode2NNN(WORD opcode);
	void		Opcode3XNN(WORD opcode);
	void		Opcode4XNN(WORD opcode);
	void		Opcode5XY0(WORD opcode);
	void		Opcode6XNN(WORD opcode);
	void		Opcode7XNN(WORD opcode);
	void		Opcode8XY0(WORD opcode);
	void		Opcode8XY1(WORD opcode);
	void		Opcode8XY2(WORD opcode);
	void		Opcode8XY3(WORD opcode);
	void		Opcode8XY4(WORD opcode);
	void		Opcode8XY5(WORD opcode);
	void		Opcode8XY6(WORD opcode);
	void		Opcode8XY7(WORD opcode);
	void		Opcode8XYE(WORD opcode);
	void		Opcode9XY0(WORD opcode);
	void		OpcodeANNN(WORD opcode);
	void		OpcodeBNNN(WORD opcode);
	void		OpcodeCXNN(WORD opcode);
	void		OpcodeDXYN(WORD opcode);
	void		OpcodeEX9E(WORD opcode);
	void		OpcodeEXA1(WORD opcode);
	void		OpcodeFX07(WORD opcode);
	void		OpcodeFX0A(WORD opcode);
	void		OpcodeFX15(WORD opcode);
	void		OpcodeFX18(WORD opcode);
	void		OpcodeFX1E(WORD opcode);
	void		OpcodeFX29(WORD opcode);
	void		OpcodeFX33(WORD opcode);
	void		OpcodeFX55(WORD opcode);
	void		OpcodeFX65(WORD opcode);


public:
	//Input, Delaytimer and sound
	BYTE m_KeyState[16];
	BYTE delayTimer;
	BYTE soundTimer;

	BYTE m_GameMemory[0xFFF];
	BYTE m_Registers[16];
	WORD m_AddressI;
	WORD m_ProgramCounter;
	vector<WORD> m_Stack;

	bool makeSound = false;

	BYTE m_ScreenData[320][640];

	void DecreaseTimers();
	
	void CPUReset();
	void LoadRom(string romPath);
	void ClearScreen();

	WORD GetNextOpcode();
	void ExecuteOpcode();
	
	int GetKeyPressed();
};

#endif // !CPU_H
