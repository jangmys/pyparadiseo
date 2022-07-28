#ifndef PYEOT_H_
#define PYEOT_H_

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <PO.h>
#include <core/MOEO.h>
#include <core/moeoObjectiveVectorTraits.h>
#include <core/moeoObjectiveVector.h>
#include <core/moeoRealObjectiveVector.h>

#include <fitness.h>

#include "utils/to_std_vector.h"
#include "utils/index_error.h"

namespace bp=boost::python;
namespace np=boost::python::numpy;

typedef DoubleFitness<FitnessTraits> doubleFitness;
typedef moeoRealObjectiveVector<moeoObjectiveVectorTraits> realObjVec;

/* solution base class - python equivalent of EO and MOEO

has a fitness, objective vector and diversity (but no encoding), comparison (fitness)
*/
class PyEO : public MOEO< realObjVec, doubleFitness, double>
{
public:
    typedef doubleFitness Fitness;
    typedef double Diversity;
    typedef realObjVec ObjectiveVector;

    //for copy ctor
    bp::object copyMod = bp::import("copy");
    bp::object deepcopy = copyMod.attr("deepcopy");

    PyEO() : MOEO(){}

    PyEO(const PyEO& p) : MOEO()
    {
        setFitness(p.deepcopy(p.getFitness()));
        setObjectiveVector(p.getObjectiveVector());
        setDiversity(p.deepcopy(p.getDiversity()));
    }

    PyEO& operator=(const PyEO& p)
    {
        setFitness(p.deepcopy(p.getFitness()));
        setObjectiveVector(p.getObjectiveVector());
        setDiversity(p.deepcopy(p.getDiversity()));

        return *this;
    }

    //FITNESS
    //==========================================
    /*
    getter/setter for fitness, exposed to python as a "property"
    on the C++ side a fitness is a double
    on the Python side, anything convertible to a double
    */
    boost::python::object getFitness() const {
        //if invalid return "None" object else construct Python object from C++ double
        return invalidFitness()? boost::python::object(): boost::python::object(fitness().get());
    }
    void setFitness(boost::python::object f) {
        // NOT : if(!f) //this will be true for 0.0
        if(f.ptr() == Py_None)
        {
            invalidateFitness();
            return;
        }

        boost::python::extract<double> x(f);
        // boost::python::extract<Fitness> x(f);
        if(x.check())
        {
            double d = x();

            // std::cout<<"val=\t"<<d<<std::endl;
            // Fitness d = x();
            fitness(d);
        }else{
            throw index_error("fitness : failed to extract double\n");
        }
    }

    //MOEO overrrides operator< from EO
    //we inherit from MOEO but don't want this by default ... reversing without interfering in MOEO module...
    bool operator<(const PyEO& _other) const
    {
        if(getFitness().is_none())
        std::cout<<"can't compare< NoneType\n";

        //this is Fitness<FitnessTraits>::operator<(Fitness &lhs, Fitness& rhn)
        //default is maximization, i.e TRUE iff this < other, reversed for minimization
        return fitness() < _other.fitness();
    }



    //OBJECTIVE VECTOR
    //======================================================
    //C++ functions use objectiveVector...so this should always return : an objectiveVector!
    //if invalid...(what should happen? see pickling...)
    ObjectiveVector getObjectiveVector() const
    {
        //if invalid...default ctor...else return
        return invalidObjectiveVector()?ObjectiveVector():objectiveVector();
    }
    //objective vector is set at EACH f-eval...doing this right seems quite critical !!!
    //could have several Python-side proporties (ex. objectiveVector,npObjectiveVector,intObjectiveVector,...)
    //that all create an MOEO objectiveVector...
    void setObjectiveVector(boost::python::object f) {
        if(f.ptr() != Py_None){
            //convert to std::vector<double> and call MOEO's setter
            objectiveVector(to_std_vector<double>(f));
        }
        else
        {
            invalidateObjectiveVector();
        }
    }
    //overload (used in copy ctor)
    void setObjectiveVector(const ObjectiveVector& f) {
        objectiveVector(f);
    }

