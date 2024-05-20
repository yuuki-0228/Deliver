#include "ConstantStruct.h"
#include "..\FileManager\FileManager.h"

// 変数名を文字列で取得マクロ.
#define TO_STRING( VariableName ) # VariableName

namespace {
	constexpr int _COMMENT	= 0; // コメントの位置.
	constexpr int _DATA		= 1; // データの位置.
	constexpr int _FIRST	= 1; // 1個目の位置.
	constexpr int _SECOND	= 2; // 2個目の位置.
	constexpr int _X		= 1; // xの位置.
	constexpr int _Y		= 2; // yの位置.
	constexpr int _Z		= 3; // zの位置.
	constexpr int _W		= 3; // wの位置.
	constexpr int _R		= 1; // 赤の位置.
	constexpr int _G		= 2; // 緑の位置.
	constexpr int _B		= 3; // 青の位置.
	constexpr int _A		= 4; // アルファ値の位置.

	// ファイルパスの取得.
	std::string GetFilePath( const std::string& Name ) {
		const std::string& FilePath = "Data\\Parameter\\Constant\\" + Name + ".json";
		CLog::PushLog( FilePath + " 読み込み ： 成功");
		return FilePath;
	}

	// 配列のサイズを取得.
	int GetSize( const json& j ) {
		// 一番最初にコメントがあるため要素数を1減らしておく.
		return static_cast<int>( j.size() ) - 1;
	}

	// 文字化けしないようにstd::stringに変換して取得.
	std::string GetString( const std::string& string ) {
		// jsonはUTF8なため一度文字列を変換する.
		std::wstring wString = StringConversion::to_wString( string, ECodePage::UTF8 );
		return StringConversion::to_String( wString );
	}
}

