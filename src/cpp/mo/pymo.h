#ifndef PYMO_H_
#define PYMO_H_

#include "pyeot.h"

#include <neighborhood/moNeighbor.h>
#include <neighborhood/moIndexNeighbor.h>
#include <neighborhood/moBackableNeighbor.h>
#include <neighborhood/moDummyNeighbor.h>

namespace bp = boost::python;

template<typename SolutionType>
struct PyNeighbor : moIndexNeighbor<SolutionType>,moBackableNeighbor<SolutionType>,bp::wrapper<moIndexNeighbor<SolutionType>>
{
public:
    typedef SolutionType EOT;

    PyNeighbor() : moIndexNeighbor<SolutionType>() { }

    PyNeighbor(bp::object move) : moIndexNeighbor<SolutionType>(),move_op(move) { }

    PyNeighbor(bp::object move,bp::object move_back) : moIndexNeighbor<SolutionType>(),move_op(move),move_back_op(move_back) { }

    PyNeighbor(const PyNeighbor<SolutionType>& _n) : moNeighbor<SolutionType>(),moIndexNeighbor<SolutionType>(_n),moBackableNeighbor<SolutionType>(_n){
        this->fitness(_n.fitness());
        move_op = _n.move_op;
        move_back_op = _n.move_op;
    };

    void setMove(bp::object obj) {
        move_op = obj;
    }

    void setMoveBack(bp::object obj) {
        move_back_op = obj;
    }

    //overridng Nieghbor move (pure virtual) to with Python callback
    void move(SolutionType& _solution)
    {
        if(move_op.ptr() != Py_None)
        {
            bp::call<void>(move_op.ptr(), bp::ptr(this), boost::ref(_solution));
        }
        else
            std::cout<<"no move defined : do nothing";
    }

//     //overridng Nieghbor moveBack (pure virtual) to with Python callback
    void moveBack(SolutionType& _solution)
    {
        if(move_back_op.ptr() != Py_None)
        {
            bp::call<void>(move_back_op.ptr(), bp::ptr(this), boost::ref(_solution));
        }
        else
            std::cout<<"no moveBack defined : do nothing";
    }

    //virtual with defaults
    bool equals(PyNeighbor<SolutionType>& _neighbor)
    {
        if (bp::override eq = this->get_override("equals"))
        {
            return eq(_neighbor);
        }
        return PyNeighbor::equals(_neighbor); //(false)
    }
    bool default_equals(PyNeighbor<SolutionType>& _neighbor) {
        return this->moNeighbor<SolutionType>::equals(_neighbor);
    }

    //like for PyEOT, "emulating" inheritance from EO ... this is not DRY
    bp::object getFitness() const {
        return this->invalid()? bp::object(): bp::object(this->fitness().get());
    }
    void setFitness(bp::object f) {
        if(f.ptr() == Py_None)
        {
            this->invalidate();
            return;
        }

        bp::extract<double> x(f);
        if(x.check())
        {
            double d = x();
            this->fitness(d);
        }else{
            throw index_error("can't extract fitness\n");
        }
    }

    bool operator<(const PyNeighbor<SolutionType>& _other) const
    {
        return this->fitness() < _other.fitness();
        // return getFitness() < _other.getFitness();
    }

    std::string to_string() const
    {
        std::string result;
        result += ' ';
        if(!this->invalid())
            result += bp::extract<const char*>(bp::str(getFitness()));
        else result += std::string("invalid");
        result += ' ';
        result += std::to_string(this->key);
        return result;
    }
//
private:
    bp::object move_op;
    bp::object move_back_op;
};

#endif
