#include "Python.h"
#include "sqlite3.h"

#define UNUSED(x) (void)(x);

/* Assume the pysqlite_Connection object's first non-PyObject member is the
 * sqlite3 database */
typedef struct
{
  PyObject_HEAD
  sqlite3* db;
} Connection;

static PyObject*
sqlite3_address(PyObject* self, PyObject* args)
{
  UNUSED(self);
  PyObject* con = NULL;
  if (!PyArg_ParseTuple(args, "O", &con)) {
    return NULL;
  }
  // get the address of the sqlite3* database
  Py_ssize_t address = (Py_ssize_t)((Connection*)con)->db;
  return PyLong_FromLong(address);
}

static PyMethodDef cslumba_methods[] = {
  { "sqlite3_address",
    (PyCFunction)sqlite3_address,
    METH_VARARGS,
    "Get the address of the sqlite3* db instance.\n\n"
    "Returns\n"
    "-------\n"
    "int\n"
    "    The address of the pysqlite_Connection's sqlite3* member\n\n" },
  { NULL, NULL, 0, NULL } /* sentinel */
};

static struct PyModuleDef cslumbamodule = {
  PyModuleDef_HEAD_INIT, "cslumba", NULL, -1, cslumba_methods,
};

PyMODINIT_FUNC
PyInit_cslumba(void)
{
  PyObject* module = PyModule_Create(&cslumbamodule);
  if (module == NULL) {
    return NULL;
  }

  if (PyModule_AddIntMacro(module, SQLITE_NULL) == -1) {
    return PyErr_Format(PyExc_RuntimeError,
                        "Unable to add SQLITE_NULL int constant with value %d",
                        SQLITE_NULL);
  }

  if (PyModule_AddIntMacro(module, SQLITE_OK) == -1) {
    return PyErr_Format(PyExc_RuntimeError,
                        "Unable to add SQLITE_OK int constant with value %d",
                        SQLITE_OK);
  }

  if (PyModule_AddIntMacro(module, SQLITE_DETERMINISTIC) == -1) {
    return PyErr_Format(
      PyExc_RuntimeError,
      "Unable to add SQLITE_DETERMINISTIC int constant with value %d",
      SQLITE_DETERMINISTIC);
  }

  if (PyModule_AddIntMacro(module, SQLITE_UTF8) == -1) {
    return PyErr_Format(PyExc_RuntimeError,
                        "Unable to add SQLITE_UTF8 int constant with value %d",
                        SQLITE_UTF8);
  }

  if (PyModule_AddStringMacro(module, SQLITE_VERSION) == -1) {
    return PyErr_Format(
      PyExc_RuntimeError,
      "Unable to add SQLITE_VERSION string constant with value %s",
      SQLITE_VERSION);
  }

  return module;
}