    //DIVERSITY
    //====================================================================
    //getter/setter for diversity, exposed to python as a "property"
    //on the C++ side a diversity is a double
    //on the Python side, anything convertible to a double
    boost::python::object getDiversity() const {
        return invalidDiversity()? boost::python::object(): boost::python::object(diversity());
    }
    void setDiversity(boost::python::object f) {
        if(f.ptr() == Py_None)
        {
            invalidateDiversity();
            return;
        }

        boost::python::extract<double> x(f);
        if(x.check())
        {
            double d = x();
            diversity(d);
        }else{
            throw index_error("diversity : failed to extract double\n");
        }
    }


    //VALIDITY FLAGS
    bool invalid() const
    {
        return invalidFitness();
    }

    //I/O
    std::string to_string() const
    {
        std::string result;

        result += " ======================\n";
        result += "Fitness: ";
        if(!invalidFitness())
            result += boost::python::extract<const char*>(boost::python::str(fitness().get()));
        else result += std::string("[]");
        result += "\n";

        if(!invalidObjectiveVector()){
            result += " , ObjVect: ";
            for(auto &&v : objectiveVector()){
                result += std::to_string(v);
                result += ' ';
            }
            result += "\n";
        }
        // result += PyEO::to_string();

        if(!invalidDiversity()){
            result += ", Diversity: ";
            result += boost::python::extract<const char*>(boost::python::str(diversity()));
            result += "\n";
        }
        // result += " >";

        return result;
    }

    std::string repr() const
    {
        std::string s;

        s += "(";
        // s += boost::python::extract<const char*>(boost::python::str(encoding));
        // s += ",";
        if(invalidFitness())
            s+="";
        else s += boost::python::extract<const char*>(boost::python::str(fitness().get()));
        s += ",";

        if(invalidObjectiveVector())
            s+="";
        else s += boost::python::extract<const char*>(boost::python::str(objectiveVector()));
        s += ",";

        if(invalidDiversity())
            s+="";
        else s += boost::python::extract<const char*>(boost::python::str(diversity()));
        s += ")";

        return s;
    }

    void printOn(std::ostream & _os) const
    {
        _os << to_string() << ' ';
    }
};

/*
solution with generic encoding (bp::object)
*/
class PyEOT : public PyEO
{
public:
    PyEOT() : PyEO(),encoding(bp::object())
    {    }

    PyEOT(boost::python::object _enc) : PyEO(),encoding(_enc)
    {    }

    //for copy ctor
    bp::object copyMod = bp::import("copy");
    bp::object deepcopy = copyMod.attr("deepcopy");

    PyEOT(const PyEOT& p) : PyEO(p)
    {
        set_encoding(p.deepcopy(p.get_encoding()));
    }

    PyEOT& operator=(const PyEOT& p)
    {
        PyEO::operator=(p);
        set_encoding(p.deepcopy(p.get_encoding()));
        return *this;
    }

    //ENCODING
    //==========================================
    //maybe I should make that private?
    //solution encoding is a python object --> a property of pyEOT
    boost::python::object encoding;

    boost::python::object get_encoding() const { return encoding; }
    void set_encoding(boost::python::object enc){ encoding = enc; }


    //depend on type of object passed...

    //should check if the object is sliceable, indexable etc...
    boost::python::object get_item(int key) const
    {
        return encoding[key];
    }

    void set_item(int index, boost::python::object key)
    {
        encoding[index]=key;
    }

    boost::python::object get_len() const
    {
        ssize_t l = boost::python::len(encoding);
        return boost::python::object(l);
    }
    bool operator==(const PyEOT& _other)
    {
        return encoding == _other.encoding;
    }

    int size(){
        return boost::python::extract<int>(get_len())();
    }

    //I/O
    std::string to_string() const
    {
        std::string result;

        result += PyEO::to_string();
        result += "\t";

        result += boost::python::extract<const char*>(boost::python::str(encoding));

        return result;
    }

    std::string repr() const
    {
        std::string s;

        s += "Solution(";
        s += PyEO::repr();
        s += ",";
        s += boost::python::extract<const char*>(boost::python::str(encoding));
        s += ")";

        return s;
    }


