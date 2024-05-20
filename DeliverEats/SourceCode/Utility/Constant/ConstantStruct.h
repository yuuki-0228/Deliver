#pragma once
#include "..\..\Global.h"

/************************************************
*	�萔�\����.
**/
namespace ConstantStruct {
	// �v���C���[�̒萔�\����.
	struct stPlayer {
		float			JETPACK_POWER;				// �W�F�b�g�p�b�N�̗�.
		float			GRAVITY_POWER;				// �d�̗͂�.
		float			POWER_TO_RESIST;			// ��R�����.
		float			AIR_FRICTION;				// ��C���C.
		float			GROUND_FRICTION;			// �n�ʖ��C.
		float			ACCELERATION_MIN;			// �����x�̉���.
		float			ACCELERATION_MAX;			// �����x�̏��.
		float			START_TILE;					// �v���C���[���X���Ă����J�n�X��.
		float			END_TILE;					// �v���C���[�̌X���̏��(�x).
		float			RETURN_TILT_SPEED;			// �X����߂����x(�x).
		float			MAX_ROTATION_Z;				// �v���C���[��Z�̉�]�̏��(�x).
		float			INERTIA_USAGE_RATE;			// �����̎g�p���銄��(0.0f ~ 1.0f).
		float			JETPACK_ROT_SPEED;			// �W�F�b�g�p�b�N�̉�]���x(�x).
		float			JETPACK_ROT_END;			// �W�F�b�g�p�b�N�̉�]�̏I���p�x(�x).
		float			VERTICAL_RAY_LENGTH;		// �c�̃��C�̒���.
		float			SIDE_RAY_LENGTH;			// ���̃��C�̒���.
		float			GROUND_CLOSE_RAY_LENGTH;	// �n�ʂɋ߂������ׂ�p�̃��C�̒���.
		float			JETPACK_EFFECT_RAY_LENGTH;	// �W�F�b�g�p�b�N�̃G�t�F�N�g�p�̃��C�̒���.
		float			CEILING_MOVE_SPEED;			// �V��ɂԂ��������̈ړ����x(0�ɋ߂Â��قǑ����Ȃ�).
		float			LANDING_EFFECT_TIME;		// ���n�G�t�F�N�g���Đ����Ă����Ԋu�̎���(�b).
		float			LANDING_ANIM_SPPED;			// ���n�A�j���[�V�������x(�x/�b).
		float			GET_FOOD_ANIM_SPPED;		// ���i�擾�A�j���[�V�������x(�x/�b).
		float			LOOK_POS_ADD_Y;				// �X��/���q���񂪌���ʒu�̒ǉ�����Y���W.
		float			JETPACK_STOP_TIME;			// �W�F�b�g�p�b�N���~�����鎞��(�b).
		float			JETPACK_STOP_CNT_DIFF;		// ��~������J�E���g��ǉ�����ω��̍�(0.0f ~ 1.0f).
		float			SHADOW_ALPHA;				// �e�̃A���t�@�l.
		float			DEAD_DOWN_POS_Y;			// ���U���g�Ɉړ������鉺����Y���W.
		float			DEAD_UP_POS_Y;				// ���U���g�Ɉړ�������㑤��Y���W.
		float			DEAD_RADIUS;				// ���U���g�Ɉړ������锼�a.
		float			DEAD_GROUND_SPEED;			// ���U���g�Ɉړ�������n�ʂ̏Փˑ��x.
		float			DEAD_WALL_SPEED;			// ���U���g�Ɉړ�������ǂ̏Փˑ��x.
		float			WARNING_SIZE;				// �x�����o���͈͂̃T�C�Y.
		float			LANDING_SE_PLAY_SPEED;		// ���nSE���Đ����闎�����x.
		float			ADD_SE_VOLUME;				// SE�̑��₵�Ă�������(�b).
		float			VIBRATION_POWER;			// �U���̎g�p��.
		float			SHADOW_DISP_DISTANCE;		// �e��\�����鋗��.
		float			STAGE_OUT_TIME;				// �X�e�[�W�O�Ɉړ��������̉��o�̎���(�b).
		int				JETPACK_INPUT_LIST_MAX;		// �W�F�b�g�p�b�N�̓��̓��X�g�̏��.
		int				STOP_CHANGE_NUM;			// �W�F�b�g�p�b�N���~������ω�������.
		std::string		DOWN_WARNING_MESSAGE;		// �������̌x���̒ʒm��.
		std::string		UP_WARNING_MESSAGE;			// ������̌x���̒ʒm��.
		std::string		RADIUS_WARNING_MESSAGE;		// ���a�ȏ�̎��̌x���̒ʒm��.
		D3DXSCALE3		SHADOW_MIN_SCALE;			// �e�̍ŏ��T�C�Y.
		D3DXSCALE3		SHADOW_ADD_ACALE;			// �e�̒ǉ��T�C�Y.