//---------------------------.
// プレイヤーの定数の読み込み関数.
//---------------------------.
void ConstantStruct::stPlayer::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Player" ) );
	JETPACK_POWER				= j[TO_STRING( JETPACK_POWER						)][_DATA];
	GRAVITY_POWER				= j[TO_STRING( GRAVITY_POWER						)][_DATA];
	POWER_TO_RESIST				= j[TO_STRING( POWER_TO_RESIST						)][_DATA];
	AIR_FRICTION				= j[TO_STRING( AIR_FRICTION							)][_DATA];
	GROUND_FRICTION				= j[TO_STRING( GROUND_FRICTION						)][_DATA];
	ACCELERATION_MIN			= j[TO_STRING( ACCELERATION_MIN						)][_DATA];
	ACCELERATION_MAX			= j[TO_STRING( ACCELERATION_MAX						)][_DATA];
	START_TILE					= j[TO_STRING( START_TILE							)][_DATA];
	END_TILE					= j[TO_STRING( END_TILE								)][_DATA];
	RETURN_TILT_SPEED			= j[TO_STRING( RETURN_TILT_SPEED					)][_DATA];
	MAX_ROTATION_Z				= j[TO_STRING( MAX_ROTATION_Z						)][_DATA];
	INERTIA_USAGE_RATE			= j[TO_STRING( INERTIA_USAGE_RATE					)][_DATA];
	JETPACK_ROT_SPEED			= j[TO_STRING( JETPACK_ROT_SPEED					)][_DATA];
	JETPACK_ROT_END				= j[TO_STRING( JETPACK_ROT_END						)][_DATA];
	VERTICAL_RAY_LENGTH			= j[TO_STRING( VERTICAL_RAY_LENGTH					)][_DATA];
	SIDE_RAY_LENGTH				= j[TO_STRING( SIDE_RAY_LENGTH						)][_DATA];
	GROUND_CLOSE_RAY_LENGTH		= j[TO_STRING( GROUND_CLOSE_RAY_LENGTH				)][_DATA];
	JETPACK_EFFECT_RAY_LENGTH	= j[TO_STRING( JETPACK_EFFECT_RAY_LENGTH			)][_DATA];
	CEILING_MOVE_SPEED			= j[TO_STRING( CEILING_MOVE_SPEED					)][_DATA];
	LANDING_EFFECT_TIME			= j[TO_STRING( LANDING_EFFECT_TIME					)][_DATA];
	LANDING_ANIM_SPPED			= j[TO_STRING( LANDING_ANIM_SPPED					)][_DATA];
	GET_FOOD_ANIM_SPPED			= j[TO_STRING( GET_FOOD_ANIM_SPPED					)][_DATA];
	LOOK_POS_ADD_Y				= j[TO_STRING( LOOK_POS_ADD_Y						)][_DATA];
	JETPACK_STOP_TIME			= j[TO_STRING( JETPACK_STOP_TIME					)][_DATA];
	JETPACK_STOP_CNT_DIFF		= j[TO_STRING( JETPACK_STOP_CNT_DIFF				)][_DATA];
	SHADOW_ALPHA				= j[TO_STRING( SHADOW_ALPHA							)][_DATA];
	DEAD_DOWN_POS_Y				= j[TO_STRING( DEAD_DOWN_POS_Y						)][_DATA];
	DEAD_UP_POS_Y				= j[TO_STRING( DEAD_UP_POS_Y						)][_DATA];
	DEAD_RADIUS					= j[TO_STRING( DEAD_RADIUS							)][_DATA];
	DEAD_GROUND_SPEED			= j[TO_STRING( DEAD_GROUND_SPEED					)][_DATA];
	DEAD_WALL_SPEED				= j[TO_STRING( DEAD_WALL_SPEED						)][_DATA];
	WARNING_SIZE				= j[TO_STRING( WARNING_SIZE							)][_DATA];
	LANDING_SE_PLAY_SPEED		= j[TO_STRING( LANDING_SE_PLAY_SPEED				)][_DATA];
	ADD_SE_VOLUME				= j[TO_STRING( ADD_SE_VOLUME						)][_DATA];
	VIBRATION_POWER				= j[TO_STRING( VIBRATION_POWER						)][_DATA];
	SHADOW_DISP_DISTANCE		= j[TO_STRING( SHADOW_DISP_DISTANCE					)][_DATA];
	STAGE_OUT_TIME				= j[TO_STRING( STAGE_OUT_TIME						)][_DATA];
	JETPACK_INPUT_LIST_MAX		= j[TO_STRING( JETPACK_INPUT_LIST_MAX				)][_DATA];
	STOP_CHANGE_NUM				= j[TO_STRING( STOP_CHANGE_NUM						)][_DATA];
	DOWN_WARNING_MESSAGE		= GetString( j[TO_STRING( DOWN_WARNING_MESSAGE		)][_DATA] );
	UP_WARNING_MESSAGE			= GetString( j[TO_STRING( UP_WARNING_MESSAGE		)][_DATA] );
	RADIUS_WARNING_MESSAGE		= GetString( j[TO_STRING( RADIUS_WARNING_MESSAGE	)][_DATA] );
	SHADOW_MIN_SCALE.x			= j[TO_STRING( SHADOW_MIN_SCALE						)][_DATA];
	SHADOW_MIN_SCALE.y			= SHADOW_MIN_SCALE.x;
	SHADOW_MIN_SCALE.z			= SHADOW_MIN_SCALE.x;
	SHADOW_ADD_ACALE.x			= j[TO_STRING( SHADOW_ADD_ACALE						)][_DATA];
	SHADOW_ADD_ACALE.y			= SHADOW_ADD_ACALE.x;
	SHADOW_ADD_ACALE.z			= SHADOW_ADD_ACALE.x;
}

