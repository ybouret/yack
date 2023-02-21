

#include "yack/gfx/broker/split.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/concurrent/loop/mono.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace graphic;

template <size_t CH>
void get_chan( rgba &target, const uint8_t &source ) noexcept
{
    target.r = target.g = target.b = 0;
    target.a = 0xff;
    *(&target.r + CH) = source;
}

YACK_UTEST(split)
{
    images &IMG = images::standard();
    const concurrent::topology  topo;
    engine  parEngine = new concurrent::simd(topo);
    engine  serEngine = new concurrent::mono();

    if(argc>1)
    {
        pixmap<rgba>     source = IMG.load(argv[1],NULL);
        broker           par(parEngine,source);
        broker           ser(serEngine,source);
        pixmaps<uint8_t> target(source,4);
        {
            const size_t ch[] = { 0,1,2,3 };
            broker_split::build(target,source,ch,4,ser);
            //broker_split::build(target,source,ch,4,par);
        }
        pixmap<rgba> output(source.w,source.h);
        broker_transform::apply(output,target[1],par,get_chan<0>);
        IMG.save(output,"red.png",NULL);
        broker_transform::apply(output,target[2],par,get_chan<1>);
        IMG.save(output,"green.png",NULL);
        broker_transform::apply(output,target[3],par,get_chan<2>);
        IMG.save(output,"blue.png",NULL);
    }
}
YACK_UDONE()

