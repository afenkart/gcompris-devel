#include <Python.h>
#include <pygobject.h>
#include "gcompris/gcompris.h"
#include "py-mod-admin.h"
#include "py-gcompris-board.h"
#include "py-gcompris-profile.h"

/* All functions provided by this python module
 * wraps a gcompris function. Each "py_*" function wraps the
 * "*" C function.
 */

static PyObject*
py_board_run_next (PyObject* self, PyObject* args)
{
  PyObject* pyObject;
  pyGcomprisBoardObject* pyGcomprisBoard;
  GcomprisBoard* cGcomprisBoard;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, "O:board_run_next", &pyObject))
    return NULL;
  pyGcomprisBoard = (pyGcomprisBoardObject*) pyObject;
  cGcomprisBoard = pyGcomprisBoard->cdata;

  /* Call the corresponding C function */
  board_run_next(cGcomprisBoard);

  /* Create and return the result */
  Py_INCREF(Py_None);
  return Py_None;

}

static PyObject*
py_board_config_start (PyObject* self, PyObject* args)
{
  PyObject* pyBoard;
  PyObject* pyProfile;
  pyGcomprisBoardObject* pyGcomprisBoard;
  pyGcomprisProfileObject* pyGcomprisProfile;
  GcomprisBoard* cGcomprisBoard;
  GcomprisProfile* cGcomprisProfile;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, 
		       "OO:board_config_start", 
		       &pyBoard, 
		       &pyProfile))
    return NULL;

  pyGcomprisBoard = (pyGcomprisBoardObject*) pyBoard;
  pyGcomprisProfile = (pyGcomprisProfileObject*) pyProfile;
  cGcomprisBoard = pyGcomprisBoard->cdata;
  cGcomprisProfile = pyGcomprisProfile->cdata;

  /* Call the corresponding C function */
  board_config_start(cGcomprisBoard, cGcomprisProfile);

  /* Create and return the result */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject*
py_board_config_stop (PyObject* self, PyObject* args)
{
  /* Parse arguments */
  if(!PyArg_ParseTuple(args, ":gcompris.board_config_stop"))
    return NULL;

  /* Call the corresponding C function */
  board_config_stop();

  /* Create and return the result */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject*
py_gcompris_get_boards_list (PyObject* self, PyObject* args)
{
  GList *boards_list;
  GList *list;
  PyObject *pylist;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, ":gcompris.get_boards_list"))
    return NULL;

  /* Call the corresponding C function */
  boards_list = gcompris_get_boards_list();

  pylist = PyList_New(0);
  for (list = boards_list; list != NULL; list = list->next){
    PyList_Append(pylist, gcompris_new_pyGcomprisBoardObject((GcomprisBoard*) list->data));
  }
  /* Create and return the result */
  return pylist;
}

static PyObject*
py_gcompris_get_profile_from_id (PyObject* self, PyObject* args)
{
  GcomprisProfile *profile;
  int profile_id;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, "i:gcompris.get_profile_from_id", &profile_id))
    return NULL;

  /* Call the corresponding C function */
  profile = gcompris_get_profile_from_id (profile_id);

  /* Create and return the result */
  return gcompris_new_pyGcomprisProfileObject(profile) ;
}

static PyObject*
py_gcompris_get_board_from_id (PyObject* self, PyObject* args)
{
  GcomprisBoard *board;
  int board_id;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, "i:gcompris.get_board_from_id", &board_id))
    return NULL;

  /* Call the corresponding C function */
  board = gcompris_get_board_from_id (board_id);

  /* Create and return the result */
  return gcompris_new_pyGcomprisBoardObject(board) ;
}

static PyObject*
py_gcompris_get_group_from_id (PyObject* self, PyObject* args)
{
  GcomprisGroup *group;
  int group_id;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, "i:gcompris.get_group_from_id", &group_id))
    return NULL;

  /* Call the corresponding C function */
  group = gcompris_get_group_from_id (group_id);

  /* Create and return the result */
  return gcompris_new_pyGcomprisGroupObject(group) ;
}

static PyObject*
py_gcompris_get_user_from_id (PyObject* self, PyObject* args)
{
  GcomprisUser *user;
  int user_id;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, "i:gcompris.get_user_from_id", &user_id))
    return NULL;

  /* Call the corresponding C function */
  user = gcompris_get_user_from_id (user_id);

  /* Create and return the result */
  return gcompris_new_pyGcomprisUserObject(user) ;
}

static PyObject*
py_gcompris_get_class_from_id (PyObject* self, PyObject* args)
{
  GcomprisClass *class;
  int class_id;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, "i:gcompris.get_class_from_id", &class_id))
    return NULL;

  /* Call the corresponding C function */
  class = gcompris_get_class_from_id (class_id);

  /* Create and return the result */
  return gcompris_new_pyGcomprisClassObject(class) ;
}

