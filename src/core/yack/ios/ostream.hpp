

//! \file
#ifndef YACK_IOS_OSTREAM_INCLUDED
#define YACK_IOS_OSTREAM_INCLUDED 1

#include "yack/ios/stream.hpp"
#include "yack/type/ints.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! output stream base class
        //
        //______________________________________________________________________
        class ostream
        {
        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual void write(const char) = 0;             //!< write one char
            virtual void frame(const void *, const size_t); //!< default: write all block as chars
            virtual void operator()(const char *fmt,...);   //!< default it not implemented yet

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            ostream & operator<<(const char C);     //!< uses write
            ostream & operator<<(const char *msg);  //!< uses frame

            void put(const uint8_t  &);
            void put(const uint16_t &);
            void put(const uint32_t &);
            void put(const uint64_t &);

            template <typename T>
            inline void emit(const T x)
            {
                const typename unsigned_for<T>::type y(x);
                put(y);
            }

            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~ostream() throw(); //!< cleanup

        protected:
            explicit ostream() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ostream);
        };
    }
}

#endif