    // void printOn(std::ostream & _os) const
    // {
    //     _os << to_string() << ' ';
    // }

};


template<typename T>
class VectorSolution : public PyEO
{
public:
    typedef T AtomType;

    VectorSolution(unsigned int _size = 0) : PyEO(),vec(std::vector<T>(_size)),
        encoding(
            np::from_data(vec.data(),
            np::dtype::get_builtin<T>(),
            bp::make_tuple(_size),
            bp::make_tuple(sizeof(T)),
            bp::object())
        ){}

        ///ctor clashing with previous one!!
    // VectorSolution(boost::python::object _obj) :

    VectorSolution(const VectorSolution& p) : PyEO(p),vec(p.vec),
        encoding(
            np::from_data(vec.data(),
            np::dtype::get_builtin<T>(),
            bp::make_tuple(vec.size()),
            bp::make_tuple(sizeof(T)),
            bp::object())
        ){}

    np::ndarray get_encoding() const {
        return encoding;
    }
    void set_encoding(boost::python::object enc){
        auto l=boost::python::len(enc);
        if(l != (long)vec.size())
        {
            //resize and re-base ndarray
            vec.resize(l);

            encoding=np::from_data(vec.data(),
                np::dtype::get_builtin<T>(),
                bp::make_tuple(vec.size()),
                bp::make_tuple(sizeof(T)),
                bp::object()
            );
        }

        // std::cout<<l<<" "<<encoding.shape(0)<<std::endl;
        for(unsigned i=0;i<boost::python::len(enc);i++)
        {
            encoding[i] = enc[i];//,np::dtype::get_builtin<T>());
        }
    }



    np::ndarray get_array() const {
        return encoding;
    }

    void set_array(boost::python::object enc){
    // void set_array(np::ndarray enc){
        auto tmp = np::array(enc);
        // auto tmp = np::array(enc,np::dtype::get_builtin<T>());

        int input_size = tmp.shape(0);
        T* input_ptr = reinterpret_cast<T*>(tmp.get_data());

        for (int i = 0; i < input_size; ++i)
            vec[i] = *(input_ptr + i);

        encoding = np::from_data(vec.data(),
            np::dtype::get_builtin<T>(),
            bp::make_tuple(vec.size()),
            bp::make_tuple(sizeof(T)),
            bp::object()
        );
    }


    VectorSolution& operator=(const VectorSolution& p)
    {
        PyEO::operator=(p);

        vec = p.vec;
        // std::vector<T>::operator=(p);

        // encoding = p.encoding.copy();

        encoding = np::from_data(vec.data(),
            np::dtype::get_builtin<T>(),
            bp::make_tuple(vec.size()),
            bp::make_tuple(sizeof(T)),
            bp::object()
        );

        return *this;
    }



    //vector stuff
    //(make solution behave like vector in c++, avoiding public inheritance from std::vector<T>)
    size_t size() const
    {
        return vec.size();
    }

    void resize(size_t i)
    {
        vec.resize(i);

        encoding = np::from_data(vec.data(),
            np::dtype::get_builtin<T>(),
            bp::make_tuple(vec.size()),
            bp::make_tuple(sizeof(T)),
            bp::object()
        );
    }

    T* data()
    {
        return vec.data();
    }

    const T* data() const
    {
        return vec.data();
    }


    T& operator[](size_t i)
    {
        return vec.operator[](i);
    }

    const T& operator[](size_t i) const
    {
        return vec.operator[](i);
    }

    bool operator==(const VectorSolution& rhs){
        return (vec == rhs.vec);
    }

    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    iterator begin(){
        return vec.begin();
    }
    iterator end(){
        return vec.end();
    }

    const_iterator cbegin(){
        return vec.cbegin();
    }
    const_iterator cend(){
        return vec.cend();
    }

    //I/O
    std::string to_string() const
    {
        std::string result;

        result += PyEO::to_string();
        result += "\t";

        for(unsigned i=0;i<size();i++){
            result += boost::python::extract<const char*>(boost::python::str(vec[i]));
            result += " ";
        }

        return result;
    }

