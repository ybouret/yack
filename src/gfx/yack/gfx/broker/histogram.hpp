
//! \file

#ifndef YACK_GFX_BROKER_HISTOGRAM_INCLUDED
#define YACK_GFX_BROKER_HISTOGRAM_INCLUDED 1

#include "yack/gfx/broker.hpp"
#include "yack/gfx/histogram.hpp"
#include "yack/gfx/memory.hpp"
#include "yack/type/gateway.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T> class pixmap; //!< forward declaration

        class broker_histogram : public gateway<histogram>
        {
        public:
            explicit broker_histogram(const broker &device);
            virtual ~broker_histogram() throw();

            void initialize() throw();
            void finalize()   throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(broker_histogram);

            histogram                             self;
            cxx_array<histogram,memory_allocator> hist;
            
            virtual const_type & bulk() const throw();
        };

    }

}

#endif
