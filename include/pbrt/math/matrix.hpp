#pragma once

#include "pbrt/alignment_of.hpp"
#include "pbrt/assertions.hpp"
#include "pbrt/export.hpp"
#include "pbrt/inline.hpp"
#include "pbrt/math/constants.hpp"
#include "pbrt/math/traits.hpp"
#include "pbrt/math/vector.hpp"
#include "pbrt/simd.hpp" // IWYU pragma: keep
#include "pbrt/types.hpp"

#include <array>

namespace pbrt::math
{
/**
 * @brief A matrix class template that represents a mathematical matrix with a fixed number of rows
 * and columns.
 *
 * @tparam T The type of the elements in the matrix. It must be an arithmetic type.
 * @tparam Rows The number of rows in the matrix. It must be greater than 0.
 * @tparam Cols The number of columns in the matrix. It must be greater than 0.
 */
template <Arithmetic T, usize Rows, usize Cols>
  requires(Rows > 0 && Cols > 0)
class Matrix
{
public:
  using ValueType = T;
  using SizeType = usize;
  using Reference = T &;
  using ConstReference = const T &;
  using Pointer = T *;
  using ConstPointer = const T *;
  using Iterator = typename std::array<T, Rows * Cols>::iterator;
  using ConstIterator = typename std::array<T, Rows * Cols>::const_iterator;
  using ReverseIterator = typename std::array<T, Rows * Cols>::reverse_iterator;
  using ConstReverseIterator = typename std::array<T, Rows * Cols>::const_reverse_iterator;

  /**
   * @brief Default constructor for the Matrix class. Initializes the matrix to zero.
   */
  constexpr Matrix() = default;

  /**
   * @brief Constructor that takes a variable number of arguments to initialize the matrix.
   *
   * @tparam U The type of the arguments. It must be convertible to T.
   */
  template <ConvertibleTo<T>... U>
    requires(sizeof...(U) == Rows * Cols)
  constexpr Matrix(U &&...args) noexcept : m_data{static_cast<T>(std::forward<U>(args))...}
  {
  }

  /**
   * @brief Constructor from another Matrix of the same size but potentially different type.
   *
   * @tparam U The type of the other matrix. It must be convertible to T.
   */
  template <ConvertibleTo<T> U>
  constexpr explicit Matrix(Matrix<U, Rows, Cols> const &other) noexcept
  {
    std::transform(other.cbegin(), other.cend(), begin(),
                   [](U const &value) { return static_cast<T>(value); });
  }

  /**
   * @brief Fill constructor for the Matrix class. Initializes all elements of the matrix to the
   * specified value.
   */
  constexpr explicit Matrix(T const &value) noexcept
  {
    m_data.fill(value);
  }

  /**
   * @brief Accesses an element of the matrix at the specified row and column indices.
   *
   * @param row The row index of the element to access. It must be less than Rows.
   * @param col The column index of the element to access. It must be less than Cols.
   * @return Reference The element at the specified row and column indices.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference operator()(SizeType row, SizeType col) noexcept
  {
    PBRT_ASSERT_MSG(row < Rows || col < Cols, "Matrix indices out of bounds.");
    return m_data[index(row, col)];
  }

  /**
   * @brief Accesses an element of the matrix at the specified row and column indices.
   *
   * @param row The row index of the element to access. It must be less than Rows.
   * @param col The column index of the element to access. It must be less than Cols.
   * @return ConstReference The element at the specified row and column indices.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference operator()(SizeType row,
                                                                SizeType col) const noexcept
  {
    PBRT_ASSERT_MSG(row < Rows || col < Cols, "Matrix indices out of bounds.");
    return m_data[index(row, col)];
  }

  /**
   * @brief Bounds-checked access to an element of the matrix at the specified row and column
   * indices.
   *
   * @param row The row index of the element to access. It must be less than Rows.
   * @param col The column index of the element to access. It must be less than Cols.
   * @return Reference Reference The element at the specified row and column indices.
   */
  [[nodiscard]] PBRT_INLINE constexpr Reference at(SizeType row, SizeType col) noexcept
  {
    PBRT_ASSERT_MSG(row < Rows || col < Cols, "Matrix indices out of bounds.");
    return (*this)(row, col);
  }

  /**
   * @brief Bounds-checked access to an element of the matrix at the specified row and column
   * indices.
   *
   * @param row The row index of the element to access. It must be less than Rows.
   * @param col The column index of the element to access. It must be less than Cols.
   * @return ConstReference The element at the specified row and column indices.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReference at(SizeType row, SizeType col) const noexcept
  {
    PBRT_ASSERT_MSG(row < Rows || col < Cols, "Matrix indices out of bounds.");
    return (*this)(row, col);
  }

  /**
   * @brief Accesses a row of the matrix as a Vector.
   *
   * @param row The row index to access. It must be less than Rows.
   * @return Vector<T, Cols> A Vector representing the specified row of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector<T, Cols> row(SizeType row) const noexcept
  {
    PBRT_ASSERT_MSG(row < Rows, "Row index out of bounds.");
    Vector<T, Cols> result;
    for (SizeType j = 0; j < Cols; ++j)
    {
      result[j] = (*this)(row, j);
    }
    return result;
  }

  /**
   * @brief Accesses a column of the matrix as a Vector.
   *
   * @param col The column index to access. It must be less than Cols.
   * @return Vector<T, Rows> A Vector representing the specified column of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector<T, Rows> column(SizeType col) const noexcept
  {
    PBRT_ASSERT_MSG(col < Cols, "Column index out of bounds.");
    Vector<T, Rows> result;
    for (SizeType i = 0; i < Rows; ++i)
    {
      result[i] = (*this)(i, col);
    }
    return result;
  }

  /**
   * @brief Add another matrix to this matrix.
   *
   * @param other The matrix to add to this matrix. It must have the same size as this matrix.
   * @return Matrix & A reference to this matrix after the addition.
   */
  PBRT_INLINE constexpr Matrix &operator+=(Matrix const &other) noexcept
  {
    for (SizeType i = 0; i < size(); ++i)
    {
      m_data[i] += other.m_data[i];
    }
    return *this;
  }

  /**
   * @brief Add a scalar value to each element of the matrix.
   *
   * @param scalar The scalar value to add to each element of the matrix. It must be convertible to
   * T.
   * @return Matrix & A reference to this matrix after the addition.
   */
  PBRT_INLINE constexpr Matrix &operator+=(T const &scalar) noexcept
  {
    for (SizeType i = 0; i < size(); ++i)
    {
      m_data[i] += scalar;
    }
    return *this;
  }

  /**
   * @brief Subtract another matrix from this matrix.
   *
   * @param other The matrix to subtract from this matrix. It must have the same size as this
   * matrix.
   * @return Matrix & A reference to this matrix after the subtraction.
   */
  PBRT_INLINE constexpr Matrix &operator-=(Matrix const &other) noexcept
  {
    for (SizeType i = 0; i < size(); ++i)
    {
      m_data[i] -= other.m_data[i];
    }
    return *this;
  }

  /**
   * @brief Subtract a scalar value from each element of the matrix.
   *
   * @param scalar The scalar value to subtract from each element of the matrix. It must be
   * convertible to T.
   * @return Matrix & A reference to this matrix after the subtraction.
   */
  PBRT_INLINE constexpr Matrix &operator-=(T const &scalar) noexcept
  {
    for (SizeType i = 0; i < size(); ++i)
    {
      m_data[i] -= scalar;
    }
    return *this;
  }