//---------------------------.
// 渡しに行くお客さんの定数の読み込み関数.
//---------------------------.
void ConstantStruct::stCustomerDire::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "CustomerDire" ) );
	MOVE_ANIM_SPEED			= j[TO_STRING( MOVE_ANIM_SPEED			)][_DATA];
	DISTANCE				= j[TO_STRING( DISTANCE					)][_DATA];
	ALPHA_START_DISTANCE	= j[TO_STRING( ALPHA_START_DISTANCE		)][_DATA];
	ALPHA_DEC_DISTANCE		= j[TO_STRING( ALPHA_DEC_DISTANCE		)][_DATA];
	CUSTOMER_ADD_POS.x		= j[TO_STRING( CUSTOMER_ADD_POS			)][_X];
	CUSTOMER_ADD_POS.y		= j[TO_STRING( CUSTOMER_ADD_POS			)][_Y];
	CUSTOMER_ADD_POS.z		= j[TO_STRING( CUSTOMER_ADD_POS			)][_Z];

	const int SizeList_Size = GetSize( j[TO_STRING( SIZE_LIST )] ) / 3;
	SIZE_LIST.resize( SizeList_Size );
	for ( int Num = 0; Num < SizeList_Size; ++Num ) {
		const int No		= 1 + Num * 3;
		const int Pos_x		= No + 0;
		const int Pos_y		= No + 1;
		const int Pos_z		= No + 2;
		SIZE_LIST[Num].x	= j[TO_STRING( SIZE_LIST				)][Pos_x];
		SIZE_LIST[Num].y	= j[TO_STRING( SIZE_LIST				)][Pos_y];
		SIZE_LIST[Num].z	= j[TO_STRING( SIZE_LIST				)][Pos_z];
	}
	const int AlphaList_Size = GetSize( j[TO_STRING( ALPHA_LIST )] );
	ALPHA_LIST.resize( AlphaList_Size );
	for ( int i = 0; i < AlphaList_Size; ++i ) {
		const int No		= i + 1;
		ALPHA_LIST[i]		= j[TO_STRING( ALPHA_LIST				)][No];
	}
}

//---------------------------.
// フリーカメラ定数の読み込み関数.
//---------------------------.
void ConstantStruct::stFreeCamera::Load()
{
	json j = FileManager::JsonLoad( GetFilePath ( "FreeCamera" ) );
	MOVE_SPEED				= j[TO_STRING( MOVE_SPEED				)][_DATA];
	ROTATION_SPEED			= j[TO_STRING( ROTATION_SPEED			)][_DATA];
}

//---------------------------.
// カメラ定数の読み込み関数.
//---------------------------.
void ConstantStruct::stCamera::Load()
{
	json j = FileManager::JsonLoad( GetFilePath ( "Camera" ) );
	IS_REVERSE				= j[TO_STRING( IS_REVERSE				)][_DATA];
	INIT_ROTATION			= j[TO_STRING( INIT_ROTATION			)][_DATA];
	MOVABLE_MIN				= j[TO_STRING( MOVABLE_MIN				)][_DATA];
	MOVABLE_MAX				= j[TO_STRING( MOVABLE_MAX				)][_DATA];
	INIT_RETURN_VALUE		= j[TO_STRING( INIT_RETURN_VALUE		)][_DATA];
	INIT_ADD_VALUE			= j[TO_STRING( INIT_ADD_VALUE			)][_DATA];
	ADJUST_ANGLE			= j[TO_STRING( ADJUST_ANGLE				)][_DATA];
	ADD_DELAY_ANGLE			= j[TO_STRING( ADD_DELAY_ANGLE			)][_DATA];
	INVALID_LENGTH			= j[TO_STRING( INVALID_LENGTH			)][_DATA];
}

//---------------------------.
// リュックの定数の読み込み関数.
//---------------------------.
void ConstantStruct::stRuckSack::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "RuckSack" ) );
	DISCOUNT_SPPED			= j[TO_STRING( DISCOUNT_SPPED			)][_DATA];
	COOLDOWN_SPEED			= j[TO_STRING( COOLDOWN_SPEED			)][_DATA];
	MAX_DISCOUNT			= j[TO_STRING( MAX_DISCOUNT				)][_DATA];
	MIN_DISCOUNT			= j[TO_STRING( MIN_DISCOUNT				)][_DATA];
	GAUGE_LR_SPPED			= j[TO_STRING( GAUGE_LR_SPPED			)][_DATA];
	GAUGE_UD_SPPED			= j[TO_STRING( GAUGE_UD_SPPED			)][_DATA];
	GAUGE_CENTER_SPPED		= j[TO_STRING( GAUGE_CENTER_SPPED		)][_DATA];
	UD_GAUGE_DOWN_SPEED		= j[TO_STRING( UD_GAUGE_DOWN_SPEED		)][_DATA];
	MAX_TEMPERATURE			= j[TO_STRING( MAX_TEMPERATURE			)][_DATA];
	MIN_TEMPERATURE			= j[TO_STRING( MIN_TEMPERATURE			)][_DATA];
	MAX_GAUGE				= j[TO_STRING( MAX_GAUGE				)][_DATA];
	INIT_GAUGE				= j[TO_STRING( INIT_GAUGE				)][_DATA];
	MIN_GAUGE				= j[TO_STRING( MIN_GAUGE				)][_DATA];
	BAR_MOVE_SCALE			= j[TO_STRING( BAR_MOVE_SCALE			)][_DATA];
}

