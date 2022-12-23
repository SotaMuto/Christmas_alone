//=============================================================================
//
// UI���� [ui.cpp]
// Author : GP12A132_29_�����D��
//
//=============================================================================
#include "enemy.h"
#include "sprite.h"
#include "input.h"
#include "bg.h"
#include "player.h"
#include "main.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// �e�N�X�`���̕�
#define TEXTURE_HEIGHT				(80)	// �e�N�X�`���̍���
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/wood.png",
	"data/TEXTURE/jump.png",
};

static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static UI		g_UI[TITLE_UI_MAX];			// UI�\����



int counter = 3;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitUI(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// UI�\���̂̏�����
	for (int i = 0; i < TITLE_UI_MAX; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].pos = XMFLOAT3(530.0f, 50.0f, 0.0f);	// ���ォ��\��
		g_UI[i].w = TEXTURE_WIDTH;
		g_UI[i].h = TEXTURE_HEIGHT;
		g_UI[i].texNo = 0;

	}
	g_UI[3].texNo = 1;

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUI(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			if (g_Texture[i])
			{
				g_Texture[i]->Release();
				g_Texture[i] = NULL;
			}
		}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateUI(void)
{
	//if (GetKeyboardTrigger(DIK_D))
	//{// d����������A���j���[�o�[��؂�ւ���
	//	counter++;

	//	//��ԉE�ɂ������獶�Ƀo�[���ړ�
	//	if (counter > 2)
	//	{
	//		counter = 0;
	//	}
	//}
	//else if (GetKeyboardTrigger(DIK_A))
	//{// ������������A���j���[�o�[��؂�ւ���
	//	counter--;

	//	//��ԍ��ɂ�������E�Ƀo�[���ړ�
	//	if (counter < 0)
	//	{
	//		counter = 2;
	//	}
	//}

	//switch (counter)
	//{

	//case 0:
	//	g_UI[3].use = TRUE;
	//	g_UI[4].use = FALSE;
	//	g_UI[5].use = FALSE;
	//	break;

	//case 1:
	//	g_UI[3].use = FALSE;
	//	g_UI[4].use = TRUE;
	//	g_UI[5].use = FALSE;
	//	break;
	//case 2:
	//	g_UI[3].use = FALSE;
	//	g_UI[4].use = FALSE;
	//	g_UI[5].use = TRUE;
	//	break;

	//default:
	//	break;
	//}

	if(GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�A�C�e�����g�p����
		switch (counter)
		{
		case 0:	//��

			break;

		case 1:	//�g�����|����

			break;

		default:
			break;
		}
	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawUI(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	int j = 0;

	for (int i = 0; i < TITLE_UI_MAX; i++)
	{

		if (g_UI[i].use == TRUE)		// ����UI���g���Ă���H
		{

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[i].texNo]);
				
			//�e�N�X�`���ɂ���ď�����ς���
			if ( i <= 3 )
			{
					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteLTColor(g_VertexBuffer,
						g_UI[i].pos.x + (110.0f * j), g_UI[i].pos.y, g_UI[i].w, g_UI[i].h,
						0.0f, 0.0f, 1.0f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
					// �|���S���`��
					GetDeviceContext()->Draw(4, 0);
					j++;
			}
			else
			{
			}
		}

		//// �Q�[�W�̃e�X�g
		//{
		//	// ���~���̃Q�[�W�i�g�I�ȕ��j
		//	// �e�N�X�`���ݒ�
		//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		//	//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		//	float px = 600.0f;		// �Q�[�W�̕\���ʒuX
		//	float py = 10.0f;		// �Q�[�W�̕\���ʒuY
		//	float pw = 300.0f;		// �Q�[�W�̕\����
		//	float ph = 30.0f;		// �Q�[�W�̕\������

		//	float tw = 1.0f;	// �e�N�X�`���̕�
		//	float th = 1.0f;	// �e�N�X�`���̍���
		//	float tx = 0.0f;	// �e�N�X�`���̍���X���W
		//	float ty = 0.0f;	// �e�N�X�`���̍���Y���W

		//	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//	SetSpriteLTColor(g_VertexBuffer,
		//		px, py, pw, ph,
		//		tx, ty, tw, th,
		//		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		//	// �|���S���`��
		//	GetDeviceContext()->Draw(4, 0);


		//}


	}


}
