#ifndef __MATH_MATRIXIMPL_H__
#define __MATH_MATRIXIMPL_H__

#include <cmath>

namespace SDK
{

	namespace Math
	{

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns>::Matrix()
		{
			for (size_t i = 0; i < Rows*Collumns; ++i)
				m_data[i] = CoordinateType(0);
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns>::Matrix(CoordinateType* ip_data)
		{
			memcpy(m_data, ip_data, sizeof(CoordinateType)*Rows*Collumns);
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns>::Matrix(const Matrix<CoordinateType, Rows, Collumns>& i_other)
		{
			for (size_t i = 0; i < Rows*Collumns; ++i)
				m_data[i] = i_other.m_data[i];
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns>& Matrix<CoordinateType, Rows, Collumns>::operator = (const Matrix<CoordinateType, Rows, Collumns>& i_other)
		{
			for (size_t i = 0; i < Rows*Collumns; ++i)
				m_data[i] = i_other.m_data[i];
			return *this;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		void Matrix<CoordinateType, Rows, Collumns>::Identity()
		{
			for (size_t i = 0; i < Rows; ++i)
				for (size_t j = 0; j < Collumns; ++j)
				{
					if (i == j)
						m_data_[i][j] = 1;
					else
						m_data_[i][j] = 0;
				}
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		bool Matrix<CoordinateType, Rows, Collumns>::IsIdentity() const
		{
			for (size_t i = 0; i < Rows; ++i)
				for (size_t j = 0; j < Collumns; ++j)
				{
					if (i == j && m_data_[i][j] != 1)
						return false;
					else if (m_data_[i][j] != 0)
						return false;
				}
			return true;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		void Matrix<CoordinateType, Rows, Collumns>::Zero()
		{
			for (size_t i = 0; i < Rows; ++i)
				for (size_t j = 0; j < Collumns; ++j)
				{
					m_data_[i][j] = 0;
				}
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		bool Matrix<CoordinateType, Rows, Collumns>::IsZero() const
		{
			for (size_t i = 0; i < Rows; ++i)
				for (size_t j = 0; j < Collumns; ++j)
				{
					if (m_data_[i][j] != 0)
						return false;
				}
			return true;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		void Matrix<CoordinateType, Rows, Collumns>::Transpose()
		{
			ThisMatrix matrix(*this);
			for (size_t i = 0; i < Rows; ++i)
				for (size_t j = 0; j < Collumns; ++j)
					matrix(i, j) = m_data_[j][i];

			*this = matrix;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns> Matrix<CoordinateType, Rows, Collumns>::GetTransposed() const
		{
			ThisMatrix matrix(*this);
			matrix.Transpose();
			return matrix;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns> Matrix<CoordinateType, Rows, Collumns>::GetInverseGeneric() const
		{
			if (Rows != Collumns)
				throw std::logic_error("Invrese can be only on square matrix like 2x2 or 3x3");

			// A. Construct augment matrix [Rows, Rows*2]
			Matrix<CoordinateType, Rows, Rows*2> aug_matrix;
			for (size_t i = 0; i < Rows; ++i)
			{
				// copy source matrix
				for (size_t j = 0; j < Rows * 2; ++j)
				{
					aug_matrix(i, j) = m_data_[i][j];
				}
				// construct additional part of matrix
				for (size_t j = Rows; j < Rows * 2; ++j)
				{
					if (i == j - Rows)
						aug_matrix(i, j) = 1;
					else
						aug_matrix(i, j) = 0;
				}
			}
			
			// B. loop through collumns 1 to n
			for (size_t j = 0; j < Collumns; ++j)
			{
				size_t row_number = j;
				CoordinateType max_abs_value = CoordinateType(0);
				// C. find row with maximum absolute value in M[i, j]
				for (size_t i = j; i < Rows; ++i)
				{
					const CoordinateType abs_val = std::abs(aug_matrix(i, j));
					if (abs_val > max_abs_value)
					{
						max_abs_value = aug_matrix(i, j);
						row_number = i;
					}
				}

				if (max_abs_value == 0)
					throw std::logic_error("There is no max element in collumn. All elements are zero");

				// D. change rows i and j
				if (row_number != j)
				{
					for (size_t k = 0; k < Collumns * 2; ++k)
						std::swap(aug_matrix(row_number, k), aug_matrix(j, k));
				}

				// E. Multiply row j by 1/M[i,j]
				const CoordinateType coef = static_cast<CoordinateType>(1. / aug_matrix(j, j));
				for (size_t k = 0; k < Collumns * 2; ++k)
					aug_matrix(j, k) *= coef;

				// F. Add row j*-M[r,j] to r
				for (size_t r = 0; r < Rows; ++r)
				{
					if (r == j)
						continue;
					const CoordinateType coef = -aug_matrix(r, j);
					for (size_t k = 0; k < Collumns*2; ++k)
					{
						aug_matrix(r, k) += coef*aug_matrix(j, k);
					}
				}
			}
			
			// Get inverse matrix
			ThisMatrix inverse_matrix;
			for (size_t i = 0; i < Rows; ++i)
			{
				for (size_t j = 0; j < Collumns; ++j)
					inverse_matrix(i, j) = aug_matrix(i, j + Rows);
			}

			return inverse_matrix;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		bool Matrix<CoordinateType, Rows, Collumns>::Inverse()
		{
			try
			{
				this = this->GetInverseGeneric();
			}
			catch (std::logic_error&)
			{
				return false;
			}

			return true;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		template <size_t R, size_t ColsNum>
		inline Matrix<CoordinateType, Rows, ColsNum> Matrix<CoordinateType, Rows, Collumns>::Multiply(const Matrix<CoordinateType, R, ColsNum>& i_other) const
		{
			Matrix<CoordinateType, RowsNumber, ColsNum> result;

			for (size_t i = 0; i < RowsNumber; ++i)
			{
				for (size_t j = 0; j < ColsNum; ++j)
				{
					for (size_t k = 0; k < R; ++k)
						result(i, j) += m_data_[i][k] * i_other(k, j);
				}
			}

			return result;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns>& Matrix<CoordinateType, Rows, Collumns>::operator *= (CoordinateType i_value)
		{
			for (size_t i = 0; i < Rows; ++i)
				for (size_t j = 0; j < Collumns; ++j)
					m_data_[i][j] *= i_value;
			return *this;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns> Matrix<CoordinateType, Rows, Collumns>::operator * (CoordinateType i_value) const
		{
			ThisMatrix matrix(*this);
			matrix *= i_value;
			return matrix;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns>& Matrix<CoordinateType, Rows, Collumns>::operator += (const Matrix<CoordinateType, Rows, Collumns>& i_other)
		{
			for (size_t i = 0; i < Rows; ++i)
				for (size_t j = 0; j < Collumns; ++j)
					m_data_[i][j] += i_other.m_data_[i][j];
			return *this;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns> Matrix<CoordinateType, Rows, Collumns>::operator + (const Matrix<CoordinateType, Rows, Collumns>& i_other) const
		{
			Matrix<CoordinateType, Rows, Collumns> matrix(*this);
			matrix += i_other;
			return matrix;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns>& Matrix<CoordinateType, Rows, Collumns>::operator -= (const Matrix<CoordinateType, Rows, Collumns>& i_other)
		{
			for (size_t i = 0; i < Rows; ++i)
				for (size_t j = 0; j < Collumns; ++j)
					m_data_[i][j] -= i_other.m_data_[i][j];
			return *this;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		Matrix<CoordinateType, Rows, Collumns> Matrix<CoordinateType, Rows, Collumns>::operator - (const Matrix<CoordinateType, Rows, Collumns>& i_other) const
		{
			Matrix<CoordinateType, Rows, Collumns> matrix(*this);
			matrix -= i_other;
			return matrix;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		bool Matrix<CoordinateType, Rows, Collumns>::operator == (const Matrix<CoordinateType, Rows, Collumns>& i_other) const
		{
			for (size_t i = 0; i < Rows; ++i)
			{
				for (size_t j = 0; j < Collumns; ++j)
				{
					if (m_data_[i][j] != i_other.m_data_[i][j])
						return false;
				}
			}

			return true;
		}

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		bool Matrix<CoordinateType, Rows, Collumns>::operator != (const Matrix<CoordinateType, Rows, Collumns>& i_other) const
		{
			return (*this == i_other) == false;
		}

	} // Math
} // SDK

#endif