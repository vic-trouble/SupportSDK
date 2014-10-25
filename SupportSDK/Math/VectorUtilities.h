#ifndef __MATHVECTORUTILITIES_H__
#define __MATHVECTORUTILITIES_H__

#include "Vector.h"

namespace SDK
	{

	namespace Math
		{

		template <typename CoordinateType, size_t Dimension>
		struct VectorUtilities
			{

			static Vector<CoordinateType, Dimension> Projection (const Vector<CoordinateType, Dimension>& i_p, const Vector<CoordinateType, Dimension>& i_q)
				{
				// proj = (DotProduct(p, q)/Length(Q)^2)*Q
				// For better performance for thre dimension space
				// 1. Dot product this * i_target
				CoordinateType dot_product = i_p.DotProduct(i_q);
				// 2. Dot product result / target`s length^2
				dot_product /= i_q.LengthSq();
				// 3. i_target * dot_product
				Vector<CoordinateType, Dimension> projection = i_q;
				projection *= dot_product;

				return projection;
				}

			/// perpendicular component of Pwith respect to Q
			static Vector<CoordinateType, Dimension> PerpendicularComponent (const Vector<CoordinateType, Dimension>& i_p, const Vector<CoordinateType, Dimension>& i_q)
				{
				auto perp = i_p;
				perp -= Projection(i_p, i_q);
				return perp;
				}

			/*
			Use Kronecker delta for each pair of vectors to find if they are lineary independent
				This is true
			Complexity: O(n^2)
			*/
			template <typename Container>
			static bool IsVectorsLinearyIndependent (const Container& i_vectors)
				{
				auto it_end = i_vectors.end();
				for (auto it_cur = i_vectors.begin(); it_cur != it_end; ++it_cur)
					{
					for (auto next_to_current = it_cur+1; next_to_current != it_end; ++next_to_current)
						{
						if ((*it_cur).DotProduct(*next_to_current) != 0)
							return false;
						}
					}

				return true;
				}
			
			// Use Gram-Schmidt Orthogonalization algorithm
			//		The basic idea is to subtract away the projection of each vector onto the vectors preceding it in the set. 
			//		Whatever vector is left over must then be orthogonal to its predecessors.
			// Complexity: O(n^2)
			template <typename Container>
			static Container ComputeOrthonormalVectors (const Container& i_vectors)
				{
				#ifdef _DEBUG
				if (i_vectors.size() > Dimension)
					throw std::logic_error("There cannot be more orthonormal vectors than space dimension.");
				#endif
				
				Container orthonormal_vectors(i_vectors);

				const size_t vectors_size = i_vectors.size();
				for (size_t i = 1; i < vectors_size; ++i)
					{
					Vector<CoordinateType, Dimension> projection_sum;
					for (size_t j = 0; j < i; ++j)
						{
						projection_sum += Projection(i_vectors[i], orthonormal_vectors[j]);
						}
					orthonormal_vectors[i] -= projection_sum;
					}
				return std::move(orthonormal_vectors);
				}

			};

		} // Math

	} // SDK

#endif