    std::string repr() const
    {
        std::string s;

        s += "Solution(";
        s += PyEO::repr();
        s += ",";
        for(unsigned i=0;i<size();i++){
            s += boost::python::extract<const char*>(boost::python::str(vec[i]));
            s += " ";
        }
        // s += boost::python::extract<const char*>(boost::python::str(encoding));
        s += ")";

        return s;
    }

    void printOn(std::ostream & _os) const
    {
        _os << to_string() << ' ';
    }

    std::vector<T> vec;
    np::ndarray encoding;
};


typedef VectorSolution<int> IntSolution;
//for some reason the compiler doesn't accept <bool>
typedef VectorSolution<unsigned int> BinarySolution;
// typedef IntSolution BinarySolution;
typedef VectorSolution<double> RealSolution;



// public inheritance from std::vector<T>
//
// template<typename T>
// class VectorSolution : public PyEO, public std::vector<T>
// {
// public:
//     using std::vector<T>::data;
//     using std::vector<T>::size;
//     using std::vector<T>::resize;
//     using std::vector<T>::operator[];
//
//     VectorSolution(unsigned int _size = 0) : PyEO(),std::vector<T>(_size),
//         encoding(
//             np::from_data(data(),
//             np::dtype::get_builtin<T>(),
//             bp::make_tuple(_size),
//             bp::make_tuple(sizeof(T)),
//             bp::object())
//         ){}
//
//
//     VectorSolution(const VectorSolution<T>& p) : PyEO(p),std::vector<T>(p),
//         encoding(
//             np::from_data(data(),
//             np::dtype::get_builtin<T>(),
//             bp::make_tuple(size()),
//             bp::make_tuple(sizeof(T)),
//             bp::object())
//         ){}
//
//     VectorSolution& operator=(const VectorSolution& p)
//     {
//         PyEO::operator=(p);
//         std::vector<T>::operator=(p);
//
//         // encoding = p.encoding.copy();
//
//         encoding = np::from_data(data(),
//             np::dtype::get_builtin<T>(),
//             bp::make_tuple(size()),
//             bp::make_tuple(sizeof(T)),
//             bp::object()
//         );
//
//         return *this;
//     }
//
//     np::ndarray get_array() const { return encoding; }
//
//     void set_array(np::ndarray enc){
//         int input_size = enc.shape(0);
//         T* input_ptr = reinterpret_cast<T*>(enc.get_data());
//
//         for (int i = 0; i < input_size; ++i)
//             operator[](i) = *(input_ptr + i);
//
//         encoding = np::from_data(data(),
//             np::dtype::get_builtin<T>(),
//             bp::make_tuple(size()),
//             bp::make_tuple(sizeof(T)),
//             bp::object()
//         );
//     }
//
//     //I/O
//     std::string to_string() const
//     {
//         std::string result;
//
//         result += PyEO::to_string();
//         result += "\t";
//
//         for(unsigned i=0;i<size();i++){
//             result += boost::python::extract<const char*>(boost::python::str(operator[](i)));
//             result += " ";
//         }
//
//         return result;
//     }
//
//     std::string repr() const
//     {
//         std::string s;
//
//         s += "Solution(";
//         s += PyEO::repr();
//         s += ",";
//         for(unsigned i=0;i<size();i++){
//             s += boost::python::extract<const char*>(boost::python::str(operator[](i)));
//             s += " ";
//         }
//         // s += boost::python::extract<const char*>(boost::python::str(encoding));
//         s += ")";
//
//         return s;
//     }
//
//     void printOn(std::ostream & _os) const
//     {
//         _os << to_string() << ' ';
//     }
//
//     np::ndarray encoding;
// };
//
//
// template<class T>
// void vec_resize(VectorSolution<T>& vec, unsigned i) {
//     vec.resize(i);
//
//     vec.encoding = np::from_data(vec.data(),
//         np::dtype::get_builtin<T>(),
//         bp::make_tuple(vec.size()),
//         bp::make_tuple(sizeof(T)),
//         bp::object()
//     );
// }




#endif