		void Load();
		stPlayer() { Load(); }
	} typedef SPlayer;

	// �n���ɍs�����q����̕����̒萔�\����.
	struct stCustomerDire {
		float						MOVE_ANIM_SPEED;		// �ړ��A�j���[�V�������x(�x/�b).
		float						DISTANCE;				// ���̋���.
		float						ALPHA_START_DISTANCE;	// �A���t�@�l�����炵�Ă����J�n����.
		float						ALPHA_DEC_DISTANCE;		// �A���t�@�l�����炵�Ă�������.
		D3DXPOSITION3				CUSTOMER_ADD_POS;		// ���q����̍��W�ɒǉ�������W.
		std::vector<D3DXSCALE3>		SIZE_LIST;				// ���̃T�C�Y���X�g(2��).
		std::vector<float>			ALPHA_LIST;				// �A���t�@�l���X�g(2��).

		void Load();
		stCustomerDire() { Load(); }
	} typedef SCustomerDire;

	// �t���[�J�����̒萔�\����.
	struct stFreeCamera {
		float	MOVE_SPEED;				// �J�����̈ړ����x.
		float	ROTATION_SPEED;			// �J�����̉�]���x.

		void Load();
		stFreeCamera() { Load(); }
	} typedef SFreeCamera;

	// �J�����̒萔�\����.
	struct stCamera {
		bool	IS_REVERSE;				// �X�e�B�b�N�̓��͂𔽓]�����邩?
		float	INIT_ROTATION;			// �J�����̊p�x�̏����l.
		float	MOVABLE_MIN;			// �X������ŏ��p�x.
		float	MOVABLE_MAX;			// �X������ő�p�x.
		float	INIT_RETURN_VALUE;		// ���̊p�x�ɖ߂��Ƃ��ɉ��Z����l�̏����l.
		float	INIT_ADD_VALUE;			// ���̊p�x�ɖ߂��Ƃ��ɉ��Z����l�ɉ��Z����l�̏����l.
		float	ADJUST_ANGLE;			// �J�����̉�]�p�x�̒����l.
		float	ADD_DELAY_ANGLE;		// �^�[�Q�b�g�̏��~�ɂ���ĉ�]�p�x�ɉ��Z�����l.
		float	INVALID_LENGTH;			// �����J�����𖳌������鋗��.

		void Load();
		stCamera() { Load(); }
	} typedef SCamera;