  /**
   * @brief Multiply this matrix by a scalar value.
   *
   * @param scalar The scalar value to multiply each element of the matrix by. It must be
   * convertible to T.
   * @return Matrix& A reference to this matrix after the multiplication.
   */
  PBRT_INLINE constexpr Matrix &operator*=(T const &scalar) noexcept
  {
    for (SizeType i = 0; i < size(); ++i)
    {
      m_data[i] *= scalar;
    }
    return *this;
  }

  /**
   * @brief Divide this matrix by a scalar value.
   *
   * @param scalar The scalar value to divide each element of the matrix by. It must be convertible
   * to T and not equal to zero.
   * @return Matrix & A reference to this matrix after the division. If the scalar is zero, it
   * returns a matrix filled with zeros.
   */
  PBRT_INLINE constexpr Matrix &operator/=(T const &scalar) noexcept
  {
    if (scalar == T{0}) [[unlikely]]
    {
      return *this;
    }

    for (SizeType i = 0; i < size(); ++i)
    {
      m_data[i] /= scalar;
    }
    return *this;
  }

  /**
   * @brief Add another matrix to this matrix and return the result as a new matrix.
   *
   * @param other The matrix to add to this matrix. It must have the same size as this matrix.
   * @return Matrix A new matrix that is the result of adding the other matrix to this matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix operator+(Matrix const &other) const noexcept
  {
    Matrix result{*this};
    result += other;
    return result;
  }

  /**
   * @brief Add a scalar value to each element of the matrix and return the result as a new matrix.
   *
   * @param scalar The scalar value to add to each element of the matrix. It must be convertible to
   * T.
   * @return Matrix A new matrix that is the result of adding the scalar value to each element of
   * this matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix operator+(T const &scalar) const noexcept
  {
    Matrix result{*this};
    result += scalar;
    return result;
  }

  /**
   * @brief Negate the matrix and return the result as a new matrix.
   *
   * @return Matrix A new matrix that is the negation of this matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix operator-() const noexcept
  {
    Matrix result;
    for (SizeType i = 0; i < size(); ++i)
    {
      result.m_data[i] = -m_data[i];
    }
    return result;
  }

  /**
   * @brief Subtract another matrix from this matrix and return the result as a new matrix.
   *
   * @param other The matrix to subtract from this matrix. It must have the same size as this
   * matrix.
   * @return Matrix A new matrix that is the result of subtracting the other matrix from this
   * matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix operator-(Matrix const &other) const noexcept
  {
    Matrix result{*this};
    result -= other;
    return result;
  }

  /**
   * @brief Subtract a scalar value from each element of the matrix and return the result as a new
   * matrix.
   *
   * @param scalar The scalar value to subtract from each element of the matrix. It must be
   * convertible to T.
   * @return Matrix A new matrix that is the result of subtracting the scalar value from each
   * element of this matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix operator-(T const &scalar) const noexcept
  {
    Matrix result{*this};
    result -= scalar;
    return result;
  }

  /**
   * @brief Multiply this matrix by a scalar value and return the result as a new matrix.
   *
   * @param scalar The scalar value to multiply each element of the matrix by. It must be
   * convertible to T.
   * @return Matrix A new matrix that is the result of multiplying each element of this matrix by
   * the scalar value.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix operator*(T const &scalar) const noexcept
  {
    Matrix result{*this};
    result *= scalar;
    return result;
  }

  /**
   * @brief Multiply this matrix by another matrix and return the result as a new matrix.
   *
   * @param vector The matrix to multiply this matrix by. The number of columns in this matrix must
   * @return Matrix<T, Rows, Cols> A new matrix that is the result of multiplying this matrix by the
   */
  [[nodiscard]] PBRT_INLINE constexpr Vector<T, Rows> operator*(
      Vector<T, Cols> const &vector) const noexcept
  {
    Vector<T, Rows> result{};

    if constexpr (Rows == 2 && Cols == 2)
    {
      result[0] = (*this)(0, 0) * vector[0] + (*this)(0, 1) * vector[1];
      result[1] = (*this)(1, 0) * vector[0] + (*this)(1, 1) * vector[1];
    }
    else if constexpr (Rows == 3 && Cols == 3)
    {
      result[0] = (*this)(0, 0) * vector[0] + (*this)(0, 1) * vector[1] + (*this)(0, 2) * vector[2];
      result[1] = (*this)(1, 0) * vector[0] + (*this)(1, 1) * vector[1] + (*this)(1, 2) * vector[2];
      result[2] = (*this)(2, 0) * vector[0] + (*this)(2, 1) * vector[1] + (*this)(2, 2) * vector[2];
    }
    else if constexpr (Rows == 4 && Cols == 4)
    {
      result[0] = (*this)(0, 0) * vector[0] + (*this)(0, 1) * vector[1] +
                  (*this)(0, 2) * vector[2] + (*this)(0, 3) * vector[3];
      result[1] = (*this)(1, 0) * vector[0] + (*this)(1, 1) * vector[1] +
                  (*this)(1, 2) * vector[2] + (*this)(1, 3) * vector[3];
      result[2] = (*this)(2, 0) * vector[0] + (*this)(2, 1) * vector[1] +
                  (*this)(2, 2) * vector[2] + (*this)(2, 3) * vector[3];
      result[3] = (*this)(3, 0) * vector[0] + (*this)(3, 1) * vector[1] +
                  (*this)(3, 2) * vector[2] + (*this)(3, 3) * vector[3];
    }
    else
    {
      for (SizeType i = 0; i < Rows; ++i)
      {
        T sum{0};
        for (SizeType j = 0; j < Cols; ++j)
        {
          sum += (*this)(i, j) * vector[j];
        }
        result[i] = sum;
      }
    }

    return result;
  }

  /**
   * @brief Multiply this matrix by another matrix and return the result as a new matrix.
   *
   * @tparam OtherCols The number of columns in the other matrix. It must be greater than 0.
   * @param other The matrix to multiply this matrix by. The number of rows in this matrix must
   * @return Matrix<T, Rows, OtherCols> A new matrix that is the result of multiplying this matrix
   * by the other matrix.
   */
  template <usize OtherCols>
  [[nodiscard]] PBRT_INLINE constexpr Matrix<T, Rows, OtherCols> operator*(
      Matrix<T, Cols, OtherCols> const &other) const noexcept
  {
    if constexpr (Rows > 8 && Cols > 8 && OtherCols > 8)
    {
      return multiply_blocked(other);
    }
    else
    {
      return multiply_naive(other);
    }
  }

