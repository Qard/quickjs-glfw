#include "glfw.h"

#include "workarea.h"

// Constructor/Destructor
JSValue glfw_workarea_ctor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv) {
  GLFWWorkArea* workarea = js_mallocz(ctx, sizeof(*workarea));
  if (!workarea) return JS_EXCEPTION;

  GLFWPosition* position = js_mallocz(ctx, sizeof(*position));
  if (!position) return JS_EXCEPTION;

  GLFWSize* size = js_mallocz(ctx, sizeof(*size));
  if (!size) return JS_EXCEPTION;

  workarea->position = position;
  workarea->size = size;

  if (JS_ToInt32(ctx, &position->x, argv[0]))
    goto fail;

  if (JS_ToInt32(ctx, &position->y, argv[1]))
    goto fail;

  if (JS_ToInt32(ctx, &size->width, argv[2]))
    goto fail;

  if (JS_ToInt32(ctx, &size->height, argv[3]))
    goto fail;

  return glfw_workarea_new_instance(ctx, workarea);
 fail:
  js_free(ctx, workarea);
  return JS_EXCEPTION;
}

void glfw_workarea_finalizer(JSRuntime* rt, JSValue val) {
  GLFWWorkArea* workarea = JS_GetOpaque(val, glfw_workarea_class_id);
  js_free_rt(rt, workarea);
}

// Properties
JSValue glfw_workarea_get_position_or_size(JSContext* ctx, JSValueConst this_val, int magic) {
  GLFWWorkArea* workarea = JS_GetOpaque2(ctx, this_val, glfw_workarea_class_id);
  if (!workarea) return JS_EXCEPTION;

  if (magic == 0)
    return glfw_position_new_instance(ctx, workarea->position);
  else
    return glfw_size_new_instance(ctx, workarea->size);
}

JSValue glfw_workarea_set_position_or_size(JSContext* ctx, JSValueConst this_val, JSValue val, int magic) {
  GLFWWorkArea* workarea = JS_GetOpaque2(ctx, this_val, glfw_workarea_class_id);
  if (!workarea) return JS_EXCEPTION;

  if (magic == 0) {
    workarea->position = JS_GetOpaque2(ctx, val, glfw_position_class_id);
  } else {
    workarea->size = JS_GetOpaque2(ctx, val, glfw_size_class_id);
  }

  return JS_UNDEFINED;
}

// Initialization
JSClassDef glfw_workarea_class_def = {
  "WorkArea",
  .finalizer = glfw_workarea_finalizer,
}; 

const JSCFunctionListEntry glfw_workarea_proto_funcs[] = {
  JS_CGETSET_MAGIC_DEF("position", glfw_workarea_get_position_or_size, glfw_workarea_set_position_or_size, 0),
  JS_CGETSET_MAGIC_DEF("size", glfw_workarea_get_position_or_size, glfw_workarea_set_position_or_size, 1),
};

JSValue glfw_workarea_proto, glfw_workarea_class;

JSValue glfw_workarea_constructor(JSContext* ctx) {
  JSRuntime* rt = JS_GetRuntime(ctx);

  if (!JS_IsRegisteredClass(rt, glfw_workarea_class_id)) {
    JS_NewClassID(&glfw_workarea_class_id);
    JS_NewClass(rt, glfw_workarea_class_id, &glfw_workarea_class_def);

    glfw_workarea_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, glfw_workarea_proto, glfw_workarea_proto_funcs, countof(glfw_workarea_proto_funcs));
    JS_SetClassProto(ctx, glfw_workarea_class_id, glfw_workarea_proto);
    
    glfw_workarea_class = JS_NewCFunction2(ctx, glfw_workarea_ctor, "WorkArea", 2, JS_CFUNC_constructor, 0);
    /* set proto.constructor and ctor.prototype */
    JS_SetConstructor(ctx, glfw_workarea_class, glfw_workarea_proto);
  }

  return glfw_workarea_class;
}

JSValue glfw_workarea_new_instance(JSContext* ctx, GLFWWorkArea* workarea) {
  JSValue ctor = glfw_workarea_constructor(ctx);
  JSValue proto = JS_GetPropertyStr(ctx, ctor, "prototype");
  if (JS_IsException(proto)) {
    JS_FreeValue(ctx, proto);
    return JS_EXCEPTION;
  }

  JSValue obj = JS_NewObjectProtoClass(ctx, proto, glfw_workarea_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(proto)) {
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
  }

  JS_SetOpaque(obj, workarea);
  return obj;
}

int glfw_workarea_init(JSContext* ctx, JSModuleDef* m) {
  JS_SetModuleExport(ctx, m, "WorkArea", glfw_workarea_constructor(ctx));
  return 0;
}

int glfw_workarea_export(JSContext* ctx, JSModuleDef* m) {
  return JS_AddModuleExport(ctx, m, "WorkArea");
}
