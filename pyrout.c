/*
Check password for a userid.
Note: requires program-controlled environment
To build:
c99 -o checkpass checkpass.c i
extattr +p checkpass
*/
#define _POSIX_THREADS
#define _UNIX03_THREADS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>
#include <env.h>
#include </zdo/conda/include/python3.7m/Python.h>



static PyObject *method_verify(PyObject *self, PyObject *args) {
  char *username, *password = NULL;

  if(!PyArg_ParseTuple(args, "ss", &username, &password)) {
        fprintf(stderr,"Failed Parsing Arguments :(");
        return Py_False;
  }

  setenv("_EDC_ADD_ERRNO2", "1", 1);

  if (__passwd(username, password, NULL)) {
    if (errno == EACCES) {
      return Py_False; 
    } 

    else {
      return Py_False;
    }
  }
  return Py_True; /* no message - password valid */
}


static PyMethodDef myMethods[] = {
    {"verify", method_verify, METH_VARARGS, "Python interface for RACROUTE VERIFY"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef pyroutmodule = {
    PyModuleDef_HEAD_INIT,
    "pyrout",
    "Python interface for RACROUT functions",
    -1,
    myMethods
}; 


PyMODINIT_FUNC PyInit_pyrout(void) {
    return PyModule_Create(&pyroutmodule);
}
