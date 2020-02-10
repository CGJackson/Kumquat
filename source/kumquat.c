#include "kumquat.hpp"
#include "integration_routines.h"

#define GAUSS_KRONROD_DOCS "Performs Gauss-Kronrod quadrature, returning a complex result and a real error estimate\n\nParameters:\n\tf: Callable. Function to be integrated. Must take a point in the integration range as a float in its first argument and return a complex. Additional arguments can be passed to f via the args and kwargs parameters\n\ta: float. Lower limit of integration\n\tb: float. Upper limit of integration. Must be strictly greater than a\n\nOptional Parameters:\n\targs: tuple. Additional positional arguments to be passed to f. The position in the integration region must still be the first argument of f. default None.\n\tkwargs: dict. Additional keyword arguments to be passed to f. Default None.\n\nKeyword Parameters:\n\tfull_output: bool. If true returns a dict containing additional infomation about the integration performed, in addition to the result and error estemate. This dict contains an estimate of the L1 norm of f, a list of the abscissa used in the integration, and a list of the weights used in the integration. Default False.\n\tmax_levels: int. The maximum number of levels of adaptive quadrature to be used in the integration. Set to 0 for non-adaptive quadrature. default 15\n\ttolarence: float. The maximum relative error in the result. Should not be set too close to machine precision, Default sqrt of machine precision\n\tpoints: int. Must be chosen from {15,31,41,51,61}. Number of points being used in each level of Gaussian quadrature."
#define TANH_SINH_DOCS "Performs tanh-sinh quadrature, returning a complex result and a real error estimate\n\nParameters:\n\tf: Callable. Function to be integrated. Must take a point in the integration range as a float in its first argument and return a complex. Additional arguments can be passed to f via the args and kwargs parameters\n\ta: float. Lower limit of integration. May be -inf.\n\tb: float. Upper limit of integration. Must be strictly greater than a. May be +inf\n\nOptional Parameters:\n\targs: tuple. Additional positional arguments to be passed to f. The position in the integration region must still be the first argument of f. default None.\n\tkwargs: dict. Additional keyword arguments to be passed to f. Default None.\n\nKeyword Parameters:\n\tfull_output: bool. If true returns a dict containing additional infomation about the integration performed, in addition to the result and error estemate. This dict contains an estimate of the L1 norm of f and the number of levels of adaptive quadrature used to achieve the required tolarence. Default False.\n\tmax_levels: int. The maximum number of levels of adaptive quadrature to be used in the integration. Set to 0 for non-adaptive quadrature. default 15\n\ttolarence: float. The maximum relative error in the result. Should not be set too close to machine precision, Default sqrt of machine precision."
#define SINH_SINH_DOCS "Performs sinh-sinh quadrature, returning a complex result and a real error estimate\n\nParameters:\n\tf: Callable. Function to be integrated. Must take a float in its first argument and return a complex. Additional arguments can be passed to f via the args and kwargs parameters\n\nOptional Parameters:\n\targs: tuple. Additional positional arguments to be passed to f. The position in the integration region must still be the first argument of f. default None.\n\tkwargs: dict. Additional keyword arguments to be passed to f. Default None.\n\nKeyword Parameters:\n\tfull_output: bool. If true returns a dict containing additional infomation about the integration performed, in addition to the result and error estemate. This dict contains an estimate of the L1 norm of f and the number of levels of adaptive quadrature used to achieve the required tolarence. Default False.\n\tmax_levels: int. The maximum number of levels of adaptive quadrature to be used in the integration. Set to 0 for non-adaptive quadrature. default 15\n\ttolarence: float. The maximum relative error in the result. Should not be set too close to machine precision, Default sqrt of machine precision."
#define EXP_SINH_DOCS NULL
#define TRAPEZOIDAL_DOCS "Performs trapezoidal quadrature, returning a complex result and a real error estimate\n\nParameters:\n\tf: Callable. Function to be integrated. Must take a point in the integration range as a float in its first argument and return a complex. Additional arguments can be passed to f via the args and kwargs parameters\n\ta: float. Lower limit of integration\n\tb: float. Upper limit of integration. Must be strictly greater than a\n\nOptional Parameters:\n\targs: tuple. Additional positional arguments to be passed to f. The position in the integration region must still be the first argument of f. default None.\n\tkwargs: dict. Additional keyword arguments to be passed to f. Default None.\n\nKeyword Parameters:\n\tfull_output: bool. If true returns a dict containing additional infomation about the integration performed, in addition to the result and error estemate. This dict contains an estimate of the L1 norm of f. Default False.\n\tmax_levels: int. The maximum number of levels of adaptive quadrature to be used in the integration. Set to 0 for non-adaptive quadrature. default 12\n\ttolarence: float. The maximum relative error in the result. Should not be set too close to machine precision, Default sqrt of machine precision."

/* Method Table */
static PyMethodDef KumquatMethods[] = {
    {"trapezoidal", (PyCFunction) trapezoidal, METH_VARARGS | METH_KEYWORDS,
    TRAPEZOIDAL_DOCS},
    {"gauss_kronrod", (PyCFunction) gauss_kronrod, METH_VARARGS | METH_KEYWORDS,
    GAUSS_KRONROD_DOCS },
    {"tanh_sinh", (PyCFunction) tanh_sinh, METH_VARARGS | METH_KEYWORDS,
    TANH_SINH_DOCS},
    {"sinh_sinh", (PyCFunction) sinh_sinh, METH_VARARGS | METH_KEYWORDS,
    SINH_SINH_DOCS},
    {"exp_sinh", (PyCFunction) exp_sinh, METH_VARARGS | METH_KEYWORDS,
    EXP_SINH_DOCS},
    {NULL,NULL,0,NULL}
};

/* Module docs. Must be a C constant expression, hence the define */
#define KUMQUAT_DOCS "Provides routine to perform efficient complex valued numeric integration\n\nContains:\n\tgauss_kronrod: Performs Gauss-Kronrod quadrature over a finite interval\n\ttanh_sinh: Performs tanh-sinh quadrature over a finite, semi-infinite or infinite interval\n\tsinh_sinh: Performs sinh-sinh quadrature over an infinite interval"

/* Module definition Structure */
static struct PyModuleDef KumquatModule = {
    PyModuleDef_HEAD_INIT,
    "kumquat",/* Module name */
    KUMQUAT_DOCS, 
    -1, /* Module keeps state at global scope */
    KumquatMethods
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_kumquat(void){
    return PyModule_Create(&KumquatModule);
}