//---------------------------.
// 風の定数の読み込み関数.
//---------------------------.
void ConstantStruct::stWind::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Wind" ) );
	WIND_TIME				= j[TO_STRING( WIND_TIME					)][_DATA];
	WIND_STRONG_SPEED		= j[TO_STRING( WIND_STRONG_SPEED			)][_DATA];
	WIND_WEAK_SPEED			= j[TO_STRING( WIND_WEAK_SPEED				)][_DATA];
	WIND_HEIGHT_POWER		= j[TO_STRING( WIND_HEIGHT_POWER			)][_DATA];
	ADD_SE_VOLUME			= j[TO_STRING( ADD_SE_VOLUME				)][_DATA];
	WIND_MESSAGE			= GetString( j[TO_STRING( WIND_MESSAGE		)][_DATA] );
	WIND_STOP_MESSAGE		= GetString( j[TO_STRING( WIND_STOP_MESSAGE )][_DATA] );
}

void ConstantStruct::stWindUI::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "WindUI" ) );
	WIND_LINE_DISP_SPEED			= j[TO_STRING( WIND_LINE_DISP_SPEED				)][_DATA];
	WIND_LINE_SPPED_MIN				= j[TO_STRING( WIND_LINE_SPPED					)][_FIRST];
	WIND_LINE_SPPED_MAX				= j[TO_STRING( WIND_LINE_SPPED					)][_SECOND];
	WIND_LINE_SCALE_MIN				= j[TO_STRING( WIND_LINE_SCALE					)][_FIRST];
	WIND_LINE_SCALE_MAX				= j[TO_STRING( WIND_LINE_SCALE					)][_SECOND];
	WIND_LINE_ALPHA_MIN				= j[TO_STRING( WIND_LINE_ALPHA					)][_FIRST];
	WIND_LINE_ALPHA_MAX				= j[TO_STRING( WIND_LINE_ALPHA					)][_SECOND];
	WIND_LINE_WIND_POWER_ADD_SPPED	= j[TO_STRING( WIND_LINE_WIND_POWER_ADD_SPPED	)][_DATA];
}

//---------------------------.
// ステージの定数の読み込み関数.
//---------------------------.
void ConstantStruct::stStage::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Stage" ) );
	CLOUD_DOWN_POS_Y		= j[TO_STRING( CLOUD_DOWN_POS_Y			)][_DATA];
	CLOUD_INTERVAL			= j[TO_STRING( CLOUD_INTERVAL			)][_DATA];
	CLOUD_MAX_SPEED			= j[TO_STRING( CLOUD_MAX_SPEED			)][_DATA];
	CLOUD_SCALE				= j[TO_STRING( CLOUD_SCALE				)][_DATA];
}

//---------------------------.
// マンションの定数の読み込み関数.
//---------------------------.
void ConstantStruct::stApartment::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Apartment" ) );
	START_DISTANCE			= j[TO_STRING( START_DISTANCE			)][_DATA];
	ALPHA_DISTANCE			= j[TO_STRING( ALPHA_DISTANCE			)][_DATA];
	CAMERA_ALPHA_MIN		= j[TO_STRING( CAMERA_ALPHA_MIN			)][_DATA];
	CAMERA_ALPHA_MAX		= j[TO_STRING( CAMERA_ALPHA_MAX			)][_DATA];
	CAMERA_ALPHA_SPEED		= j[TO_STRING( CAMERA_ALPHA_SPEED		)][_DATA];
	APARTMENT_ADD_MONEY		= j[TO_STRING( APARTMENT_ADD_MONEY		)][_DATA];
	FLOOR_SIZE				= j[TO_STRING( FLOOR_SIZE				)][_DATA];
	MOVE_SPEED				= j[TO_STRING( MOVE_SPEED				)][_DATA];
	WINDOW_MOVE_SPEED		= j[TO_STRING( WINDOW_MOVE_SPEED		)][_DATA];
	ROT_SPPED				= j[TO_STRING( ROT_SPPED				)][_DATA];
	START_NOROOM_FLOOR_NUM	= j[TO_STRING( START_NOROOM_FLOOR_NUM	)][_DATA];
}