	// �����b�N�̒萔�\����.
	struct stRuckSack {
		float	DISCOUNT_SPPED;			// �������x(%/�b).
		float	COOLDOWN_SPEED;			// ���x���܂��Ă������x(��/�b).
		float	MAX_DISCOUNT;			// �ō��̌X��������.
		float	MIN_DISCOUNT;			// �Œ�̌X��������.
		float	GAUGE_LR_SPPED;			// ���E�Q�[�W�̐i�ޑ��x.
		float	GAUGE_UD_SPPED;			// �㉺�Q�[�W�̐i�ޑ��x.
		float	GAUGE_CENTER_SPPED;		// �Q�[�W�̖߂鑬�x.
		float	UD_GAUGE_DOWN_SPEED;	// �㉺�Q�[�W�̑��x���������Ă������x.
		float	MAX_TEMPERATURE;		// �ő�̉��x.
		float	MIN_TEMPERATURE;		// �Œ�̉��x.
		float	MAX_GAUGE;				// �ő�̃Q�[�W�̈ʒu.
		float	INIT_GAUGE;				// �����̃Q�[�W�̈ʒu.
		float	MIN_GAUGE;				// �Œ�̃Q�[�W�̈ʒu.
		float	BAR_MOVE_SCALE;			// �o�[�̓����Ɋ|���鐔�l.

		void Load();
		stRuckSack() { Load(); }
	} typedef SRuckSack;

	// ���̒萔�\����.
	struct stWind {
		float			WIND_TIME;			// ������������~�܂����肷�鎞��(�b).
		float			WIND_STRONG_SPEED;	// ���������Ȃ��Ă������x(�b).
		float			WIND_WEAK_SPEED;	// �����キ�Ȃ��Ă������x(�b).
		float			WIND_HEIGHT_POWER;	// ���̍����ɂ�鍂���̋���.
		float			ADD_SE_VOLUME;		// SE�̉��ʂ����Z���Ă�����(�b).
		std::string		WIND_MESSAGE;		// ���������n�߂����̒ʒm��.
		std::string		WIND_STOP_MESSAGE;	// �����~�܂������̒ʒm��.

		void Load();
		stWind() { Load(); }
	} typedef SWind;

	// ��UI�̒萔�\����.
	struct stWindUI {
		float			WIND_LINE_DISP_SPEED;			// �_���o�������Ă������x(�b).
		float			WIND_LINE_SPPED_MIN;			// ���̐��̑��x�̍ŏ��l.
		float			WIND_LINE_SPPED_MAX;			// ���̐��̑��x�̍ő�l.
		float			WIND_LINE_SCALE_MIN;			// ���̐��̊g�k�̍ŏ��l.
		float			WIND_LINE_SCALE_MAX;			// ���̐��̊g�k�̍ő�l.
		float			WIND_LINE_ALPHA_MIN;			// ���̐��̃A���t�@�̍ŏ��l.
		float			WIND_LINE_ALPHA_MAX;			// ���̐��̃A���t�@�̍ő�l.
		float			WIND_LINE_WIND_POWER_ADD_SPPED;	// ���̐��̕��̋����ɂ���Ēǉ����鑬�x.
		void Load();
		stWindUI() { Load(); }
	} typedef SWindUI;

	// �X�e�[�W�̒萔�\����.
	struct stStage {
		float	CLOUD_DOWN_POS_Y;			// ��ԉ��̉_��Y���W.
		float	CLOUD_INTERVAL;				// �_��z�u���Ă����Ԋu.
		float	CLOUD_MAX_SPEED;			// �_�̍ő��UV�X�N���[�����x.
		float	CLOUD_SCALE;				// �_�̑傫��.

		void Load();
		stStage() { Load(); }
	} typedef SStage;

	// �}���V�����̒萔�\����.
	struct stApartment {
		float	START_DISTANCE;				// �����ɂ��t�F�[�h���J�n���鋗��.
		float	ALPHA_DISTANCE;				// �A���t�@�l�����炵�Ă�������.
		float	CAMERA_ALPHA_MIN;			// �J�����ɂ��A���t�@�l�̍ŏ��l.
		float	CAMERA_ALPHA_MAX;			// �J�����ɂ��A���t�@�l�̍ő�l.
		float	CAMERA_ALPHA_SPEED;			// �J�����ɂ��A���t�@�̑��x.
		float	APARTMENT_ADD_MONEY;		// �}���V�����^�C�v�̒ǉ����邨��.
		float	FLOOR_SIZE;					// �}���V������1�t���A�̃T�C�Y.
		float	MOVE_SPEED;					// ���q����̈ړ����x(�ړ��l/�b).
		float	WINDOW_MOVE_SPEED;			// ���̈ړ����x(�ړ��l/�b).
		float	ROT_SPPED;					// ���q����̉�]���x(�x/�b).
		int		START_NOROOM_FLOOR_NUM;		// �J�n�̕����̖����t���A�̐�.

