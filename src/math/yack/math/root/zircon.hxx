
namespace yack
{

    namespace math
    {

        template <>
        zircon<real_t>:: ~zircon() throw()
        {
        }


        template <>
        zircon<real_t>:: zircon(const size_t dims) :
        tableaux(4,dims),
        X( next() ),
        F( next() ),
        G( next() ),
        V( next() ),
        f0(0),
        J(dims,dims),
        Jt(dims,dims),
        iJ(dims,dims),
        fdjac(0)
        {
        }

        template <>
        void zircon<real_t>:: setup(size_t dims)
        {
            make(dims);
            J.make(dims,dims);
            Jt.make(dims,dims);
            iJ.make(dims,dims);
        }

        template <>
        real_t zircon<real_t>:: objective(const array_type &FF) throw()
        {
            static const real_t half(0.5);
            for(size_t i=FF.size();i>0;--i)
                V[i] = squared(FF[i]);
            return sorted::sum(V,sorted::by_value) * half;
        }


        template <>
        void zircon<real_t>:: analyze()
        {

            Jt.assign(J,transposed);
            iJ.assign(J);
            Jt(G,F);
            f0 = objective(F);
            std::cerr << "X=" << X << std::endl;
            std::cerr << "F=" << F << std::endl;
            std::cerr << "J=" << J << std::endl;
            std::cerr << "G=" << G << std::endl;
            std::cerr << "f0=" << f0 << std::endl;
        }


    }
}