//---------------------------.
// ピザ窯の炎の定数の読み込み関数.
//---------------------------.
void ConstantStruct::stPizzaKilnFire::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "PizzaKilnFire" ) );
	START_DISTANCE			= j[TO_STRING( START_DISTANCE			)][_DATA];
	ALPHA_DISTANCE			= j[TO_STRING( ALPHA_DISTANCE			)][_DATA];
	ALPHA					= j[TO_STRING( ALPHA					)][_DATA];
	ADD_COLOR_SPEED			= j[TO_STRING( ADD_COLOR_SPEED			)][_DATA];
	START_COLOR.x			= j[TO_STRING( START_COLOR				)][_R];
	START_COLOR.y			= j[TO_STRING( START_COLOR				)][_G];
	START_COLOR.z			= j[TO_STRING( START_COLOR				)][_B];
	END_COLOR.x				= j[TO_STRING( END_COLOR				)][_R];
	END_COLOR.y				= j[TO_STRING( END_COLOR				)][_G];
	END_COLOR.z				= j[TO_STRING( END_COLOR				)][_B];
}

//---------------------------.
// 浮島の定数構造体.
//---------------------------.
void ConstantStruct::stIsland::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Island" ) );
	START_DISTANCE			= j[TO_STRING( START_DISTANCE			)][_DATA];
	ALPHA_DISTANCE			= j[TO_STRING( ALPHA_DISTANCE			)][_DATA];
	CAMERA_ALPHA_MIN		= j[TO_STRING( CAMERA_ALPHA_MIN			)][_DATA];
	CAMERA_ALPHA_MAX		= j[TO_STRING( CAMERA_ALPHA_MAX			)][_DATA];
	CAMERA_ALPHA_SPEED		= j[TO_STRING( CAMERA_ALPHA_SPEED		)][_DATA];
	MOVE_SPEED				= j[TO_STRING( MOVE_SPEED				)][_DATA];
	DOOR_MOVE_SPEED			= j[TO_STRING( DOOR_MOVE_SPEED			)][_DATA];
	ROT_SPPED				= j[TO_STRING( ROT_SPPED				)][_DATA];
	COLL_STOP_SPHERE_RADIUS = j[TO_STRING( COLL_STOP_SPHERE_RADIUS	)][_DATA];
}

