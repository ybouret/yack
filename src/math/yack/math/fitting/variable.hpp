//! \file

#ifndef YACK_FITTING_VARIABLE_INCLUDED
#define YACK_FITTING_VARIABLE_INCLUDED 1


#include "yack/string.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! variable interface
            //
            //__________________________________________________________________
            class variable : public object, public counted
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef ark_ptr<string,const variable> pointer; //!< alias
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~variable() throw(); //!< cleanup


                //______________________________________________________________
                //
                // public virtual interface
                //______________________________________________________________
                virtual const string & alias()      const throw() = 0; //!< get alias anme
                virtual bool           is_primary() const throw() = 0; //!< true/false

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________
                const string & key()        const throw(); //!< name
                size_t         operator*()  const throw(); //!< check indx()
                bool           is_replica() const throw(); //!< !is_primary

                //! acces ARRAY item
                template <typename ARRAY> inline
                typename ARRAY::type & operator()(ARRAY &arr) const throw()
                {
                    assert( indx() >= 1 );
                    assert( indx() <= arr.size() );
                    return arr[ **this ];
                }

                //! output information
                friend std::ostream & operator<<(std::ostream &, const variable &);

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string   name; //!< identifier

            protected:
                explicit variable(const string &); //!< setup name

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(variable);
                virtual size_t indx() const throw() = 0;
            };

        }

    }

}



#endif

