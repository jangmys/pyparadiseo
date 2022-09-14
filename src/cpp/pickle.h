#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/smart_ptr.hpp>


namespace bp=boost::python;


//GENERIC COPY and DEEPCOPY

// https://mail.python.org/pipermail/cplusplus-sig/2009-May/014505.html
#define PYTHON_ERROR(TYPE, REASON) \
{ \
    PyErr_SetString(TYPE, REASON); \
    throw bp::error_already_set(); \
}

template<class T>
inline PyObject * managingPyObject(T *p)
{
    return typename bp::manage_new_object::apply<T *>::type()(p);
}

template<class Copyable>
bp::object generic__copy__(bp::object copyable)
{
    Copyable *newCopyable(new Copyable(bp::extract<const Copyable
&>(copyable)));
    bp::object
result(bp::detail::new_reference(managingPyObject(newCopyable)));

    bp::extract<bp::dict>(result.attr("__dict__"))().update(
        copyable.attr("__dict__"));

    return result;
}

template<class Copyable>
bp::object generic__deepcopy__(bp::object copyable, bp::dict memo)
{
    bp::object copyMod = bp::import("copy");
    bp::object deepcopy = copyMod.attr("deepcopy");

    Copyable *newCopyable(new Copyable(bp::extract<const Copyable
&>(copyable)));
    bp::object
result(bp::detail::new_reference(managingPyObject(newCopyable)));

    // HACK: copyableId shall be the same as the result of id(copyable) in Python -
    // please tell me that there is a better way! (and which ;-p)
    int copyableId = (long long)(copyable.ptr());
    memo[copyableId] = result;

    bp::extract<bp::dict>(result.attr("__dict__"))().update(
        deepcopy(bp::extract<bp::dict>(copyable.attr("__dict__"))(),
memo));

    return result;
}

//================================
//PICKLING
//================================
typedef moeoRealObjectiveVector<moeoObjectiveVectorTraits> ObjectiveVector;

struct ObjectiveVector_pickle_suite : boost::python::pickle_suite
  {
    static
    boost::python::tuple
    getinitargs(ObjectiveVector const& ovec)
    {
        return boost::python::make_tuple(
            boost::python::list(ovec)
        );
    }
  };


  //TODO: __copy__ OK, error when trying to __deepcopy__
  template<typename PyEOT>
  struct PyEOT_pickle_suite : bp::pickle_suite
  {
      typedef doubleFitness Fitness;
      typedef double Diversity;
      typedef realObjVec ObjectiveVector;

      static bp::tuple getstate(const PyEOT& p)
      // static bp::tuple getstate(bp::object _eot)
      {
          // PyEOT const& p = bp::extract<PyEOT const&>(_eot)();

          return bp::make_tuple(
                  p.invalidObjectiveVector()?bp::object():bp::object(p.objectiveVector()),
                  p.invalidFitness()?bp::object():bp::object(p.fitness().get()),
                  p.invalidDiversity()?bp::object():bp::object(p.diversity()),
                  p.get_encoding());
      }

      static void setstate(PyEOT& p, bp::tuple state)
      // static void setstate(bp::object _eot, bp::tuple state)
      {
          using namespace boost::python;
          // PyEOT& p = bp::extract<PyEOT&>(_eot);

          //https://stackoverflow.com/questions/22674774/get-single-element-from-a-boostpythonobject-list-for-use-in-python-routine
          /*
          The boost::python::object's operator[] returns a boost::python::proxy object. While the proxy class has an implicit conversion to boost::python::object, there are many areas in the API where an explicit conversion is required.

          Explicitly constructing a boost::python::object from the proxy should resolve the conversion exception:
          */
          p.setObjectiveVector( bp::object(state[0]) );
          p.setFitness( bp::object(state[1]) );
          p.setDiversity( state[2] ); //checks for None

          if(bp::object(state[3]).ptr() != Py_None)
          {
              p.set_encoding(state[3]);
          }else{
              p.set_encoding(bp::object())
              ;
          }
      }
  };
