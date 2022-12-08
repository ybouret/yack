
//! \file

#ifndef YACK_APEX_WORTHY_INCLUDED
#define YACK_APEX_WORTHY_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/large-object.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! Wary ORTHho Yield
    //
    //__________________________________________________________________________
    struct worthy
    {


        //______________________________________________________________________
        //
        //! memory model for each array of coefficients
        //______________________________________________________________________
        typedef memory::dyadic memory_model;

        //______________________________________________________________________
        //
        //! array of rational coefficients
        //______________________________________________________________________
        typedef cxx_array<apq,memory_model> coefficients;


        class qmetrics : public large_object
        {
        public:
            explicit qmetrics(const size_t dims);
            qmetrics(const qmetrics &) throw();
            virtual ~qmetrics() throw();
            
            const size_t dimension;

        private:
            YACK_DISABLE_ASSIGN(qmetrics);
        };

        class qcache;

        //______________________________________________________________________
        //
        //! array of coefficients with its norm
        //______________________________________________________________________
        class qarray : public qmetrics
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
           
            //! setup : load and simplify coefficients, compute nrm2
            template <typename T>
            explicit qarray(const readable<T> &cof) :
            qmetrics(cof.size()), next(0), prev(0), coef(cof,transmogrify), nrm2(0) {
                setup( );
            }
            
            //! cleanup
            virtual ~qarray() throw();

            //! hard copy (for list copy constructor)
            qarray(const qarray &other);
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! compute Gram-Schmidt weight = <coef|v>/nrm2
            apq weight(const readable<apq> &v) const;
            
            //! display
            friend std::ostream & operator<<(std::ostream &, const qarray &);

            //! test all coefficients equality
            friend bool operator==(const qarray &lhs, const qarray &rhs) throw();

            //! test coefficients difference
            friend bool operator!=(const qarray &lhs, const qarray &rhs) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            qarray            *next;  //!< for list
            qarray            *prev;  //!< for list
            const coefficients coef;  //!< coefficients, simplified
            const apn          nrm2;  //!< |coef|^2

        private:
            YACK_DISABLE_ASSIGN(qarray);
            friend class qcache;
            void setup();
        };


        //! cache of qarrays with same dims
        class qcache : public qmetrics, public counted
        {
        public:
            explicit qcache(const size_t dims);
            virtual ~qcache() throw();

            const pool_of<qarray> & operator*()  const throw();
            const pool_of<qarray> * operator->() const throw();

            void    keep(qarray *q) throw();

            //! return a setup qarray
            template <typename T> inline
            qarray *make(const readable<T> &cof)
            {
                assert(cof.size() == dimension);
                if(used.size<=0)
                {
                    return new qarray(cof);
                }
                else
                {
                    qarray        &qa = *used.head; assert(dimension==qa.dimension);
                    writable<apq> &cf = coerce(qa.coef);
                    for(size_t i=dimension;i>0;--i) cf[i] = cof[i];
                    qa.setup();
                    return used.query();
                }
            }

            qarray *copy(const qarray &);

            void release() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qcache);
            cxx_pool_of<qarray> used;
        };

        typedef arc_ptr<qcache> qshared;


        class qarrays
        {
        public:
            explicit qarrays(const qshared &cache) throw();
            virtual ~qarrays() throw();
            qarrays(const qarrays &other);

            void release() throw();  

            template <typename T> inline
            const qarray &push_back(const readable<T> &cof)
            {
                return * base.push_back( repo->make(cof) );
            }

            void pop_back() throw();


            const list_of<qarray> & operator*()  const throw();
            const list_of<qarray> * operator->() const throw();


        private:
            cxx_list_of<qarray> base;

        public:
            qshared             repo;

        private:
            YACK_DISABLE_ASSIGN(qarrays);
            void quit() throw();
        };






        //! family maturity
        enum maturity
        {
            in_progress, //! size < dims-1
            almost_done, //! size == dims-1, next vector is unique!
            fully_grown  //! size == dims
        };

        //! initialize maturity from a dimension and a size
        static maturity maturity_of(const size_t dims, const size_t size) throw();

        //______________________________________________________________________
        //
        //! family of simplified, univocal vectors
        //______________________________________________________________________
        class qfamily  : public qmetrics
        {
        public:
            typedef cxx_series<coefficients*,memory_model> coeffs_ptr;
            typedef cxx_series<size_t,memory_model>        indexing_t;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qfamily(const qshared &cache); //!< setup with cache
            virtual ~qfamily() throw();             //!< cleanup
            qfamily(const qfamily &);               //!< copy
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void reset() throw(); //!< release/free all memory

            
            //! grow algorithm
            /**
             - apply Gram-Schmidt, returns as soon as possible
             - all the vectors are univocal
             */
            template <typename T>
            bool grow(const readable<T> &arr)
            {
                assert(arr.size()==dimension);
                for(size_t i=dimension;i>0;--i)
                     u_k[i] = v_k[i] = arr[i];
                return try_grow();
            }

            const list_of<qarray> * operator->()  const throw(); //!< access
            const list_of<qarray> & operator*()   const throw(); //!< access

            //! display
            friend std::ostream & operator<<(std::ostream &, const qfamily &);

            //! test equality of families
            static bool eq(const qfamily &lhs, const qfamily &rhs) throw();


            //! check same families, calling eq(lhs,rhs)
            friend bool operator==(const qfamily &lhs, const qfamily &rhs) throw();

            //! return simplified projected vector
            /**
             should be the identity for any vector
             in the span of the current basis
             */
            template <typename T>
            const readable<apq> &project(const readable<T> &vec)
            {
                assert(vec.size()==dimension);
                for(size_t i=dimension;i>0;--i)
                {
                    v_k[i] = vec[i];
                    u_k[i].ldz();
                }
                return project_();
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const maturity     situation; //!< current maturity

        private:
            YACK_DISABLE_ASSIGN(qfamily);
            coefficients          u_k;       //!< workspace
            coefficients          v_k;       //!< workspace
            qarrays               U;         //!< current list of vectors
            coeffs_ptr            Q;         //!< pointer to vectors
            indexing_t            I;         //!< current indices

            bool                 try_grow();
            const readable<apq> &project_();
            
        };
        
        
        
        
    };
}

#endif

