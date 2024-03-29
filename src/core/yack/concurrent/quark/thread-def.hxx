namespace yack
{
    namespace concurrent
    {
        namespace quark
        {



            class thread
            {
            public:
              

#if             defined(YACK_BSD)
                typedef pthread_t handle;                 //!< system thread handle
                typedef pthread_t ID;                     //!< system thread identifier
                typedef void *  (*system_routine)(void*); //!< system threadable interface
#define         YACK_THREAD_LAUNCHER_RETURN void *
#define         YACK_THREAD_LAUNCHER_PARAMS void *
#endif

#if             defined(YACK_WIN)
                typedef HANDLE handle;                         //!< system thread handle
                typedef DWORD  ID;                             //!< system thread identifier
                typedef LPTHREAD_START_ROUTINE system_routine; //!< system threadable interface
#define         YACK_THREAD_LAUNCHER_RETURN DWORD WINAPI
#define         YACK_THREAD_LAUNCHER_PARAMS LPVOID

#endif
                const ID     uuid;
                const handle self;

                inline thread(threadable &exe) :
                uuid(0),
                self( launch(entry,&exe,coerce(uuid)) )
                {
                }

                inline ~thread() noexcept
                {
                    finish( coerce(self), coerce(uuid) );
                }

                static inline handle get_current_handle() noexcept
                {
#if defined(YACK_BSD)
                    return pthread_self();
#endif

#if defined(YACK_WIN)
                    return ::GetCurrentThread();
#endif
                }

                static inline ID get_current_id()      noexcept
                {
#if defined(YACK_BSD)
                    return pthread_self();
#endif

#if defined(YACK_WIN)
                    return ::GetCurrentThreadId();
#endif
                }

                static void assign(handle h, const size_t j);

            private:

                //! system threadable routine
                static inline
                YACK_THREAD_LAUNCHER_RETURN entry(YACK_THREAD_LAUNCHER_PARAMS info)
                {
                    assert(info!=NULL);
                    static_cast<threadable *>(info)->run();
                    return 0;
                }

                //! join/wait handle
                static inline void finish(handle &h, ID &tid) noexcept
                {
#if                 defined(YACK_BSD)
                    const int res = pthread_join( h, 0 );
                    if(res!=0) system_error::critical_bsd(res,"pthread_join");
#endif

#if                 defined(YACK_WIN)
                    if( ::WaitForSingleObject(h,INFINITE ) != WAIT_OBJECT_0 )
                    {
                        system_error::critical_win( ::GetLastError(), "WaitForSingleObject" );
                    }
                    ::CloseHandle(h);
#endif
                    out_of_reach::zset(&h,sizeof(handle));
                    out_of_reach::zset(&tid,sizeof(ID));
                }

                //! create threadable code
                static inline handle launch(system_routine code, void *data, ID &tid)
                {
                    assert(NULL!=code);
                    assert(NULL!=data);
                    out_of_reach::zset(&tid,sizeof(ID));

#if                 defined(YACK_BSD)
                    const int res = pthread_create(&tid, NULL, code, data);
                    if (res != 0) throw libc::exception(res, "pthread_create");
                    return tid;
#endif

#if                 defined(YACK_WIN)
                    handle h = ::CreateThread(0,
                                              0,
                                              code,
                                              data,
                                              0,
                                              &tid);
                    if (NULL == h)
                    {
                        const DWORD res = ::GetLastError();
                        throw win32::exception(res, "::CreateThread");
                    }
                    return h;
#endif
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(thread);
            };


        }

    }

}

