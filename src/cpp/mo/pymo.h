#ifndef PYMO_H_
#define PYMO_H_

#include "pyeot.h"

#include <neighborhood/moNeighbor.h>
#include <neighborhood/moIndexNeighbor.h>
#include <neighborhood/moBackableNeighbor.h>
#include <neighborhood/moDummyNeighbor.h>

namespace bp = boost::python;

//Neighbor and derivatives IndexNeighbor and BackableNeighbor are ABC with move, moveBack as pure virtuals.
//In paradiseo users will specialize those before instantiating algorithms with derived neighbor types (specializing )

// template<typename SolutionType>
//BASE Neighbor : PyEO + MOVE
//MOVE needs solution type

template<typename SolutionType>
struct PyNeighbor : SolutionType
{
    typedef SolutionType EOT;

    PyNeighbor() : SolutionType(){}

    // PyNeighbor is not supposed to be instantiated directly, move_op is static
    // PyNeighbor(bp::object move,bp::object move_back) : move_op(move),move_back_op(move_back) { }

    static void setMove(bp::object obj) {
        move_op = obj;
    }

    static void setMoveBack(bp::object obj) {
        move_back_op = obj;
    }

    //overridng Nieghbor move (pure virtual) to with Python callback
    virtual void move(SolutionType& _solution)
    {
        if(move_op.ptr() != Py_None)
        {
            move_op(bp::ptr(this), boost::ref(_solution));
            //should be a bit faster but less generic...
            // move_op(bp::ptr(this), _solution.encoding);
        }
        else
            std::cout<<"no move defined : do nothing"<<std::endl;
    }

//     //overridng Nieghbor moveBack (pure virtual) to with Python callback
    virtual void moveBack(SolutionType& _solution)
    {
        if(move_back_op.ptr() != Py_None)
        {
            move_back_op(bp::ptr(this), boost::ref(_solution));
            // bp::call<void>(move_back_op.ptr(), bp::ptr(this), boost::ref(_solution));
        }
        else
            std::cout<<"no moveBack defined : do nothing";
    }

    //from IndexNeighbor
	bool equals(PyNeighbor& _neighbor) {
		return (key == _neighbor.index());
	}

    inline unsigned int index() const {
		return key;
	}

    void index(unsigned int _key) {
      key = _key;
    }

    virtual void index(EOT & _solution, unsigned int _key) {
      key = _key;
    }

    PyNeighbor& operator=(const PyNeighbor& _source) {
		SolutionType::operator=(_source);
		this->key = _source.key;
        this->move_op = _source.move_op;
        this->move_back_op = _source.move_back_op;

		return *this;
	}

    unsigned int key;

protected:
    //base
    static bp::object move_op;
    //backable
    static bp::object move_back_op;
};

//static members
template<typename SolutionType>
boost::python::object PyNeighbor<SolutionType>::move_op;

template<typename SolutionType>
boost::python::object PyNeighbor<SolutionType>::move_back_op;



//BinFlipNeighbor ....
//provide predefined neighbors (== moves) this way
struct BinNeighbor : PyNeighbor<BinarySolution>
{
    typedef BinarySolution EOT;

    BinNeighbor() : PyNeighbor<BinarySolution>(){ }
//
//     // NeighborBin(bp::object move) : moIndexNeighbor<SolutionType>(),move_op(move) { }
//     //
//     PyBinNeighbor(bp::object move,bp::object move_back) : BinarySolution(),move_op(move),move_back_op(move_back) { }
//
//     // NeighborBin(const NeighborBin& _n) : PyNeighbor<BinarySolution>(_n){}
//
    void move(BinarySolution& _solution)
    {
        if(this->move_op.ptr() != Py_None)
        {
            this->move_op(bp::ptr(this), boost::ref(_solution));
        }else{
            _solution[index()] = !_solution[index()];
            _solution.invalidate();
        }
    }

