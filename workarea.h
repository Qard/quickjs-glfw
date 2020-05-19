#ifndef GLFW_MODULE_WORKAREA
#define GLFW_MODULE_WORKAREA 1

#include "position.h"
#include "size.h"

typedef struct {
  GLFWPosition* position;
  GLFWSize* size;
} GLFWWorkArea;

JSClassID glfw_workarea_class_id;
JSValue glfw_workarea_new_instance(JSContext* ctx, GLFWWorkArea* workarea);
int glfw_workarea_init(JSContext* ctx, JSModuleDef* m);
int glfw_workarea_export(JSContext* ctx, JSModuleDef* m);

#endif
