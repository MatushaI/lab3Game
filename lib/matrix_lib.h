#ifndef MATRIXTEMPLATECL_MATRIX_LIB_H
#define MATRIXTEMPLATECL_MATRIX_LIB_H
#include <climits>
#include <iostream>
#include <iterator>

template <typename T>
class Matrix {
private:
    T* data_ = nullptr;
    size_t rows_ = 0;
    size_t columns_ = 0;
public:
    //Конструкторы
    Matrix() noexcept = default;

    Matrix(size_t rows, size_t columns) noexcept {
        if(rows > 0 && columns > 0) {
            rows_ = rows;
            columns_ = columns;
            data_ = new T[rows_ * columns_];
        }
    }

    Matrix(size_t rows, size_t columns, T const& elem) noexcept : Matrix(rows, columns) {
        std::fill(data_, data_ + rows*columns, elem);
    }

    Matrix(std::initializer_list< std::initializer_list<T> > list) {
        rows_ = list.size();
        size_t max = (std::max_element(list.begin(), list.end(), [](std::initializer_list<T> const& x,
                std::initializer_list<T> const& y) {return x.size() < y.size();}))->size();
        columns_ = max;

        rows_ = columns_ ? rows_ : 0;

        data_ = new T[rows_*columns_];
        int i = 0;
        for (auto const& row : list) {
            T const* iter = row.begin();
            for (int j = 0; j < columns_; j++, i++) {
                if(iter != row.end()) {
                    data_[i] = std::move(*iter);
                    std::advance(iter, 1);
                } else {
                    data_[i] = T();
                }
            }
        }
    }

    Matrix(std::initializer_list<T> list) : columns_(list.size()), rows_(list.size() ? 1 : 0) {
        data_ = new T [columns_ * rows_];
        T *data = data_;
        T const* iter = list.begin();
        while (iter != list.end()) {
            *data = std::move(*iter);
            iter++;
            data++;
        }
    }
    //Копирующий конструктор
    Matrix(Matrix const& matrix) {
        rows_ = matrix.rows_;
        columns_ = matrix.columns_;
        data_ = new T [matrix.rows_ * matrix.columns_];
        std::copy(matrix.data_, matrix.data_ + matrix.columns_ * matrix.rows_, data_);
    }
    // Перемещающее копирование
    Matrix(Matrix && matrix) noexcept : columns_(matrix.columns_), rows_(matrix.rows_) {
        data_ = matrix.data_;
        matrix.data_ = nullptr;
        matrix.rows_ = 0;
        matrix.columns_ = 0;
    }
    // Присваивание
    Matrix & operator=(Matrix const& old) {
        if(this != &old) {
            T* newdata = new T [old.columns_ * old.rows_];

            std::copy(old.data_, old.data_ + old.columns_ * old.rows_, newdata);
            delete [] data_;
            data_ = newdata;
            columns_ = old.columns_;
            rows_ = old.rows_;
        }
        return *this;
    }
    // Перемещающее присваивание
    Matrix & operator=(Matrix && old) noexcept {
        delete [] data_;
        data_ = old.data_;
        columns_ = old.columns_;
        rows_ = old.rows_;

        old.data_ = nullptr;
        old.rows_ = 0;
        old.columns_ = 0;
        return *this;
    }

    ~Matrix(){
        delete [] data_;
    }
    // размер
    std::pair<size_t, size_t> size() {
        return {rows_, columns_};
    }

    size_t getColumns() {
        return columns_;
    }

    size_t getRows() {
        return rows_;
    }

    // Итератор

    struct const_TIterator {

        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T const& reference;
        typedef std::random_access_iterator_tag iterator_category;

        const_TIterator(T* ptr, T* ptr_begin, T* ptr_end) : ptr_(ptr), ptr_begin_(ptr_begin), ptr_end_(ptr_end) {}
        const_TIterator() = default;
        const_TIterator(const_TIterator const& iter) {
            ptr_ = iter.ptr_;
            ptr_begin_ = iter.ptr_begin_;
            ptr_end_ = iter.ptr_end_;
        }
        const_TIterator & operator=(const_TIterator const& iter) = default;

        difference_type ptrPos() const { return ptr_ - ptr_begin_; }

        T const& operator*() const { return *ptr_; }
        T const& operator[](size_t index) const { return *(ptr_begin_ + index); }

