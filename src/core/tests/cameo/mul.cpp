#include "yack/cameo/mul.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"

using namespace yack;

namespace {

	template <typename T>
	static inline void perform(randomized::bits& ran)
	{
		cameo::mul<T>  xmul;
		static const T ten(10);
		static const T amp(20);

		for (size_t n = 0; n <= 10; ++n)
		{
			xmul.free();
			T  res = n > 0 ? 1 : 0;
			for(size_t i=0;i<n;++i)
			{ 
				const T rxp = ran.symm<T>(); 
				const T tmp = (ran.choice() ? -1 : 1) * std::pow(ten, amp*rxp);
				res *= tmp;
				xmul << tmp;
			}
			std::cerr << res << " / " << xmul.product() << std::endl;
		}
	}
}

YACK_UTEST(cameo_mul)
{
	randomized::rand_ ran;

	{ cameo::mul<float>  xmul; }
	{ cameo::mul<double> xmul(7); }

	perform<float>(ran);
	perform<double>(ran);
	perform<long double>(ran);

	{
		cameo::mul<double> xm;
		xm.upower(3, 3);
		std::cerr << xm << std::endl;
		xm.ipower(4, 2);
		xm.ipower(4, -3);
		std::cerr << xm << std::endl;
	}
}
YACK_UDONE()
