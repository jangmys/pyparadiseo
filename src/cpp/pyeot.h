#ifndef PYEOT_H_
#define PYEOT_H_

#include <boost/python.hpp>

#include <core/MOEO.h>
#include <core/moeoObjectiveVectorTraits.h>
#include <core/moeoObjectiveVector.h>
#include <core/moeoRealObjectiveVector.h>

#include <fitness.h>

#include "utils/to_std_vector.h"
#include "utils/index_error.h"

namespace bp=boost::python;


typedef moeoRealObjectiveVector<moeoObjectiveVectorTraits> realObjVec;


class PyEOT : public MOEO< realObjVec, doubleFitness, double>
{
public:
    typedef doubleFitness Fitness;
    typedef double Diversity;
    typedef realObjVec ObjectiveVector;

    PyEOT() : MOEO(),encoding(bp::object())
    {    }

    PyEOT(boost::python::object _enc) : MOEO(),encoding(_enc)
    {    }

    //for copy ctor
    bp::object copyMod = bp::import("copy");
    bp::object deepcopy = copyMod.attr("deepcopy");

    PyEOT(const PyEOT& p) : MOEO()
    {
        setEncoding(p.deepcopy(p.getEncoding()));
        setFitness(p.deepcopy(p.getFitness()));
        setObjectiveVector(p.getObjectiveVector());
        setDiversity(p.deepcopy(p.getDiversity()));
    }

    PyEOT& operator=(const PyEOT& p)
    {
        setEncoding(p.deepcopy(p.getEncoding()));
        setFitness(p.deepcopy(p.getFitness()));
        setObjectiveVector(p.getObjectiveVector());
        setDiversity(p.deepcopy(p.getDiversity()));

        return *this;
    }

    //FITNESS
    //==========================================
    //MOEO overrrides operator< from EO
    //we inherit from MOEO but don't want this by default ... reversing without interfering in MOEO module...
    bool operator<(const PyEOT & _other) const
    {
        if(getFitness().is_none())
            std::cout<<"can't compare< NoneType\n";

        //this is Fitness<FitnessTraits>::operator<(Fitness &lhs, Fitness& rhn)
        //default is maximization, i.e TRUE iff this < other, reversed for minimization
        return fitness() < _other.fitness();
    }

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


    //ENCODING
    //==========================================
    //solution encoding is a python object --> a property of pyEOT
    boost::python::object encoding;

    boost::python::object getEncoding() const { return encoding; }
    void setEncoding(boost::python::object enc){ encoding = enc; }

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

        result += "< ";
        result += boost::python::extract<const char*>(boost::python::str(encoding));
        result += " , ";

        result += "Fitness: ";
        if(!invalidFitness())
            result += boost::python::extract<const char*>(boost::python::str(fitness().get()));
        else result += std::string("[]");

        if(!invalidObjectiveVector()){
            result += " , ObjVect: ";
            for(auto &&v : objectiveVector()){
                result += std::to_string(v);
                result += ' ';
            }
        }
        if(!invalidDiversity()){
            result += ", Diversity: ";
            result += boost::python::extract<const char*>(boost::python::str(diversity()));

        }
        result += " >";

        return result;
    }

    std::string repr() const
    {
        std::string s;

        s += "Solution(";
        s += boost::python::extract<const char*>(boost::python::str(encoding));
        s += ",";
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

#endif