        const_TIterator::difference_type operator- (const_TIterator const& iter) const { return ptr_ - iter.ptr_; }
        const_TIterator operator++(int) { const_TIterator temp = *this; ptr_++; return temp; }
        const_TIterator operator--(int) { const_TIterator temp = *this; ptr_--; return temp; }
        const_TIterator & operator++() { ptr_++; return *this; }
        const_TIterator & operator--() { ptr_--; return *this; }
        const_TIterator operator+(size_t count) const { return TIterator(ptr_ + count, ptr_begin_, ptr_end_); }
        const_TIterator operator-(size_t count) const { return TIterator(ptr_ - count, ptr_begin_, ptr_end_); }
        friend const_TIterator operator+ (size_t count, const_TIterator const& iter) {return const_TIterator(iter.ptr_ + count, iter.ptr_begin_, iter.ptr_end_);}
        const_TIterator & operator+=(size_t count) { ptr_ += count; return *this; }
        const_TIterator & operator-=(size_t count) { ptr_ -= count; return *this; }
        const_TIterator * operator->() { return &(*this); };
        friend bool operator==(const_TIterator const& it1, const_TIterator const& it2) { return it1.ptr_ == it2.ptr_; }
        friend bool operator!=(const_TIterator const& it1, const_TIterator const& it2) { return it1.ptr_ != it2.ptr_; }
        friend bool operator<(const_TIterator const& it1, const_TIterator const& it2) { return it1.ptr_ < it2.ptr_; }
        friend bool operator>(const_TIterator const& it1, const_TIterator const& it2) { return it1.ptr_ > it2.ptr_; }
        friend bool operator<=(const_TIterator const& it1, const_TIterator const& it2) { return it1.ptr_ <= it2.ptr_; }
        friend bool operator>=(const_TIterator const& it1, const_TIterator const& it2) { return it1.ptr_ >= it2.ptr_; }

        ~const_TIterator() = default;
    private:
        T* ptr_ = nullptr;
        T* ptr_begin_ = nullptr;
        T* ptr_end_ = nullptr;
    };

    struct const_stringIterator {

        typedef ptrdiff_t difference_type;
        typedef const_TIterator value_type;
        typedef const_TIterator* pointer;
        typedef const_TIterator reference;
        typedef std::random_access_iterator_tag iterator_category;

        const_stringIterator(size_t rowCount, size_t columnsCount, T *data, size_t row): rowCount_(rowCount),
                                                                                         columnsCount_(columnsCount), iterData_(data), actualRow_(row) {};
        const_stringIterator() = default;
        const_stringIterator(const_stringIterator const& iter) = default;
        const_stringIterator & operator=(const_stringIterator const& iter) = default;

        const_TIterator operator[](size_t index) const {
            const_TIterator(iterData_ + actualRow_ * columnsCount_ + index, iterData_ + actualRow_ * columnsCount_,
                      iterData_ + actualRow_ * columnsCount_ + columnsCount_);
        }

        const_stringIterator::difference_type operator- (const_stringIterator const& iter) const { return actualRow_ - iter.actualRow_; }
        const_stringIterator operator++(int count) {actualRow_++; return *this;}
        const_stringIterator operator--(int count) {actualRow_--; return *this;}
        const_stringIterator & operator++() { const_stringIterator temp = *this; actualRow_++; return temp; }
        const_stringIterator & operator--() { const_stringIterator temp = *this; actualRow_--; return temp; }
        const_stringIterator operator+(size_t count) const { return stringIterator(rowCount_, columnsCount_, iterData_, actualRow_ + count); }
        const_stringIterator operator-(size_t count) const { return stringIterator(rowCount_, columnsCount_, iterData_, actualRow_ - count); }
        friend const_stringIterator operator+ (size_t count, const_stringIterator const& iter) { return const_stringIterator(iter.rowCount_, iter.columnsCount_, iter.iterData_, iter.actualRow_ + count); }
        const_stringIterator & operator+=(size_t count) { return actualRow_ += count; return *this; }
        const_stringIterator & operator-=(size_t count) { return actualRow_ -= count; return *this; }
        const_stringIterator * operator->() { return &(*this); };
        friend bool operator==(const_stringIterator const& it1, const_stringIterator const& it2) { return it1.actualRow_ == it2.actualRow_; }
        friend bool operator!=(const_stringIterator const& it1, const_stringIterator const& it2) { return it1.actualRow_ != it2.actualRow_; }
        friend bool operator<(const_stringIterator const& it1, const_stringIterator const& it2) { return it1.actualRow_ < it2.actualRow_; }
        friend bool operator>(const_stringIterator const& it1, const_stringIterator const& it2) { return it1.actualRow_ > it2.actualRow_; }
        friend bool operator>=(const_stringIterator const& it1, const_stringIterator const& it2) { return it1.actualRow_ >= it2.actualRow_; }
        friend bool operator<=(const_stringIterator const& it1, const_stringIterator const& it2) { return it1.actualRow_ <= it2.actualRow_; }

        const_TIterator operator*() const noexcept {
            return const_TIterator(iterData_ + actualRow_ * columnsCount_, iterData_ + actualRow_ * columnsCount_,
                                   iterData_ + actualRow_ * columnsCount_ + columnsCount_);
        }