static PyObject*
py_gcompris_get_profiles_list (PyObject* self, PyObject* args)
{
  GList *profiles_list;
  GList *list;
  PyObject *pylist;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, ":gcompris.get_profiles_list"))
    return NULL;

  /* Call the corresponding C function */
  profiles_list = gcompris_get_profiles_list();

  pylist = PyList_New(0);
  for (list = profiles_list; list != NULL; list = list->next){
    PyList_Append(pylist, gcompris_new_pyGcomprisProfileObject((GcomprisProfile*) list->data));
  }
  /* Create and return the result */
  return pylist;
}

static PyObject*
py_gcompris_get_users_list (PyObject* self, PyObject* args)
{
  GList *users_list;
  GList *list;
  PyObject *pylist;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, ":gcompris.get_users_list"))
    return NULL;

  /* Call the corresponding C function */
  users_list = gcompris_get_users_list();

  pylist = PyList_New(0);
  for (list = users_list; list != NULL; list = list->next){
    PyList_Append(pylist, gcompris_new_pyGcomprisUserObject((GcomprisUser*) list->data));
  }
  /* Create and return the result */
  return pylist;
}

static PyObject*
py_gcompris_get_groups_list (PyObject* self, PyObject* args)
{
  GList *groups_list;
  GList *list;
  PyObject *pylist;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, ":gcompris.get_groups_list"))
    return NULL;

  /* Call the corresponding C function */
  groups_list = gcompris_get_groups_list();

  pylist = PyList_New(0);
  for (list = groups_list; list != NULL; list = list->next){
    PyList_Append(pylist, gcompris_new_pyGcomprisGroupObject((GcomprisGroup*) list->data));
  }
  /* Create and return the result */
  return pylist;
}

static PyObject*
py_gcompris_get_classes_list (PyObject* self, PyObject* args)
{
  GList *classes_list;
  GList *list;
  PyObject *pylist;

  /* Parse arguments */
  if(!PyArg_ParseTuple(args, ":gcompris.get_classes_list"))
    return NULL;

  /* Call the corresponding C function */
  classes_list = gcompris_get_classes_list();

  pylist = PyList_New(0);
  for (list = classes_list; list != NULL; list = list->next){
    PyList_Append(pylist, gcompris_new_pyGcomprisClassObject((GcomprisClass*) list->data));
  }
  /* Create and return the result */
  return pylist;
}


static PyMethodDef PythonGcomprisAdminModule[] = {
  { "board_run_next",  py_board_run_next, METH_VARARGS, "board_run_next" },
  { "board_config_start",  py_board_config_start, METH_VARARGS, "board_config_start" },
  { "board_config_stop",  py_board_config_stop, METH_VARARGS, "board_config_stop" },
  { "get_profile_from_id",  py_gcompris_get_profile_from_id, METH_VARARGS, "gcompris_get_profile_from_id" },
  { "get_profiles_list",  py_gcompris_get_profiles_list, METH_VARARGS, "gcompris_get_profiles_list" },
  { "get_user_from_id",  py_gcompris_get_user_from_id, METH_VARARGS, "gcompris_get_user_from_id" },
  { "get_users_list",  py_gcompris_get_users_list, METH_VARARGS, "gcompris_get_users_list" },
  { "get_group_from_id",  py_gcompris_get_group_from_id, METH_VARARGS, "gcompris_get_group_from_id" },
  { "get_groups_list",  py_gcompris_get_groups_list, METH_VARARGS, "gcompris_get_groups_list" },
  { "get_class_from_id",  py_gcompris_get_class_from_id, METH_VARARGS, "gcompris_get_class_from_id" },
  { "get_classes_list",  py_gcompris_get_classes_list, METH_VARARGS, "gcompris_get_classes_list" },
  { "get_board_from_id",  py_gcompris_get_board_from_id, METH_VARARGS, "gcompris_get_board_from_id" },
  { "get_boards_list",  py_gcompris_get_boards_list, METH_VARARGS, "gcompris_get_boards_list" },
  { NULL, NULL, 0, NULL}
};


void python_gcompris_admin_module_init(void)
{
  PyObject* module;
  module = Py_InitModule("_gcompris_admin", PythonGcomprisAdminModule);
}

/* Some usefull code parts ... */
/*
static PyObject*
py_gcompris_(PyObject* self, PyObject* args)
{
*/  /* Parse arguments */
/*  if(!PyArg_ParseTuple(args, ":gcompris_"))
    return NULL;
*/
  /* Call the corresponding C function */
/*  gcompris_();
*/
  /* Create and return the result */
/*  Py_INCREF(Py_None);
  return Py_None;
}
*/
/*
  { "",  py_gcompris_, METH_VARARGS, "gcompris_" },
*/