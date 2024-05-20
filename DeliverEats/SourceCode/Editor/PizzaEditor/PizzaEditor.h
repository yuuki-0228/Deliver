#pragma once
#include "..\EditorBase.h"
#include "..\..\Common\Font\Font.h"

class CStageManager;

/************************************************
*	ピザエディタ.
*		﨑田友輝.
**/
class CPizzaEditor
	: public CEditorBase
{
public:
	using MaskList			= std::vector<std::vector<ID3D11ShaderResourceView*>>;
	using MaskNoMap			= std::unordered_map<ID3D11ShaderResourceView*, std::pair<int, int>>;
	using SavePizzaDataList = std::vector<std::pair<std::string, json>>;

public:
	CPizzaEditor();
	~CPizzaEditor();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// ImGui描画.
	virtual void ImGuiRender() override {};
	// モデルの描画.
	virtual void ModelRender() override {};
	// UIの描画.
	virtual void UIRender() override;
	// エフェクト描画.
	virtual void EffectRneder() override {};

private:
	// カーソルの移動.
	void CursorMove();

	// 新しいソースの取得.
	void NewSauce();
	// 新しい具材の取得.
	void NewIngredient();

	// 具材の掴みを開始する.
	void GrabStart();
	// 具材の掴みを終了する.
	void IngredientGrabEnd();
	// 具材のソースの掴みを終了する.
	void IngredientSauceGrabEnd();
	// ソースの掴みを終了する.
	void SauceGrabEnd();

	// 掴みの更新.
	void GradUpdate();
	// 掴んでる具材の回転.
	void GradRotation();

	// 上下ボタンを押した時の更新.
	void PushUpDownButton();
	// 左右ボタンを押した時の更新.
	void PushLeftRightButton();
	// ソースの変更.
	void ChangeSauce();
	// 具材の変更.
	void ChangeIngredient();
	// テキストボタンを押した時の更新.
	void PushTextButton();
	// ピザの保存.
	void PizzaSave( const int ButtonNo );
	// ピザの読み込み.
	void PizzaLoad( const int ButtonNo );
	// ピザの削除.
	void PizzaDelete( const int ButtonNo );
	// ピザのダウンロード.
	void PizzaDownload( const int ButtonNo );

	// 削除エリアの更新.
	void TrashAreaUpdate();
	// 具材の削除の更新.
	void IngredientDeleteUpdate();

	// 窯に入れるアニメーションの更新.
	void PutInKilnAnimUpdate();
	// ヘラを表示.
	void SpatulaDisp();
	// ヘラを戻す.
	void SpatulaBack();
	// 窯に入れる.
	void PutInKiln();
	// ピザを焼く.
	void CookPizza();
	// 窯から出す.
	void RemoveFromKiln();

	// ポーズに移動.
	void MovePause();
	// ポーズ画面を開く.
	void PauseOpen();
	// ポーズ画面を閉じる.
	void PauseClose();
	// ポーズ中の更新.
	void PauseUpdate();

	// 操作説明の更新.
	void TextUpdate();

	// セーブエリアの更新.
	void SaveAreaUpdate();
	// セーブエリアのボタンの更新.
	void SaveAreaButtonUpdate();
	// 保存しているピザのサンプルの初期化.
	void SavePizzaSampleInit();
	// 保存しているピザのサンプルの描画.
	void SavePizzaSampleRender();

	// ドラック&ドロップの更新.
	void DragAndDropUpdate();

private:
	CSprite*							m_pBack;						// 背景.
	CSprite*							m_pPizzaBase;					// 生地.
	CSprite*							m_pIngredient;					// 具材.
	CSprite*							m_pIngredientSauce;				// 具材のソース.
	CSprite*							m_pIngredientArea;				// 具材エリア.
	CSprite*							m_pSauce;						// ソース.
	CSprite*							m_pSauceArea;					// ソースエリア.
	CSprite*							m_pSpatula;						// ヘラ.
	CSprite*							m_pKiln;						// ピザ窯.
	CSprite*							m_pCursor;						// カーソル.
	CSprite*							m_pTrashArea;					// 削除エリア.
	CSprite*							m_pUpDownButton;				// 上下ボタン.
	CSprite*							m_pLeftRightButton;				// 左右ボタン.
	CSprite*							m_pTextButton;					// テキストボタン.
	CSprite*							m_pPauseBack;					// ポーズの背景画像.
	CSprite*							m_pPauseCursor;					// ポーズ時のカーソル画像.
	CSprite*							m_pPauseText;					// ポーズ時のテキスト画像.
	CSprite*							m_pText;						// 操作説明テキスト画像.
	CSprite*							m_pSaveArea;					// 保存エリア画像.
	CFont*								m_pFont;						// フォント.
	MaskList							m_pSauceMaskList;				// ソースのマスク画像リスト.
	MaskNoMap							m_pSauceMaskNoMap;				// ソースのマスク画像のNoを取得用.
	SFontRenderState					m_FontState;					// フォントの情報.
	SSpriteRenderState					m_PizzaBaseState;				// ピザ生地の情報.
	SSpriteRenderState					m_SamplePizzaBaseState;			// ピザ生地の情報.
	SSpriteRenderState					m_IngredientStateBase;			// 具材の情報ベース.
	SSpriteRenderState					m_IngredientSauceState;			// 具材のソースの情報リスト.
	SSpriteRenderState					m_IngredientAreaStateBase;		// 具材エリアの情報ベース.
	SSpriteRenderState					m_SauceStateBase;				// ソースの情報ベース.
	SSpriteRenderState					m_SauceAreaStateBase;			// ソースエリアの情報ベース.
	SSpriteRenderState					m_AnimSpatulaState;				// アニメーション用のヘラの情報.
	SSpriteRenderState					m_SpatulaState;					// ヘラの情報.
	SSpriteRenderState					m_KilnState;					// ピザ窯の情報.
	SSpriteRenderState					m_CursorState;					// カーソルの情報.
	SSpriteRenderState					m_PauseBackState;				// ポーズの背景の情報.
	SSpriteRenderState					m_PauseCursorState;				// ポーズのカーソルの情報.
	SSpriteRenderState					m_PauseTextStateBase;			// ポーズ時のテキストの情報ベース.
	SSpriteRenderState*					m_pSelectSaveAreaState;			// 選んだセーブエリアの情報.
	SSpriteRenderStateList				m_TextButtonStateList;			// テキストボタンの情報リスト.
	SSpriteRenderStateList				m_PauseTextStateList;			// ポーズ時のテキストの情報リスト.
	SSpriteRenderStateList				m_TrashAreaStateList;			// 削除エリア情報リスト.
	SSpriteRenderStateList				m_UpDownButtonStateList;		// 上下ボタン情報リスト.
	SSpriteRenderStateList				m_LeftRightButtonStateList;		// 左右ボタン情報リスト.
	SSpriteRenderStateList				m_IngredientStateList;			// 具材の情報リスト.
	SSpriteRenderStateList				m_IngredientAreaStateList;		// 具材エリアの情報リスト.
	SSpriteRenderStateList				m_SauceStateList;				// ソースの情報リスト.
	SSpriteRenderStateList				m_CheeseStateList;				// チーズの情報リスト.
	SSpriteRenderStateList				m_SauceAreaStateList;			// ソースエリアの情報リスト.
	SSpriteRenderStateList				m_TextStateList;				// 操作説明のテキストの情報リスト.
	SSpriteRenderStateList				m_SaveAreaStateList;			// 保存エリアの情報リスト.
	std::vector<SSpriteRenderStateList>	m_SampleSauceStateList;			// 保存しているピザのサンプル用のソースの情報リスト.
	std::vector<SSpriteRenderStateList>	m_SampleCheeseStateList;		// 保存しているピザのサンプル用のチーズの情報リスト.
	std::vector<SSpriteRenderStateList>	m_SampleIngredientStateList;	// 保存しているピザのサンプル用の具材の情報リスト.
	D3DXPOSITION3						m_GrabPosition;					// 掴んだ場所.
	D3DXVECTOR3							m_PutInKilnAnimVector;			// 窯に入れるアニメーション用のベクトル.
	std::vector<std::string>			m_SauceNameList;				// ソースの名前リスト.
	std::vector<std::string>			m_IngredientNameList;			// 具材の名前リスト.
	std::vector<std::pair<int, int>>	m_IngredientTypeAreaList;		// 具材の種類の範囲リスト<開始, 終了>.
	std::vector<std::pair<int, int>>	m_SauceList;					// ソースの状態リスト<ソースのリストNo, チーズのリストNo>.
	std::string							m_SelectObject;					// 選択しているオブジェクト.
	std::vector<float>					m_CursorMoveSpeedList;			// カーソルの移動速度リスト.
	std::vector<float>					m_IngredientRotSpeedList;		// 具材の回転速度リスト.
	SavePizzaDataList					m_SavePizzaData;				// 保存しているピザデータリスト<名前, json>.
	float								m_PutInKilnAnimCnt;				// 窯に入れるアニメーション用のカウント.
	float								m_ButtonAnimCnt;				// ボタンのアニメーション用のカウント.
	int									m_CursorMoveSpeedNo;			// カーソルの移動速度No.
	int									m_IngredientRotSpeedNo;			// 具材の回転速度No.
	int									m_AreaPageNo;					// 具材置き場のページNo.
	int									m_GrabSauceNo;					// 掴んでるソースNo.
	int									m_GrabIngredientNo;				// 掴んでる具材No.
	int									m_PutInKilnAnimNo;				// 窯に入れるアニメーションNo.
	int									m_Choice;						// 選択箇所.
	int									m_SavePizzaNum;					// 保存しているピザの数.
	int									m_SavePizzaPageNo;				// 保存しているピザのページNo.
	bool								m_IsBButtonAction;				// Bボタンで行う行動を行ったか.
	bool								m_IsDispSauce;					// ソースを表示しているか.
	bool								m_IsPutInKiln;					// 窯に入れる.
	bool								m_IsPause;						// ポーズ中か.
	bool								m_IsPauseAnim;					// ポーズのアニメーション中か.
	bool								m_IsPizzaSave;					// ピザを保存するか.
	bool								m_IsPizzaLoad;					// ピザを読み込むか.
	bool								m_IsPizzaDelete;				// ピザを削除するか.
	bool								m_IsPizzaDownload;				// ピザをダウンロードするか.
};