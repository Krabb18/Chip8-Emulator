#include "Chip8CPU.h"

void Chip8CPU::LoadRom(string romPath)
{
	CPUReset();
	//ClearScreen();

	FILE* file;
	file = fopen(romPath.c_str(), "rb");
	if (file == 0) { cout << "Failed to load rom :(" << endl; }
	else
	{
		fread(&m_GameMemory[0x200], ROMSIZE, 1, file);
		fclose(file);
	}
}

void Chip8CPU::DecreaseTimers()
{
	if(delayTimer > 0){delayTimer--;}
	if(soundTimer > 0)
	{
		//Hier aknn ncoh der beep sound abgespielt werden
		makeSound = true;
		soundTimer--;
	}

}

void Chip8CPU::CPUReset()
{
	m_AddressI = 0;
	m_ProgramCounter = 0x200;
	memset(m_Registers, 0, sizeof(m_Registers));
	memset(m_GameMemory,0,sizeof(m_GameMemory)) ;
	memset(m_KeyState,0,sizeof(m_KeyState)) ;
	soundTimer = 0;
	delayTimer = 0;
}

void Chip8CPU::ClearScreen()
{
	for(int i = 0; i<320; i++)
	{
		for(int j = 0; j<640; j++)
		{
			m_ScreenData[i][j] = 255;
		}
	}
}

int Chip8CPU::GetKeyPressed()
{
	int res = -1;
	for(int i = 0; i<16; i++)
	{
		if(m_KeyState[i] > 0)
		{
			return i;			
		}
	}

	return res;
}

WORD Chip8CPU::GetNextOpcode()
{
	//Weil opcodes vom chip8 2 bytes lang sind aber der speicher eine gr��e von einem byte hat kombiniert man die letzten zwei speicherwerte

	WORD res = 0;
	res = m_GameMemory[m_ProgramCounter];
	res <<= 8;
	res |= m_GameMemory[m_ProgramCounter + 1];
	m_ProgramCounter += 2;
	return res;
}

void Chip8CPU::ExecuteOpcode()
{
	WORD opcode = GetNextOpcode();
	switch(opcode & 0xF000)
	{
		case 0x0000: DecodeOpcode0(opcode); break;
		case 0x1000: Opcode1NNN(opcode); break;
		case 0x2000: Opcode2NNN(opcode); break;
		case 0x3000: Opcode3XNN(opcode); break;
		case 0x4000: Opcode4XNN(opcode); break;
		case 0x5000: Opcode5XY0(opcode); break;
		case 0x6000: Opcode6XNN(opcode); break;
		case 0x7000: Opcode7XNN(opcode); break;
		case 0x8000: DecodeOpcode8(opcode); break;
		case 0x9000: Opcode9XY0(opcode); break;
		case 0xA000: OpcodeANNN(opcode); break;
		case 0xB000: OpcodeBNNN(opcode); break;
		case 0xC000: OpcodeCXNN(opcode); break;
		case 0xD000: OpcodeDXYN(opcode); break;
		case 0xE000: DecodeOpcodeE(opcode); break;
		case 0xF000: DecodeOpcodeF(opcode); break;
		default: break;
	}
}

void Chip8CPU::DecodeOpcode0(WORD opcode)
{
	switch(opcode & 0xF)
	{
		case 0x0: ClearScreen(); break;
		case 0xE: Opcode00EE(); break;
	}
}

void Chip8CPU::DecodeOpcode8(WORD opcode)
{
	switch (opcode & 0xF)
	{
		case 0x0: Opcode8XY0(opcode); break;
		case 0x1: Opcode8XY1(opcode); break;
		case 0x2: Opcode8XY2(opcode); break;
		case 0x3: Opcode8XY3(opcode); break;
		case 0x4: Opcode8XY4(opcode); break;
		case 0x5: Opcode8XY5(opcode); break;
		case 0x6: Opcode8XY6(opcode); break;
		case 0x7: Opcode8XY7(opcode); break;
		case 0xE: Opcode8XYE(opcode); break;
		default: break;
	}
}

void Chip8CPU::DecodeOpcodeE(WORD opcode)
{
	switch (opcode & 0xF)
	{
		case 0x1: OpcodeEXA1(opcode); break;
		case 0xE: OpcodeEX9E(opcode); break;
		default: break;
	}
}