		void Load();
		stApartment() { Load(); }
	} typedef SApartment;

	// �s�U�q�̉��̒萔�\����.
	struct stPizzaKilnFire {
		float		START_DISTANCE;			// �����ɂ��t�F�[�h���J�n���鋗��.
		float		ALPHA_DISTANCE;			// �A���t�@�l�����炵�Ă�������.
		float		ALPHA;					// �A���t�@�l.
		float		ADD_COLOR_SPEED;		// ���Z���Ă����F�̕ω����x(�x/�b).
		D3DXCOLOR3	START_COLOR;			// �J�n���̐F.
		D3DXCOLOR3	END_COLOR;				// �I�����̐F.

		void Load();
		stPizzaKilnFire() { Load(); }
	} typedef SPizzaKilnFire;

	// �����̒萔�\����.
	struct stIsland {
		float	START_DISTANCE;				// �����ɂ��t�F�[�h���J�n���鋗��.
		float	ALPHA_DISTANCE;				// �A���t�@�l�����炵�Ă�������.
		float	CAMERA_ALPHA_MIN;			// �J�����ɂ��A���t�@�l�̍ŏ��l.
		float	CAMERA_ALPHA_MAX;			// �J�����ɂ��A���t�@�l�̍ő�l.
		float	CAMERA_ALPHA_SPEED;			// �J�����ɂ��A���t�@�̑��x.
		float	MOVE_SPEED;					// ���q����̈ړ����x(�ړ��l/�b).
		float	DOOR_MOVE_SPEED;			// ���̈ړ����x(�ړ��l/�b).
		float	ROT_SPPED;					// ���q����̉�]���x(�x/�b).
		float	COLL_STOP_SPHERE_RADIUS;	// �����蔻����s�������f���鋅�̂̓����蔻��̔��a.

		void Load();
		stIsland() { Load(); }
	} typedef SIsland;

	// ���q����ƓX���̒萔�\����.
	struct stDeliveryCharacter {
		float						START_DISTANCE;				// �����ɂ��t�F�[�h���J�n���鋗��.
		float						ALPHA_DISTANCE;				// �A���t�@�l�����炵�Ă�������.
		float						SPHERE_SIZE;				// ���̃T�C�Y(�Ŋ��̂��X/���q����ȂǂŎg�p).
		float						CYLINDER_RADIUS;			// �~���̔��a(�󂯓n���\�͈͂Ŏg�p).
		float						CYLINDER_HEIGHT;			// �~���̍���(�󂯓n���\�͈͂Ŏg�p).
		float						CYLINDER_ADD_POS_Y;			// �~���̓����蔻��̒ǉ�����Y���W.
		float						GAUGE_SPEED;				// �󂯓n���Q�[�W���x(�b).
		float						GAUGE_DEC_SPEED;			// �󂯓n���Q�[�W�̌������x(�b).
		float						GAUGE_DELETE_TIME;			// �󂯓n���Q�[�W�̍폜���鎞��(�b).
		float						GAUGE_SPEED_UP;				// �󂯓n���Q�[�W�̑��x��������{��.
		float						DETACH_COOL_TIME;			// �A���ŕR�Â�����Ȃ��悤�ɂ���N�[���^�C��(�b).
		float						MONEY;						// �Œ�����炦�邨��.
		float						HIGHT_MONEY_INTERVAL;		// �����ɂ�邨�������Z���Ă����Ԋu.
		float						HIGHT_ADD_MONEY;			// �����ɂ��ǉ����Ă����l�i.
		float						DISTANCE_MONEY_INTERVAL;	// �����ɂ�邨�������Z���Ă����Ԋu.
		float						WAVEHANDS_ANIMATION_SPEED;	// ���U��A�j���[�V�������x(�b).
		int							NORMAL_MESSAGE_SIZE;		// �ʏ�̎��̔z�B�������̒ʒm���̃T�C�Y.
		int							HOT_MESSAGE_SIZE;			// �A�c�A�c�̎��̔z�B�������̒ʒm���̃T�C�Y.
		int							MESSY_MESSAGE_SIZE;			// �����Ⴎ����̎��̔z�B�������̒ʒm���̃T�C�Y.
		int							MONEY_MAX;					// �Œ�����炦�邨���̏��.
		std::vector<std::string>	NORMAL_MESSAGE;				// �ʏ�̎��̔z�B�������̒ʒm��.
		std::vector<std::string>	HOT_MESSAGE;				// �A�c�A�c�̎��̔z�B�������̒ʒm��.
		std::vector<std::string>	MESSY_MESSAGE;				// �����Ⴎ����̎��̔z�B�������̒ʒm��.

