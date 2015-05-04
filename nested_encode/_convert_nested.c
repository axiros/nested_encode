#include "Python.h"

/* Forward declaration. */

typedef PyObject* (*convert_func)(PyObject *ob);
static PyObject * convert_nested(PyObject *ob, convert_func convert_string);

static PyObject*
convert_dict(PyObject *ob, convert_func convert_string)
{
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    PyObject *new_ob = PyDict_New();

    while (PyDict_Next(ob, &pos, &key, &value)) {
        if ((key = convert_nested(key, convert_string)) == NULL) {
            Py_DECREF(new_ob);
            return NULL;
        }

        if ((value = convert_nested(value, convert_string)) == NULL) {
            Py_DECREF(new_ob);
            Py_DECREF(key);
            return NULL;
        }

        if(PyDict_SetItem(new_ob, key, value) == -1) {
            Py_DECREF(key);
            Py_DECREF(value);
            Py_DECREF(new_ob);
            return NULL;
        }
        else {
            Py_DECREF(key);
            Py_DECREF(value);
        }

    }
    return new_ob;
}

static PyObject*
convert_seq(PyObject *ob, convert_func convert_string)
{
    PyObject *seq = PySequence_Fast(ob, "ob is not a sequence");
    if (seq == NULL) {
        return NULL;
    }

    Py_ssize_t i = 0;
    Py_ssize_t seq_len = PySequence_Fast_GET_SIZE(seq);
    PyObject **items = PySequence_Fast_ITEMS(seq);

    for (i=0; i < seq_len; ++i) {
        PyObject *old_item = items[i];
        PyObject *new_item = convert_nested(old_item, convert_string);
        if (new_item == NULL) {
            Py_DECREF(seq);
            return NULL;
        }
        items[i] = new_item;
        Py_DECREF(old_item);
    }

    return seq;
}

static PyObject*
convert_nested(PyObject *ob, convert_func convert_string)
{
    /* dict. */
    if (PyDict_CheckExact(ob)) {
        return convert_dict(ob, convert_string);
    }

    /* sequence. */
    if (PyTuple_CheckExact(ob) || PyList_CheckExact(ob)) {
        return convert_seq(ob, convert_string);
    }

    /* numbers. */
    if (PyInt_CheckExact(ob) || PyLong_CheckExact(ob) || PyFloat_CheckExact(ob)) {
        Py_INCREF(ob);
        return ob;
    }

    /* bool. */
    if (PyBool_Check(ob)) {
        Py_INCREF(ob);
        return ob;
    }

    /* none. */
    if (ob == Py_None) {
        Py_INCREF(ob);
        return ob;
    }

    if (PyString_CheckExact(ob) || PyUnicode_CheckExact(ob)) {
        return convert_string(ob);
    }

    return PyErr_Format(
        PyExc_TypeError,
        "Got wrong type: %s",
        ob->ob_type->tp_name);
}

static PyObject*
encode_string(PyObject *ob)
{
    /* String is already encoded. */
    if (PyString_CheckExact(ob)) {
        Py_INCREF(ob);
        return ob;
    }

    return PyUnicode_AsUTF8String(ob);
}

static PyObject*
encode_nested(PyObject *self, PyObject *args)
{
    PyObject *ob;
    if (!PyArg_ParseTuple(args, "O", &ob)) {
        return NULL;
    }

    return convert_nested(ob, &encode_string);
}

static PyMethodDef convert_nested_methods[] = {
    {"encode_nested", encode_nested, METH_VARARGS, "Docu"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
init_convert_nested(void)
{
    Py_InitModule("_convert_nested", convert_nested_methods);
}
