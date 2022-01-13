
//! \file

#ifndef YACK_IOS_ASCII_LOADER_INCLUDED
#define YACK_IOS_ASCII_LOADER_INCLUDED 1

#include "yack/ios/ascii/converter.hpp"
#include "yack/container/sequence.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/object.hpp"
#include "yack/system/rtti.hpp"
#include "yack/ios/istream.hpp"

namespace yack
{

    namespace ios
    {
        namespace ascii
        {

            //__________________________________________________________________
            //
            //
            //! loader for heterogeneous data
            //
            //__________________________________________________________________
            class loader
            {
            public:

                //______________________________________________________________
                //
                //
                //! a column linked to a given sequence
                //
                //______________________________________________________________
                class column : public object
                {
                public:
                    //__________________________________________________________
                    //
                    // local types and definitions
                    //__________________________________________________________
                    typedef void (*append)(column&,const char *,const size_t, const char*); //!< alias
                    typedef void (*erase)(column&);                                         //!< alias

                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________

                    //! cleanup
                    virtual ~column() throw();

                    //! build with given procedure
                    template <typename T> inline
                    column(sequence<T>  &seq,
                           const size_t  num,
                           const builder cnv,
                           const append  inc) throw() :
                    next(0),
                    prev(0),
                    addr(&seq),
                    indx(num),
                    conv(cnv),
                    grow(inc),
                    drop( zero<T> )
                    {
                    }

                    //__________________________________________________________
                    //
                    // methods
                    //__________________________________________________________
                    void       push(const char *,const size_t, const char*);    //!< use conv/grow
                    void       free()                                  throw(); //!< use zero
                    static int compare(const column *, const column *) throw(); //!< compare by index

                    //__________________________________________________________
                    //
                    // members
                    //__________________________________________________________
                    column      *next; //!< for list
                    column      *prev; //!< for list
                    void * const addr; //!< sequence address
                    const size_t indx; //!< sequence index
                    builder      conv; //!< ascii converter



                private:
                    YACK_DISABLE_COPY_AND_ASSIGN(column);
                    append       grow;
                    erase        drop;

                    template <typename T> static inline
                    void zero(column &self) throw() { static_cast< sequence<T> *>(self.addr)->free(); }
                };


                typedef cxx_list_of<column> columns; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit loader() throw(); //!< init empty
                virtual ~loader() throw(); //!< cleanup

                //______________________________________________________________
                //
                //! use another sequence with a given index
                //______________________________________________________________
                template <typename T>
                void operator()(sequence<T> &data, size_t indx)
                {
                    typedef typename sequence<T>::mutable_type type;
                    static const rtti &rtid = rtti::use<type>();
                    builder            conv = converter::of(rtid);
                    column::append     grow = incr<type>;
                    check_index(indx,rtid.name());
                    use( new column(data,indx,conv,grow) );
                }

                //______________________________________________________________
                //
                //! load from input, skip header, empty and comment lines
                //______________________________________________________________
                size_t load(ios::istream &,size_t nskip=0);


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(loader);
                columns cols;
                void    check_index(const size_t indx, const string &name) const;
                void    use(column *) throw();

                template <typename T> static inline
                void incr(column      &self,
                          const char  *buff,
                          const size_t size,
                          const char  *info)
                {
                    T target;
                    self.conv(&target,buff,size,info);
                    sequence<T> &data = *static_cast< sequence<T> *>(self.addr);
                    data.push_back(target);
                }

            };

        }
    }
}

#endif
