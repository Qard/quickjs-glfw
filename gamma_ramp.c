#include "glfw.h"

#include "gamma_ramp.h"

// Constructor/Destructor
JSValue glfw_gamma_ramp_ctor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv) {
  JS_ThrowInternalError(ctx, "VideoMode can not be constructed directly");
  return JS_EXCEPTION;
}

// Properties
JSValue glfw_gamma_ramp_get_prop(JSContext* ctx, JSValueConst this_val, int magic) {
  GLFWgammaramp* gamma_ramp = JS_GetOpaque2(ctx, this_val, glfw_gamma_ramp_class_id);
  if (!gamma_ramp) return JS_EXCEPTION;

  int value;
  switch (magic) {
    case 0: value = (int) gamma_ramp->red; break;
    case 1: value = (int) gamma_ramp->green; break;
    case 2: value = (int) gamma_ramp->blue; break;
    case 3: value = gamma_ramp->size; break;
  }

  return JS_NewInt32(ctx, value);
}

// Initialization
JSClassDef glfw_gamma_ramp_class_def = {
  "GammaRamp",
}; 

const JSCFunctionListEntry glfw_gamma_ramp_proto_funcs[] = {
  JS_CGETSET_MAGIC_DEF("red", glfw_gamma_ramp_get_prop, NULL, 0),
  JS_CGETSET_MAGIC_DEF("green", glfw_gamma_ramp_get_prop, NULL, 1),
  JS_CGETSET_MAGIC_DEF("blue", glfw_gamma_ramp_get_prop, NULL, 2),
  JS_CGETSET_MAGIC_DEF("size", glfw_gamma_ramp_get_prop, NULL, 3),
};

JSValue glfw_gamma_ramp_proto, glfw_gamma_ramp_class;

JSValue glfw_gamma_ramp_constructor(JSContext* ctx) {
  JSRuntime* rt = JS_GetRuntime(ctx);

  if (!JS_IsRegisteredClass(rt, glfw_gamma_ramp_class_id)) {
    JS_NewClassID(&glfw_gamma_ramp_class_id);
    JS_NewClass(JS_GetRuntime(ctx), glfw_gamma_ramp_class_id, &glfw_gamma_ramp_class_def);

    glfw_gamma_ramp_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, glfw_gamma_ramp_proto, glfw_gamma_ramp_proto_funcs, countof(glfw_gamma_ramp_proto_funcs));
    JS_SetClassProto(ctx, glfw_gamma_ramp_class_id, glfw_gamma_ramp_proto);

    glfw_gamma_ramp_class = JS_NewCFunction2(ctx, glfw_gamma_ramp_ctor, "GammaRamp", 2, JS_CFUNC_constructor, 0);
    /* set proto.constructor and ctor.prototype */
    JS_SetConstructor(ctx, glfw_gamma_ramp_class, glfw_gamma_ramp_proto);
  }

  return glfw_gamma_ramp_class;
}

JSValue glfw_gamma_ramp_new_instance(JSContext* ctx, const GLFWgammaramp* gamma_ramp) {
  JSValue ctor = glfw_gamma_ramp_constructor(ctx);

  JSValue proto = JS_GetPropertyStr(ctx, ctor, "prototype");
  if (JS_IsException(proto)) {
    JS_FreeValue(ctx, proto);
    return JS_EXCEPTION;
  }

  JSValue obj = JS_NewObjectProtoClass(ctx, proto, glfw_gamma_ramp_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(proto)) {
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
  }

  JS_SetOpaque(obj, (void*) gamma_ramp);
  return obj;
}

int glfw_gamma_ramp_init(JSContext* ctx, JSModuleDef* m) {
  JS_SetModuleExport(ctx, m, "GammaRamp", glfw_gamma_ramp_constructor(ctx));
  return 0;
}

int glfw_gamma_ramp_export(JSContext* ctx, JSModuleDef* m) {
  return JS_AddModuleExport(ctx, m, "GammaRamp");
}
