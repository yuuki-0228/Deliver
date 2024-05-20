#pragma once
#include "..\..\Global.h"

/************************************************
*	定数構造体.
**/
namespace ConstantStruct {
	// プレイヤーの定数構造体.
	struct stPlayer {
		float			JETPACK_POWER;				// ジェットパックの力.
		float			GRAVITY_POWER;				// 重力の力.
		float			POWER_TO_RESIST;			// 抵抗する力.
		float			AIR_FRICTION;				// 空気摩擦.
		float			GROUND_FRICTION;			// 地面摩擦.
		float			ACCELERATION_MIN;			// 加速度の下限.
		float			ACCELERATION_MAX;			// 加速度の上限.
		float			START_TILE;					// プレイヤーを傾けていく開始傾き.
		float			END_TILE;					// プレイヤーの傾きの上限(度).
		float			RETURN_TILT_SPEED;			// 傾きを戻す速度(度).
		float			MAX_ROTATION_Z;				// プレイヤーのZの回転の上限(度).
		float			INERTIA_USAGE_RATE;			// 慣性の使用する割合(0.0f ~ 1.0f).
		float			JETPACK_ROT_SPEED;			// ジェットパックの回転速度(度).
		float			JETPACK_ROT_END;			// ジェットパックの回転の終了角度(度).
		float			VERTICAL_RAY_LENGTH;		// 縦のレイの長さ.
		float			SIDE_RAY_LENGTH;			// 横のレイの長さ.
		float			GROUND_CLOSE_RAY_LENGTH;	// 地面に近いか調べる用のレイの長さ.
		float			JETPACK_EFFECT_RAY_LENGTH;	// ジェットパックのエフェクト用のレイの長さ.
		float			CEILING_MOVE_SPEED;			// 天井にぶつかった時の移動速度(0に近づくほど速くなる).
		float			LANDING_EFFECT_TIME;		// 着地エフェクトを再生していく間隔の時間(秒).
		float			LANDING_ANIM_SPPED;			// 着地アニメーション速度(度/秒).
		float			GET_FOOD_ANIM_SPPED;		// 商品取得アニメーション速度(度/秒).
		float			LOOK_POS_ADD_Y;				// 店員/お客さんが見る位置の追加するY座標.
		float			JETPACK_STOP_TIME;			// ジェットパックを停止させる時間(秒).
		float			JETPACK_STOP_CNT_DIFF;		// 停止させるカウントを追加する変化の差(0.0f ~ 1.0f).
		float			SHADOW_ALPHA;				// 影のアルファ値.
		float			DEAD_DOWN_POS_Y;			// リザルトに移動させる下側のY座標.
		float			DEAD_UP_POS_Y;				// リザルトに移動させる上側のY座標.
		float			DEAD_RADIUS;				// リザルトに移動させる半径.
		float			DEAD_GROUND_SPEED;			// リザルトに移動させる地面の衝突速度.
		float			DEAD_WALL_SPEED;			// リザルトに移動させる壁の衝突速度.
		float			WARNING_SIZE;				// 警告を出す範囲のサイズ.
		float			LANDING_SE_PLAY_SPEED;		// 着地SEを再生する落下速度.
		float			ADD_SE_VOLUME;				// SEの増やしていく音量(秒).
		float			VIBRATION_POWER;			// 振動の使用率.
		float			SHADOW_DISP_DISTANCE;		// 影を表示する距離.
		float			STAGE_OUT_TIME;				// ステージ外に移動した時の演出の時間(秒).
		int				JETPACK_INPUT_LIST_MAX;		// ジェットパックの入力リストの上限.
		int				STOP_CHANGE_NUM;			// ジェットパックを停止させる変化した回数.
		std::string		DOWN_WARNING_MESSAGE;		// 下方向の警告の通知文.
		std::string		UP_WARNING_MESSAGE;			// 上方向の警告の通知文.
		std::string		RADIUS_WARNING_MESSAGE;		// 半径以上の時の警告の通知文.
		D3DXSCALE3		SHADOW_MIN_SCALE;			// 影の最小サイズ.
		D3DXSCALE3		SHADOW_ADD_ACALE;			// 影の追加サイズ.

