//! \file

#ifndef YACK_CONTAINER_MATRIX_INCLUDED
#define YACK_CONTAINER_MATRIX_INCLUDED 1

#include "yack/container/matrix/data.hpp"
#include "yack/container/matrix/metrics.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/ptr/arc.hpp"
#include <iostream>

namespace yack
{
    
    //! constructor for matrix
#define YACK_MATRIX_CTOR(ROWS,COLS) \
/**/      matrix_data<T>(),         \
/**/      matrix_metrics((void**)&line,(void**)&head,ROWS,COLS,sizeof(T),build_row_at)

    //__________________________________________________________________________
    //
    //
    //! data for matrix
    //
    //__________________________________________________________________________
    template <typename T>
    class matrix : public matrix_data<T>, public matrix_metrics
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                    //!< aliases
        typedef typename matrix_data<T>::row row;  //!< alias
        typedef arc_ptr< matrix<T> >         ptr;  //!< alias
        using matrix_data<T>::line;
        using matrix_data<T>::head;


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~matrix() noexcept { clear(items); }

        //! setup empty
        inline matrix() noexcept: matrix_data<T>(), matrix_metrics() {}

        //! setup empty or full of defaut items
        inline matrix(const size_t r, const size_t c) :
        YACK_MATRIX_CTOR(r,c)
        {
            setup();
        }

        //! copy relying on copy constructor for type
        inline matrix(const matrix &M) :
        YACK_MATRIX_CTOR(M.rows,M.cols)
        {
            setup_from(M);
        }

        //! copy with transposition
        template <typename U>
        inline matrix(const matrix<U> &M, const transposed_t &) :
        YACK_MATRIX_CTOR(M.cols,M.rows)
        {
            setup_from_transposed<U>(M);
        }


        //! copy relying on copy constructor for type
        template <typename U>
        inline matrix(const matrix<U> &M, const transmogrify_t &) :
        YACK_MATRIX_CTOR(M.rows,M.cols)
        {
            setup_from<U>(M);
        }


        //! setup with transformation
        template <typename FUNC, typename U>
        inline matrix(FUNC &func, const matrix<U> &M) :
        YACK_MATRIX_CTOR(M.rows,M.cols)
        {
            setup();
            apply(func,M);
        }

        //! setup with transformation and transposition
        template <typename FUNC, typename U>
        inline matrix(FUNC &func, const matrix<U> &M, const transposed_t &) :
        YACK_MATRIX_CTOR(M.cols,M.rows)
        {
            setup();
            apply(func,M);
        }



        //! assign by copy/swap
        inline matrix & operator=( const matrix &other )
        {
            matrix tmp(other);
            xch(tmp);
            return *this;
        }

        //______________________________________________________________________
        //
        // interface methods
        //______________________________________________________________________

