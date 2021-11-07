//! \file

#ifndef YACK_CONTAINER_MATRIX_INCLUDED
#define YACK_CONTAINER_MATRIX_INCLUDED 1

#include "yack/container/matrix/data.hpp"
#include "yack/container/matrix/metrics.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/out-of-reach.hpp"
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
        using matrix_data<T>::line;
        using matrix_data<T>::head;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~matrix() throw() { clear(items); }

        //! setup empty
        inline matrix() throw(): matrix_data<T>(), matrix_metrics() {}

        //! setup empty or not
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
        inline matrix(const matrix &M, const transposed_t &) :
        YACK_MATRIX_CTOR(M.cols,M.rows)
        {
            setup_from_transposed(M);
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
        // methods
        //______________________________________________________________________

        //! row[1..rows]
        inline row &operator[](const size_t r) throw()
        { assert(r>=1); assert(r<=rows); return line[r]; }

        //! row[1..rows] const
        inline const row &operator[](const size_t r) const throw()
        { assert(r>=1); assert(r<=rows); return line[r]; }

        //! no throw exchange
        inline void xch(matrix &other) throw()
        {
            matrix_data<T>::swap_with(other);
            matrix_metrics::swap_with(other);
        }

        //! load same value
        inline void ld(param_type args)
        {
            type *p = head;
            for(size_t i=items;i>0;--i) *(p++) = args;
        }

        //! swap rows content
        inline void swap_rows(const size_t r1, const size_t r2) throw()
        {
            assert(r1>=1); assert(r1<=rows);
            assert(r2>=1); assert(r2<=rows);
            out_of_reach::swap(&line[r1][1],&line[r2][1],stride);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        template <typename U>
        inline void operator()(writable<T> &lhs, const readable<U> &rhs) const
        {
            assert(lhs.size()<=rows);
            assert(lhs.size()==rhs.size());
            for(size_t i=lhs.size();i>0;--i)
            {
                lhs[i] = line[i].template dot<U>(rhs);
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


    private:

        //! creation of a row
        static inline void build_row_at(void *row_addr, void *data_ptr, const size_t num_cols) throw()
        { new (row_addr) row(static_cast<mutable_type *>(data_ptr),num_cols); }

        //! clear built objects
        inline void clear(size_t built) throw() {
            while(built-- > 0)
                destruct(head+built);
        }

        
#define YACK_MATRIX_SETUP_ENTER() try                                  //!< enter setup
#define YACK_MATRIX_SETUP_LEAVE() catch(...) { clear(built); throw; }  //!< leave setup
        //!
        //! setup items
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

        //! setup items
        inline void setup_from(const matrix &M)
        {
            assert(have_same_sizes(*this,M));

            size_t built = 0;
            YACK_MATRIX_SETUP_ENTER() {
                const_type   *source = M.head;
                mutable_type *target =   head;
                while(built<items) {
                    new (target++) mutable_type(*(source++));
                    ++built;
                }
            } YACK_MATRIX_SETUP_LEAVE()
        }

        //! setup items
        inline void setup_from_transposed(const matrix &M)
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

        inline void display(std::ostream &os) const
        {
            switch(rows)
            {
                case 0: os << "[]"; return;
                case 1: os << *head; return;
                default:
                    break;
            }
            os << '[';
            if(rows>0)
            {
                line[1].display(os);
                for(size_t r=2;r<=rows;++r) line[r].display(os<<';');
            }
            os << ']';
        }

    };
    
}

#endif