		void Load();
		stPlayer() { Load(); }
	} typedef SPlayer;

	// 渡しに行くお客さんの方向の定数構造体.
	struct stCustomerDire {
		float						MOVE_ANIM_SPEED;		// 移動アニメーション速度(度/秒).
		float						DISTANCE;				// 矢印の距離.
		float						ALPHA_START_DISTANCE;	// アルファ値を減らしていく開始距離.
		float						ALPHA_DEC_DISTANCE;		// アルファ値を減らしていく距離.
		D3DXPOSITION3				CUSTOMER_ADD_POS;		// お客さんの座標に追加する座標.
		std::vector<D3DXSCALE3>		SIZE_LIST;				// 矢印のサイズリスト(2個).
		std::vector<float>			ALPHA_LIST;				// アルファ値リスト(2個).

		void Load();
		stCustomerDire() { Load(); }
	} typedef SCustomerDire;

	// フリーカメラの定数構造体.
	struct stFreeCamera {
		float	MOVE_SPEED;				// カメラの移動速度.
		float	ROTATION_SPEED;			// カメラの回転速度.

		void Load();
		stFreeCamera() { Load(); }
	} typedef SFreeCamera;

	// カメラの定数構造体.
	struct stCamera {
		bool	IS_REVERSE;				// スティックの入力を反転させるか?
		float	INIT_ROTATION;			// カメラの角度の初期値.
		float	MOVABLE_MIN;			// 傾けられる最小角度.
		float	MOVABLE_MAX;			// 傾けられる最大角度.
		float	INIT_RETURN_VALUE;		// 元の角度に戻すときに加算する値の初期値.
		float	INIT_ADD_VALUE;			// 元の角度に戻すときに加算する値に加算する値の初期値.
		float	ADJUST_ANGLE;			// カメラの回転角度の調整値.
		float	ADD_DELAY_ANGLE;		// ターゲットの昇降によって回転角度に加算される値.
		float	INVALID_LENGTH;			// 注視カメラを無効化する距離.

		void Load();
		stCamera() { Load(); }
	} typedef SCamera;

	// リュックの定数構造体.
	struct stRuckSack {
		float	DISCOUNT_SPPED;			// 割引速度(%/秒).
		float	COOLDOWN_SPEED;			// 温度を冷ましていく速度(℃/秒).
		float	MAX_DISCOUNT;			// 最高の傾き割引率.
		float	MIN_DISCOUNT;			// 最低の傾き割引率.
		float	GAUGE_LR_SPPED;			// 左右ゲージの進む速度.
		float	GAUGE_UD_SPPED;			// 上下ゲージの進む速度.
		float	GAUGE_CENTER_SPPED;		// ゲージの戻る速度.
		float	UD_GAUGE_DOWN_SPEED;	// 上下ゲージの速度が下がっていく速度.
		float	MAX_TEMPERATURE;		// 最大の温度.
		float	MIN_TEMPERATURE;		// 最低の温度.
		float	MAX_GAUGE;				// 最大のゲージの位置.
		float	INIT_GAUGE;				// 初期のゲージの位置.
		float	MIN_GAUGE;				// 最低のゲージの位置.
		float	BAR_MOVE_SCALE;			// バーの動きに掛ける数値.

		void Load();
		stRuckSack() { Load(); }
	} typedef SRuckSack;

	// 風の定数構造体.
	struct stWind {
		float			WIND_TIME;			// 風が吹いたり止まったりする時間(秒).
		float			WIND_STRONG_SPEED;	// 風が強くなっていく速度(秒).
		float			WIND_WEAK_SPEED;	// 風が弱くなっていく速度(秒).
		float			WIND_HEIGHT_POWER;	// 風の高さによる高さの強さ.
		float			ADD_SE_VOLUME;		// SEの音量を加算していく量(秒).
		std::string		WIND_MESSAGE;		// 風が吹き始めた時の通知文.
		std::string		WIND_STOP_MESSAGE;	// 風が止まった時の通知文.