//---------------------------.
// お客さんと店員の定数の読み込み関数.
//---------------------------.
void ConstantStruct::stDeliveryCharacter::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "DeliveryCharacter" ) );
	START_DISTANCE				= j[TO_STRING( START_DISTANCE				)][_DATA];
	ALPHA_DISTANCE				= j[TO_STRING( ALPHA_DISTANCE				)][_DATA];
	SPHERE_SIZE					= j[TO_STRING( SPHERE_SIZE					)][_DATA];
	CYLINDER_RADIUS				= j[TO_STRING( CYLINDER_RADIUS				)][_DATA];
	CYLINDER_HEIGHT				= j[TO_STRING( CYLINDER_HEIGHT				)][_DATA];
	CYLINDER_ADD_POS_Y			= j[TO_STRING( CYLINDER_ADD_POS_Y			)][_DATA];
	GAUGE_SPEED					= j[TO_STRING( GAUGE_SPEED					)][_DATA];
	GAUGE_DEC_SPEED				= j[TO_STRING( GAUGE_DEC_SPEED				)][_DATA];
	GAUGE_DELETE_TIME			= j[TO_STRING( GAUGE_DELETE_TIME			)][_DATA];
	GAUGE_SPEED_UP				= j[TO_STRING( GAUGE_SPEED_UP				)][_DATA];
	DETACH_COOL_TIME			= j[TO_STRING( DETACH_COOL_TIME				)][_DATA];
	MONEY						= j[TO_STRING( MONEY						)][_DATA];
	HIGHT_MONEY_INTERVAL		= j[TO_STRING( HIGHT_MONEY_INTERVAL			)][_DATA];
	HIGHT_ADD_MONEY				= j[TO_STRING( HIGHT_ADD_MONEY				)][_DATA];
	DISTANCE_MONEY_INTERVAL		= j[TO_STRING( DISTANCE_MONEY_INTERVAL		)][_DATA];
	WAVEHANDS_ANIMATION_SPEED	= j[TO_STRING( WAVEHANDS_ANIMATION_SPEED	)][_DATA];
	MONEY_MAX					= j[TO_STRING( MONEY_MAX					)][_DATA];
	NORMAL_MESSAGE_SIZE		= GetSize( j[TO_STRING( NORMAL_MESSAGE )] );
	NORMAL_MESSAGE.resize( NORMAL_MESSAGE_SIZE );
	for ( int i = 0; i < NORMAL_MESSAGE_SIZE; ++i ) {
		const int No		= i + 1;
		NORMAL_MESSAGE[i]	= GetString( j[TO_STRING( NORMAL_MESSAGE )][No] );
	}
	HOT_MESSAGE_SIZE		= GetSize( j[TO_STRING( HOT_MESSAGE )] );
	HOT_MESSAGE.resize( HOT_MESSAGE_SIZE );
	for ( int i = 0; i < HOT_MESSAGE_SIZE; ++i ) {
		const int No		= i + 1;
		HOT_MESSAGE[i]		= GetString( j[TO_STRING( HOT_MESSAGE )][No] );
	}
	MESSY_MESSAGE_SIZE		= GetSize( j[TO_STRING( MESSY_MESSAGE )] );
	MESSY_MESSAGE.resize( MESSY_MESSAGE_SIZE );
	for ( int i = 0; i < MESSY_MESSAGE_SIZE; ++i ) {
		const int No		= i + 1;
		MESSY_MESSAGE[i]	= GetString( j[TO_STRING( MESSY_MESSAGE )][No] );
	}
}

//---------------------------.
// 紐づけの定数の読み込み関数.
//---------------------------.
void ConstantStruct::stLinking::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Linking" ) );
	NORMAL_LINK_TIME		= j[TO_STRING( NORMAL_LINK_TIME			)][_DATA];
	GOLDEN_TIME_LINK_TIME	= j[TO_STRING( GOLDEN_TIME_LINK_TIME	)][_DATA];
	LINK_END_TIME			= j[TO_STRING( LINK_END_TIME			)][_DATA];
	START_LINK_NUM			= j[TO_STRING( START_LINK_NUM			)][_DATA];

	const int Size = GetSize( j[TO_STRING( COLOR_LIST )] ) / 3;
	COLOR_LIST.resize( Size );
	for ( int Num = 0; Num < Size; ++Num ) {
		const int No		= 1 + Num * 3;
		const int Pos_x		= No + 0;
		const int Pos_y		= No + 1;
		const int Pos_z		= No + 2;
		COLOR_LIST[Num].x	= j[TO_STRING( COLOR_LIST				)][Pos_x];
		COLOR_LIST[Num].y	= j[TO_STRING( COLOR_LIST				)][Pos_y];
		COLOR_LIST[Num].z	= j[TO_STRING( COLOR_LIST				)][Pos_z];
	}
}