        const_TIterator begin() const { return const_TIterator(iterData_ + actualRow_ * columnsCount_,
                                                               iterData_ + actualRow_ * columnsCount_,
                                                               iterData_ + actualRow_ * columnsCount_ + columnsCount_); }
        const_TIterator end() const { return const_TIterator(iterData_ + actualRow_ * columnsCount_ + columnsCount_,
                                                             iterData_ + actualRow_ * columnsCount_,
                                                             iterData_ + actualRow_ * columnsCount_ + columnsCount_); }

        ~const_stringIterator() = default;
    private:
        size_t rowCount_ = 0;
        size_t columnsCount_ = 0;
        T *iterData_ = nullptr;
        size_t actualRow_ = INT_MIN;
    };

    struct TIterator {

        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::random_access_iterator_tag iterator_category;

        TIterator() = default;
        TIterator(T* ptr, T* ptr_begin, T* ptr_end) : ptr_(ptr), ptr_begin_(ptr_begin), ptr_end_(ptr_end) {}

        TIterator(TIterator const& iter) {
            ptr_ = iter.ptr_;
            ptr_begin_ = iter.ptr_begin_;
            ptr_end_ = iter.ptr_end_;
        }
        TIterator & operator=(TIterator const& iter) = default;

        difference_type ptrPos() const { return ptr_ - ptr_begin_; }
        //T& operator*() { return *ptr_; }
        T& operator*() const { return *ptr_; }

        T& operator[](size_t index) const { return *(ptr_begin_ + index); }

        TIterator::difference_type operator- (TIterator const& iter) const { return ptr_ - iter.ptr_; }
        TIterator operator++(int) { TIterator temp = *this; ptr_++; return temp; }
        TIterator operator--(int) { TIterator temp = *this; ptr_--; return temp; }
        TIterator & operator++() { ptr_++; return *this; }
        TIterator & operator--() { ptr_--; return *this; }
        TIterator operator+(size_t count) const { return TIterator(ptr_ + count, ptr_begin_, ptr_end_); }
        TIterator operator-(size_t count) const { return TIterator(ptr_ - count, ptr_begin_, ptr_end_); }
        friend TIterator operator+(size_t count, TIterator const& iter) { return TIterator(iter.ptr_ + count, iter.ptr_begin_, iter.ptr_end_); }
        TIterator & operator+=(size_t count) { ptr_ += count; return *this; }
        TIterator & operator-=(size_t count) { ptr_ -= count; return *this; }
        TIterator * operator->() { return &(*this); };
        friend bool operator==(TIterator const& it1, TIterator const& it2) { return it1.ptr_ == it2.ptr_; }
        friend bool operator!=(TIterator const& it1, TIterator const& it2) { return it1.ptr_ != it2.ptr_; }
        friend bool operator<(TIterator const& it1, TIterator const& it2) { return it1.ptr_ < it2.ptr_; }
        friend bool operator>(TIterator const& it1, TIterator const& it2) { return it1.ptr_ > it2.ptr_; }
        friend bool operator<=(TIterator const& it1, TIterator const& it2) { return it1.ptr_ <= it2.ptr_; }
        friend bool operator>=(TIterator const& it1, TIterator const& it2) { return it1.ptr_ >= it2.ptr_; }

        operator const_TIterator() {return const_TIterator(ptr_, ptr_begin_, ptr_end_); }
        ~TIterator() = default;
    private:
        T* ptr_ = nullptr;
        T* ptr_begin_ = nullptr;
        T* ptr_end_ = nullptr;
    };

    struct stringIterator {

        typedef ptrdiff_t difference_type;
        typedef TIterator value_type;
        typedef TIterator* pointer;
        typedef TIterator reference;
        typedef std::random_access_iterator_tag iterator_category;

        stringIterator();
        stringIterator(size_t rowCount, size_t columnsCount, T *data, size_t row): rowCount_(rowCount),
            columnsCount_(columnsCount), iterData_(data), actualRow_(row) {};

        stringIterator(stringIterator const& iter) = default;
        stringIterator & operator=(stringIterator const& iter) = default;

        reference operator[](size_t index) const {
            return TIterator(iterData_ + actualRow_ * columnsCount_ + index, iterData_ + actualRow_ * columnsCount_,
                             iterData_ + actualRow_ * columnsCount_ + columnsCount_);
        }