  /**
   * @brief Divide each element of the matrix by a scalar value and return the result as a new
   * matrix.
   *
   * @param scalar The scalar value to divide each element of the matrix by. It must be convertible
   * to T and not equal to zero.
   * @return Matrix A new matrix that is the result of dividing each element of this matrix by the
   * scalar value.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix operator/(T const &scalar) const noexcept
  {
    Matrix result{*this};
    result /= scalar;
    return result;
  }

  /**
   * @brief Compare this matrix with another matrix for equality.
   *
   * @param other The matrix to compare with this matrix. It must have the same size as this matrix.
   * @return true if the matrices are equal, false otherwise.
   */
  [[nodiscard]] PBRT_INLINE constexpr bool operator==(Matrix const &other) const noexcept
  {
    for (SizeType i = 0; i < size(); ++i)
    {
      if (m_data[i] != other.m_data[i])
      {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Compare this matrix with another matrix for inequality.
   *
   * @param other The matrix to compare with this matrix. It must have the same size as this matrix.
   * @return true if the matrices are not equal, false otherwise.
   */
  [[nodiscard]] PBRT_INLINE constexpr bool operator!=(Matrix const &other) const noexcept
  {
    return !(*this == other);
  }

  /**
   * @brief Check if this matrix is approximately equal to another matrix within a given epsilon.
   *
   * @param other The matrix to compare with this matrix. It must have the same size as this matrix.
   * @param epsilon The tolerance for approximate equality. It must be convertible to T.
   * @return true if the matrices are approximately equal, false otherwise.
   */
  [[nodiscard]] PBRT_INLINE constexpr bool approx_equal(
      Matrix const &other, T const &epsilon = EPSILON<T>) const noexcept
    requires FloatingPoint<T>
  {
    for (SizeType i = 0; i < size(); ++i)
    {
      if (std::abs(m_data[i] - other.m_data[i]) > epsilon)
      {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Transpose the matrix and return the result as a new matrix.
   *
   * @return Matrix<T, Cols, Rows> A new matrix that is the transpose of this matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix<T, Cols, Rows> transpose() const noexcept
  {
    Matrix<T, Cols, Rows> result;
    for (SizeType i = 0; i < Rows; ++i)
    {
      for (SizeType j = 0; j < Cols; ++j)
      {
        result(j, i) = (*this)(i, j);
      }
    }
    return result;
  }

  /**
   * @brief Calculate the determinant of the matrix.
   *
   * @return T The determinant of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr T determinant() const noexcept
    requires(Rows == Cols)
  {
    if constexpr (Rows == 1)
    {
      return (*this)(0, 0);
    }

    if constexpr (Rows == 2)
    {
      return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
    }

    if constexpr (Rows == 3)
    {
      return (*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1)) -
             (*this)(0, 1) * ((*this)(1, 0) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 0)) +
             (*this)(0, 2) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0));
    }

    if constexpr (Rows == 4)
    {
      T const &a00{(*this)(0, 0)};
      T const &a01{(*this)(0, 1)};
      T const &a02{(*this)(0, 2)};
      T const &a03{(*this)(0, 3)};
      T const &a10{(*this)(1, 0)};
      T const &a11{(*this)(1, 1)};
      T const &a12{(*this)(1, 2)};
      T const &a13{(*this)(1, 3)};
      T const &a20{(*this)(2, 0)};
      T const &a21{(*this)(2, 1)};
      T const &a22{(*this)(2, 2)};
      T const &a23{(*this)(2, 3)};
      T const &a30{(*this)(3, 0)};
      T const &a31{(*this)(3, 1)};
      T const &a32{(*this)(3, 2)};
      T const &a33{(*this)(3, 3)};

      return a03 * a12 * a21 * a30 - a02 * a13 * a21 * a30 - a03 * a11 * a22 * a30 +
             a01 * a13 * a22 * a30 + a02 * a11 * a23 * a30 - a01 * a12 * a23 * a30 -
             a03 * a12 * a20 * a31 + a02 * a13 * a20 * a31 + a03 * a10 * a22 * a31 -
             a00 * a13 * a22 * a31 - a02 * a10 * a23 * a31 + a00 * a12 * a23 * a31 +
             a03 * a11 * a20 * a32 - a01 * a13 * a20 * a32 - a03 * a10 * a21 * a32 +
             a00 * a13 * a21 * a32 + a01 * a10 * a23 * a32 - a00 * a11 * a23 * a32 -
             a02 * a11 * a20 * a33 + a01 * a12 * a20 * a33 + a02 * a10 * a21 * a33 -
             a00 * a12 * a21 * a33 - a01 * a10 * a22 * a33 + a00 * a11 * a22 * a33;
    }

    Matrix<T, Rows, Cols> lu{*this};
    T det{1};

    for (SizeType i = 0; i < Rows; ++i)
    {
      SizeType pivotRow = i;
      T maxVal = std::abs(lu(i, i));

      for (SizeType k = i + 1; k < Rows; ++k)
      {
        T val = std::abs(lu(k, i));
        if (val > maxVal)
        {
          maxVal = val;
          pivotRow = k;
        }
      }

      if (maxVal < PRECISION_EPSILON<T>)
      {
        return T{0};
      }

      if (pivotRow != i)
      {
        for (SizeType j = 0; j < Cols; ++j)
        {
          std::swap(lu.m_data[lu.index(i, j)], lu.m_data[lu.index(pivotRow, j)]);
        }
        det = -det;
      }

      det *= lu(i, i);

      for (SizeType k = i + 1; k < Rows; ++k)
      {
        T factor = lu(k, i) / lu(i, i);
        for (SizeType j = i + 1; j < Cols; ++j)
        {
          lu(k, j) -= factor * lu(i, j);
        }
      }
    }

    return det;
  }

  /**
   * @brief inverse the matrix and return the result as a new matrix.
   *
   * @return Matrix A new matrix that is the inverse of this matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix inverse() const noexcept
    requires(Rows == Cols && FloatingPoint<T>)
  {
    if constexpr (Rows == 1)
    {
      return inverse_1();
    }
    else if constexpr (Rows == 2)
    {
      return inverse_2();
    }
    else if constexpr (Rows == 3)
    {
      return inverse_3();
    }
    else if constexpr (Rows == 4)
    {
      return inverse_4();
    }

    return inverse_n();
  }

  /**
   * @brief Get the absolute value of each element in the matrix and return the result as a new
   * matrix.
   *
   * @return Matrix A new matrix that contains the absolute values of each element of this matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Matrix abs() const noexcept
  {
    Matrix result{*this};
    for (SizeType i = 0; i < size(); ++i)
    {
      result.m_data[i] = std::abs(m_data[i]);
    }
    return result;
  }

  /**
   * @brief Get the zero matrix.
   *
   * @return Matrix A new matrix with all elements initialized to zero.
   */
  [[nodiscard]] PBRT_INLINE static constexpr Matrix zero() noexcept
  {
    return {};
  }

  /**
   * @brief Get the identity matrix.
   *
   * @return Matrix A new identity matrix with ones on the diagonal and zeros elsewhere.
   */
  [[nodiscard]] PBRT_INLINE static constexpr Matrix identity() noexcept
    requires(Rows == Cols)
  {
    return diagonal(T{1});
  }

  /**
   * @brief Get a diagonal matrix with the specified diagonal elements.
   *
   * @param value The value to fill the diagonal of the matrix with. It must be convertible to T.
   * @return Matrix A new diagonal matrix with the specified value on the diagonal.
   */
  [[nodiscard]] PBRT_INLINE static constexpr Matrix diagonal(T const &value) noexcept
    requires(Rows == Cols)
  {
    Matrix result{};

    for (SizeType i = 0; i < Rows; ++i)
    {
      result(i, i) = value;
    }

    return result;
  }

  /**
   * @brief Get the beginning iterator of the matrix.
   *
   * @return Iterator An iterator to the beginning of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Iterator begin() noexcept
  {
    return m_data.begin();
  }

  /**
   * @brief Get the beginning const iterator of the matrix.
   *
   * @return ConstIterator A const iterator to the beginning of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstIterator begin() const noexcept
  {
    return m_data.cbegin();
  }

  /**
   * @brief Get the beginning const iterator of the matrix.
   *
   * @return ConstIterator A const iterator to the beginning of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstIterator cbegin() const noexcept
  {
    return m_data.cbegin();
  }

  /**
   * @brief Get the end iterator of the matrix.
   *
   * @return Iterator An iterator to the end of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Iterator end() noexcept
  {
    return m_data.end();
  }

  /**
   * @brief Get the end const iterator of the matrix.
   *
   * @return ConstIterator A const iterator to the end of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstIterator end() const noexcept
  {
    return m_data.cend();
  }

  /**
   * @brief Get the end const iterator of the matrix.
   *
   * @return ConstIterator A const iterator to the end of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstIterator cend() const noexcept
  {
    return m_data.cend();
  }

  /**
   * @brief Get the reverse beginning iterator of the matrix.
   *
   * @return ReverseIterator A reverse iterator to the beginning of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ReverseIterator rbegin() noexcept
  {
    return m_data.rbegin();
  }

  /**
   * @brief Get the reverse beginning const iterator of the matrix.
   *
   * @return ConstReverseIterator A const reverse iterator to the beginning of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReverseIterator rbegin() const noexcept
  {
    return m_data.crbegin();
  }

  /**
   * @brief Get the reverse beginning const iterator of the matrix.
   *
   * @return ConstReverseIterator A const reverse iterator to the beginning of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReverseIterator crbegin() const noexcept
  {
    return m_data.rend();
  }

  /**
   * @brief Get the reverse end iterator of the matrix.
   *
   * @return ReverseIterator A reverse iterator to the end of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ReverseIterator rend() noexcept
  {
    return m_data.crend();
  }

  /**
   * @brief Get the reverse end const iterator of the matrix.
   *
   * @return ConstReverseIterator A const reverse iterator to the end of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReverseIterator rend() const noexcept
  {
    return m_data.crend();
  }

  /**
   * @brief Get the reverse end const iterator of the matrix.
   *
   * @return ConstReverseIterator A const reverse iterator to the end of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstReverseIterator crend() const noexcept
  {
    return m_data.crend();
  }

  /**
   * @brief Get the size of the matrix.
   *
   * @return SizeType The size of the matrix, which is the product of Rows and Cols.
   */
  [[nodiscard]] PBRT_INLINE constexpr SizeType size() const noexcept
  {
    return Rows * Cols;
  }

  /**
   * @brief Get the maximum size of the matrix.
   *
   * @return SizeType The maximum size of the matrix, which is the product of Rows and Cols.
   */
  [[nodiscard]] PBRT_INLINE constexpr SizeType max_size() const noexcept
  {
    return Rows * Cols;
  }

  /**
   * @brief Check if the matrix is empty.
   *
   * @return true if the matrix is empty, false otherwise.
   */
  [[nodiscard]] PBRT_INLINE constexpr bool empty() const noexcept
  {
    return false;
  }

  /**
   * @brief Get a pointer to the underlying data of the matrix.
   *
   * @return Pointer A pointer to the first element of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr Pointer data() noexcept
  {
    return m_data.data();
  }

  /**
   * @brief Get a const pointer to the underlying data of the matrix.
   *
   * @return ConstPointer A const pointer to the first element of the matrix.
   */
  [[nodiscard]] PBRT_INLINE constexpr ConstPointer data() const noexcept
  {
    return m_data.data();
  }

  /**
   * @brief Get the number of columns in the matrix.
   *
   * @return SizeType The number of columns in the matrix, which is equal to Cols.
   */
  [[nodiscard]] PBRT_INLINE constexpr SizeType rows() const noexcept
  {
    return Rows;
  }

  /**
   * @brief Get the number of rows in the matrix.
   *
   * @return SizeType The number of rows in the matrix, which is equal to Rows.
   */
  [[nodiscard]] PBRT_INLINE constexpr SizeType columns() const noexcept
  {
    return Cols;
  }

private:
  alignas(ALIGN_OF(T)) std::array<T, Rows * Cols> m_data{};

  [[nodiscard]] PBRT_INLINE constexpr SizeType index(SizeType row, SizeType col) const noexcept
  {
    PBRT_ASSERT_MSG(row < Rows && col < Cols, "Matrix indices out of bounds.");
    return (row * Cols) + col;
  }

  [[nodiscard]] PBRT_INLINE constexpr bool is_invertible(
      T const &epsilon = PRECISION_EPSILON<T>) const noexcept
    requires(Rows == Cols && FloatingPoint<T>)
  {
    return std::abs(determinant()) > epsilon;
  }

  template <usize OtherCols>
  [[nodiscard]] PBRT_INLINE constexpr Matrix<T, Rows, OtherCols> multiply_blocked(
      Matrix<T, Cols, OtherCols> const &other) const noexcept
  {
    Matrix<T, Rows, OtherCols> result{};
    constexpr SizeType blockSize{32};

    for (SizeType ii = 0; ii < Rows; ii += blockSize)
    {
      for (SizeType jj = 0; jj < OtherCols; jj += blockSize)
      {
        for (SizeType kk = 0; kk < Cols; kk += blockSize)
        {
          multiply_block(result, other, ii, jj, kk, blockSize);
        }
      }
    }

    return result;
  }

  template <usize OtherCols>
  [[nodiscard]] PBRT_INLINE constexpr Matrix<T, Rows, OtherCols> multiply_naive(
      Matrix<T, Cols, OtherCols> const &other) const noexcept
  {
    Matrix<T, Rows, OtherCols> result{};

    for (SizeType i = 0; i < Rows; ++i)
    {
      for (SizeType j = 0; j < OtherCols; ++j)
      {
        T sum{0};

        for (SizeType k = 0; k < Cols; ++k)
        {
          sum += (*this)(i, k) * other(k, j);
        }

        result(i, j) = sum;
      }
    }

    return result;
  }

  template <usize OtherCols>
  PBRT_INLINE constexpr void multiply_block(Matrix<T, Rows, OtherCols> &result,
                                            Matrix<T, Cols, OtherCols> const &other, SizeType ii,
                                            SizeType jj, SizeType kk,
                                            SizeType blockSize) const noexcept
  {
    SizeType iMax{std::min(ii + blockSize, Rows)};
    SizeType jMax{std::min(jj + blockSize, OtherCols)};
    SizeType kMax{std::min(kk + blockSize, Cols)};

    for (SizeType i = ii; i < iMax; ++i)
    {
      for (SizeType j = jj; j < jMax; ++j)
      {
        T sum{result(i, j)};

        for (SizeType k = kk; k < kMax; ++k)
        {
          sum += (*this)(i, k) * other(k, j);
        }

        result(i, j) = sum;
      }
    }
  }

  [[nodiscard]] PBRT_INLINE constexpr Matrix inverse_1() const noexcept
  {
    T det{(*this)(0, 0)};
    if (std::abs(det) <= PRECISION_EPSILON<T>) [[unlikely]]
    {
      return {};
    }

    return {T{1} / det};
  }

  [[nodiscard]] PBRT_INLINE constexpr Matrix inverse_2() const noexcept
  {
    T a{(*this)(0, 0)};
    T b{(*this)(0, 1)};
    T c{(*this)(1, 0)};
    T d{(*this)(1, 1)};

    T det{a * d - b * c};
    if (std::abs(det) <= PRECISION_EPSILON<T>) [[unlikely]]
    {
      return {};
    }

    T inverseDet{T{1} / det};
    return {d * inverseDet, -b * inverseDet, -c * inverseDet, a * inverseDet};
  }

  [[nodiscard]] PBRT_INLINE constexpr Matrix inverse_3() const noexcept
  {
    T m00{(*this)(0, 0)};
    T m01{(*this)(0, 1)};
    T m02{(*this)(0, 2)};
    T m10{(*this)(1, 0)};
    T m11{(*this)(1, 1)};
    T m12{(*this)(1, 2)};
    T m20{(*this)(2, 0)};
    T m21{(*this)(2, 1)};
    T m22{(*this)(2, 2)};

    T c00{m11 * m22 - m12 * m21};
    T c01{m10 * m22 - m12 * m20};
    T c02{m10 * m21 - m11 * m20};
    T c10{m01 * m22 - m02 * m21};
    T c11{m00 * m22 - m02 * m20};
    T c12{m00 * m21 - m01 * m20};
    T c20{m01 * m12 - m02 * m11};
    T c21{m00 * m12 - m02 * m10};
    T c22{m00 * m11 - m01 * m10};

    T det{m00 * c00 - m01 * c10 + m02 * c20};
    if (std::abs(det) <= PRECISION_EPSILON<T>) [[unlikely]]
    {
      return {};
    }

    T inverseDet{T{1} / det};

    return {c00 * inverseDet,  -c10 * inverseDet, c20 * inverseDet,
            -c01 * inverseDet, c11 * inverseDet,  -c21 * inverseDet,
            c02 * inverseDet,  -c12 * inverseDet, c22 * inverseDet};
  }

  [[nodiscard]] PBRT_INLINE constexpr Matrix inverse_4() const noexcept
  {
    T m00{(*this)(0, 0)};
    T m01{(*this)(0, 1)};
    T m02{(*this)(0, 2)};
    T m03{(*this)(0, 3)};
    T m10{(*this)(1, 0)};
    T m11{(*this)(1, 1)};
    T m12{(*this)(1, 2)};
    T m13{(*this)(1, 3)};
    T m20{(*this)(2, 0)};
    T m21{(*this)(2, 1)};
    T m22{(*this)(2, 2)};
    T m23{(*this)(2, 3)};
    T m30{(*this)(3, 0)};
    T m31{(*this)(3, 1)};
    T m32{(*this)(3, 2)};
    T m33{(*this)(3, 3)};

    T s0{m00 * m11 - m01 * m10};
    T s1{m00 * m12 - m02 * m10};
    T s2{m00 * m13 - m03 * m10};
    T s3{m01 * m12 - m02 * m11};
    T s4{m01 * m13 - m03 * m11};
    T s5{m02 * m13 - m03 * m12};

    T c5{m22 * m33 - m23 * m32};
    T c4{m21 * m33 - m23 * m31};
    T c3{m21 * m32 - m22 * m31};
    T c2{m20 * m33 - m23 * m30};
    T c1{m20 * m32 - m22 * m30};
    T c0{m20 * m31 - m21 * m30};

    T det{s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0};
    if (std::abs(det) <= PRECISION_EPSILON<T>) [[unlikely]]
    {
      return {};
    }

    T inverseDet{T{1} / det};

    return {(m11 * c5 - m12 * c4 + m13 * c3) * inverseDet,
            (-m01 * c5 + m02 * c4 - m03 * c3) * inverseDet,
            (m31 * s5 - m32 * s4 + m33 * s3) * inverseDet,
            (-m21 * s5 + m22 * s4 - m23 * s3) * inverseDet,

            (-m10 * c5 + m12 * c2 - m13 * c1) * inverseDet,
            (m00 * c5 - m02 * c2 + m03 * c1) * inverseDet,
            (-m30 * s5 + m32 * s2 - m33 * s1) * inverseDet,
            (m20 * s5 - m22 * s2 + m23 * s1) * inverseDet,

            (m10 * c4 - m11 * c2 + m13 * c0) * inverseDet,
            (-m00 * c4 + m01 * c2 - m03 * c0) * inverseDet,
            (m30 * s4 - m31 * s2 + m33 * s0) * inverseDet,
            (-m20 * s4 + m21 * s2 - m23 * s0) * inverseDet,

            (-m10 * c3 + m11 * c1 - m12 * c0) * inverseDet,
            (m00 * c3 - m01 * c1 + m02 * c0) * inverseDet,
            (-m30 * s3 + m31 * s1 - m32 * s0) * inverseDet,
            (m20 * s3 - m21 * s1 + m22 * s0) * inverseDet};
  }

  // NOLINTNEXTLINE
  [[nodiscard]] PBRT_INLINE constexpr Matrix inverse_n() const noexcept
    requires(Rows == Cols && FloatingPoint<T>)
  {
    Matrix<T, Rows, Rows * 2> augmented{};

    for (SizeType i = 0; i < Rows; ++i)
    {
      for (SizeType j = 0; j < Rows; ++j)
      {
        augmented(i, j) = (*this)(i, j);
        augmented(i, j + Rows) = (i == j) ? T{1} : T{0};
      }
    }

    for (SizeType i = 0; i < Rows; ++i)
    {
      SizeType pivotRow = i;
      T maxVal = std::abs(augmented(i, i));

      for (SizeType k = i + 1; k < Rows; ++k)
      {
        T val = std::abs(augmented(k, i));
        if (val > maxVal)
        {
          maxVal = val;
          pivotRow = k;
        }
      }

      if (maxVal <= PRECISION_EPSILON<T>) [[unlikely]]
      {
        return {};
      }

      if (pivotRow != i)
      {
        for (SizeType j = 0; j < Rows * 2; ++j)
        {
          std::swap(augmented(i, j), augmented(pivotRow, j));
        }
      }

      T pivot = augmented(i, i);
      for (SizeType j = 0; j < Rows * 2; ++j)
      {
        augmented(i, j) /= pivot;
      }

      for (SizeType k = 0; k < Rows; ++k)
      {
        if (k != i)
        {
          T factor = augmented(k, i);
          for (SizeType j = 0; j < Rows * 2; ++j)
          {
            augmented(k, j) -= factor * augmented(i, j);
          }
        }
      }
    }

    Matrix result{};
    for (SizeType i = 0; i < Rows; ++i)
    {
      for (SizeType j = 0; j < Rows; ++j)
      {
        result(i, j) = augmented(i, j + Rows);
      }
    }

    return result;
  }
};

/**
 * @brief Scalar multiplication of a matrix.
 *
 * @tparam T The type of the matrix components.
 * @tparam Rows The number of rows in the matrix.
 * @tparam Cols The number of columns in the matrix.
 * @param scalar The scalar value to multiply the matrix by.
 * @param matrix The matrix to multiply.
 * @return Matrix A new matrix with each component multiplied by the scalar.
 */
template <Arithmetic T, usize Rows, usize Cols>
  requires(Rows > 0 && Cols > 0)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, Rows, Cols> operator*(
    T const &scalar, Matrix<T, Rows, Cols> const &matrix) noexcept
{
  return matrix * scalar;
}

/**
 * @brief Transpose of a matrix.
 *
 * @tparam T The type of the matrix components.
 * @tparam Rows The number of rows in the matrix.
 * @tparam Cols The number of columns in the matrix.
 * @param matrix The matrix to transpose.
 * @return Matrix<T, Cols, Rows> A new matrix that is the transpose of the input matrix.
 */
template <Arithmetic T, usize Rows, usize Cols>
  requires(Rows > 0 && Cols > 0)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, Cols, Rows> transpose(
    Matrix<T, Rows, Cols> const &matrix) noexcept
{
  return matrix.transpose();
}

/**
 * @brief Calculate the determinant of a matrix. Requires a square matrix.
 *
 * @tparam T The type of the matrix components.
 * @tparam N The size of the square matrix.
 * @param matrix The matrix to calculate the determinant of.
 * @return T The determinant of the matrix.
 */
template <Arithmetic T, usize N>
  requires(N > 0)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr T determinant(Matrix<T, N, N> const &matrix) noexcept
{
  return matrix.determinant();
}

/**
 * @brief Calculate the inverse of a matrix. Requires a square matrix.
 *
 * @tparam T The type of the matrix components.
 * @tparam N The size of the square matrix.
 * @param matrix The matrix to calculate the inverse of.
 * @return Matrix<T, N, N> The inverse of the matrix.
 */
template <Arithmetic T, usize N>
  requires(N > 0 && FloatingPoint<T>)
[[nodiscard]] PBRT_API PBRT_INLINE constexpr Matrix<T, N, N> inverse(
    Matrix<T, N, N> const &matrix) noexcept
{
  return matrix.inverse();
}

using Mat2f = Matrix<f32, 2, 2>;
using Mat3f = Matrix<f32, 3, 3>;
using Mat4f = Matrix<f32, 4, 4>;

using Mat2d = Matrix<f64, 2, 2>;
using Mat3d = Matrix<f64, 3, 3>;
using Mat4d = Matrix<f64, 4, 4>;

using Mat2i = Matrix<i32, 2, 2>;
using Mat3i = Matrix<i32, 3, 3>;
using Mat4i = Matrix<i32, 4, 4>;

using Mat2u = Matrix<u32, 2, 2>;
using Mat3u = Matrix<u32, 3, 3>;
using Mat4u = Matrix<u32, 4, 4>;

using Mat2 = Mat2f;
using Mat3 = Mat3f;
using Mat4 = Mat4f;

template <>
PBRT_INLINE constexpr Matrix<f32, 4, 4> &Matrix<f32, 4, 4>::operator+=(Matrix const &other) noexcept
{
#if defined(PBRT_HAS_SSE42)
  for (SizeType i = 0; i < 16; i += 4)
  {
    __m128 a{_mm_loadu_ps(&m_data[i])};
    __m128 b{_mm_loadu_ps(&other.m_data[i])};
    __m128 result{_mm_add_ps(a, b)};

    _mm_storeu_ps(&m_data[i], result);
  }

  return *this;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    m_data[i] += other.m_data[i];
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Matrix<f32, 4, 4> &Matrix<f32, 4, 4>::operator-=(Matrix const &other) noexcept
{
#if defined(PBRT_HAS_SSE42)
  for (SizeType i = 0; i < 16; i += 4)
  {
    __m128 a{_mm_loadu_ps(&m_data[i])};
    __m128 b{_mm_loadu_ps(&other.m_data[i])};
    __m128 result{_mm_sub_ps(a, b)};

    _mm_storeu_ps(&m_data[i], result);
  }

  return *this;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    m_data[i] -= other.m_data[i];
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Matrix<f32, 4, 4> &Matrix<f32, 4, 4>::operator*=(f32 const &scalar) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 scalarVec{_mm_set1_ps(scalar)};

  for (SizeType i = 0; i < 16; i += 4)
  {
    __m128 vec{_mm_loadu_ps(&m_data[i])};
    __m128 result{_mm_mul_ps(vec, scalarVec)};

    _mm_storeu_ps(&m_data[i], result);
  }

  return *this;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    m_data[i] *= scalar;
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Matrix<f32, 4, 4> &Matrix<f32, 4, 4>::operator/=(f32 const &scalar) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 scalarVec{_mm_set1_ps(scalar)};

  for (SizeType i = 0; i < 16; i += 4)
  {
    __m128 vec{_mm_loadu_ps(&m_data[i])};
    __m128 result{_mm_div_ps(vec, scalarVec)};

    _mm_storeu_ps(&m_data[i], result);
  }

  return *this;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    m_data[i] /= scalar;
  }
  return *this;
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr Vector<f32, 4> Matrix<f32, 4, 4>::operator*(
    Vector<f32, 4> const &vector) const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 vec{_mm_loadu_ps(vector.data())};

  __m128 row0{_mm_loadu_ps(m_data.data())};
  __m128 row1{_mm_loadu_ps(&m_data[4])};
  __m128 row2{_mm_loadu_ps(&m_data[8])};
  __m128 row3{_mm_loadu_ps(&m_data[12])};

  __m128 res0{_mm_dp_ps(row0, vec, 0xF1)};
  __m128 res1{_mm_dp_ps(row1, vec, 0xF2)};
  __m128 res2{_mm_dp_ps(row2, vec, 0xF4)};
  __m128 res3{_mm_dp_ps(row3, vec, 0xF8)};

  __m128 result{_mm_or_ps(_mm_or_ps(res0, res1), _mm_or_ps(res2, res3))};

  Vector<f32, 4> out{};
  _mm_storeu_ps(out.data(), result);

  return out;
#else
  Vector<f32, 4> result{};
  for (usize i = 0; i < 4; ++i)
  {
    f32 sum = 0.0F;
    for (usize j = 0; j < 4; ++j)
    {
      sum += (*this)(i, j) * vector[j];
    }
    result[i] = sum;
  }
  return result;
#endif
}

template <>
template <usize OtherCols>
[[nodiscard]] PBRT_INLINE constexpr Matrix<f32, 4, OtherCols> Matrix<f32, 4, 4>::operator*(
    Matrix<f32, 4, OtherCols> const &other) const noexcept
{
#if defined(PBRT_HAS_SSE42)
  if constexpr (OtherCols == 4)
  {
    Matrix<f32, 4, 4> result{};

    __m128 col0{_mm_loadu_ps(&other.m_data[0])};
    __m128 col1{_mm_loadu_ps(&other.m_data[4])};
    __m128 col2{_mm_loadu_ps(&other.m_data[8])};
    __m128 col3{_mm_loadu_ps(&other.m_data[12])};

    _MM_TRANSPOSE4_PS(col0, col1, col2, col3);

    for (usize i = 0; i < 4; ++i)
    {
      __m128 row{_mm_loadu_ps(&m_data[i * 4])};

      __m128 res0{_mm_dp_ps(row, col0, 0xF1)};
      __m128 res1{_mm_dp_ps(row, col1, 0xF2)};
      __m128 res2{_mm_dp_ps(row, col2, 0xF4)};
      __m128 res3{_mm_dp_ps(row, col3, 0xF8)};

      __m128 finalResult{_mm_or_ps(_mm_or_ps(res0, res1), _mm_or_ps(res2, res3))};
      _mm_storeu_ps(&result.m_data[i * 4], finalResult);
    }

    return result;
  }
#endif

  return multiply_naive(other);
}

template <>
[[nodiscard]] PBRT_INLINE constexpr bool Matrix<f32, 4, 4>::operator==(
    Matrix const &other) const noexcept
{
#if defined(PBRT_HAS_SSE42)
  for (SizeType i = 0; i < 16; i += 4)
  {
    __m128 a{_mm_loadu_ps(&m_data[i])};
    __m128 b{_mm_loadu_ps(&other.m_data[i])};
    __m128 cmp{_mm_cmpeq_ps(a, b)};
    int mask{_mm_movemask_ps(cmp)};

    if ((mask & 0xF) != 0xF)
    {
      return false;
    }
  }

  return true;
#endif

  for (SizeType i = 0; i < size(); ++i)
  {
    if (m_data[i] != other.m_data[i])
    {
      return false;
    }
  }
  return true;
}

template <>
[[nodiscard]] PBRT_INLINE constexpr bool Matrix<f32, 4, 4>::approx_equal(
    Matrix const &other, f32 const &epsilon) const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 epsVec{_mm_set1_ps(epsilon)};
  for (SizeType i = 0; i < 16; i += 4)
  {
    __m128 a{_mm_loadu_ps(&m_data[i])};
    __m128 b{_mm_loadu_ps(&other.data()[i])};
    __m128 diff{_mm_sub_ps(a, b)};
    __m128 absDiff{_mm_and_ps(diff, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)))};
    __m128 cmp{_mm_cmple_ps(absDiff, epsVec)};
    int mask{_mm_movemask_ps(cmp)};
    if ((mask & 0xF) != 0xF)
    {
      return false;
    }
  }

  return true;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    if (std::abs(m_data[i] - other.m_data[i]) > epsilon)
    {
      return false;
    }
  }

  return true;
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr Matrix<f32, 4, 4> Matrix<f32, 4, 4>::transpose() const noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 row0{_mm_loadu_ps(data())};
  __m128 row1{_mm_loadu_ps(&data()[4])};
  __m128 row2{_mm_loadu_ps(&data()[8])};
  __m128 row3{_mm_loadu_ps(&data()[12])};

  __m128 tmp0{_mm_unpacklo_ps(row0, row1)};
  __m128 tmp1{_mm_unpacklo_ps(row2, row3)};
  __m128 tmp2{_mm_unpackhi_ps(row0, row1)};
  __m128 tmp3{_mm_unpackhi_ps(row2, row3)};

  Matrix<f32, 4, 4> result{};
  _mm_storeu_ps(result.data(), _mm_movelh_ps(tmp0, tmp1));
  _mm_storeu_ps(&result.data()[4], _mm_movehl_ps(tmp1, tmp0));
  _mm_storeu_ps(&result.data()[8], _mm_movelh_ps(tmp2, tmp3));
  _mm_storeu_ps(&result.data()[12], _mm_movehl_ps(tmp3, tmp2));

  return result;
#else
  Matrix<f32, 4, 4> result;
  for (SizeType i = 0; i < 4; ++i)
  {
    for (SizeType j = 0; j < 4; ++j)
    {
      result(j, i) = (*this)(i, j);
    }
  }
  return result;
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr Matrix<f32, 4, 4> Matrix<f32, 4, 4>::abs() const noexcept
{
#if defined(PBRT_HAS_SSE42)
  Matrix<f32, 4, 4> result{};

  for (SizeType i = 0; i < 16; i += 4)
  {
    __m128 data{_mm_loadu_ps(&m_data[i])};
    __m128 absMask{_mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))};
    __m128 absData{_mm_and_ps(data, absMask)};
    _mm_storeu_ps(&result.m_data[i], absData);
  }

  return result;
#else
  Matrix<f32, 4, 4> result{};

  for (SizeType i = 0; i < size(); ++i)
  {
    result.m_data[i] = std::abs(m_data[i]);
  }

  return result;
#endif
}

template <>
PBRT_INLINE constexpr Matrix<f64, 4, 4> &Matrix<f64, 4, 4>::operator+=(Matrix const &other) noexcept
{
#if defined(PBRT_HAS_AVX2)
  for (SizeType i = 0; i < 16; i += 4)
  {
    __m256d a{_mm256_loadu_pd(&m_data[i])};
    __m256d b{_mm256_loadu_pd(&other.m_data[i])};
    __m256d result{_mm256_add_pd(a, b)};
    _mm256_storeu_pd(&m_data[i], result);
  }
  return *this;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    m_data[i] += other.m_data[i];
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Matrix<f32, 4, 4> &Matrix<f32, 4, 4>::operator+=(f32 const &scalar) noexcept
{
#if defined(PBRT_HAS_SSE42)
  __m128 scalarVec{_mm_set1_ps(scalar)};

  for (SizeType i = 0; i < 16; i += 4)
  {
    __m128 vec{_mm_loadu_ps(&m_data[i])};
    __m128 result{_mm_add_ps(vec, scalarVec)};
    _mm_storeu_ps(&m_data[i], result);
  }

  return *this;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    m_data[i] += scalar;
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Matrix<f64, 4, 4> &Matrix<f64, 4, 4>::operator-=(Matrix const &other) noexcept
{
#if defined(PBRT_HAS_AVX2)
  for (SizeType i = 0; i < 16; i += 4)
  {
    __m256d a{_mm256_loadu_pd(&m_data[i])};
    __m256d b{_mm256_loadu_pd(&other.m_data[i])};
    __m256d result{_mm256_sub_pd(a, b)};
    _mm256_storeu_pd(&m_data[i], result);
  }
  return *this;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    m_data[i] -= other.m_data[i];
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Matrix<f64, 4, 4> &Matrix<f64, 4, 4>::operator*=(f64 const &scalar) noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d scalarVec{_mm256_set1_pd(scalar)};

  for (SizeType i = 0; i < 16; i += 4)
  {
    __m256d vec{_mm256_loadu_pd(&m_data[i])};
    __m256d result{_mm256_mul_pd(vec, scalarVec)};
    _mm256_storeu_pd(&m_data[i], result);
  }
  return *this;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    m_data[i] *= scalar;
  }
  return *this;
#endif
}

template <>
PBRT_INLINE constexpr Matrix<f64, 4, 4> &Matrix<f64, 4, 4>::operator/=(f64 const &scalar) noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d scalarVec{_mm256_set1_pd(scalar)};

  for (SizeType i = 0; i < 16; i += 4)
  {
    __m256d vec{_mm256_loadu_pd(&m_data[i])};
    __m256d result{_mm256_div_pd(vec, scalarVec)};
    _mm256_storeu_pd(&m_data[i], result);
  }
  return *this;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    m_data[i] /= scalar;
  }
  return *this;
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr Vector<f64, 4> Matrix<f64, 4, 4>::operator*(
    Vector<f64, 4> const &vector) const noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d vec{_mm256_loadu_pd(vector.data())};

