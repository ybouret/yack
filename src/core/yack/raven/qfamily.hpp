//! \file

#ifndef YACK_RAVEN_QFAMILY_INCLUDED
#define YACK_RAVEN_QFAMILY_INCLUDED 1

#include "yack/raven/qmatrix.hpp"
#include "yack/data/set.hpp"
#include "yack/ptr/clone.hpp"
#include "yack/container/matrix.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace raven
    {
        //______________________________________________________________________
        //
        //
        // aliases for internal indices
        //
        //______________________________________________________________________
        typedef data_set<size_t>   qList; //!< list of indices
        typedef ds_zpool<size_t>   qBank; //!< bank of indices
        typedef qBank::pointer     qFund; //!< shared bank of indices


        //! for sanity checks
#define YACK_RAVEN_CHECK()   assert(basis->size+ready->size==mu.rows); assert(basis.excludes(ready))

        //______________________________________________________________________
        //
        //
        //! family built from a set of vectors
        //
        //______________________________________________________________________
        class qfamily : public object
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef cxx_list_of<qfamily> list_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~qfamily() throw(); //!< cleanup


            //! start a new family
            /**
             \param id indices[1...nr], id[nr] = starting index
             \param mu matrix of row vectors
             \param rk rank(mu)
             \param io cache for lists of indices
             */
            template <typename T> inline
            explicit qfamily(readable<size_t> &id,
                             const matrix<T>  &mu,
                             const size_t      rk,
                             const qFund      &io) :
            object(),
            qbase( new qmatrix(mu.cols,rk) ),
            basis(io),
            ready(io),
            next(0),
            prev(0)
            {
                const size_t nr = mu.rows;  assert(nr>=2);
                const size_t ir = id[nr];   assert(ir>=1); assert(ir<=mu.rows);
                qmatrix     &Q  = *qbase;
                if(!Q(mu[ir])) throw_singular_matrix(ir);
                basis += ir;
                for(size_t i=1;i<nr;++i) ready += id[i];

                // sanity check
                assert(1==basis->size);
                assert(nr-1==ready->size);
                YACK_RAVEN_CHECK();
            }

            //! hard copy
            qfamily(const qfamily &);


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const qfamily &); //!< display

            //__________________________________________________________________
            //
            // generate all possible lineage from this family
            //__________________________________________________________________
            template <typename T> inline
            void generate(list_of<qfamily> &lineage,
                          const matrix<T>  &mu) const
            {
                assert(ready->size+basis->size==mu.rows);
                switch(qbase->active_state)
                {
                        //------------------------------------------------------
                        // end of lineage at this point
                    case fully_grown: return;
                        //------------------------------------------------------


                        //------------------------------------------------------
                        // at most one final generation
                    case almost_done:
                        final_generation(lineage,mu);
                        return;
                        //------------------------------------------------------

                    case in_progress:
                        std::cerr << "In Progress!" << std::endl;
                        exit(0);
                        break;
                }
            }

            qmatrix       & operator*()       throw() { return *qbase; }
            const qmatrix & operator*() const throw() { return *qbase; }


            clone_ptr<qmatrix> qbase;
            qList              basis;
            qList              ready;
            qfamily           *next;
            qfamily           *prev;


            static void reduce(list_of<qfamily> &lineage);

        private:
            YACK_DISABLE_ASSIGN(qfamily);
            void throw_singular_matrix(const size_t ir) const;

            template <typename T>
            bool bloom(const matrix<T> &mu)
            {
                assert(almost_done==qbase->active_state);
                YACK_RAVEN_CHECK();

                qmatrix &Q     = *qbase;
                bool     found = false;

                //--------------------------------------------------------------
                // find a final vector, any will produce the same result
                //--------------------------------------------------------------
                while(ready->size)
                {
                    const size_t ir = ready.pull_lower(); // extract index
                    basis += ir;                          // record in basis
                    YACK_RAVEN_CHECK();
                    if( Q(mu[ir]) )
                    {
                        found = true;
                        assert(fully_grown==qbase->active_state);
                        break;
                    }
                }

                //--------------------------------------------------------------
                // cleanup
                //--------------------------------------------------------------
                basis += ready;
                ready.free();
                YACK_RAVEN_CHECK();

                //--------------------------------------------------------------
                // done!
                //--------------------------------------------------------------
                return found;
            }

            template <typename T> inline
            void final_generation(list_of<qfamily> &lineage,
                                  const matrix<T>  &mu) const
            {
                auto_ptr<qfamily> children = new qfamily(*this);
                if(children->bloom(mu))
                    lineage.push_back( children.yield() );
            }



        };

    }

}

#endif

