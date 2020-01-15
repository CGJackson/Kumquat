#include "kumquat.hpp"

#include <complex>
#include <utility>
#include <vector>
#include <stdexcept>

#include "IntegrandFunctionWrapper.hpp"
#include "utils.hpp"

namespace kumquat_internal {
using std::complex;

IntegrandFunctionWrapper::IntegrandFunctionWrapper(const IntegrandFunctionWrapper& other)
            :callback{other.callback}, args{other.args} {
            Py_INCREF(other.callback);
            for(auto a: args){
                Py_INCREF(a);
            }
        }

        //I can't think of a more efficent way to leave the orignial
        //in a valid state after moving than simply copying
IntegrandFunctionWrapper::IntegrandFunctionWrapper(IntegrandFunctionWrapper&& other)
            :callback{other.callback}, args{other.args} {
            Py_INCREF(other.callback);
            for(auto a: args){
                Py_INCREF(a);
            }
        }
IntegrandFunctionWrapper::IntegrandFunctionWrapper(PyObject * func, 
                                        PyObject * new_args)
    :callback{func}, args{} {
    if( func == NULL){
        if(PyErr_Occurred() == NULL){
                PyErr_SetString(PyExc_TypeError,"No valid Python object passed to IntegrandFunctionWrapper to wrap"); 
        }
        throw unable_to_construct_wrapper("Function passed to IntegrandFunctionWrapper cannot be NULL");
    }

    if(new_args == NULL){
        if(PyErr_Occurred() == NULL){
            PyErr_SetString(PyExc_TypeError, "No valid Python object was passed to IntegrandFuncitonWrapper as funciton arguments");
        }
        throw unable_to_construct_wrapper("Function arguments passed to IntegrandFunctionWrapper cannot be NULL");
    }

    if(!PyCallable_Check(func)){
        throw function_not_callable("The Python Object for IntegrandFunctionWrapper to wrap was not callable", "Unable to wrap uncallable object");
    }

    Py_INCREF(func); 

    if(new_args  == Py_None){
        return;
    }

    if(!PyTuple_Check(new_args)){
        Py_DECREF(func);
        throw arg_list_not_tuple("The argument list given to IntegrandFunctionWrapper was not a Python Tuple", "The extra arguments passed to the function wrapper were not a valid python tuple");
    }
        
    const Py_ssize_t extra_arg_count = PyTuple_GET_SIZE(new_args);
    if(extra_arg_count >= PY_SSIZE_T_MAX){
        PyErr_SetString(PyExc_TypeError,"Too many arguments provided to integrand function");
        Py_DECREF(func);
        throw unable_to_construct_wrapper("Too many arguments provided");
    }

    args.reserve(extra_arg_count);

    for(Py_ssize_t i = 0; i < extra_arg_count; ++i){
        PyObject* fixed_arg = PyTuple_GET_ITEM(new_args,i);
        args.push_back(fixed_arg);
        Py_INCREF(fixed_arg);
    }
}

PyObject* IntegrandFunctionWrapper::buildArgTuple(Real x) const{
    PyObject* py_x = PyFloat_FromDouble(x);
    if(py_x == NULL){
        throw unable_to_construct_py_object("error converting callback arg to Py_Float");
    }

    PyObject* arg_tuple = PyTuple_New(this->args.size()+1);
    if(arg_tuple == NULL){
        throw unable_to_form_arg_tuple("unable construct arg tuple");
    }

    PyTuple_SET_ITEM(arg_tuple,0,py_x);

    for(Py_ssize_t i = 1; i <= this->args.size(); ++i){
        PyTuple_SET_ITEM(arg_tuple, i, this->args[i]);
        Py_INCREF(this->args[i]);
    }

    return arg_tuple;
}

complex<Real> IntegrandFunctionWrapper::operator()(Real x) const{
    // Calls the Python function callback with x as a python float
    // and args as its other arguments and reutrns the result as a
    // std::complex
    
    
    PyObject* arg_tuple = this->buildArgTuple(x);

    PyObject* py_result = PyObject_CallObject(callback, arg_tuple);
    

    if(py_result == NULL){
        throw PythonError("Error occured in integrand function");
    }

    Py_DECREF(arg_tuple); // Note error in py_result must be resolved before DECREF, as __del__ may call arbitrary code
    
    if(!convertable_to_py_complex(py_result)){
        Py_DECREF(py_result);
        throw function_did_not_return_complex("The return value of the integrand function could not be converted to a complex number", 
                "The function passed to IntegrandFunctionWrapper did not return a value that culd be converted to complex");
    }
    
    complex<Real> cpp_result = complex_from_c_complex(PyComplex_AsCComplex(py_result));

    Py_DECREF(py_result);
    
    return cpp_result;
}




}