  Vector<f64, 4> result{};

  for (SizeType i = 0; i < 4; ++i)
  {
    __m256d row{_mm256_loadu_pd(&(*this)(i, 0))};
    __m256d mul{_mm256_mul_pd(row, vec)};

    __m256d hadd1{_mm256_hadd_pd(mul, mul)};

    __m128d high{_mm256_extractf128_pd(hadd1, 1)};
    __m128d low{_mm256_castpd256_pd128(hadd1)};
    __m128d sum{_mm_add_pd(high, low)};

    _mm_store_sd(&result[i], sum);
  }

  return result;
#else
  Vector<f64, 4> result{};
  for (SizeType i = 0; i < 4; ++i)
  {
    f64 sum{0};
    for (SizeType j = 0; j < 4; ++j)
    {
      sum += (*this)(i, j) * vector[j];
    }
    result[i] = sum;
  }
  return result;
#endif
}

template <>
template <usize OtherCols>
[[nodiscard]] PBRT_INLINE constexpr Matrix<f64, 4, OtherCols> Matrix<f64, 4, 4>::operator*(
    Matrix<f64, 4, OtherCols> const &other) const noexcept
{
#if defined(PBRT_HAS_AVX2)
  Matrix<f64, 4, OtherCols> result;

  __m256d row0 = _mm256_loadu_pd(&(*this)(0, 0));
  __m256d row1 = _mm256_loadu_pd(&(*this)(1, 0));
  __m256d row2 = _mm256_loadu_pd(&(*this)(2, 0));
  __m256d row3 = _mm256_loadu_pd(&(*this)(3, 0));

  for (usize col = 0; col < OtherCols; ++col)
  {
    __m256d otherCol = _mm256_set_pd(other(3, col), other(2, col), other(1, col), other(0, col));

    __m256d prod0 = _mm256_mul_pd(row0, otherCol);
    __m128d low0 = _mm256_castpd256_pd128(prod0);
    __m128d high0 = _mm256_extractf128_pd(prod0, 1);
    __m128d sum0 = _mm_add_pd(low0, high0);
    __m128d final0 = _mm_hadd_pd(sum0, sum0);
    result(0, col) = _mm_cvtsd_f64(final0);

    __m256d prod1 = _mm256_mul_pd(row1, otherCol);
    __m128d low1 = _mm256_castpd256_pd128(prod1);
    __m128d high1 = _mm256_extractf128_pd(prod1, 1);
    __m128d sum1 = _mm_add_pd(low1, high1);
    __m128d final1 = _mm_hadd_pd(sum1, sum1);
    result(1, col) = _mm_cvtsd_f64(final1);

    __m256d prod2 = _mm256_mul_pd(row2, otherCol);
    __m128d low2 = _mm256_castpd256_pd128(prod2);
    __m128d high2 = _mm256_extractf128_pd(prod2, 1);
    __m128d sum2 = _mm_add_pd(low2, high2);
    __m128d final2 = _mm_hadd_pd(sum2, sum2);
    result(2, col) = _mm_cvtsd_f64(final2);

    __m256d prod3 = _mm256_mul_pd(row3, otherCol);
    __m128d low3 = _mm256_castpd256_pd128(prod3);
    __m128d high3 = _mm256_extractf128_pd(prod3, 1);
    __m128d sum3 = _mm_add_pd(low3, high3);
    __m128d final3 = _mm_hadd_pd(sum3, sum3);
    result(3, col) = _mm_cvtsd_f64(final3);
  }

  return result;

#else
  return multiply_naive(other);
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr bool Matrix<f64, 4, 4>::operator==(
    Matrix const &other) const noexcept
{
#if defined(PBRT_HAS_AVX2)
  for (SizeType i = 0; i < 16; i += 4)
  {
    __m256d a{_mm256_loadu_pd(&m_data[i])};
    __m256d b{_mm256_loadu_pd(&other.m_data[i])};
    __m256d cmp{_mm256_cmp_pd(a, b, _CMP_EQ_OQ)};
    int mask{_mm256_movemask_pd(cmp)};

    if (mask != 0xF)
    {
      return false;
    }
  }
  return true;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    if (m_data[i] != other.m_data[i])
    {
      return false;
    }
  }
  return true;
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr bool Matrix<f64, 4, 4>::approx_equal(
    Matrix const &other, f64 const &epsilon) const noexcept
{
#if defined(PBRT_HAS_AVX2)
  __m256d epsVec{_mm256_set1_pd(epsilon)};
  for (SizeType i = 0; i < 16; i += 4)
  {
    __m256d a{_mm256_loadu_pd(&m_data[i])};
    __m256d b{_mm256_loadu_pd(&other.data()[i])};
    __m256d diff{_mm256_sub_pd(a, b)};
    __m256d absDiff{
        _mm256_and_pd(diff, _mm256_castsi256_pd(_mm256_set1_epi64x(0x7FFFFFFFFFFFFFFF)))};
    __m256d cmp{_mm256_cmp_pd(absDiff, epsVec, _CMP_LE_OQ)};
    int mask{_mm256_movemask_pd(cmp)};
    if (mask != 0xF)
    {
      return false;
    }
  }
  return true;
#else
  for (SizeType i = 0; i < size(); ++i)
  {
    if (std::abs(m_data[i] - other.m_data[i]) > epsilon)
    {
      return false;
    }
  }
  return true;
#endif
}

template <>
[[nodiscard]] PBRT_INLINE constexpr Matrix<f64, 4, 4> Matrix<f64, 4, 4>::transpose() const noexcept
{
#if defined(PBRT_HAS_AVX2)
  Matrix<f64, 4, 4> result{};

  __m256d row0{_mm256_loadu_pd(data())};
  __m256d row1{_mm256_loadu_pd(&data()[4])};
  __m256d row2{_mm256_loadu_pd(&data()[8])};
  __m256d row3{_mm256_loadu_pd(&data()[12])};

  __m256d t0{_mm256_unpacklo_pd(row0, row1)};
  __m256d t1{_mm256_unpackhi_pd(row0, row1)};
  __m256d t2{_mm256_unpacklo_pd(row2, row3)};
  __m256d t3{_mm256_unpackhi_pd(row2, row3)};

  __m256d r0{_mm256_permute2f128_pd(t0, t2, 0x20)};
  __m256d r1{_mm256_permute2f128_pd(t1, t3, 0x20)};
  __m256d r2{_mm256_permute2f128_pd(t0, t2, 0x31)};
  __m256d r3{_mm256_permute2f128_pd(t1, t3, 0x31)};

  _mm256_storeu_pd(result.data(), r0);
  _mm256_storeu_pd(&result.data()[4], r1);
  _mm256_storeu_pd(&result.data()[8], r2);
  _mm256_storeu_pd(&result.data()[12], r3);

  return result;
#else
  Matrix<f64, 4, 4> result{};
  for (SizeType i = 0; i < 4; ++i)
  {
    for (SizeType j = 0; j < 4; ++j)
    {
      result(j, i) = (*this)(i, j);
    }
  }
  return result;
#endif
}

template <>
PBRT_INLINE constexpr Matrix<f64, 4, 4> Matrix<f64, 4, 4>::abs() const noexcept
{
#if defined(PBRT_HAS_AVX2)
  Matrix<f64, 4, 4> result{};

  for (SizeType i = 0; i < 16; i += 4)
  {
    __m256d data{_mm256_loadu_pd(&m_data[i])};
    __m256d absMask{_mm256_castsi256_pd(_mm256_set1_epi64x(0x7FFFFFFFFFFFFFFF))};
    __m256d absData{_mm256_and_pd(data, absMask)};
    _mm256_storeu_pd(&result.m_data[i], absData);
  }

  return result;
#else
  Matrix<f64, 4, 4> result{};

  for (SizeType i = 0; i < size(); ++i)
  {
    result.m_data[i] = std::abs(m_data[i]);
  }

  return result;
#endif
}
} // namespace pbrt::math
