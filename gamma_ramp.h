#ifndef GLFW_MODULE_GAMMA_RAMP
#define GLFW_MODULE_GAMMA_RAMP 1

JSClassID glfw_gamma_ramp_class_id;
JSValue glfw_gamma_ramp_new_instance(JSContext* ctx, const GLFWgammaramp* mode);
int glfw_gamma_ramp_init(JSContext* ctx, JSModuleDef* m);
int glfw_gamma_ramp_export(JSContext* ctx, JSModuleDef* m);

#endif
