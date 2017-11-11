#include "Command.h"
#include < assert.h>
#include < memory>

Command::Command(int newcount)
	: timer(newcount)
	, anyinput(false)
	, pretimer(0)
	, timercount(0.0f)
	, AttackName(L"")
{
	for (int i = 0; i != 256; i++) {
		Prevkey[i] = false;
		newkey[i] = 0;
	}
	Add();
}
Command::~Command()
{
}
void Command::Add()
{
	Buffer.clear();
	moves.clear();

	moves = {
		{ L"Jump",{ A }, true },
		{ L"Punch",{ X }, true },
		{ L"Guard",{ DOWN }, true },
		{ L"Double Punch",{ X,X }, true },
		{ L"Punch Punch",{ X,X,X }, true },

		{ L"SP Punch",{ X,X,X,X }, false },
		{ L"Double Jump",{ A,A }, false },
		{ L"Jump Kick",{ A | X }, false },
		{ L"Quad Punch",{ X,Y,X,Y }, false },
		{ L"Fireball",{ DOWN,DownRight,RIGHT_X }, false },
		{ L"Long Jump",{ UP,UP,A }, false },
		{ L"Dash",{ RIGHT,RIGHT }, false },
		{ L"Dash",{ LEFT,LEFT }, false },
		{ L"Tornado",{ RIGHT,LEFT,RIGHT,LEFT,RIGHT,LEFT }, false },
		{ L"Tornado",{ LEFT,RIGHT,LEFT,RIGHT,LEFT,RIGHT }, false },
		{ L"Back Flip",{ DOWN,DOWN,A }, false },
		{ L"30 Lives",{ UP,UP,DOWN,DOWN,LEFT,RIGHT,LEFT,RIGHT,B,A }, false }
	};
}
bool Command::Matches(Move move)
{
	// ���삪�o�b�t�@�[���������ꍇ�A������v�ł��܂���B
	if (Buffer.size() < move.Size)
		return false;

	// �ŐV�̓��͂Ɉ�v����悤�ɋt�����Ƀ��[�v���܂��B
	std::vector< int>::const_reverse_iterator itr = move.Sequence.rbegin();
	std::list< int>::const_reverse_iterator itr2 = Buffer.rbegin();
	while (itr != move.Sequence.rend())
	{
		if ((*itr) != (*itr2)) return false;

		itr++;
		itr2++;
	}
	// ���̓��삪�傫�ȃV�[�P���X�̃R���|�[�l���g�łȂ�����A
	if (!move.IsSubMove)
	{
		// �g�p�ς݂̓��͂��g�p���܂��B
		Buffer.clear();
	}
	return true;
}
const TCHAR* Command::DetectMove()
{
	std::vector< Move>::const_reverse_iterator itr = moves.rbegin();
	while (itr != moves.rend())
	{
		if (Matches((*itr))) {
			return (*itr).Name;
		}
		itr++;
	}
	return L"";
}
void Command::CommandUpdate()
{
	// �܂����������t���[���� 2 �̃{�^�����������Ƃ͔��ɍ���ł��B
	// �قƂ�Ǎ����Ȃ��ꍇ�́A�����ɉ����ꂽ���̂Ƃ݂Ȃ��܂��B
	bool mergeInput = (Buffer.size() > 0 && timercount < 0.1f);

	int buttons = 0;
	if (newkey[KEY_INPUT_A] == 1)buttons |= A;
	if (newkey[KEY_INPUT_B] == 1)buttons |= B;
	if (newkey[KEY_INPUT_X] == 1)buttons |= X;
	if (newkey[KEY_INPUT_Y] == 1)buttons |= Y;
	if (newkey[KEY_INPUT_S] == 1)buttons |= S;
	if (newkey[KEY_INPUT_C] == 1)buttons |= C;
	if (newkey[KEY_INPUT_UP] == 1)buttons |= UP;
	if (newkey[KEY_INPUT_DOWN] == 1)buttons |= DOWN;
	if (newkey[KEY_INPUT_RIGHT] == 1)buttons |= RIGHT;
	if (newkey[KEY_INPUT_LEFT] == 1)buttons |= LEFT;
	if (newkey[KEY_INPUT_SPACE] == 1)buttons |= SPACE;
	if (buttons) {
		if (mergeInput && (Buffer.back() != buttons))
		{
			Buffer.back() |= buttons;
		}
		else {
			Buffer.push_back(buttons);
			timercount = 0.0f;//�^�C�}�[���Z�b�g
		}

		AttackName = DetectMove();
	}
}
void Command::Update()
{
	pretimer = timer;
	timer = GetNowCount();
	const float E = static_cast< float>(timer - pretimer) / 1000.0f;
	timercount += E;

	if (timercount > 0.5f && Buffer.size() > 0){
		Buffer.clear();
	}
	if (CheckHitKeyAll(DX_CHECKINPUT_KEY) == 0x00) {
		if (!anyinput) return;
		anyinput = false;
	}
	char buf[256];
	GetHitKeyStateAll(buf);

	for (int i = 0; i < 256; i++) {
		Prevkey[i] = newkey[i] > 0 ? true : false;
		buf[i] ? newkey[i]++ : newkey[i] = 0;
	}

	CommandUpdate();
	
	if (!anyinput)
		anyinput = true;
}
void Command::Draw()
{
	TCHAR str[32];
	int count = 0;
	std::list< int>::const_iterator itr = Buffer.begin();
	while (itr != Buffer.end())
	{
		switch ((*itr))
		{
		case A: lstrcpy(str, L"A"); break;
		case B:lstrcpy(str, L"B"); break;
		case X:lstrcpy(str, L"X"); break;
		case Y:lstrcpy(str, L"Y"); break;
		case S:lstrcpy(str, L"S"); break;
		case C:lstrcpy(str, L"C"); break;
		case UP:lstrcpy(str, L"UP"); break;
		case DOWN:lstrcpy(str, L"DOWN"); break;
		case RIGHT:lstrcpy(str, L"RIGHT"); break;
		case LEFT:lstrcpy(str, L"LEFT"); break;
		case SPACE:lstrcpy(str, L"SPACE"); break;
		case UpLeft:lstrcpy(str, L"UpLeft"); break;
		case UpRight:lstrcpy(str, L"UpRight"); break;
		case DownLeft:lstrcpy(str, L"DownLeft"); break;
		case DownRight:lstrcpy(str, L"DownRight"); break;
		case RIGHT_X:lstrcpy(str, L"RIGHT_X"); break;
		default:lstrcpy(str, L"*"); break;
		}
		DrawFormatString(0, 60 + (count * 15), GetColor(255, 255, 255), str);
		count++;

		itr++;
	}
	DrawFormatString(128, 60, GetColor(255, 255, 255), AttackName);
}
// �L�[�{�[�h���
const bool Command::IsKeyState(int vKey)
{
	return newkey[vKey] > 0;
}
const bool Command::IsKeyStatePrev(int vKey)
{
	return Prevkey[vKey];
}
const bool Command::IsHoldingKey(int vKey)
{
	return Prevkey[vKey] && newkey[vKey] > 0;
}
const bool Command::IsReleasingKey(int vKey)
{
	return !Prevkey[vKey] && newkey[vKey] == 0;
}
const bool Command::WasKeyPressed(int vKey)
{
	return !Prevkey[vKey] && newkey[vKey] > 0;
}
const bool Command::HasReleasedKey(int vKey)
{
	return Prevkey[vKey] && newkey[vKey] == 0;
}
const int Command::InputtimeCountKey(int vKey)
{
	return newkey[vKey];
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ɐݒ�
	SetGraphMode(800, 600, 32);
	SetDrawScreen(DX_SCREEN_BACK); //�`���𗠉�ʂɐݒ�

	if (DxLib_Init() == -1)	// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	std::unique_ptr< Command> command(new(std::nothrow) Command(GetNowCount()));
	while (ProcessMessage() == 0)
	{
		command->Update();
		ClearDrawScreen(); // ��ʂ�����
		command->Draw();
		ScreenFlip(); //����ʂ�\��ʂɔ��f
	}
	return DxLib_End();
}