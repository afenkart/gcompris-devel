#include <Python.h>
#include <pygobject.h>
#include "gcompris/gcompris.h"
#include "py-gcompris-board.h"
#include "py-mod-anim.h"

static int Animation_init(py_GcomprisAnimation *self, PyObject*, PyObject*);
static void Animation_free(py_GcomprisAnimation *self);

static int AnimCanvas_init(py_GcomprisAnimCanvas*, PyObject*, PyObject*);
static void AnimCanvas_free(py_GcomprisAnimCanvas*);
static PyObject *AnimCanvas_getattr(py_GcomprisAnimCanvas*, char*);

/* AnimCanvas methods */
static PyObject *py_gcompris_animcanvas_setstate(PyObject*, PyObject*);

static PyMethodDef AnimCanvasMethods[] = {
  {"setState", py_gcompris_animcanvas_setstate, METH_VARARGS,
    "gcompris_animcanvas_setstate"},
  {NULL, NULL, 0, NULL}
};

static PyMethodDef AnimationMethods[] = {
  {NULL, NULL, 0, NULL}
};

static PyTypeObject py_GcomprisAnimationType = {
  PyObject_HEAD_INIT(NULL)
  0,                            /* ob_size */
  "pyGcomprisAnimation",        /* tp_name */
  sizeof(py_GcomprisAnimation), /* tp_basicsize */
  0,                            /* tp_itemsize */
  (destructor)Animation_free,   /* tp_dealloc */
  0,                            /* tp_print */
  0,                            /* tp_getattr */
  0,                            /* tp_setattr */
  0,                            /* tp_compare */
  0,                            /* tp_repr */
  0,                            /* tp_as_number */
  0,                            /* tp_as_sequence */
  0,                            /* tp_as_mapping */
  0,                            /* tp_hash */
  0,                            /* tp_call */
  0,                            /* tp_str */
  0,                            /* tp_getattro */
  0,                            /* tp_setattro */
  0,                            /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,           /* tp_flags */
  "Animation objects",          /* tp_doc */
  0,                            /* tp_traverse */
  0,                            /* tp_clear */
  0,                            /* tp_richcompare */
  0,                            /* tp_weaklistoffset */
  0,                            /* tp_iter */
  0,                            /* tp_iternext */
  AnimationMethods,             /* tp_methods */
  0,                            /* tp_members */
  0,                            /* tp_getset */
  0,                            /* tp_base */
  0,                            /* tp_dict */
  0,                            /* tp_descr_get */
  0,                            /* tp_descr_set */
  0,                            /* tp_dictoffset */
  (initproc)Animation_init,     /* tp_init */
  0,                            /* tp_alloc */
  0,                            /* tp_new */
};

static PyTypeObject py_GcomprisAnimCanvasType = {
  PyObject_HEAD_INIT(NULL)
  0,                                /* ob_size */
  "pyGcomprisAnimCanvas",           /* tp_name */
  sizeof(py_GcomprisAnimCanvas),    /* tp_basicsize */
  0,                                /* tp_itemsize */
  (destructor)AnimCanvas_free,      /* tp_dealloc */
  0,                                /* tp_print */
  (getattrfunc)AnimCanvas_getattr,  /* tp_getattr */
  0,                            /* tp_setattr */
  0,                            /* tp_compare */
  0,                            /* tp_repr */
  0,                            /* tp_as_number */
  0,                            /* tp_as_sequence */
  0,                            /* tp_as_mapping */
  0,                            /* tp_hash */
  0,                            /* tp_call */
  0,                            /* tp_str */
  0,                            /* tp_getattro */
  0,                            /* tp_setattro */
  0,                            /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,           /* tp_flags */
  "Animated canvas objects",    /* tp_doc */
  0,                            /* tp_traverse */
  0,                            /* tp_clear */
  0,                            /* tp_richcompare */
  0,                            /* tp_weaklistoffset */
  0,                            /* tp_iter */
  0,                            /* tp_iternext */
  AnimCanvasMethods,            /* tp_methods */
  0,                            /* tp_members */
  0,                            /* tp_getset */
  0,                            /* tp_base */
  0,                            /* tp_dict */
  0,                            /* tp_descr_get */
  0,                            /* tp_descr_set */
  0,                            /* tp_dictoffset */
  (initproc)AnimCanvas_init,    /* tp_init */
  0,                            /* tp_alloc */
  0,                            /* tp_new */
};