		void Load();
		stDeliveryCharacter() { Load(); }
	} typedef SDeliveryCharacter;

	// �R�Â��̒萔�\����.
	struct stLinking {
		float						NORMAL_LINK_TIME;		// �ʏ펞�̕R�Â����x(�b).
		float						GOLDEN_TIME_LINK_TIME;	// �S�[���f���^�C�����̕R�Â����x(�b).
		float						LINK_END_TIME;			// �R�Â���������(�b).
		int							START_LINK_NUM;			// �J�n���̕R�Â��̐�.
		std::vector<D3DXCOLOR3>		COLOR_LIST;				// �A�C�R���̐F���X�g.

		void Load();
		stLinking() { Load(); }
	} typedef SLinking;

	// ���̒萔�\����.
	struct stSmoke {
		float		START_DISTANCE;				// �����ɂ��t�F�[�h���J�n���鋗��.
		float		ALPHA_DISTANCE;				// �A���t�@�l�����炵�Ă�������.
		float		MOVEVEC_RAND_ROT_MAX;		// �ړ��x�N�g���̃����_����]�̍ő�l(�x).
		float		MOVEVEC_RAND_MOVEY_MAX;		// �ړ��x�N�g���̃����_��Y���W�ړ��̍ő�l.
		float		MOVEVEC_RAND_MOVEY_MIN;		// �ړ��x�N�g���̃����_��Y���W�ړ��̍ŏ��l.
		float		STARTPOS_RAND_MOVE_MAX;		// �J�n���W�̃����_�����ړ��̍ő�l.
		float		SCALE_MAX;					// �ő�T�C�Y�̔{��.
		float		EXPANSION_SPEED;			// �g�呬�x(�x/�b).
		float		SHRINK_SPEED;				// �k�����x(�x/�b).
		float		MOVE_SPEED;					// �ړ����x.
		float		COOL_TIME;					// ���Ɖ��̊Ԃ̎���(�b).
		float		ALPHA;						// �A���t�@�l.
		D3DXCOLOR3	COLOR;						// �F(R,G,B)	.

		void Load();
		stSmoke() { Load(); }
	} typedef SSmoke;

	// �~�j�}�b�v�̒萔�\����.
	struct stMiniMap {
		float		MAGNIFICATION;					// �~�j�}�b�v�̔{��.
		float		GRID_SPACE;						// �O���b�h���̊Ԋu.
		float		DIRE_ICON_DISTANCE;				// �n���ɍs�����q����̕����̃A�C�R���̕\�����鋗��.
		float		DIRE_ICON_ALPHA_START_DISTANCE;	// �n���ɍs�����q����̕����̃A�C�R���̓����ɂ��Ă����J�n����.
		float		DIRE_ICON_ALPHA_DEC_DISTANCE;	// �n���ɍs�����q����̕����̃A�C�R���̓����ɂ��Ă��������̊Ԋu.
		int			ICON_CHANGE_MOVE_NO;			// �A�C�R����ύX���ɓ�������.