		void Load();
		stWind() { Load(); }
	} typedef SWind;

	// 風UIの定数構造体.
	struct stWindUI {
		float			WIND_LINE_DISP_SPEED;			// 雲を出現させていく速度(秒).
		float			WIND_LINE_SPPED_MIN;			// 風の線の速度の最小値.
		float			WIND_LINE_SPPED_MAX;			// 風の線の速度の最大値.
		float			WIND_LINE_SCALE_MIN;			// 風の線の拡縮の最小値.
		float			WIND_LINE_SCALE_MAX;			// 風の線の拡縮の最大値.
		float			WIND_LINE_ALPHA_MIN;			// 風の線のアルファの最小値.
		float			WIND_LINE_ALPHA_MAX;			// 風の線のアルファの最大値.
		float			WIND_LINE_WIND_POWER_ADD_SPPED;	// 風の線の風の強さによって追加する速度.
		void Load();
		stWindUI() { Load(); }
	} typedef SWindUI;

	// ステージの定数構造体.
	struct stStage {
		float	CLOUD_DOWN_POS_Y;			// 一番下の雲のY座標.
		float	CLOUD_INTERVAL;				// 雲を配置していく間隔.
		float	CLOUD_MAX_SPEED;			// 雲の最大のUVスクロール速度.
		float	CLOUD_SCALE;				// 雲の大きさ.

		void Load();
		stStage() { Load(); }
	} typedef SStage;

	// マンションの定数構造体.
	struct stApartment {
		float	START_DISTANCE;				// 距離によるフェードを開始する距離.
		float	ALPHA_DISTANCE;				// アルファ値を減らしていく距離.
		float	CAMERA_ALPHA_MIN;			// カメラによるアルファ値の最小値.
		float	CAMERA_ALPHA_MAX;			// カメラによるアルファ値の最大値.
		float	CAMERA_ALPHA_SPEED;			// カメラによるアルファの速度.
		float	APARTMENT_ADD_MONEY;		// マンションタイプの追加するお金.
		float	FLOOR_SIZE;					// マンションの1フロアのサイズ.
		float	MOVE_SPEED;					// お客さんの移動速度(移動値/秒).
		float	WINDOW_MOVE_SPEED;			// 窓の移動速度(移動値/秒).
		float	ROT_SPPED;					// お客さんの回転速度(度/秒).
		int		START_NOROOM_FLOOR_NUM;		// 開始の部屋の無いフロアの数.

		void Load();
		stApartment() { Load(); }
	} typedef SApartment;

	// ピザ窯の炎の定数構造体.
	struct stPizzaKilnFire {
		float		START_DISTANCE;			// 距離によるフェードを開始する距離.
		float		ALPHA_DISTANCE;			// アルファ値を減らしていく距離.
		float		ALPHA;					// アルファ値.
		float		ADD_COLOR_SPEED;		// 加算していく色の変化速度(度/秒).
		D3DXCOLOR3	START_COLOR;			// 開始時の色.
		D3DXCOLOR3	END_COLOR;				// 終了時の色.

		void Load();
		stPizzaKilnFire() { Load(); }
	} typedef SPizzaKilnFire;

	// 浮島の定数構造体.
	struct stIsland {
		float	START_DISTANCE;				// 距離によるフェードを開始する距離.
		float	ALPHA_DISTANCE;				// アルファ値を減らしていく距離.
		float	CAMERA_ALPHA_MIN;			// カメラによるアルファ値の最小値.
		float	CAMERA_ALPHA_MAX;			// カメラによるアルファ値の最大値.
		float	CAMERA_ALPHA_SPEED;			// カメラによるアルファの速度.
		float	MOVE_SPEED;					// お客さんの移動速度(移動値/秒).
		float	DOOR_MOVE_SPEED;			// 窓の移動速度(移動値/秒).
		float	ROT_SPPED;					// お客さんの回転速度(度/秒).
		float	COLL_STOP_SPHERE_RADIUS;	// 当たり判定を行うか判断する球体の当たり判定の半径.

		void Load();
		stIsland() { Load(); }
	} typedef SIsland;

