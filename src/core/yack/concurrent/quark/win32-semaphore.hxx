
namespace yack
{
    //==========================================================================
    //
    //
    // <win32::semaphore>, for legacy win32 conditions
    //
    //
    //==========================================================================
    namespace win32
    {

        class semaphore
        {
        public:
            static const long lMinCount = 0;
            static const long lMaxCount = 65535;

            inline  semaphore() : sem_(0) { if( !(sem_ = ::CreateSemaphore(NULL,lMinCount,lMaxCount,NULL) ) ) throw win32::exception(::GetLastError(), "::CreateSemaphore()"); }
            inline ~semaphore() noexcept { assert(sem_); ::CloseHandle(sem_); sem_ = NULL; }

            inline void wait() noexcept
            {
                assert(sem_ != NULL);
                const DWORD res = ::WaitForSingleObject(sem_, INFINITE);
                if (res != WAIT_OBJECT_0) system_error::critical_win(::GetLastError(), "WaitForSingleObject( SEMAPHORE )");
            }

            inline void post() noexcept
            {
                assert(sem_ != NULL); if (!::ReleaseSemaphore(sem_, 1, NULL)) system_error::critical_win(::GetLastError(), "::ReleaseSemaphore");
            }

        private:
            HANDLE sem_;
            YACK_DISABLE_COPY_AND_ASSIGN(semaphore);
        };
    }
    //==========================================================================
    //
    //
    // <win32::semaphore/>, for legacy win32 conditions
    //
    //
    //==========================================================================
}
