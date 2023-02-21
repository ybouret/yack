namespace yack
{
    namespace concurrent
    {
        namespace quark
        {

            struct mutex {
                
                yack_mutex impl;

                //! syntax helper
                inline yack_mutex * operator*() noexcept { return &impl; }

                //! smart pointer
                class handle
                {
                public:
                    inline  handle() : pointee( mutex_api::init() ) {}
                    inline ~handle() noexcept  { mutex_api::quit(pointee); }

                    inline mutex & operator*() noexcept { assert(pointee); return *pointee;            }
                    inline void lock()         noexcept { assert(pointee); mutex_api::lock(pointee);   }
                    inline void unlock()       noexcept { assert(pointee); mutex_api::unlock(pointee); }

                private:
                    mutex *pointee;
                    YACK_DISABLE_COPY_AND_ASSIGN(handle);
                };
            };

#if defined(YACK_BSD)
            //==================================================================
            //
            //
            //! <pthread::mutex::attribute>
            //
            //
            //==================================================================
            class mutex_attribute
            {
            public:
                inline explicit mutex_attribute() : attr()
                {
                    {
                        const int  res = pthread_mutexattr_init(&attr);
                        if (res != 0) throw libc::exception(res, "pthread_mutexattr_init");
                    }

                    {
                        const int res = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
                        if (res != 0)
                        {
                            pthread_mutexattr_destroy(&attr);
                            throw libc::exception(res, "pthread_mutexattr_settype(RECURSIVE)");
                        }
                    }
                }

                inline virtual ~mutex_attribute() noexcept
                {
                    (void) pthread_mutexattr_destroy(&attr);
                }

                pthread_mutexattr_t attr;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(mutex_attribute);
            };
            //==================================================================
            //
            //
            // <pthread::mutex::attribute/>
            //
            //
            //==================================================================
#endif

        }
    }
}