    void moveBack(BinarySolution& _solution)
    {
        if(this->move_back_op.ptr() != Py_None)
        {
            this->move_op(bp::ptr(this), boost::ref(_solution));
        }

        move(_solution);
    }
};





// template<typename SolutionType>
// struct PyNeighbor :  moIndexNeighbor<SolutionType>,moBackableNeighbor<SolutionType>,bp::wrapper<moIndexNeighbor<SolutionType>>

// template<typename SolutionType>
// struct PyNeighbor :  moIndexNeighbor<SolutionType>,bp::wrapper<moIndexNeighbor<SolutionType>>
// {
// public:
//     typedef SolutionType EOT;
//
//     PyNeighbor() : moIndexNeighbor<SolutionType>() { }
//
//     PyNeighbor(bp::object move) : moIndexNeighbor<SolutionType>(),move_op(move) { }
//
//     PyNeighbor(bp::object move,bp::object move_back) : moIndexNeighbor<SolutionType>(),move_op(move),move_back_op(move_back) { }
//
//     PyNeighbor(const PyNeighbor<SolutionType>& _n) : moIndexNeighbor<SolutionType>(_n){
//         this->fitness(_n.fitness());
//         move_op = _n.move_op;
//         move_back_op = _n.move_op;
//     };
//
//     void setMove(bp::object obj) {
//         move_op = obj;
//     }
//
//     void setMoveBack(bp::object obj) {
//         move_back_op = obj;
//     }
//
//     //overridng Nieghbor move (pure virtual) to with Python callback
//     void move(SolutionType& _solution)
//     {
//         if(move_op.ptr() != Py_None)
//         {
//             move_op(bp::ptr(this), boost::ref(_solution));
//             //should be a bit faster but less generic...
//             // move_op(bp::ptr(this), _solution.encoding);
//         }
//         else
//             std::cout<<"no move defined : do nothing";
//     }
//
// //     //overridng Nieghbor moveBack (pure virtual) to with Python callback
//     void moveBack(SolutionType& _solution)
//     {
//         if(move_back_op.ptr() != Py_None)
//         {
//             move_back_op(bp::ptr(this), boost::ref(_solution));
//             // bp::call<void>(move_back_op.ptr(), bp::ptr(this), boost::ref(_solution));
//         }
//         else
//             std::cout<<"no moveBack defined : do nothing";
//     }
//
//     //virtual with defaults
//     bool equals(PyNeighbor<SolutionType>& _neighbor)
//     {
//         if (bp::override eq = this->get_override("equals"))
//         {
//             return eq(_neighbor);
//         }
//         return PyNeighbor::equals(_neighbor); //(false)
//     }
//     bool default_equals(PyNeighbor<SolutionType>& _neighbor) {
//         return this->moNeighbor<SolutionType>::equals(_neighbor);
//     }
//
//     //like for PyEOT, "emulating" inheritance from EO ... this is not DRY
//     bp::object getFitness() const {
//         return this->invalid()? bp::object(): bp::object(this->fitness().get());
//     }
//     void setFitness(bp::object f) {
//         if(f.ptr() == Py_None)
//         {
//             this->invalidate();
//             return;
//         }
//
//         bp::extract<double> x(f);
//         if(x.check())
//         {
//             double d = x();
//             this->fitness(d);
//         }else{
//             throw index_error("can't extract fitness\n");
//         }
//     }
//
//     bool operator<(const PyNeighbor<SolutionType>& _other) const
//     {
//         return this->fitness() < _other.fitness();
//         // return getFitness() < _other.getFitness();
//     }
//
//     std::string to_string() const
//     {
//         std::string result;
//         result += ' ';
//         if(!this->invalid())
//             result += bp::extract<const char*>(bp::str(getFitness()));
//         else result += std::string("invalid");
//         result += ' ';
//         result += std::to_string(this->key);
//         return result;
//     }
// //
// protected:
//     bp::object move_op;
//     bp::object move_back_op;
// };







#endif