	// お客さんと店員の定数構造体.
	struct stDeliveryCharacter {
		float						START_DISTANCE;				// 距離によるフェードを開始する距離.
		float						ALPHA_DISTANCE;				// アルファ値を減らしていく距離.
		float						SPHERE_SIZE;				// 球体サイズ(最寄りのお店/お客さんなどで使用).
		float						CYLINDER_RADIUS;			// 円柱の半径(受け渡し可能範囲で使用).
		float						CYLINDER_HEIGHT;			// 円柱の高さ(受け渡し可能範囲で使用).
		float						CYLINDER_ADD_POS_Y;			// 円柱の当たり判定の追加するY座標.
		float						GAUGE_SPEED;				// 受け渡しゲージ速度(秒).
		float						GAUGE_DEC_SPEED;			// 受け渡しゲージの減少速度(秒).
		float						GAUGE_DELETE_TIME;			// 受け渡しゲージの削除する時間(秒).
		float						GAUGE_SPEED_UP;				// 受け渡しゲージの速度をあげる倍率.
		float						DETACH_COOL_TIME;			// 連続で紐づけされないようにするクールタイム(秒).
		float						MONEY;						// 最低限もらえるお金.
		float						HIGHT_MONEY_INTERVAL;		// 高さによるお金を加算していく間隔.
		float						HIGHT_ADD_MONEY;			// 高さによる追加していく値段.
		float						DISTANCE_MONEY_INTERVAL;	// 距離によるお金を加算していく間隔.
		float						WAVEHANDS_ANIMATION_SPEED;	// 手を振るアニメーション速度(秒).
		int							NORMAL_MESSAGE_SIZE;		// 通常の時の配達した時の通知文のサイズ.
		int							HOT_MESSAGE_SIZE;			// アツアツの時の配達した時の通知文のサイズ.
		int							MESSY_MESSAGE_SIZE;			// ぐちゃぐちゃの時の配達した時の通知文のサイズ.
		int							MONEY_MAX;					// 最低限もらえるお金の上限.
		std::vector<std::string>	NORMAL_MESSAGE;				// 通常の時の配達した時の通知文.
		std::vector<std::string>	HOT_MESSAGE;				// アツアツの時の配達した時の通知文.
		std::vector<std::string>	MESSY_MESSAGE;				// ぐちゃぐちゃの時の配達した時の通知文.

		void Load();
		stDeliveryCharacter() { Load(); }
	} typedef SDeliveryCharacter;

	// 紐づけの定数構造体.
	struct stLinking {
		float						NORMAL_LINK_TIME;		// 通常時の紐づけ速度(秒).
		float						GOLDEN_TIME_LINK_TIME;	// ゴールデンタイム時の紐づけ速度(秒).
		float						LINK_END_TIME;			// 紐づけ解除時間(秒).
		int							START_LINK_NUM;			// 開始時の紐づけの数.
		std::vector<D3DXCOLOR3>		COLOR_LIST;				// アイコンの色リスト.

		void Load();
		stLinking() { Load(); }
	} typedef SLinking;

	// 煙の定数構造体.
	struct stSmoke {
		float		START_DISTANCE;				// 距離によるフェードを開始する距離.
		float		ALPHA_DISTANCE;				// アルファ値を減らしていく距離.
		float		MOVEVEC_RAND_ROT_MAX;		// 移動ベクトルのランダム回転の最大値(度).
		float		MOVEVEC_RAND_MOVEY_MAX;		// 移動ベクトルのランダムY座標移動の最大値.
		float		MOVEVEC_RAND_MOVEY_MIN;		// 移動ベクトルのランダムY座標移動の最小値.
		float		STARTPOS_RAND_MOVE_MAX;		// 開始座標のランダム横移動の最大値.
		float		SCALE_MAX;					// 最大サイズの倍率.
		float		EXPANSION_SPEED;			// 拡大速度(度/秒).
		float		SHRINK_SPEED;				// 縮小速度(度/秒).
		float		MOVE_SPEED;					// 移動速度.
		float		COOL_TIME;					// 煙と煙の間の時間(秒).
		float		ALPHA;						// アルファ値.
		D3DXCOLOR3	COLOR;						// 色(R,G,B)	.

