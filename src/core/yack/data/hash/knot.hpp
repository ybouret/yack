#ifndef YACK_HASH_KNOT_INCLUDED
#define YACK_HASH_KNOT_INCLUDED 1

namespace yack
{
    template <typename T, typename NODE>
    class hash_knot : public object
    {
    public:
        
        
        const NODE *node; //!< node within hash table
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_knot);
    };
}

#endif