//---------------------------.
// 煙定数の読み込み関数.
//---------------------------.
void ConstantStruct::stSmoke::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Smoke" ) );
	START_DISTANCE			= j[TO_STRING( START_DISTANCE			)][_DATA];
	ALPHA_DISTANCE			= j[TO_STRING( ALPHA_DISTANCE			)][_DATA];
	MOVEVEC_RAND_ROT_MAX	= j[TO_STRING( MOVEVEC_RAND_ROT_MAX		)][_DATA];
	MOVEVEC_RAND_MOVEY_MAX	= j[TO_STRING( MOVEVEC_RAND_MOVEY_MAX	)][_DATA];
	MOVEVEC_RAND_MOVEY_MIN	= j[TO_STRING( MOVEVEC_RAND_MOVEY_MIN	)][_DATA];
	STARTPOS_RAND_MOVE_MAX	= j[TO_STRING( STARTPOS_RAND_MOVE_MAX	)][_DATA];
	SCALE_MAX				= j[TO_STRING( SCALE_MAX				)][_DATA];
	EXPANSION_SPEED			= j[TO_STRING( EXPANSION_SPEED			)][_DATA];
	SHRINK_SPEED			= j[TO_STRING( SHRINK_SPEED				)][_DATA];
	MOVE_SPEED				= j[TO_STRING( MOVE_SPEED				)][_DATA];
	COOL_TIME				= j[TO_STRING( COOL_TIME				)][_DATA];
	ALPHA					= j[TO_STRING( ALPHA					)][_DATA];
	COLOR.x					= j[TO_STRING( COLOR					)][_R];
	COLOR.y					= j[TO_STRING( COLOR					)][_G];
	COLOR.z					= j[TO_STRING( COLOR					)][_B];
}

//---------------------------.
// ミニマップ定数の読み込み関数.
//---------------------------.
void ConstantStruct::stMiniMap::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "MiniMap" ) );
	MAGNIFICATION					= j[TO_STRING( MAGNIFICATION					)][_DATA];
	GRID_SPACE						= j[TO_STRING( GRID_SPACE						)][_DATA];
	DIRE_ICON_DISTANCE				= j[TO_STRING( DIRE_ICON_DISTANCE				)][_DATA];
	DIRE_ICON_ALPHA_START_DISTANCE	= j[TO_STRING( DIRE_ICON_ALPHA_START_DISTANCE	)][_DATA];
	DIRE_ICON_ALPHA_DEC_DISTANCE	= j[TO_STRING( DIRE_ICON_ALPHA_DEC_DISTANCE		)][_DATA];
	ICON_CHANGE_MOVE_NO				= j[TO_STRING( ICON_CHANGE_MOVE_NO				)][_DATA];
}

//---------------------------.
// 通知定数の読み込み関数.
//---------------------------.
void ConstantStruct::stNotification::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Notification" ) );
	DISP_TIME				= j[TO_STRING( DISP_TIME				)][_DATA];
	ALPHA_SPEED				= j[TO_STRING( ALPHA_SPEED				)][_DATA];
	MOVE_SPEED				= j[TO_STRING( MOVE_SPEED				)][_DATA];
	SCALE_SPEED				= j[TO_STRING( SCALE_SPEED				)][_DATA];
	DISP_ICON_POSITION_Y	= j[TO_STRING( DISP_ICON_POSITION_Y		)][_DATA];
	FONT_POSITION.x			= j[TO_STRING( FONT_POSITION			)][_DATA];
	FONT_POSITION.y			= j[TO_STRING( FONT_POSITION			)][_DATA];
	ICON_POSITION.x			= j[TO_STRING( ICON_POSITION			)][_X];
	ICON_POSITION.y			= j[TO_STRING( ICON_POSITION			)][_Y];
}

