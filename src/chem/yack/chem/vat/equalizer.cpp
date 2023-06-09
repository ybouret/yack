#include "yack/chem/vat/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/keto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace Chemical
    {


        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer(const size_t maxClusterSize,
                              const size_t maximumSpecies) :
        speciesFund( new Species::Bank() ),
        cursorsFund( new Cursor::Bank()  ),

        reac( speciesFund, cursorsFund   ),
        prod( speciesFund, cursorsFund   ),
        side( speciesFund, cursorsFund   ),

        inUse( maxClusterSize ),
        valid( speciesFund ),
        wrong( speciesFund ),
        nomad( speciesFund ),
        Corg(maximumSpecies,as_capacity),
        Ctmp(maximumSpecies,as_capacity),
        Cend(maximumSpecies,as_capacity),
        Ceqz(maxClusterSize, maxClusterSize ? maximumSpecies : 0),
        xadd()
        {
        }


     
        
    }

}

