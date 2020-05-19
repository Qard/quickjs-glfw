#ifndef GLFW_MODULE_MONITOR
#define GLFW_MODULE_MONITOR 1

JSClassID glfw_monitor_class_id;
int glfw_monitor_init(JSContext* ctx, JSModuleDef* m);
int glfw_monitor_export(JSContext* ctx, JSModuleDef* m);

JSValue glfw_monitor_new_instance(JSContext* ctx, GLFWmonitor* monitor);

#endif