//---------------------------.
// 時間定数の読み込み関数.
//---------------------------.
void ConstantStruct::stTime::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Time" ) );
	TIME_MAX					= j[TO_STRING( TIME_MAX							)][_DATA];
	CLEAR_SCENE_MOVE_TIME_MAX	= j[TO_STRING( CLEAR_SCENE_MOVE_TIME_MAX		)][_DATA];
	GAME_END_HOUR				= j[TO_STRING( GAME_END_HOUR					)][_DATA];
	CLEAR_SCENE_MOVE_HOUR		= j[TO_STRING( CLEAR_SCENE_MOVE_HOUR			)][_DATA];
	LIGHT_INTENSITY_MAX			= j[TO_STRING( LIGHT_INTENSITY_MAX				)][_DATA];
	LIGHT_INTENSITY_MIN			= j[TO_STRING( LIGHT_INTENSITY_MIN				)][_DATA];
	ONE_SECOND_TIME				= j[TO_STRING( ONE_SECOND_TIME					)][_DATA];
	START_HOUR					= j[TO_STRING( START_HOUR						)][_DATA];
	GOLDEN_TIME_MESSAGE			= GetString( j[TO_STRING( GOLDEN_TIME_MESSAGE	)][_DATA] );
	NORMAL_TIME_MESSAGE			= GetString( j[TO_STRING( NORMAL_TIME_MESSAGE	)][_DATA] );

	TIME_NOTIFICATION_MAX	= GetSize( j[TO_STRING( NOTIFICATION_TIMING )] ) / 2;
	NOTIFICATION_TIMING.resize( TIME_NOTIFICATION_MAX );
	for ( int i = 0; i < TIME_NOTIFICATION_MAX; ++i ) {
		const int FirstPos				= i * 2 + _FIRST;
		const int SecondPos				= i * 2 + _SECOND;
		NOTIFICATION_TIMING[i].first	= j[TO_STRING( NOTIFICATION_TIMING	)][FirstPos];
		NOTIFICATION_TIMING[i].second	= j[TO_STRING( NOTIFICATION_TIMING	)][SecondPos];
	}

	NOTIFICATION_MESSAGE.resize( TIME_NOTIFICATION_MAX );
	for ( int i = 0; i < TIME_NOTIFICATION_MAX; ++i ) {
		const int No					= i + 1;
		NOTIFICATION_MESSAGE[i]			= GetString( j[TO_STRING( NOTIFICATION_MESSAGE )][No] );
	}

	const int Normal_Size	= GetSize( j[TO_STRING( NORMAL_TIME_HOUR )] ) / 2;
	NORMAL_TIME_HOUR.resize( Normal_Size );
	for ( int i = 0; i < Normal_Size; ++i ) {
		const int FirstPos				= i * 2 + _FIRST;
		const int SecondPos				= i * 2 + _SECOND;
		NORMAL_TIME_HOUR[i].first		= j[TO_STRING( NORMAL_TIME_HOUR		)][FirstPos];
		NORMAL_TIME_HOUR[i].second		= j[TO_STRING( NORMAL_TIME_HOUR		)][SecondPos];
	}

	const int Golden_Size	= GetSize( j[TO_STRING( GOLDEN_TIME_HOUR )] ) / 2;
	GOLDEN_TIME_HOUR.resize( Golden_Size );
	for ( int i = 0; i < Golden_Size; ++i ) {
		const int FirstPos				= i * 2 + _FIRST;
		const int SecondPos				= i * 2 + _SECOND;
		GOLDEN_TIME_HOUR[i].first		= j[TO_STRING( GOLDEN_TIME_HOUR		)][FirstPos];
		GOLDEN_TIME_HOUR[i].second		= j[TO_STRING( GOLDEN_TIME_HOUR		)][SecondPos];
	}
}

//---------------------------.
// お店の詳細情報の定数構造体.
//---------------------------.
void ConstantStruct::stShopInfo::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "ShopInfo" ) );
	DISP_POSITION_Y						= j[TO_STRING( DISP_POSITION_Y			)][_DATA];
	CLOSE_OVER_MOVE_Y					= j[TO_STRING( CLOSE_OVER_MOVE_Y		)][_DATA];
	MONEY_FONT_POSITION_Y				= j[TO_STRING( MONEY_FONT_POSITION_Y	)][_DATA];
	DISTANCE_FONT_POSITION_Y			= j[TO_STRING( DISTANCE_FONT_POSITION_Y )][_DATA];
	ANIM_COOL_TIME						= j[TO_STRING( ANIM_COOL_TIME			)][_DATA];
	SHOP_DISTANCE_ADJUS					= j[TO_STRING( SHOP_DISTANCE_ADJUS		)][_DATA];
	ICON_POSITION.x						= j[TO_STRING( ICON_POSITION			)][_X];
	ICON_POSITION.y						= j[TO_STRING( ICON_POSITION			)][_Y];
	ICON_POSITION.z						= j[TO_STRING( ICON_POSITION			)][_Z];
	FONT_SCALE.x						= j[TO_STRING( FONT_SCALE				)][_X];
	FONT_SCALE.y						= j[TO_STRING( FONT_SCALE				)][_Y];
	FONT_SCALE.z						= j[TO_STRING( FONT_SCALE				)][_Z];
}