		void Load();
		stSmoke() { Load(); }
	} typedef SSmoke;

	// ミニマップの定数構造体.
	struct stMiniMap {
		float		MAGNIFICATION;					// ミニマップの倍率.
		float		GRID_SPACE;						// グリッド線の間隔.
		float		DIRE_ICON_DISTANCE;				// 渡しに行くお客さんの方向のアイコンの表示する距離.
		float		DIRE_ICON_ALPHA_START_DISTANCE;	// 渡しに行くお客さんの方向のアイコンの透明にしていく開始距離.
		float		DIRE_ICON_ALPHA_DEC_DISTANCE;	// 渡しに行くお客さんの方向のアイコンの透明にしていく距離の間隔.
		int			ICON_CHANGE_MOVE_NO;			// アイコンを変更時に動かす数.

		void Load();
		stMiniMap() { Load(); }
	} typedef SMiniMap;

	// 通知の定数構造体.
	struct stNotification {
		float			DISP_TIME;					// 通知の表示時間(秒).
		float			ALPHA_SPEED;				// 透明/不透明にしていく速度.
		float			MOVE_SPEED;					// 通知の表示/非表示にする時の移動速度.
		float			SCALE_SPEED;				// 縮小していく速度.
		float			DISP_ICON_POSITION_Y;		// 通知を表示するY座標.
		D3DXPOSITION2	FONT_POSITION;				// フォントの座標.
		D3DXPOSITION2	ICON_POSITION;				// アイコンを表示する座標.

		void Load();
		stNotification() { Load(); }
	} typedef SNotification;

	// 時間の定数構造体.
	struct stTime {
		float								TIME_MAX;					// ゲームを終了させる経過時間.
		float								CLEAR_SCENE_MOVE_TIME_MAX;	// タイトルに移動させる経過時間.
		float								GAME_END_HOUR;				// ゲームを終了させる時間.
		float								CLEAR_SCENE_MOVE_HOUR;		// タイトルに移動させる時間.
		float								LIGHT_INTENSITY_MAX;		// ライトの強さの最大.
		float								LIGHT_INTENSITY_MIN;		// ライトの強さの最小.
		float								ONE_SECOND_TIME;			// 現実の1秒で進む時間(秒).
		int									START_HOUR;					// 開始時の時間(時).
		int									TIME_NOTIFICATION_MAX;		// 時間で送る通知の数.
		std::string							GOLDEN_TIME_MESSAGE;		// ゴールデンタイムになった時の通知文.
		std::string							NORMAL_TIME_MESSAGE;		// 通常の時間になった時の通知文.
		std::vector<std::pair<int, int>>	NOTIFICATION_TIMING;		// 通知を送る時間<時, 分>.
		std::vector<std::string>			NOTIFICATION_MESSAGE;		// 時間で送る通知文.
		std::vector<std::pair<int, int>>	NORMAL_TIME_HOUR;			// 通常の時の時間<開始, 終了>.
		std::vector<std::pair<int, int>>	GOLDEN_TIME_HOUR;			// ゴールデンタイムの時間<開始, 終了>.

		void Load();
		stTime() { Load(); }
	} typedef STime;

	// お店の詳細情報の定数構造体.
	struct stShopInfo {
		float			DISP_POSITION_Y;			// 表示するY座標.
		float			CLOSE_OVER_MOVE_Y;			// 閉じる時に大き目に移動する量.
		float			MONEY_FONT_POSITION_Y;		// お金の文字のY座標.
		float			DISTANCE_FONT_POSITION_Y;	// 距離の文字のY座標.
		float			ANIM_COOL_TIME;				// アニメーションのクールタイム(秒).
		int				SHOP_DISTANCE_ADJUS;		// お店の距離の調整値.
		D3DXPOSITION3	ICON_POSITION;				// アイコンの座標.
		D3DXPOSITION3	FONT_SCALE;					// 文字のサイズ.

		void Load();
		stShopInfo() { Load(); }
	} typedef SShopInfo;
}