static PyMethodDef PythonGcomprisAnimModule[] = {
  {NULL, NULL, 0, NULL}
};

/*============================================================================*/
/*                      GcomprisAnimation functions                           */
/*============================================================================*/
static int
Animation_init(py_GcomprisAnimation *self, PyObject *args, PyObject *key)
{
  static char *kwlist[] =
  {
    "filename", "dataset", "categories", "mimetype", "name", NULL
  };

  char *file=NULL, *data=NULL, *cat=NULL, *mime=NULL, *name=NULL;

  if(!PyArg_ParseTupleAndKeywords(args, key, "|sssss", kwlist,
                                   &file, &data, &cat, &mime, &name))
    {
      return -1;
    }

  if(file)
    {
      self->a = gcompris_load_animation(file);
    }
  else
    {
      if( !data || !cat || !mime || !name )
          return -1;
      self->a = gcompris_load_animation_asset(data, cat, mime, name);
    }

  if(!self->a)
    {
      return -1;
    }
  return 0;
}

static void Animation_free(py_GcomprisAnimation *self)
{
  printf("*** Garbage collecting Animation ***\n");
  gcompris_free_animation(self->a);
  PyObject_DEL(self);
}

/*============================================================================*/
/*                            Animation Methods                               */
/*============================================================================*/

static int
AnimCanvas_init(py_GcomprisAnimCanvas *self, PyObject *args, PyObject *key)
{
  GcomprisAnimCanvasItem *item;
  GcomprisAnimation *anim;
  GnomeCanvasGroup *parent;
  PyObject *py_p, *py_a;

  if(!PyArg_ParseTuple(args, "OO:AnimCanvas_init", &py_a, &py_p))
      return -1;

  parent = (GnomeCanvasGroup*) pygobject_get(py_p);
  anim = ( (py_GcomprisAnimation*)py_a )->a;
  item = (GcomprisAnimCanvasItem*) gcompris_activate_animation(parent, anim);
  self->item = item;
  self->anim = py_a;

  Py_INCREF(self->anim);
  return 0;
}

static void
AnimCanvas_free(py_GcomprisAnimCanvas *self)
{
  printf("*** garbage collecting AnimCanvas ***\n");
  gcompris_deactivate_animation(self->item);
  Py_DECREF(self->anim);
  PyObject_DEL(self);
}

static PyObject *AnimCanvas_getattr(py_GcomprisAnimCanvas *self, char *name)
{
  if(!strcmp(name, "gnome_canvas"))
    return (PyObject*) pygobject_new( (GObject*) self->item->canvas );
  else if(!strcmp(name, "num_states"))
    return Py_BuildValue("i", self->item->anim->numstates);

  return Py_FindMethod(AnimCanvasMethods, (PyObject *)self, name);  
}

static PyObject*
py_gcompris_animcanvas_setstate(PyObject *self, PyObject *args)
{
  int state;
  GcomprisAnimCanvasItem *item = ( (py_GcomprisAnimCanvas*)self )->item;

  if(!PyArg_ParseTuple(args, "i:gcompris_animcanvas_setstate", &state))
    return NULL;

  gcompris_set_anim_state( item, state );

  Py_INCREF(Py_None);
  return Py_None;
}

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
python_gcompris_anim_module_init(void) 
{
  PyObject* m;

  py_GcomprisAnimationType.tp_new = PyType_GenericNew;
  py_GcomprisAnimationType.ob_type = &PyType_Type;
  py_GcomprisAnimCanvasType.tp_new = PyType_GenericNew;
  py_GcomprisAnimCanvasType.ob_type = &PyType_Type;
  if (PyType_Ready(&py_GcomprisAnimationType) < 0)
      return;
  if (PyType_Ready(&py_GcomprisAnimCanvasType) < 0)
      return;

  m = Py_InitModule("_gcompris_anim", PythonGcomprisAnimModule);

  Py_INCREF(&py_GcomprisAnimationType);
  Py_INCREF(&py_GcomprisAnimCanvasType);
  PyModule_AddObject(m, "Animation", (PyObject *)&py_GcomprisAnimationType);
  PyModule_AddObject(m, "CanvasItem", (PyObject *)&py_GcomprisAnimCanvasType);
}
