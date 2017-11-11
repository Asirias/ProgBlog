#pragma once
#include < DxLib.h>
#include < vector>
#include < list>
class Command
{
public:
	Command(int newcount);
	~Command();
	void Update();
	void Draw();
	const bool IsKeyState(int vKey);
	const bool IsKeyStatePrev(int vKey);
	const bool IsHoldingKey(int vKey);
	const bool IsReleasingKey(int vKey);
	const bool WasKeyPressed(int vKey);
	const bool HasReleasedKey(int vKey);
	const int InputtimeCountKey(int vKey);

	const bool Anyinput() { return anyinput; }
private:
	enum Key
	{
		A = 1,
		B = 2,
		X = 4,
		Y = 8,
		S = 16,
		C = 32,
		UP = 64,
		DOWN = 128,
		LEFT = 256,
		RIGHT = 512,
		SPACE = 1024,
		UpLeft = UP | LEFT,
		UpRight = UP | RIGHT,
		DownLeft = DOWN | LEFT,
		DownRight = DOWN | RIGHT,
		RIGHT_X = RIGHT | X
	};

	struct Move
	{
		TCHAR Name[64];
		// ���̓�����A�N�e�B�u�����邽�߂Ɏg�p������͂���蒷�������
		// �R���|�[�l���g�Ƃ��čė��p�ł���ꍇ�A����� true �ɐݒ肵�܂��B
		//(true���ʏ�Z false������Z)
		bool IsSubMove;
		// ���̓�����A�N�e�B�u�����邽�߂ɕK�v�ȃ{�^�������̃V�[�P���X�B
		std::vector< int> Sequence;
		size_t Size;
		Move(const TCHAR* Name, std::vector< int> Keys, bool IsSubMove) : Sequence(Keys), IsSubMove(IsSubMove)
		{
			lstrcpy(this->Name, Name);
			Size = Keys.size();
		}
	};
private:
	int pretimer;
	int timer;
	float timercount;
	bool anyinput;
	int newkey[256];
	bool Prevkey[256];
	const TCHAR* AttackName;
	std::vector< Move> moves;
	std::list< int> Buffer;	
private:
	void CommandUpdate();
	void Add();
	bool Matches(Move move);
	const TCHAR* DetectMove();
};