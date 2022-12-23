//=============================================================================
//
// UI処理 [ui.cpp]
// Author : GP12A132_29_武藤颯太
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// テクスチャの幅
#define TEXTURE_HEIGHT				(80)	// テクスチャの高さ
#define TEXTURE_MAX					(2)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/wood.png",
	"data/TEXTURE/jump.png",
};

static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static UI		g_UI[TITLE_UI_MAX];			// UI構造体



int counter = 3;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUI(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
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


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// UI構造体の初期化
	for (int i = 0; i < TITLE_UI_MAX; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].pos = XMFLOAT3(530.0f, 50.0f, 0.0f);	// 左上から表示
		g_UI[i].w = TEXTURE_WIDTH;
		g_UI[i].h = TEXTURE_HEIGHT;
		g_UI[i].texNo = 0;

	}
	g_UI[3].texNo = 1;

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
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
// 更新処理
//=============================================================================
void UpdateUI(void)
{
	//if (GetKeyboardTrigger(DIK_D))
	//{// dを押したら、メニューバーを切り替える
	//	counter++;

	//	//一番右にいったら左にバーを移動
	//	if (counter > 2)
	//	{
	//		counter = 0;
	//	}
	//}
	//else if (GetKeyboardTrigger(DIK_A))
	//{// 左を押したら、メニューバーを切り替える
	//	counter--;

	//	//一番左にいったら右にバーを移動
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
	{// Enter押したら、アイテムを使用する
		switch (counter)
		{
		case 0:	//板

			break;

		case 1:	//トランポリン

			break;

		default:
			break;
		}
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawUI(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	int j = 0;

	for (int i = 0; i < TITLE_UI_MAX; i++)
	{

		if (g_UI[i].use == TRUE)		// このUIが使われている？
		{

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[i].texNo]);
				
			//テクスチャによって処理を変える
			if ( i <= 3 )
			{
					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteLTColor(g_VertexBuffer,
						g_UI[i].pos.x + (110.0f * j), g_UI[i].pos.y, g_UI[i].w, g_UI[i].h,
						0.0f, 0.0f, 1.0f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
					j++;
			}
			else
			{
			}
		}

		//// ゲージのテスト
		//{
		//	// 下敷きのゲージ（枠的な物）
		//	// テクスチャ設定
		//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		//	//ゲージの位置やテクスチャー座標を反映
		//	float px = 600.0f;		// ゲージの表示位置X
		//	float py = 10.0f;		// ゲージの表示位置Y
		//	float pw = 300.0f;		// ゲージの表示幅
		//	float ph = 30.0f;		// ゲージの表示高さ

		//	float tw = 1.0f;	// テクスチャの幅
		//	float th = 1.0f;	// テクスチャの高さ
		//	float tx = 0.0f;	// テクスチャの左上X座標
		//	float ty = 0.0f;	// テクスチャの左上Y座標

		//	// １枚のポリゴンの頂点とテクスチャ座標を設定
		//	SetSpriteLTColor(g_VertexBuffer,
		//		px, py, pw, ph,
		//		tx, ty, tw, th,
		//		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		//	// ポリゴン描画
		//	GetDeviceContext()->Draw(4, 0);


		//}


	}


}
