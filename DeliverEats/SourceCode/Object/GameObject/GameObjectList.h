#pragma once

// オブジェクトタグリスト.
enum class enObjectTagList : unsigned int
{
	None,

	Apartment,			// マンション.
	ApartmentA,			// Aタイプマンション.
	ApartmentB,			// Bタイプマンション.
	Island,				// 浮島.
	PlayerIsland,		// プレイヤーの浮島.
	IslandObject,		// 浮島の上にあるオブジェクト.
	Player,				// プレイヤー.
	Employee,			// 店員.
	Customer,			// お客さん.
	GoHandingCustomer,	// 渡しに行くお客さん.

	Max,

} typedef EObjectTag;
