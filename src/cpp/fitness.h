class FitnessTraits
{
public:
    static void set_minimizing(bool _val)
    {
            // std::cout<<"set flag\n";
        _minimizing = _val;
    }

    static bool minimizing()
    {
        // std::cout<<"call flag\n";
        return _minimizing;
    }

private:
    static bool _minimizing;
};

template<class FitnessTraits>
class DoubleFitness
{
public:
    DoubleFitness() : value(0.0f)
    {    };

    DoubleFitness(double _val) : value(_val)
    {    };

    DoubleFitness(const DoubleFitness& other) : value(other.value) {};

    operator double(void) const { return value; }

    static void setup(bool _minimize)
    {
        FitnessTraits::set_minimizing(_minimize);
    }

    double get() const
    {
        return value;
    }

    bool operator<(const DoubleFitness<FitnessTraits>& _other) const
    {
        // std::cout<<"operator"<<std::endl;
        if(FitnessTraits::minimizing()){
            // std::cout<<"minimizing"<<std::endl;
            return get() > _other.get();
        }
        else{
            // std::cout<<"maximizing"<<std::endl;
            return get() < _other.get();
        }
    }

    void printOn(std::ostream& os) const {
        std::string result;
        result += boost::python::extract<const char*>(boost::python::str(get()));
        os << result;
        // const boost::python::object& o = *this;
        // boost::python::api::operator<<(os,o);
    }
    friend std::ostream& operator<<(std::ostream& os, const DoubleFitness& p) {
        p.printOn(os);
        return os;
    }
    friend std::istream& operator>>(std::istream& is, DoubleFitness& p) {
        boost::python::object o;
        is >> o;

        boost::python::extract<double> x(o);
        if(x.check())
        {
            p = DoubleFitness(x());
            // Fitness d = x();
            // double d = x();
            // fitness(d);
        }
        // else{
        //     throw index_error("fitness : failed to extract double\n");
        // }

        // p = DoubleFitness(o);
        return is;
    }

private:
    double value;
};

typedef DoubleFitness<FitnessTraits> doubleFitness;




// template <class F, class Cmp>
// std::ostream& operator<<(std::ostream& os, const DoubleFitness<FitnessTraits>& f)
// {
//     os << (double) f;
//     return os;
// }
//
// // template <class F, class Cmp>
// std::istream& operator>>(std::istream& is, DoubleFitness<FitnessTraits>& f)
// {
//     double value;
//     is >> value;
//     f = value;
//     return is;
// }
