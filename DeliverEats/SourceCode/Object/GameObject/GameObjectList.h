#pragma once

// �I�u�W�F�N�g�^�O���X�g.
enum class enObjectTagList : unsigned int
{
	None,

	Apartment,			// �}���V����.
	ApartmentA,			// A�^�C�v�}���V����.
	ApartmentB,			// B�^�C�v�}���V����.
	Island,				// ����.
	PlayerIsland,		// �v���C���[�̕���.
	IslandObject,		// �����̏�ɂ���I�u�W�F�N�g.
	Player,				// �v���C���[.
	Employee,			// �X��.
	Customer,			// ���q����.
	GoHandingCustomer,	// �n���ɍs�����q����.

	Max,

} typedef EObjectTag;
