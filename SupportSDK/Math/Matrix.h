#ifndef __MATH_MATRIX_H__
#define	__MATH_MATRIX_H__




namespace SDK
{
	namespace Math
	{

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		class Matrix
		{
		public:
			enum { RowsNumber = Rows, CollumnsNumber = Collumns };	// helper for expression evaluator
			typedef CoordinateType CoordinateType;
			typedef Matrix<CoordinateType, Rows, Collumns> ThisMatrix;

		protected:
			union
			{
				CoordinateType m_data[Rows*Collumns];
				CoordinateType m_data_[Rows][Collumns];
			};

		public:
			// Construct empty matrix
			inline Matrix();

			// Construct matrix from array which must be Rows*Collumns
			inline Matrix(CoordinateType* ip_data);

			inline Matrix(const Matrix& i_other);
			inline ThisMatrix& operator = (const Matrix& i_other);

#if _MSC_VER >= 1800
			// not tested
			Matrix(std::initializer_list<CoordinateType> i_coordinates)
			{
				if (i_coordinates.size() != CollumnsNumber*RowsNumber)
					throw std::logic_error("Initializer list contains wrong number of entries");

				std::copy(i_coordinates.begin(), i_coordinates.end(), m_data);
			}
#endif

			const CoordinateType& operator () (size_t i, size_t j) const
			{
				std::vector < double> i_coordinates;
				return m_data_[i][j];
			}
			CoordinateType& operator () (size_t i_row, size_t i_collumn)
			{
				return m_data_[i_row][i_collumn];
			}

			const CoordinateType* operator [] (size_t i_row) const
			{
				return m_data_[i_row];
			}
			CoordinateType* operator [] (size_t i_row)
			{
				return m_data_[i_row];
			}

			CoordinateType DeterminantGeneric();

			// Helpers
			inline void Identity();
			inline bool IsIdentity() const;
			inline void Zero();
			inline bool IsZero() const;
			inline void Transpose();
			inline ThisMatrix GetTransposed() const;

			// Generic algorithms that are more slow than specific
			// Using Gauss-Jordan Elimination
			// In specialized Matrix2, Matrix3, Matrix4 can be used shortcuts methods
			inline ThisMatrix GetInverseGeneric() const;
			inline bool Inverse();

			// Generic multiply for all types of matrices - for user defined (e.x. Matrix2, Matrix3, Matrix4) this can be
			//		optimized and done via operator *
			template <size_t R, size_t ColsNum>
			inline Matrix<CoordinateType, Rows, ColsNum> Multiply(const Matrix<CoordinateType, R, ColsNum>& i_other) const;

			inline ThisMatrix& operator *= (CoordinateType i_value);
			inline ThisMatrix operator * (CoordinateType i_value) const;

			inline ThisMatrix& operator += (const ThisMatrix& i_other);
			inline ThisMatrix operator + (const ThisMatrix& i_other) const;

			inline ThisMatrix& operator -= (const ThisMatrix& i_other);
			inline ThisMatrix operator - (const ThisMatrix& i_other) const;

			inline bool operator == (const ThisMatrix& i_other) const;
			inline bool operator != (const ThisMatrix& i_other) const;
			
			/*inline void	CreateRotation(const Vector3 & r, float32 angleInRadians);
			inline void	BuildRotation(float32 cosA, float32 sinA);
			inline void	BuildRotation(float32 angle);
			inline void	BuildTranslation(float32 x, float32 y);
			inline void	BuildTranslation(const Vector2 & vec);
			inline void	BuildScale(const Vector2 & vec);
			inline bool GetInverse(Matrix3 & out, float32 fTolerance = 1e-06) const;
			inline void Transpose();
			inline void Decomposition(Matrix3 &kQ, Vector3 &kD, Vector3 &kU) const;

			*/
		};

	} // Math

} // SDK

#include "MatrixImpl.h"

#endif