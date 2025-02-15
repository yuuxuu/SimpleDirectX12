/**
 * @file PMXModelInfo.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _PMX_MODEL_INFO_H_
#define _PMX_MODEL_INFO_H_

#include "Math/math.h"

namespace Simple
{
namespace ModelLoader
{
	// PMXヘッダー
	struct PmxHeader 
	{
		BYTE	magicNo[4];			// マジックナンバー
		float	version;			// バージョン
		BYTE	dateByteSize;		// データ列のバイトサイズ
		BYTE	encood;				// エンコード方式
		BYTE	adduvSize;			// 追加UV
		BYTE	vertexIndexSize;	// 頂点インデックスサイズ
		BYTE	textureIndexSize;	// テクスチャインデックスサイズ
		BYTE	materialIndexSize;	// 材質インデックスサイズ
		BYTE	boneIndexSize;		// ボーンインデックスサイズ
		BYTE	moorfIndexSize;		// モーフインデックスサイズ
		BYTE	rigidBodyIndexSize; // 剛体インデックスサイズ
	};

	// エンコードの種類
	typedef enum 
	{
		UTF16,
		UTF8,
	}ENCOORD_TYPE;

	// PMXテキストバッファ
	struct PmxTextBuffer 
	{
		UINT32		pmxInfoLength;	// 文字列数
		std::string	pmxInfoStr;		// 文字列
	};

	// PMXモデル情報
	struct PmxModelInfo 
	{
		PmxTextBuffer pmxModelNameJa;	// モデル名(Japane)
		PmxTextBuffer pmxModelNameEn;	// モデル名(English)
		PmxTextBuffer commentJa;		// コメント(Japane)
		PmxTextBuffer commentEn;		// コメント(English)
	};

	// PMXボーンウェイト
	struct PmxBoneWeight 
	{
		USHORT	pmxBoneIndex0;	// ボーン1インデックス
		USHORT	pmxBoneIndex1;	// ボーン2インデックス
		USHORT	pmxBoneIndex2;	// ボーン3インデックス
		USHORT	pmxBoneIndex3;	// ボーン4インデックス
		float	pmxBoneWeight0;	// ボーン1ウェイト
		float	pmxBoneWeight1;	// ボーン2ウェイト
		float	pmxBoneWeight2;	// ボーン3ウェイト
		float	pmxBoneWeight3;	// ボーン4ウェイト
		VECTOR3	SDEF_C;			// SDEF_C値(x, y, z)
		VECTOR3	SDEF_R0;		// SDEF_R0値(x, y, z)
		VECTOR3	SDEF_R1;		// SDEF_R1値(x, y, z)
	};

	// ボーンの種類
	typedef enum 
	{
		BDEF1,
		BDEF2,
		BDEF4,
		SDEF,
	}BONE_TYPE;

	// PMX頂点
	struct PmxVertex {
		VECTOR3			vertex;		// 位置
		VECTOR3			normal;		// 法線
		VECTOR2			uv;			// UV
		VECTOR4			adduv;		// 追加UV
		BYTE			weightType;	// ウェイト変更方式(0->BDEF1 1->BDEF2 2->BDEF4 3->SDEF)
		PmxBoneWeight	weight;		// ボーンウェイト
		float			edge;		// エッジ
	};

	// PMX頂点情報
	struct PmxVertexInfo 
	{
		UINT32					pmxVertexCount;	// 頂点数
		std::vector<PmxVertex>	vecPmxVertex;	// 頂点コンテナ
	};

	// PMXModel頂点インデックス
	struct PmxVertexIndex_us16 
	{
		USHORT indices[3]; // 頂点インデックス
	};

	// PMXStage頂点インデックス
	struct PmxVertexIndex_ui32 
	{
		UINT32 indices[3]; // 頂点インデックス
	};

	// PMX頂点インデックス情報
	struct PmxVertexIndexInfo 
	{
		UINT32								pmxPolygonCount;		// ポリゴンの数
		std::vector<PmxVertexIndex_us16>	vecPmxVertexIndex_us16;	// 頂点インデックスコンテナ
		std::vector<PmxVertexIndex_ui32>	vecPmxVertexIndex_ui32;	// 頂点インデックスコンテナ
	};
	// PMXテクスチャ情報
	struct PmxTextureInfo 
	{
		UINT32						pmxTextureCount;		// テクスチャの数	
		std::vector<PmxTextBuffer>	vecPmxTextureFileName;	// テクスチャのパス名コンテナ
	};

	// PMXマテリアル
	struct PmxMaterial 
	{
		PmxTextBuffer	materialNameJa;				// マテリアル名(日本語)
		PmxTextBuffer	materialNameEn;				// マテリアル名(英語)

		VECTOR4			materialDiffuse;			// マテリアルディフューズカラー
		VECTOR3			materialSpecular;			// マテリアルスペキュラーカラー
		float			materialSpecularPower;		// マテリアルスペキュラーパワー
		VECTOR3			materialAmbient;			// マテリアルアンビエントカラー
		BYTE			bitFlag;					// 描画フラグ(各ビット 0→OFF 1→ON)
		VECTOR4			materialEdge;				// マテリアルエッジカラー
		float			edgeSize;					// エッジサイズ
		BYTE			diffuseTextureIndex;		// 通常テクスチャインデックスサイズ
		BYTE			sphereTextureIndex;			// スフィアテクスチャインデックスサイズ
		BYTE			sphereMode;					// スフィアモード(0→無効 1→乗算 2→加算 3→サブテクスチャ)
		BYTE			shareToonFlag;				// 共有Toonフラグ(0→テクスチャインデックスサイズ 1→1byte)
		BYTE			toonTextureIndex;			// Toonテクスチャインデックスサイズ
		BYTE			shareToonTextureIndex;		// 共有Toonテクスチャインデックスサイズ
		PmxTextBuffer	memo;						// メモ
		UINT32			materialPolygonCount;		// マテリアルに対応するポリゴン数
	};

	// PMXマテリアル情報
	struct PmxMaterialInfo 
	{
		UINT32						materialCount;				// マテリアルの数
		std::vector<PmxMaterial>	vecPmxMaterial;				// マテリアルコンテナ
	};

	// PMXボーン
	struct PmxBone 
	{
		PmxTextBuffer	boneNameJa;				// ボーン名(日本語)
		PmxTextBuffer	boneNameEn;				// ボーン名(英語)

		VECTOR3			location;				// 位置
		USHORT			parentBoneIndex;		// 親ボーンのインデックス
		int				deformationHierarchy;	// 変形階層
		USHORT			boneFlag;				// ボーンフラグ(各bit 0→OFF 1→ON)

		VECTOR3			offSetLocation;			// 座標オフセット
		USHORT			accessPointBoneIndex;	// 接続先ボーンのインデックス

		USHORT			parentGrantBoneIndex;	// 付与親ボーンのボーンインデックス
		float			grantRate;				// 付与率

		VECTOR3			axisVector;				// 軸の方向ベクトル

		VECTOR3			xAxisVector;			// X軸の方向ベクトル
		VECTOR3			zAxisVector;			// Z軸の方向ベクトル

		int				keyValue;				// Key値

		USHORT			ikBoneIndex;			// IKターゲットボーンのインデックス
		int				ikRoop;					// IKループ回数
		float			ikRoopAngleRestriction;	// IKループ計算時の1回あたりの制限角度→ラジアン角
		int				ikRink;					// IKリンク数
	};

	// PMXボーンIK
	struct PmxBoneIk 
	{
		USHORT	rinkBoneIndex;			// リンクボーンインデックス
		BYTE	angleRestriction;		// 角度制限(0→OFF 1→ON)
		VECTOR3	angleUnderRestriction;	// 下制限(x,y,z)->ラジアン角
		VECTOR3	angleUpRestriction;		// 上制限(x,y,z)->ラジアン角
	};

	// PMXボーンフラグ
	typedef enum 
	{
		accessPoint = 0x0001,				// 接続先(0→座標オフセットで指定 1→ボーンで指定)
		anglePossible = 0x0002,				// 回転可能
		movementPossible = 0x0004,			// 移動可能
		display = 0x0008,					// 表示
		operation = 0x0010,					// 操作可の可能
		ik = 0x0020,						// IK
		localGrante = 0x0080,				// ローカル付与
		angleGrante = 0x0100,				// 回転付与
		movementGrante = 0x0200,			// 移動付与
		axisFixation = 0x0400,				// 軸固定
		localAxis = 0x0800,					// ローカル軸
		physicsDeformation = 0x1000,		// 物理後変形
		outsideParentDeformation = 0x2000,	// 外部親変形
	}PmxBoneFlag;

	// PMXボーン情報
	struct PmxBoneInfo 
	{
		UINT32					BoneCount;	// ボーンの数
		std::vector<PmxBone>	vecBone;	// ボーンコンテナ
		std::vector<PmxBoneIk>	vecBoneIk;	// ボーンIKコンテナ
	};

} // namespace ModelLoader
} // namespace Simple

#endif // _PMX_MODEL_INFO_H_