void Chip8CPU::DecodeOpcodeF(WORD opcode)
{
	switch (opcode & 0xFF)
	{
		case 0x07: OpcodeFX07(opcode); break;
		case 0x0A: OpcodeFX0A(opcode); break;
		case 0x15: OpcodeFX15(opcode); break;
		case 0x18: OpcodeFX18(opcode); break;
		case 0x1E: OpcodeFX1E(opcode); break;
		case 0x29: OpcodeFX29(opcode); break;
		case 0x33: OpcodeFX33(opcode); break;
		case 0x55: OpcodeFX55(opcode); break;
		case 0x65: OpcodeFX65(opcode); break;
		default: break;
	}
}

void Chip8CPU::Opcode2NNN(WORD opcode)
{
	m_Stack.push_back(m_ProgramCounter);
	m_ProgramCounter = opcode & 0x0FFF;
}


void Chip8CPU::Opcode5XY0(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	int regY = opcode & 0x00F0;
	regY = regY >> 4;

	if(m_Registers[regX] == m_Registers[regY])
	{
		m_ProgramCounter += 2;
	}
}

void Chip8CPU::OpcodeFX33(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;

	int value = m_Registers[regx];
	int hundreds = value / 100;
	int tens = (value / 10) % 10;
	int units = value % 10;

	m_GameMemory[m_AddressI] = hundreds;
	m_GameMemory[m_AddressI + 1] = tens;
	m_GameMemory[m_AddressI + 2] = units;
}

void Chip8CPU::OpcodeFX55(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx >>= 8;
	for (int i = 0; i <= regx; i++)
	{
		m_GameMemory[m_AddressI + i] = m_Registers[i];
	}
	m_AddressI = m_AddressI + regx + 1;

}

void Chip8CPU::OpcodeDXYN(WORD opcode)
{
	const int SCALE = 10 ;
	int regx = opcode & 0x0F00 ;
	regx = regx >> 8 ;
	int regy = opcode & 0x00F0 ;
	regy = regy >> 4 ;

	int coordx = m_Registers[regx] * SCALE;
	int coordy = m_Registers[regy] * SCALE ;
	int height = opcode & 0x000F ;

	m_Registers[0xf] = 0 ;

	for (int yline = 0; yline < height; yline++)
	{
		// this is the data of the sprite stored at m_GameMemory[m_AddressI]
		// the data is stored as a line of bytes so each line is indexed by m_AddressI + yline
		BYTE data = (m_GameMemory[m_AddressI+yline]);

		// for each of the 8 pixels in the line
		int xpixel = 0 ;
		int xpixelinv = 7 ;
		for(xpixel = 0; xpixel < 8; xpixel++, xpixelinv--)
		{
			
			// is ths pixel set to 1? If so then the code needs to toggle its state
			int mask = 1 << xpixelinv ;
			if (data & mask)
			{
				int x = (xpixel*SCALE) + coordx ;
				int y = coordy + (yline*SCALE) ;

				int colour = 0;

				// a collision has been detected
				if (m_ScreenData[y][x] == 0)
				{
					colour = 255 ;
					m_Registers[15]=1;
				}

				// colour the pixel
				for (int i = 0; i < SCALE; i++)
				{
					for (int j = 0; j < SCALE; j++)
					{
						m_ScreenData[y+i][x+j] = colour ;
					}
				}

			}
		}
	}
}



//MY OPCODES
void Chip8CPU::Opcode00EE()
{
	m_ProgramCounter = m_Stack.back();
	m_Stack.pop_back();
}

void Chip8CPU::Opcode1NNN(WORD opcode)
{
	WORD adressToGo = opcode & 0x0FFF;
	m_ProgramCounter = adressToGo;
}

void Chip8CPU::Opcode3XNN(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	int nn = opcode & 0x00FF;

	if(m_Registers[regX] == nn)
	{
		m_ProgramCounter += 2;
	}
}

void Chip8CPU::Opcode4XNN(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	int nn = opcode & 0x00FF;

	if(m_Registers[regX] != nn)
	{
		m_ProgramCounter += 2;
	}
}

void Chip8CPU::Opcode6XNN(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	int nn = opcode & 0x00FF;

	m_Registers[regX] = nn;
}

void Chip8CPU::Opcode7XNN(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	int nn = opcode & 0x00FF;

	m_Registers[regX] += nn;
}

void Chip8CPU::Opcode8XY0(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	int regY = opcode & 0x00F0;
	regY = regY >> 4;

	m_Registers[regX] = m_Registers[regY];
}

