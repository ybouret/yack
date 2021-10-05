namespace yack
{
    namespace synchronic
    {
        namespace quark
        {

            struct mutex {
                yack_mutex impl;

                //! syntax helper
                inline yack_mutex * operator*() throw() { return &impl; }

                //! smart pointer
                class handle
                {
                public:
                    inline  handle() : pointee( mutex_api::create() ) {}
                    inline ~handle() throw() { mutex_api::destruct(pointee); pointee=0; }

                    inline mutex & operator*() throw() { assert(pointee); return *pointee;            }
                    inline void lock()         throw() { assert(pointee); mutex_api::lock(pointee);   }
                    inline void unlock()       throw() { assert(pointee); mutex_api::unlock(pointee); }

                private:
                    mutex *pointee;
                    YACK_DISABLE_COPY_AND_ASSIGN(handle);
                };
            };

        }
    }
}