		void Load();
		stMiniMap() { Load(); }
	} typedef SMiniMap;

	// �ʒm�̒萔�\����.
	struct stNotification {
		float			DISP_TIME;					// �ʒm�̕\������(�b).
		float			ALPHA_SPEED;				// ����/�s�����ɂ��Ă������x.
		float			MOVE_SPEED;					// �ʒm�̕\��/��\���ɂ��鎞�̈ړ����x.
		float			SCALE_SPEED;				// �k�����Ă������x.
		float			DISP_ICON_POSITION_Y;		// �ʒm��\������Y���W.
		D3DXPOSITION2	FONT_POSITION;				// �t�H���g�̍��W.
		D3DXPOSITION2	ICON_POSITION;				// �A�C�R����\��������W.

		void Load();
		stNotification() { Load(); }
	} typedef SNotification;

	// ���Ԃ̒萔�\����.
	struct stTime {
		float								TIME_MAX;					// �Q�[�����I��������o�ߎ���.
		float								CLEAR_SCENE_MOVE_TIME_MAX;	// �^�C�g���Ɉړ�������o�ߎ���.
		float								GAME_END_HOUR;				// �Q�[�����I�������鎞��.
		float								CLEAR_SCENE_MOVE_HOUR;		// �^�C�g���Ɉړ������鎞��.
		float								LIGHT_INTENSITY_MAX;		// ���C�g�̋����̍ő�.
		float								LIGHT_INTENSITY_MIN;		// ���C�g�̋����̍ŏ�.
		float								ONE_SECOND_TIME;			// ������1�b�Ői�ގ���(�b).
		int									START_HOUR;					// �J�n���̎���(��).
		int									TIME_NOTIFICATION_MAX;		// ���Ԃő���ʒm�̐�.
		std::string							GOLDEN_TIME_MESSAGE;		// �S�[���f���^�C���ɂȂ������̒ʒm��.
		std::string							NORMAL_TIME_MESSAGE;		// �ʏ�̎��ԂɂȂ������̒ʒm��.
		std::vector<std::pair<int, int>>	NOTIFICATION_TIMING;		// �ʒm�𑗂鎞��<��, ��>.
		std::vector<std::string>			NOTIFICATION_MESSAGE;		// ���Ԃő���ʒm��.
		std::vector<std::pair<int, int>>	NORMAL_TIME_HOUR;			// �ʏ�̎��̎���<�J�n, �I��>.
		std::vector<std::pair<int, int>>	GOLDEN_TIME_HOUR;			// �S�[���f���^�C���̎���<�J�n, �I��>.

		void Load();
		stTime() { Load(); }
	} typedef STime;

	// ���X�̏ڍ׏��̒萔�\����.
	struct stShopInfo {
		float			DISP_POSITION_Y;			// �\������Y���W.
		float			CLOSE_OVER_MOVE_Y;			// ���鎞�ɑ傫�ڂɈړ������.
		float			MONEY_FONT_POSITION_Y;		// �����̕�����Y���W.
		float			DISTANCE_FONT_POSITION_Y;	// �����̕�����Y���W.
		float			ANIM_COOL_TIME;				// �A�j���[�V�����̃N�[���^�C��(�b).
		int				SHOP_DISTANCE_ADJUS;		// ���X�̋����̒����l.
		D3DXPOSITION3	ICON_POSITION;				// �A�C�R���̍��W.
		D3DXPOSITION3	FONT_SCALE;					// �����̃T�C�Y.

		void Load();
		stShopInfo() { Load(); }
	} typedef SShopInfo;
}