void Chip8CPU::Opcode8XY1(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	int regY = opcode & 0x00F0;
	regY = regY >> 4;

	m_Registers[regX] = m_Registers[regX] | m_Registers[regY];
}

void Chip8CPU::Opcode8XY2(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	int regY = opcode & 0x00F0;
	regY = regY >> 4;

	m_Registers[regX] = m_Registers[regX] & m_Registers[regY];
}

void Chip8CPU::Opcode8XY3(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	int regY = opcode & 0x00F0;
	regY = regY >> 4;

	m_Registers[regX] = m_Registers[regX] ^ m_Registers[regY];
}

void Chip8CPU::Opcode8XY4(WORD opcode)
{
	m_Registers[0xF] = 0;

	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	int regY = opcode & 0x00F0;
	regY = regY >> 4;

	//int valX = m_Registers[regX];
	//int valY = m_Registers[regY];

	int value = m_Registers[regX] + m_Registers[regY];
	if(value > 255)
	{
		m_Registers[0xF] = 1;
	}
	m_Registers[regX] = m_Registers[regX] + m_Registers[regY];
}

void Chip8CPU::Opcode8XY5(WORD opcode)
{
	m_Registers[0xF] = 1;

	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	int regY = opcode & 0x00F0;
	regY = regY >> 4;

	int xval = m_Registers[regX];
	int yval = m_Registers[regY];

	if (yval > xval)
	{
		m_Registers[0xF] = 0;
	}

	m_Registers[regX] = xval - yval;
}

void Chip8CPU::Opcode8XY6(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	m_Registers[0xF] = m_Registers[regX] & 0x1;
	m_Registers[regX] >>= 1;
}

void Chip8CPU::Opcode8XY7(WORD opcode)
{

	m_Registers[0xF] = 1;
	
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	int regY = opcode & 0x00F0;
	regY = regY >> 4;

	int valueX = m_Registers[regX];
	int valueY = m_Registers[regY];

	if(valueX > valueY)
	{
		m_Registers[0xF] = 0; //Underflow
	}

	m_Registers[regX] = m_Registers[regY] - m_Registers[regX];
}

void Chip8CPU::Opcode8XYE(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	m_Registers[0xF] = m_Registers[regX] >> 7;
	m_Registers[regX] <<= 1;
}

void Chip8CPU::Opcode9XY0(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	int regY = opcode & 0x00F0;
	regY = regY >> 4;

	if(m_Registers[regX] != m_Registers[regY])
	{
		m_ProgramCounter += 2;
	}
}


void Chip8CPU::OpcodeANNN(WORD opcode)
{
	m_AddressI = opcode & 0x0FFF;
}

void Chip8CPU::OpcodeBNNN(WORD opcode)
{
	int nnn = opcode & 0x0FFF;
	m_ProgramCounter = m_Registers[0] + nnn;
}

void Chip8CPU::OpcodeCXNN(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	int nn = opcode & 0x00FF;
	m_Registers[regX] = rand() & nn;
}

void Chip8CPU::OpcodeEX9E(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	int key = m_Registers[regX];

	if (m_KeyState[key] == 1) { m_ProgramCounter += 2; }
}

void Chip8CPU::OpcodeEXA1(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	int key = m_Registers[regX];

	if (m_KeyState[key] != 1) { m_ProgramCounter += 2; } //HIER VIELLECIHT NACHHER == 0
}

void Chip8CPU::OpcodeFX07(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	m_Registers[regX] = delayTimer;
}


//UNFINISHED
void Chip8CPU::OpcodeFX0A(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	int keyPressed = GetKeyPressed();
	if(keyPressed == -1)
	{
		m_ProgramCounter -= 2;
	}
	else
	{
		m_Registers[regX] = keyPressed;
	}
}

void Chip8CPU::OpcodeFX15(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	delayTimer = m_Registers[regX];
}

void Chip8CPU::OpcodeFX18(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;
	soundTimer = m_Registers[regX];
}

void Chip8CPU::OpcodeFX1E(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	m_AddressI += m_Registers[regX];
}

void Chip8CPU::OpcodeFX29(WORD opcode)
{
	int regX = opcode & 0x0F00;
	regX = regX >> 8;

	m_AddressI = m_Registers[regX] * 5;
}

void Chip8CPU::OpcodeFX65(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx >>= 8;
	for (int i = 0; i <= regx; i++)
	{
		 m_Registers[i] = m_GameMemory[m_AddressI + i];
	}
	m_AddressI = m_AddressI + regx + 1;
}