        //! release by exchanging with an empty matrix
        virtual void release() noexcept
        {
            matrix<T> tmp;
            xch(tmp);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! row[1..rows]
        inline row &operator[](const size_t r) noexcept
        { assert(r>=1); assert(r<=rows); return line[r]; }

        //! row[1..rows] const
        inline const row &operator[](const size_t r) const noexcept
        { assert(r>=1); assert(r<=rows); return line[r]; }

        //! no throw exchange
        inline void xch(matrix &other) noexcept
        {
            this->swap_data_with(other);
            swap_metrics_with(other);
        }

        //! load same value
        inline void ld(param_type args)
        {
            type *p = head;
            for(size_t i=items;i>0;--i) *(p++) = args;
        }

        //! load same value into row[r]
        inline void ld_row(const size_t r, param_type args)
        {
            assert(r>=1); assert(r<=rows);
            type *p = & line[r][1];
            for(size_t i=cols;i>0;--i) *(p++) = args;
        }

        //! load same value into col[c]
        inline void ld_col(const size_t c, param_type args)
        {
            assert(c>=1); assert(c<=cols);
            for(size_t i=1;i<=rows;++i)
            {
                line[i][c] = args;
            }
        }



        //! swap rows content
        inline void swap_rows(const size_t r1, const size_t r2) noexcept
        {
            assert(r1>=1); assert(r1<=rows);
            assert(r2>=1); assert(r2<=rows);
            out_of_reach::swap(&line[r1][1],&line[r2][1],stride);
        }

        //! swap cols contents
        inline void swap_cols(const size_t c1, const size_t c2) noexcept
        {
            assert(c1>=1); assert(c1<=cols);
            assert(c2>=1); assert(c2<=cols);
            for(size_t i=rows;i>0;--i) out_of_reach::swap(&line[i][c1],&line[i][c2],sizeof(T));
        }

        //! swap both rows and cols
        inline void swap_both(const size_t i, const size_t j) noexcept
        {
            swap_rows(i,j);
            swap_cols(i,j);
        }


        //! make nr x nc iff mismatching sizes
        inline void make(const size_t nr, const size_t nc)
        {
            if(rows!=nr||cols!=nc)
            {
                matrix tmp(nr,nc);
                xch(tmp);
            }
        }

        //! get column into target
        void get_column(writable<T> &target,
                        const size_t column) const
        {
            assert(column<=cols);
            assert(column>=1);
            assert(target.size()>=rows);
            for(size_t i=rows;i>0;--i)
            {
                target[i] = (*this)[i][column];
            }
        }

        //! set column from source
        void set_column(const size_t column, const readable<T> &source)
        {
            assert(column<=cols);
            assert(column>=1);
            assert(source.size()>=rows);
            for(size_t i=rows;i>0;--i)
            {
                (*this)[i][column] = source[i];
            }
        }


        //______________________________________________________________________
        //
        // algebra built-in methods
        //______________________________________________________________________

        //! lhs = M*rhs
        template <typename U, typename V>
        inline void operator()(writable<U> &lhs, const readable<V> &rhs) const
        {
            assert(lhs.size()==rows);
            assert(rhs.size()==cols);
            for(size_t i=lhs.size();i>0;--i)
            {
                lhs[i] = line[i].template dot<U>(rhs);
            }
        }

        //! this = A*B
        inline void operator()(const matrix<T> &A, const matrix<T> &B)
        {
            assert(rows==A.rows);
            assert(cols==B.cols);
            assert(A.cols==B.rows);
            const size_t nc = cols;
            const size_t nk = A.cols;
            for(size_t i=rows;i>0;--i)
            {
                matrix_row<T>       &r_i = (*this)[i];
                const matrix_row<T> &A_i = A[i];
                for(size_t j=nc;j>0;--j)
                {
                    T sum = A_i[1] * B[1][j];
                    for(size_t k=nk;k>1;--k)
                    {
                        sum += A_i[k] * B[k][j];
                    }
                    r_i[j] = sum;
                }
            }
        }

        //! access linear space
        inline thin_array<type>       get_contiguous() noexcept       { return thin_array<type>(head,items); }

        //! access linear space
        inline thin_array<const_type> get_contiguous() const noexcept { return thin_array<const_type>(head,items); }

        //! compute minor matrix
        template <typename U>
        inline void compute_minor(matrix<U> &m, const size_t i, const size_t j) const
        {
            assert(rows>1);
            assert(cols>1);
            assert(rows-1==m.rows);
            assert(cols-1==m.cols);
            assert(i>=1); assert(i<=rows);
            
            const matrix<T> &a = *this;
            for(size_t r=i-1;r>0;--r)
                minor_row(m[r],a[r],j);
            for(size_t r=rows;r>i;--r)
                minor_row(m[r-1],a[r],j);
        }

        //! assign using '=' semantics
        template <typename U> inline
        void assign(const matrix<U> &M)
        {
            assert( matrix_metrics::have_same_sizes(*this,M) );
            mutable_type                   *target = head;
            typename matrix<U>::const_type *source = M.head;
            for(size_t i=items;i>0;--i)
            {
                *(target++) = *(source++);
            }
        }

        //! assign transposed, using '=' semantics
        template <typename U> inline
        void assign(const matrix<U> &M, const transposed_t &)
        {
            assert( matrix_metrics::are_transposed(*this,M) );
            for(size_t i=rows;i>0;--i)
            {
                for(size_t j=cols;j>0;--j)
                {
                    (*this)[i][j] = M[j][i];
                }
            }
        }

        //! per item transform
        template <typename FUNC, typename U> inline
        void apply(FUNC &func, const matrix<U> &M)
        {
            mutable_type                   *target = head;
            typename matrix<U>::const_type *source = M.head;
            for(size_t i=items;i>0;--i)
            {
                *(target++) = func( *(source++) );
            }
        }

        //______________________________________________________________________
        //
        // output
        //______________________________________________________________________

        //! display as Julia matrix
        friend inline std::ostream & operator<<(std::ostream &os,const matrix &M) {
            M.display(os); return os;
        }

        //! display as C code
        template <typename NAME>
        inline void print_code(std::ostream &os, const NAME &name) const
        {
            const size_t n = std::cerr.precision();
            std::cerr.precision(15);
            for(size_t i=1;i<=rows;++i)
            {
                for(size_t j=1;j<=cols;++j)
                {
                    os << ' ' << name << "[" << i << "][" << j << "]=" << (*this)[i][j] << ";";
                }
                os << std::endl;
            }
            std::cerr.precision(n);
        }
        

    private:

        template <typename U> void minor_row(matrix_row<U> &target, const matrix_row<T> &source, const size_t j) const
        {
            assert(j>=1); assert(j<=cols);
            for(size_t c=j-1;c>0;--c)
                target[c] = source[c];
            for(size_t c=cols;c>j;--c)
                target[c-1]=source[c];
        }

        //! creation of a row
        static inline void build_row_at(void *row_addr, void *data_ptr, const size_t num_cols) noexcept
        { new (row_addr) row(static_cast<mutable_type *>(data_ptr),num_cols); }

        //! clear built objects
        inline void clear(size_t built) noexcept {
            while(built-- > 0)
                destruct(head+built);
        }

        
#define YACK_MATRIX_SETUP_ENTER() try                                  //!< enter setup
#define YACK_MATRIX_SETUP_LEAVE() catch(...) { clear(built); throw; }  //!< leave setup

        //______________________________________________________________________
        //
        //! setup items
        //______________________________________________________________________
        inline void setup()
        {
            size_t        built = 0;
            YACK_MATRIX_SETUP_ENTER() {
                mutable_type *target = head;
                while(built<items) {
                    new (target++) mutable_type();
                    ++built;
                }
            } YACK_MATRIX_SETUP_LEAVE()
        }

        //______________________________________________________________________
        //
        //! setup items by direct copy
        //______________________________________________________________________
        template<typename U>
        inline void setup_from(const matrix<U> &M)
        {
            assert(have_same_sizes(*this,M));

            size_t built = 0;
            YACK_MATRIX_SETUP_ENTER() {
                typename matrix<U>::const_type *source = M.head;
                mutable_type                   *target =   head;
                while(built<items) {
                    new (target++) mutable_type(*(source++));
                    ++built;
                }
            } YACK_MATRIX_SETUP_LEAVE()
        }

        //______________________________________________________________________
        //
        //! setup items
        //______________________________________________________________________
        template <typename U>
        inline void setup_from_transposed(const matrix<U> &M)
        {
            assert(are_transposed(*this,M));
            size_t built = 0;
            YACK_MATRIX_SETUP_ENTER() {
                mutable_type *data = head;
                for(size_t j=1;j<=M.cols;++j)
                    for(size_t i=1;i<=M.rows;++i)
                    {
                        new (data++) mutable_type(M[i][j]);
                        ++built;
                    }
            } YACK_MATRIX_SETUP_LEAVE()
        }



        //______________________________________________________________________
        //
        // display Julia's style
        //______________________________________________________________________
        inline void display(std::ostream &os) const
        {
            switch(rows)
            {
                case 0: os << "[]"; return;
                case 1: if(cols==1) { os << *head; return; } break;
                default:
                    break;
            }
            assert(rows>0);
            os << '[';
            line[1].display(os);
            for(size_t r=2;r<=rows;++r) line[r].display(os<<';');
            os << ']';
        }

    };
    
}

#endif

