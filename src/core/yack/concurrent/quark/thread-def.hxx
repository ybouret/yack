
namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            //==================================================================
            //
            //
            // <thread>
            //
            //
            //==================================================================
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

                inline thread(threadable proc, void *args) :
                uuid(0),
                self( create(proc,args,coerce(uuid)) )
                {
                }

                inline ~thread() throw()
                {
                    finish( coerce(self), coerce(uuid) );
                }


            private:
                struct stub
                {
                    threadable  proc;
                    void       *args;
                };

                static inline handle create(threadable proc, void *args, ID &tid)
                {
                    stub _ = { proc, args }; assert(NULL!=proc);
                    return launch(entry,&_,tid);
                }

                //! system threadable routine
                static inline YACK_THREAD_LAUNCHER_RETURN entry(YACK_THREAD_LAUNCHER_PARAMS info)
                {
                    assert(info!=NULL);
                    stub &_ = *static_cast<stub *>(info);
                    assert(_.proc);
                    _.proc(_.args);
                    return 0;
                }

                //! join/wait handle
                static inline void finish(handle &h, ID &tid) throw()
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
                    std::cerr << "launching thread in system routine" << std::endl;
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