        difference_type operator- (stringIterator const& iter) const { return actualRow_ - iter.actualRow_; }
        stringIterator operator++(int count) { stringIterator temp = *this; actualRow_++; return temp; }
        stringIterator operator--(int count) { stringIterator temp = *this; actualRow_--; return temp; }
        stringIterator & operator++() { actualRow_++; return *this; }
        stringIterator & operator--() { actualRow_--; return *this; }
        stringIterator operator+(size_t count) const { return stringIterator(rowCount_, columnsCount_, iterData_, actualRow_ + count); }
        friend stringIterator operator+(size_t count, stringIterator const& iter) { return stringIterator(iter.rowCount_, iter.columnsCount_, iter.iterData_, iter.actualRow_ + count); }
        stringIterator operator-(size_t count) const { return stringIterator(rowCount_, columnsCount_, iterData_, actualRow_ - count); }
        stringIterator & operator+=(size_t count) { actualRow_ += count; return *this; }
        stringIterator & operator-=(size_t count) { actualRow_ -= count; return *this; }
        stringIterator * operator->() { return &(*this); }
        friend bool operator==(stringIterator const& it1, stringIterator const& it2) { return it1.actualRow_ == it2.actualRow_; }
        friend bool operator!=(stringIterator const& it1, stringIterator const& it2) { return it1.actualRow_ != it2.actualRow_; }
        friend bool operator<(stringIterator const& it1, stringIterator const& it2) { return it1.actualRow_ < it2.actualRow_; }
        friend bool operator>(stringIterator const& it1, stringIterator const& it2) { return it1.actualRow_ > it2.actualRow_; }
        friend bool operator>=(stringIterator const& it1, stringIterator const& it2) { return it1.actualRow_ >= it2.actualRow_; }
        friend bool operator<=(stringIterator const& it1, stringIterator const& it2) { return it1.actualRow_ <= it2.actualRow_; }

        TIterator operator*() const noexcept {
            return TIterator(iterData_ + actualRow_ * columnsCount_, iterData_ + actualRow_ * columnsCount_,
                             iterData_ + actualRow_ * columnsCount_ + columnsCount_);
        }

        TIterator begin() const { return TIterator(iterData_ + actualRow_ * columnsCount_,
                                                   iterData_ + actualRow_ * columnsCount_,
                                                   iterData_ + actualRow_ * columnsCount_ + columnsCount_); }
        TIterator end() const { return TIterator(iterData_ + actualRow_ * columnsCount_ + columnsCount_,
                                                 iterData_ + actualRow_ * columnsCount_,
                                                 iterData_ + actualRow_ * columnsCount_ + columnsCount_); }

        ~stringIterator() = default;
    private:
        size_t rowCount_ = 0;
        size_t columnsCount_ = 0;
        T *iterData_ = nullptr;
        size_t actualRow_ = INT_MIN;
    };

    stringIterator begin() noexcept { return stringIterator(rows_, columns_, data_, 0); }
    const_stringIterator begin() const noexcept { return const_stringIterator(rows_, columns_, data_, 0); }
    stringIterator end() noexcept { return stringIterator(rows_, columns_, data_, rows_); }
    const_stringIterator end() const noexcept { return const_stringIterator(rows_, columns_, data_, rows_); }

    static_assert(std::random_access_iterator<stringIterator>);
    static_assert(std::random_access_iterator<TIterator>);
    static_assert(std::random_access_iterator<const_TIterator>);
    static_assert(std::random_access_iterator<const_stringIterator>);

    TIterator operator[](size_t index) {
        if(index >= rows_) {
            throw std::out_of_range("index more rows count");
        }
        return TIterator(data_ + columns_ * index, data_ + columns_ * index,
                         data_ + columns_ * index + columns_);
    }

    const_TIterator operator[](size_t index) const {
        if(index >= rows_) {
            throw std::out_of_range("index more rows count");
        }
        return const_TIterator(data_ + columns_ * index, data_ + columns_ * index,
                         data_ + columns_ * index + columns_);
    }

    void addRowPos(size_t pos) {
        if(pos <= rows_) {
            T* newdata = new T[columns_ * (rows_ + 1)];
            std::move(data_, data_ + pos * columns_, newdata);
            std::fill(newdata + pos * columns_, newdata + (pos + 1) * columns_, T());
            std::move(data_ + pos * columns_, data_ + rows_*columns_, newdata + (pos + 1) * columns_);
            delete [] data_;
            data_ = newdata;
            rows_++;
        }
    }

    void addColumn(const_TIterator const& iter) {
        long pos = iter.ptrPos();
        if(pos <= columns_) {
            T* newdata = new T[(columns_ + 1) * rows_];
            for (int i = 0; i < rows_; ++i) {
                std::move(data_ + i*columns_, data_ + i*columns_ + pos, newdata + i*(columns_+1));
                newdata[i*(columns_ + 1) + pos] = T();
                std::move(data_ + i*columns_ + pos, data_ + i*columns_ + columns_,
                          newdata + i*(columns_ + 1) + pos + 1);
            }
            delete [] data_;
            data_ = newdata;
            columns_++;
        }
    }
};

#endif //MATRIXTEMPLATECL_MATRIX_LIB_H
