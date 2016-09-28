#include <Python.h>

int main() {



     char* inputFilename = "input.txt";
     char* modName = "pyfun";
     char* someFunction = "someFunction";
     Py_Initialize();

     PyObject* myModuleString = PyString_FromString(modName);
     PyObject* myModule = PyImport_Import(myModuleString);
     Py_DECREF(myModuleString);

     if(myModule != NULL)
     {
          PyObject* myFunction = PyObject_GetAttrString(myModule,someFunction);
     }
     else
     {
          PyErr_Print();
          fprintf(stderr, "Failed to load \"%s\"\n", modName);
          return 1;
     }
     // PyObject* args = PyTuple_Pack(1,PyString_FromString(inputFilename));
     //
     // PyObject* myResult = PyObject_CallObject(myFunction,args);


     return 0;
}
