#ifndef	__GAMECORE_SHADERDEFINITIONS_H__
#define __GAMECORE_SHADERDEFINITIONS_H__

namespace SDK
{
	namespace Render
	{
		enum class ShaderVariableType : char
		{
			Float,
			FloatVec2,
			FloatVec3,
			FloatVec4,
			Double,
			DoubleVec2,
			DoubleVec3,
			DoubleVec4,
			Int,
			IntVec2,
			IntVec3,
			IntVec4,
			UInt,
			UIntVec2,
			UIntVec3,
			UIntVec4,
			Bool,
			FloatMat2,
			FloatMat3,
			FloatMat4,
			FloatMat2x3,
			FloatMat2x4,
			FloatMat3x2,
			FloatMat3x4,
			FloatMat4x2,
			FloatMat4x3,
			Sampler2D,

			Undefined
		};

		enum class UniformType
		{
			ProjectionMatrix,
			ModelviewMatrix,
			Dynamic
		};
	} // Render
} // SDK

#endif