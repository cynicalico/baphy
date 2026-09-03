/**
 * SPDX-License-Identifier: (WTFPL OR CC0-1.0) AND Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glx.h>

#ifndef GLAD_IMPL_UTIL_C_
#define GLAD_IMPL_UTIL_C_

#ifdef _MSC_VER
#define GLAD_IMPL_UTIL_SSCANF sscanf_s
#else
#define GLAD_IMPL_UTIL_SSCANF sscanf
#endif

#endif /* GLAD_IMPL_UTIL_C_ */

#ifdef __cplusplus
extern "C" {
#endif



int GLAD_GLX_VERSION_1_0 = 0;
int GLAD_GLX_VERSION_1_1 = 0;
int GLAD_GLX_VERSION_1_2 = 0;
int GLAD_GLX_VERSION_1_3 = 0;
int GLAD_GLX_VERSION_1_4 = 0;


static void _pre_call_glx_callback_default(const char *name, GLADapiproc apiproc, int len_args, ...) {
    GLAD_UNUSED(name);
    GLAD_UNUSED(apiproc);
    GLAD_UNUSED(len_args);
}
static void _post_call_glx_callback_default(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...) {
    GLAD_UNUSED(ret);
    GLAD_UNUSED(name);
    GLAD_UNUSED(apiproc);
    GLAD_UNUSED(len_args);
}

static GLADprecallback _pre_call_glx_callback = _pre_call_glx_callback_default;
void gladSetGLXPreCallback(GLADprecallback cb) {
    _pre_call_glx_callback = cb;
}
static GLADpostcallback _post_call_glx_callback = _post_call_glx_callback_default;
void gladSetGLXPostCallback(GLADpostcallback cb) {
    _post_call_glx_callback = cb;
}

PFNGLXCHOOSEFBCONFIGPROC glad_glXChooseFBConfig = NULL;
static GLXFBConfig * GLAD_API_PTR glad_debug_impl_glXChooseFBConfig(Display * dpy, int screen, const int * attrib_list, int * nelements) {
    GLXFBConfig * ret;
    _pre_call_glx_callback("glXChooseFBConfig", (GLADapiproc) glad_glXChooseFBConfig, 4, dpy, screen, attrib_list, nelements);
    ret = glad_glXChooseFBConfig(dpy, screen, attrib_list, nelements);
    _post_call_glx_callback((void*) &ret, "glXChooseFBConfig", (GLADapiproc) glad_glXChooseFBConfig, 4, dpy, screen, attrib_list, nelements);
    return ret;
}
PFNGLXCHOOSEFBCONFIGPROC glad_debug_glXChooseFBConfig = glad_debug_impl_glXChooseFBConfig;
PFNGLXCHOOSEVISUALPROC glad_glXChooseVisual = NULL;
static XVisualInfo * GLAD_API_PTR glad_debug_impl_glXChooseVisual(Display * dpy, int screen, int * attribList) {
    XVisualInfo * ret;
    _pre_call_glx_callback("glXChooseVisual", (GLADapiproc) glad_glXChooseVisual, 3, dpy, screen, attribList);
    ret = glad_glXChooseVisual(dpy, screen, attribList);
    _post_call_glx_callback((void*) &ret, "glXChooseVisual", (GLADapiproc) glad_glXChooseVisual, 3, dpy, screen, attribList);
    return ret;
}
PFNGLXCHOOSEVISUALPROC glad_debug_glXChooseVisual = glad_debug_impl_glXChooseVisual;
PFNGLXCOPYCONTEXTPROC glad_glXCopyContext = NULL;
static void GLAD_API_PTR glad_debug_impl_glXCopyContext(Display * dpy, GLXContext src, GLXContext dst, unsigned long mask) {
    _pre_call_glx_callback("glXCopyContext", (GLADapiproc) glad_glXCopyContext, 4, dpy, src, dst, mask);
    glad_glXCopyContext(dpy, src, dst, mask);
    _post_call_glx_callback(NULL, "glXCopyContext", (GLADapiproc) glad_glXCopyContext, 4, dpy, src, dst, mask);
    
}
PFNGLXCOPYCONTEXTPROC glad_debug_glXCopyContext = glad_debug_impl_glXCopyContext;
PFNGLXCREATECONTEXTPROC glad_glXCreateContext = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXCreateContext(Display * dpy, XVisualInfo * vis, GLXContext shareList, Bool direct) {
    GLXContext ret;
    _pre_call_glx_callback("glXCreateContext", (GLADapiproc) glad_glXCreateContext, 4, dpy, vis, shareList, direct);
    ret = glad_glXCreateContext(dpy, vis, shareList, direct);
    _post_call_glx_callback((void*) &ret, "glXCreateContext", (GLADapiproc) glad_glXCreateContext, 4, dpy, vis, shareList, direct);
    return ret;
}
PFNGLXCREATECONTEXTPROC glad_debug_glXCreateContext = glad_debug_impl_glXCreateContext;
PFNGLXCREATEGLXPIXMAPPROC glad_glXCreateGLXPixmap = NULL;
static GLXPixmap GLAD_API_PTR glad_debug_impl_glXCreateGLXPixmap(Display * dpy, XVisualInfo * visual, Pixmap pixmap) {
    GLXPixmap ret;
    _pre_call_glx_callback("glXCreateGLXPixmap", (GLADapiproc) glad_glXCreateGLXPixmap, 3, dpy, visual, pixmap);
    ret = glad_glXCreateGLXPixmap(dpy, visual, pixmap);
    _post_call_glx_callback((void*) &ret, "glXCreateGLXPixmap", (GLADapiproc) glad_glXCreateGLXPixmap, 3, dpy, visual, pixmap);
    return ret;
}
PFNGLXCREATEGLXPIXMAPPROC glad_debug_glXCreateGLXPixmap = glad_debug_impl_glXCreateGLXPixmap;
PFNGLXCREATENEWCONTEXTPROC glad_glXCreateNewContext = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXCreateNewContext(Display * dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct) {
    GLXContext ret;
    _pre_call_glx_callback("glXCreateNewContext", (GLADapiproc) glad_glXCreateNewContext, 5, dpy, config, render_type, share_list, direct);
    ret = glad_glXCreateNewContext(dpy, config, render_type, share_list, direct);
    _post_call_glx_callback((void*) &ret, "glXCreateNewContext", (GLADapiproc) glad_glXCreateNewContext, 5, dpy, config, render_type, share_list, direct);
    return ret;
}
PFNGLXCREATENEWCONTEXTPROC glad_debug_glXCreateNewContext = glad_debug_impl_glXCreateNewContext;
PFNGLXCREATEPBUFFERPROC glad_glXCreatePbuffer = NULL;
static GLXPbuffer GLAD_API_PTR glad_debug_impl_glXCreatePbuffer(Display * dpy, GLXFBConfig config, const int * attrib_list) {
    GLXPbuffer ret;
    _pre_call_glx_callback("glXCreatePbuffer", (GLADapiproc) glad_glXCreatePbuffer, 3, dpy, config, attrib_list);
    ret = glad_glXCreatePbuffer(dpy, config, attrib_list);
    _post_call_glx_callback((void*) &ret, "glXCreatePbuffer", (GLADapiproc) glad_glXCreatePbuffer, 3, dpy, config, attrib_list);
    return ret;
}
PFNGLXCREATEPBUFFERPROC glad_debug_glXCreatePbuffer = glad_debug_impl_glXCreatePbuffer;
PFNGLXCREATEPIXMAPPROC glad_glXCreatePixmap = NULL;
static GLXPixmap GLAD_API_PTR glad_debug_impl_glXCreatePixmap(Display * dpy, GLXFBConfig config, Pixmap pixmap, const int * attrib_list) {
    GLXPixmap ret;
    _pre_call_glx_callback("glXCreatePixmap", (GLADapiproc) glad_glXCreatePixmap, 4, dpy, config, pixmap, attrib_list);
    ret = glad_glXCreatePixmap(dpy, config, pixmap, attrib_list);
    _post_call_glx_callback((void*) &ret, "glXCreatePixmap", (GLADapiproc) glad_glXCreatePixmap, 4, dpy, config, pixmap, attrib_list);
    return ret;
}
PFNGLXCREATEPIXMAPPROC glad_debug_glXCreatePixmap = glad_debug_impl_glXCreatePixmap;
PFNGLXCREATEWINDOWPROC glad_glXCreateWindow = NULL;
static GLXWindow GLAD_API_PTR glad_debug_impl_glXCreateWindow(Display * dpy, GLXFBConfig config, Window win, const int * attrib_list) {
    GLXWindow ret;
    _pre_call_glx_callback("glXCreateWindow", (GLADapiproc) glad_glXCreateWindow, 4, dpy, config, win, attrib_list);
    ret = glad_glXCreateWindow(dpy, config, win, attrib_list);
    _post_call_glx_callback((void*) &ret, "glXCreateWindow", (GLADapiproc) glad_glXCreateWindow, 4, dpy, config, win, attrib_list);
    return ret;
}
PFNGLXCREATEWINDOWPROC glad_debug_glXCreateWindow = glad_debug_impl_glXCreateWindow;
PFNGLXDESTROYCONTEXTPROC glad_glXDestroyContext = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyContext(Display * dpy, GLXContext ctx) {
    _pre_call_glx_callback("glXDestroyContext", (GLADapiproc) glad_glXDestroyContext, 2, dpy, ctx);
    glad_glXDestroyContext(dpy, ctx);
    _post_call_glx_callback(NULL, "glXDestroyContext", (GLADapiproc) glad_glXDestroyContext, 2, dpy, ctx);
    
}
PFNGLXDESTROYCONTEXTPROC glad_debug_glXDestroyContext = glad_debug_impl_glXDestroyContext;
PFNGLXDESTROYGLXPIXMAPPROC glad_glXDestroyGLXPixmap = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyGLXPixmap(Display * dpy, GLXPixmap pixmap) {
    _pre_call_glx_callback("glXDestroyGLXPixmap", (GLADapiproc) glad_glXDestroyGLXPixmap, 2, dpy, pixmap);
    glad_glXDestroyGLXPixmap(dpy, pixmap);
    _post_call_glx_callback(NULL, "glXDestroyGLXPixmap", (GLADapiproc) glad_glXDestroyGLXPixmap, 2, dpy, pixmap);
    
}
PFNGLXDESTROYGLXPIXMAPPROC glad_debug_glXDestroyGLXPixmap = glad_debug_impl_glXDestroyGLXPixmap;
PFNGLXDESTROYPBUFFERPROC glad_glXDestroyPbuffer = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyPbuffer(Display * dpy, GLXPbuffer pbuf) {
    _pre_call_glx_callback("glXDestroyPbuffer", (GLADapiproc) glad_glXDestroyPbuffer, 2, dpy, pbuf);
    glad_glXDestroyPbuffer(dpy, pbuf);
    _post_call_glx_callback(NULL, "glXDestroyPbuffer", (GLADapiproc) glad_glXDestroyPbuffer, 2, dpy, pbuf);
    
}
PFNGLXDESTROYPBUFFERPROC glad_debug_glXDestroyPbuffer = glad_debug_impl_glXDestroyPbuffer;
PFNGLXDESTROYPIXMAPPROC glad_glXDestroyPixmap = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyPixmap(Display * dpy, GLXPixmap pixmap) {
    _pre_call_glx_callback("glXDestroyPixmap", (GLADapiproc) glad_glXDestroyPixmap, 2, dpy, pixmap);
    glad_glXDestroyPixmap(dpy, pixmap);
    _post_call_glx_callback(NULL, "glXDestroyPixmap", (GLADapiproc) glad_glXDestroyPixmap, 2, dpy, pixmap);
    
}
PFNGLXDESTROYPIXMAPPROC glad_debug_glXDestroyPixmap = glad_debug_impl_glXDestroyPixmap;
PFNGLXDESTROYWINDOWPROC glad_glXDestroyWindow = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyWindow(Display * dpy, GLXWindow win) {
    _pre_call_glx_callback("glXDestroyWindow", (GLADapiproc) glad_glXDestroyWindow, 2, dpy, win);
    glad_glXDestroyWindow(dpy, win);
    _post_call_glx_callback(NULL, "glXDestroyWindow", (GLADapiproc) glad_glXDestroyWindow, 2, dpy, win);
    
}
PFNGLXDESTROYWINDOWPROC glad_debug_glXDestroyWindow = glad_debug_impl_glXDestroyWindow;
PFNGLXGETCLIENTSTRINGPROC glad_glXGetClientString = NULL;
static const char * GLAD_API_PTR glad_debug_impl_glXGetClientString(Display * dpy, int name) {
    const char * ret;
    _pre_call_glx_callback("glXGetClientString", (GLADapiproc) glad_glXGetClientString, 2, dpy, name);
    ret = glad_glXGetClientString(dpy, name);
    _post_call_glx_callback((void*) &ret, "glXGetClientString", (GLADapiproc) glad_glXGetClientString, 2, dpy, name);
    return ret;
}
PFNGLXGETCLIENTSTRINGPROC glad_debug_glXGetClientString = glad_debug_impl_glXGetClientString;
PFNGLXGETCONFIGPROC glad_glXGetConfig = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetConfig(Display * dpy, XVisualInfo * visual, int attrib, int * value) {
    int ret;
    _pre_call_glx_callback("glXGetConfig", (GLADapiproc) glad_glXGetConfig, 4, dpy, visual, attrib, value);
    ret = glad_glXGetConfig(dpy, visual, attrib, value);
    _post_call_glx_callback((void*) &ret, "glXGetConfig", (GLADapiproc) glad_glXGetConfig, 4, dpy, visual, attrib, value);
    return ret;
}
PFNGLXGETCONFIGPROC glad_debug_glXGetConfig = glad_debug_impl_glXGetConfig;
PFNGLXGETCURRENTCONTEXTPROC glad_glXGetCurrentContext = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXGetCurrentContext(void) {
    GLXContext ret;
    _pre_call_glx_callback("glXGetCurrentContext", (GLADapiproc) glad_glXGetCurrentContext, 0);
    ret = glad_glXGetCurrentContext();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentContext", (GLADapiproc) glad_glXGetCurrentContext, 0);
    return ret;
}
PFNGLXGETCURRENTCONTEXTPROC glad_debug_glXGetCurrentContext = glad_debug_impl_glXGetCurrentContext;
PFNGLXGETCURRENTDISPLAYPROC glad_glXGetCurrentDisplay = NULL;
static Display * GLAD_API_PTR glad_debug_impl_glXGetCurrentDisplay(void) {
    Display * ret;
    _pre_call_glx_callback("glXGetCurrentDisplay", (GLADapiproc) glad_glXGetCurrentDisplay, 0);
    ret = glad_glXGetCurrentDisplay();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentDisplay", (GLADapiproc) glad_glXGetCurrentDisplay, 0);
    return ret;
}
PFNGLXGETCURRENTDISPLAYPROC glad_debug_glXGetCurrentDisplay = glad_debug_impl_glXGetCurrentDisplay;
PFNGLXGETCURRENTDRAWABLEPROC glad_glXGetCurrentDrawable = NULL;
static GLXDrawable GLAD_API_PTR glad_debug_impl_glXGetCurrentDrawable(void) {
    GLXDrawable ret;
    _pre_call_glx_callback("glXGetCurrentDrawable", (GLADapiproc) glad_glXGetCurrentDrawable, 0);
    ret = glad_glXGetCurrentDrawable();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentDrawable", (GLADapiproc) glad_glXGetCurrentDrawable, 0);
    return ret;
}
PFNGLXGETCURRENTDRAWABLEPROC glad_debug_glXGetCurrentDrawable = glad_debug_impl_glXGetCurrentDrawable;
PFNGLXGETCURRENTREADDRAWABLEPROC glad_glXGetCurrentReadDrawable = NULL;
static GLXDrawable GLAD_API_PTR glad_debug_impl_glXGetCurrentReadDrawable(void) {
    GLXDrawable ret;
    _pre_call_glx_callback("glXGetCurrentReadDrawable", (GLADapiproc) glad_glXGetCurrentReadDrawable, 0);
    ret = glad_glXGetCurrentReadDrawable();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentReadDrawable", (GLADapiproc) glad_glXGetCurrentReadDrawable, 0);
    return ret;
}
PFNGLXGETCURRENTREADDRAWABLEPROC glad_debug_glXGetCurrentReadDrawable = glad_debug_impl_glXGetCurrentReadDrawable;
PFNGLXGETFBCONFIGATTRIBPROC glad_glXGetFBConfigAttrib = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetFBConfigAttrib(Display * dpy, GLXFBConfig config, int attribute, int * value) {
    int ret;
    _pre_call_glx_callback("glXGetFBConfigAttrib", (GLADapiproc) glad_glXGetFBConfigAttrib, 4, dpy, config, attribute, value);
    ret = glad_glXGetFBConfigAttrib(dpy, config, attribute, value);
    _post_call_glx_callback((void*) &ret, "glXGetFBConfigAttrib", (GLADapiproc) glad_glXGetFBConfigAttrib, 4, dpy, config, attribute, value);
    return ret;
}
PFNGLXGETFBCONFIGATTRIBPROC glad_debug_glXGetFBConfigAttrib = glad_debug_impl_glXGetFBConfigAttrib;
PFNGLXGETFBCONFIGSPROC glad_glXGetFBConfigs = NULL;
static GLXFBConfig * GLAD_API_PTR glad_debug_impl_glXGetFBConfigs(Display * dpy, int screen, int * nelements) {
    GLXFBConfig * ret;
    _pre_call_glx_callback("glXGetFBConfigs", (GLADapiproc) glad_glXGetFBConfigs, 3, dpy, screen, nelements);
    ret = glad_glXGetFBConfigs(dpy, screen, nelements);
    _post_call_glx_callback((void*) &ret, "glXGetFBConfigs", (GLADapiproc) glad_glXGetFBConfigs, 3, dpy, screen, nelements);
    return ret;
}
PFNGLXGETFBCONFIGSPROC glad_debug_glXGetFBConfigs = glad_debug_impl_glXGetFBConfigs;
PFNGLXGETPROCADDRESSPROC glad_glXGetProcAddress = NULL;
static __GLXextFuncPtr GLAD_API_PTR glad_debug_impl_glXGetProcAddress(const GLubyte * procName) {
    __GLXextFuncPtr ret;
    _pre_call_glx_callback("glXGetProcAddress", (GLADapiproc) glad_glXGetProcAddress, 1, procName);
    ret = glad_glXGetProcAddress(procName);
    _post_call_glx_callback((void*) &ret, "glXGetProcAddress", (GLADapiproc) glad_glXGetProcAddress, 1, procName);
    return ret;
}
PFNGLXGETPROCADDRESSPROC glad_debug_glXGetProcAddress = glad_debug_impl_glXGetProcAddress;
PFNGLXGETSELECTEDEVENTPROC glad_glXGetSelectedEvent = NULL;
static void GLAD_API_PTR glad_debug_impl_glXGetSelectedEvent(Display * dpy, GLXDrawable draw, unsigned long * event_mask) {
    _pre_call_glx_callback("glXGetSelectedEvent", (GLADapiproc) glad_glXGetSelectedEvent, 3, dpy, draw, event_mask);
    glad_glXGetSelectedEvent(dpy, draw, event_mask);
    _post_call_glx_callback(NULL, "glXGetSelectedEvent", (GLADapiproc) glad_glXGetSelectedEvent, 3, dpy, draw, event_mask);
    
}
PFNGLXGETSELECTEDEVENTPROC glad_debug_glXGetSelectedEvent = glad_debug_impl_glXGetSelectedEvent;
PFNGLXGETVISUALFROMFBCONFIGPROC glad_glXGetVisualFromFBConfig = NULL;
static XVisualInfo * GLAD_API_PTR glad_debug_impl_glXGetVisualFromFBConfig(Display * dpy, GLXFBConfig config) {
    XVisualInfo * ret;
    _pre_call_glx_callback("glXGetVisualFromFBConfig", (GLADapiproc) glad_glXGetVisualFromFBConfig, 2, dpy, config);
    ret = glad_glXGetVisualFromFBConfig(dpy, config);
    _post_call_glx_callback((void*) &ret, "glXGetVisualFromFBConfig", (GLADapiproc) glad_glXGetVisualFromFBConfig, 2, dpy, config);
    return ret;
}
PFNGLXGETVISUALFROMFBCONFIGPROC glad_debug_glXGetVisualFromFBConfig = glad_debug_impl_glXGetVisualFromFBConfig;
PFNGLXISDIRECTPROC glad_glXIsDirect = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXIsDirect(Display * dpy, GLXContext ctx) {
    Bool ret;
    _pre_call_glx_callback("glXIsDirect", (GLADapiproc) glad_glXIsDirect, 2, dpy, ctx);
    ret = glad_glXIsDirect(dpy, ctx);
    _post_call_glx_callback((void*) &ret, "glXIsDirect", (GLADapiproc) glad_glXIsDirect, 2, dpy, ctx);
    return ret;
}
PFNGLXISDIRECTPROC glad_debug_glXIsDirect = glad_debug_impl_glXIsDirect;
PFNGLXMAKECONTEXTCURRENTPROC glad_glXMakeContextCurrent = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXMakeContextCurrent(Display * dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) {
    Bool ret;
    _pre_call_glx_callback("glXMakeContextCurrent", (GLADapiproc) glad_glXMakeContextCurrent, 4, dpy, draw, read, ctx);
    ret = glad_glXMakeContextCurrent(dpy, draw, read, ctx);
    _post_call_glx_callback((void*) &ret, "glXMakeContextCurrent", (GLADapiproc) glad_glXMakeContextCurrent, 4, dpy, draw, read, ctx);
    return ret;
}
PFNGLXMAKECONTEXTCURRENTPROC glad_debug_glXMakeContextCurrent = glad_debug_impl_glXMakeContextCurrent;
PFNGLXMAKECURRENTPROC glad_glXMakeCurrent = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXMakeCurrent(Display * dpy, GLXDrawable drawable, GLXContext ctx) {
    Bool ret;
    _pre_call_glx_callback("glXMakeCurrent", (GLADapiproc) glad_glXMakeCurrent, 3, dpy, drawable, ctx);
    ret = glad_glXMakeCurrent(dpy, drawable, ctx);
    _post_call_glx_callback((void*) &ret, "glXMakeCurrent", (GLADapiproc) glad_glXMakeCurrent, 3, dpy, drawable, ctx);
    return ret;
}
PFNGLXMAKECURRENTPROC glad_debug_glXMakeCurrent = glad_debug_impl_glXMakeCurrent;
PFNGLXQUERYCONTEXTPROC glad_glXQueryContext = NULL;
static int GLAD_API_PTR glad_debug_impl_glXQueryContext(Display * dpy, GLXContext ctx, int attribute, int * value) {
    int ret;
    _pre_call_glx_callback("glXQueryContext", (GLADapiproc) glad_glXQueryContext, 4, dpy, ctx, attribute, value);
    ret = glad_glXQueryContext(dpy, ctx, attribute, value);
    _post_call_glx_callback((void*) &ret, "glXQueryContext", (GLADapiproc) glad_glXQueryContext, 4, dpy, ctx, attribute, value);
    return ret;
}
PFNGLXQUERYCONTEXTPROC glad_debug_glXQueryContext = glad_debug_impl_glXQueryContext;
PFNGLXQUERYDRAWABLEPROC glad_glXQueryDrawable = NULL;
static void GLAD_API_PTR glad_debug_impl_glXQueryDrawable(Display * dpy, GLXDrawable draw, int attribute, unsigned int * value) {
    _pre_call_glx_callback("glXQueryDrawable", (GLADapiproc) glad_glXQueryDrawable, 4, dpy, draw, attribute, value);
    glad_glXQueryDrawable(dpy, draw, attribute, value);
    _post_call_glx_callback(NULL, "glXQueryDrawable", (GLADapiproc) glad_glXQueryDrawable, 4, dpy, draw, attribute, value);
    
}
PFNGLXQUERYDRAWABLEPROC glad_debug_glXQueryDrawable = glad_debug_impl_glXQueryDrawable;
PFNGLXQUERYEXTENSIONPROC glad_glXQueryExtension = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQueryExtension(Display * dpy, int * errorb, int * event) {
    Bool ret;
    _pre_call_glx_callback("glXQueryExtension", (GLADapiproc) glad_glXQueryExtension, 3, dpy, errorb, event);
    ret = glad_glXQueryExtension(dpy, errorb, event);
    _post_call_glx_callback((void*) &ret, "glXQueryExtension", (GLADapiproc) glad_glXQueryExtension, 3, dpy, errorb, event);
    return ret;
}
PFNGLXQUERYEXTENSIONPROC glad_debug_glXQueryExtension = glad_debug_impl_glXQueryExtension;
PFNGLXQUERYEXTENSIONSSTRINGPROC glad_glXQueryExtensionsString = NULL;
static const char * GLAD_API_PTR glad_debug_impl_glXQueryExtensionsString(Display * dpy, int screen) {
    const char * ret;
    _pre_call_glx_callback("glXQueryExtensionsString", (GLADapiproc) glad_glXQueryExtensionsString, 2, dpy, screen);
    ret = glad_glXQueryExtensionsString(dpy, screen);
    _post_call_glx_callback((void*) &ret, "glXQueryExtensionsString", (GLADapiproc) glad_glXQueryExtensionsString, 2, dpy, screen);
    return ret;
}
PFNGLXQUERYEXTENSIONSSTRINGPROC glad_debug_glXQueryExtensionsString = glad_debug_impl_glXQueryExtensionsString;
PFNGLXQUERYSERVERSTRINGPROC glad_glXQueryServerString = NULL;
static const char * GLAD_API_PTR glad_debug_impl_glXQueryServerString(Display * dpy, int screen, int name) {
    const char * ret;
    _pre_call_glx_callback("glXQueryServerString", (GLADapiproc) glad_glXQueryServerString, 3, dpy, screen, name);
    ret = glad_glXQueryServerString(dpy, screen, name);
    _post_call_glx_callback((void*) &ret, "glXQueryServerString", (GLADapiproc) glad_glXQueryServerString, 3, dpy, screen, name);
    return ret;
}
PFNGLXQUERYSERVERSTRINGPROC glad_debug_glXQueryServerString = glad_debug_impl_glXQueryServerString;
PFNGLXQUERYVERSIONPROC glad_glXQueryVersion = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQueryVersion(Display * dpy, int * maj, int * min) {
    Bool ret;
    _pre_call_glx_callback("glXQueryVersion", (GLADapiproc) glad_glXQueryVersion, 3, dpy, maj, min);
    ret = glad_glXQueryVersion(dpy, maj, min);
    _post_call_glx_callback((void*) &ret, "glXQueryVersion", (GLADapiproc) glad_glXQueryVersion, 3, dpy, maj, min);
    return ret;
}
PFNGLXQUERYVERSIONPROC glad_debug_glXQueryVersion = glad_debug_impl_glXQueryVersion;
PFNGLXSELECTEVENTPROC glad_glXSelectEvent = NULL;
static void GLAD_API_PTR glad_debug_impl_glXSelectEvent(Display * dpy, GLXDrawable draw, unsigned long event_mask) {
    _pre_call_glx_callback("glXSelectEvent", (GLADapiproc) glad_glXSelectEvent, 3, dpy, draw, event_mask);
    glad_glXSelectEvent(dpy, draw, event_mask);
    _post_call_glx_callback(NULL, "glXSelectEvent", (GLADapiproc) glad_glXSelectEvent, 3, dpy, draw, event_mask);
    
}
PFNGLXSELECTEVENTPROC glad_debug_glXSelectEvent = glad_debug_impl_glXSelectEvent;
PFNGLXSWAPBUFFERSPROC glad_glXSwapBuffers = NULL;
static void GLAD_API_PTR glad_debug_impl_glXSwapBuffers(Display * dpy, GLXDrawable drawable) {
    _pre_call_glx_callback("glXSwapBuffers", (GLADapiproc) glad_glXSwapBuffers, 2, dpy, drawable);
    glad_glXSwapBuffers(dpy, drawable);
    _post_call_glx_callback(NULL, "glXSwapBuffers", (GLADapiproc) glad_glXSwapBuffers, 2, dpy, drawable);
    
}
PFNGLXSWAPBUFFERSPROC glad_debug_glXSwapBuffers = glad_debug_impl_glXSwapBuffers;
PFNGLXUSEXFONTPROC glad_glXUseXFont = NULL;
static void GLAD_API_PTR glad_debug_impl_glXUseXFont(Font font, int first, int count, int list) {
    _pre_call_glx_callback("glXUseXFont", (GLADapiproc) glad_glXUseXFont, 4, font, first, count, list);
    glad_glXUseXFont(font, first, count, list);
    _post_call_glx_callback(NULL, "glXUseXFont", (GLADapiproc) glad_glXUseXFont, 4, font, first, count, list);
    
}
PFNGLXUSEXFONTPROC glad_debug_glXUseXFont = glad_debug_impl_glXUseXFont;
PFNGLXWAITGLPROC glad_glXWaitGL = NULL;
static void GLAD_API_PTR glad_debug_impl_glXWaitGL(void) {
    _pre_call_glx_callback("glXWaitGL", (GLADapiproc) glad_glXWaitGL, 0);
    glad_glXWaitGL();
    _post_call_glx_callback(NULL, "glXWaitGL", (GLADapiproc) glad_glXWaitGL, 0);
    
}
PFNGLXWAITGLPROC glad_debug_glXWaitGL = glad_debug_impl_glXWaitGL;
PFNGLXWAITXPROC glad_glXWaitX = NULL;
static void GLAD_API_PTR glad_debug_impl_glXWaitX(void) {
    _pre_call_glx_callback("glXWaitX", (GLADapiproc) glad_glXWaitX, 0);
    glad_glXWaitX();
    _post_call_glx_callback(NULL, "glXWaitX", (GLADapiproc) glad_glXWaitX, 0);
    
}
PFNGLXWAITXPROC glad_debug_glXWaitX = glad_debug_impl_glXWaitX;


static void glad_glx_load_GLX_VERSION_1_0( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_0) return;
    glad_glXChooseVisual = (PFNGLXCHOOSEVISUALPROC) load(userptr, "glXChooseVisual");
    glad_glXCopyContext = (PFNGLXCOPYCONTEXTPROC) load(userptr, "glXCopyContext");
    glad_glXCreateContext = (PFNGLXCREATECONTEXTPROC) load(userptr, "glXCreateContext");
    glad_glXCreateGLXPixmap = (PFNGLXCREATEGLXPIXMAPPROC) load(userptr, "glXCreateGLXPixmap");
    glad_glXDestroyContext = (PFNGLXDESTROYCONTEXTPROC) load(userptr, "glXDestroyContext");
    glad_glXDestroyGLXPixmap = (PFNGLXDESTROYGLXPIXMAPPROC) load(userptr, "glXDestroyGLXPixmap");
    glad_glXGetConfig = (PFNGLXGETCONFIGPROC) load(userptr, "glXGetConfig");
    glad_glXGetCurrentContext = (PFNGLXGETCURRENTCONTEXTPROC) load(userptr, "glXGetCurrentContext");
    glad_glXGetCurrentDrawable = (PFNGLXGETCURRENTDRAWABLEPROC) load(userptr, "glXGetCurrentDrawable");
    glad_glXIsDirect = (PFNGLXISDIRECTPROC) load(userptr, "glXIsDirect");
    glad_glXMakeCurrent = (PFNGLXMAKECURRENTPROC) load(userptr, "glXMakeCurrent");
    glad_glXQueryExtension = (PFNGLXQUERYEXTENSIONPROC) load(userptr, "glXQueryExtension");
    glad_glXQueryVersion = (PFNGLXQUERYVERSIONPROC) load(userptr, "glXQueryVersion");
    glad_glXSwapBuffers = (PFNGLXSWAPBUFFERSPROC) load(userptr, "glXSwapBuffers");
    glad_glXUseXFont = (PFNGLXUSEXFONTPROC) load(userptr, "glXUseXFont");
    glad_glXWaitGL = (PFNGLXWAITGLPROC) load(userptr, "glXWaitGL");
    glad_glXWaitX = (PFNGLXWAITXPROC) load(userptr, "glXWaitX");
}
static void glad_glx_load_GLX_VERSION_1_1( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_1) return;
    glad_glXGetClientString = (PFNGLXGETCLIENTSTRINGPROC) load(userptr, "glXGetClientString");
    glad_glXQueryExtensionsString = (PFNGLXQUERYEXTENSIONSSTRINGPROC) load(userptr, "glXQueryExtensionsString");
    glad_glXQueryServerString = (PFNGLXQUERYSERVERSTRINGPROC) load(userptr, "glXQueryServerString");
}
static void glad_glx_load_GLX_VERSION_1_2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_2) return;
    glad_glXGetCurrentDisplay = (PFNGLXGETCURRENTDISPLAYPROC) load(userptr, "glXGetCurrentDisplay");
}
static void glad_glx_load_GLX_VERSION_1_3( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_3) return;
    glad_glXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC) load(userptr, "glXChooseFBConfig");
    glad_glXCreateNewContext = (PFNGLXCREATENEWCONTEXTPROC) load(userptr, "glXCreateNewContext");
    glad_glXCreatePbuffer = (PFNGLXCREATEPBUFFERPROC) load(userptr, "glXCreatePbuffer");
    glad_glXCreatePixmap = (PFNGLXCREATEPIXMAPPROC) load(userptr, "glXCreatePixmap");
    glad_glXCreateWindow = (PFNGLXCREATEWINDOWPROC) load(userptr, "glXCreateWindow");
    glad_glXDestroyPbuffer = (PFNGLXDESTROYPBUFFERPROC) load(userptr, "glXDestroyPbuffer");
    glad_glXDestroyPixmap = (PFNGLXDESTROYPIXMAPPROC) load(userptr, "glXDestroyPixmap");
    glad_glXDestroyWindow = (PFNGLXDESTROYWINDOWPROC) load(userptr, "glXDestroyWindow");
    glad_glXGetCurrentReadDrawable = (PFNGLXGETCURRENTREADDRAWABLEPROC) load(userptr, "glXGetCurrentReadDrawable");
    glad_glXGetFBConfigAttrib = (PFNGLXGETFBCONFIGATTRIBPROC) load(userptr, "glXGetFBConfigAttrib");
    glad_glXGetFBConfigs = (PFNGLXGETFBCONFIGSPROC) load(userptr, "glXGetFBConfigs");
    glad_glXGetSelectedEvent = (PFNGLXGETSELECTEDEVENTPROC) load(userptr, "glXGetSelectedEvent");
    glad_glXGetVisualFromFBConfig = (PFNGLXGETVISUALFROMFBCONFIGPROC) load(userptr, "glXGetVisualFromFBConfig");
    glad_glXMakeContextCurrent = (PFNGLXMAKECONTEXTCURRENTPROC) load(userptr, "glXMakeContextCurrent");
    glad_glXQueryContext = (PFNGLXQUERYCONTEXTPROC) load(userptr, "glXQueryContext");
    glad_glXQueryDrawable = (PFNGLXQUERYDRAWABLEPROC) load(userptr, "glXQueryDrawable");
    glad_glXSelectEvent = (PFNGLXSELECTEVENTPROC) load(userptr, "glXSelectEvent");
}
static void glad_glx_load_GLX_VERSION_1_4( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_4) return;
    glad_glXGetProcAddress = (PFNGLXGETPROCADDRESSPROC) load(userptr, "glXGetProcAddress");
}



static int glad_glx_has_extension(Display *display, int screen, const char *ext) {
#ifndef GLX_VERSION_1_1
    GLAD_UNUSED(display);
    GLAD_UNUSED(screen);
    GLAD_UNUSED(ext);
#else
    const char *terminator;
    const char *loc;
    const char *extensions;

    if (glXQueryExtensionsString == NULL) {
        return 0;
    }

    extensions = glXQueryExtensionsString(display, screen);

    if(extensions == NULL || ext == NULL) {
        return 0;
    }

    while(1) {
        loc = strstr(extensions, ext);
        if(loc == NULL)
            break;

        terminator = loc + strlen(ext);
        if((loc == extensions || *(loc - 1) == ' ') &&
            (*terminator == ' ' || *terminator == '\0')) {
            return 1;
        }
        extensions = terminator;
    }
#endif

    return 0;
}

static GLADapiproc glad_glx_get_proc_from_userptr(void *userptr, const char* name) {
    return (GLAD_GNUC_EXTENSION (GLADapiproc (*)(const char *name)) userptr)(name);
}

static int glad_glx_find_extensions(Display *display, int screen) {
    GLAD_UNUSED(&glad_glx_has_extension);
    return 1;
}

static int glad_glx_find_core_glx(Display **display, int *screen) {
    int major = 0, minor = 0;
    if(*display == NULL) {
#ifdef GLAD_GLX_NO_X11
        GLAD_UNUSED(screen);
        return 0;
#else
        *display = XOpenDisplay(0);
        if (*display == NULL) {
            return 0;
        }
        *screen = XScreenNumberOfScreen(XDefaultScreenOfDisplay(*display));
#endif
    }
    glXQueryVersion(*display, &major, &minor);
    GLAD_GLX_VERSION_1_0 = (major == 1 && minor >= 0) || major > 1;
    GLAD_GLX_VERSION_1_1 = (major == 1 && minor >= 1) || major > 1;
    GLAD_GLX_VERSION_1_2 = (major == 1 && minor >= 2) || major > 1;
    GLAD_GLX_VERSION_1_3 = (major == 1 && minor >= 3) || major > 1;
    GLAD_GLX_VERSION_1_4 = (major == 1 && minor >= 4) || major > 1;
    return GLAD_MAKE_VERSION(major, minor);
}

int gladLoadGLXUserPtr(Display *display, int screen, GLADuserptrloadfunc load, void *userptr) {
    int version;
    glXQueryVersion = (PFNGLXQUERYVERSIONPROC) load(userptr, "glXQueryVersion");
    if(glXQueryVersion == NULL) return 0;
    version = glad_glx_find_core_glx(&display, &screen);

    glad_glx_load_GLX_VERSION_1_0(load, userptr);
    glad_glx_load_GLX_VERSION_1_1(load, userptr);
    glad_glx_load_GLX_VERSION_1_2(load, userptr);
    glad_glx_load_GLX_VERSION_1_3(load, userptr);
    glad_glx_load_GLX_VERSION_1_4(load, userptr);

    if (!glad_glx_find_extensions(display, screen)) return 0;


    return version;
}

int gladLoadGLX(Display *display, int screen, GLADloadfunc load) {
    return gladLoadGLXUserPtr(display, screen, glad_glx_get_proc_from_userptr, GLAD_GNUC_EXTENSION (void*) load);
}

 
void gladInstallGLXDebug(void) {
    glad_debug_glXChooseFBConfig = glad_debug_impl_glXChooseFBConfig;
    glad_debug_glXChooseVisual = glad_debug_impl_glXChooseVisual;
    glad_debug_glXCopyContext = glad_debug_impl_glXCopyContext;
    glad_debug_glXCreateContext = glad_debug_impl_glXCreateContext;
    glad_debug_glXCreateGLXPixmap = glad_debug_impl_glXCreateGLXPixmap;
    glad_debug_glXCreateNewContext = glad_debug_impl_glXCreateNewContext;
    glad_debug_glXCreatePbuffer = glad_debug_impl_glXCreatePbuffer;
    glad_debug_glXCreatePixmap = glad_debug_impl_glXCreatePixmap;
    glad_debug_glXCreateWindow = glad_debug_impl_glXCreateWindow;
    glad_debug_glXDestroyContext = glad_debug_impl_glXDestroyContext;
    glad_debug_glXDestroyGLXPixmap = glad_debug_impl_glXDestroyGLXPixmap;
    glad_debug_glXDestroyPbuffer = glad_debug_impl_glXDestroyPbuffer;
    glad_debug_glXDestroyPixmap = glad_debug_impl_glXDestroyPixmap;
    glad_debug_glXDestroyWindow = glad_debug_impl_glXDestroyWindow;
    glad_debug_glXGetClientString = glad_debug_impl_glXGetClientString;
    glad_debug_glXGetConfig = glad_debug_impl_glXGetConfig;
    glad_debug_glXGetCurrentContext = glad_debug_impl_glXGetCurrentContext;
    glad_debug_glXGetCurrentDisplay = glad_debug_impl_glXGetCurrentDisplay;
    glad_debug_glXGetCurrentDrawable = glad_debug_impl_glXGetCurrentDrawable;
    glad_debug_glXGetCurrentReadDrawable = glad_debug_impl_glXGetCurrentReadDrawable;
    glad_debug_glXGetFBConfigAttrib = glad_debug_impl_glXGetFBConfigAttrib;
    glad_debug_glXGetFBConfigs = glad_debug_impl_glXGetFBConfigs;
    glad_debug_glXGetProcAddress = glad_debug_impl_glXGetProcAddress;
    glad_debug_glXGetSelectedEvent = glad_debug_impl_glXGetSelectedEvent;
    glad_debug_glXGetVisualFromFBConfig = glad_debug_impl_glXGetVisualFromFBConfig;
    glad_debug_glXIsDirect = glad_debug_impl_glXIsDirect;
    glad_debug_glXMakeContextCurrent = glad_debug_impl_glXMakeContextCurrent;
    glad_debug_glXMakeCurrent = glad_debug_impl_glXMakeCurrent;
    glad_debug_glXQueryContext = glad_debug_impl_glXQueryContext;
    glad_debug_glXQueryDrawable = glad_debug_impl_glXQueryDrawable;
    glad_debug_glXQueryExtension = glad_debug_impl_glXQueryExtension;
    glad_debug_glXQueryExtensionsString = glad_debug_impl_glXQueryExtensionsString;
    glad_debug_glXQueryServerString = glad_debug_impl_glXQueryServerString;
    glad_debug_glXQueryVersion = glad_debug_impl_glXQueryVersion;
    glad_debug_glXSelectEvent = glad_debug_impl_glXSelectEvent;
    glad_debug_glXSwapBuffers = glad_debug_impl_glXSwapBuffers;
    glad_debug_glXUseXFont = glad_debug_impl_glXUseXFont;
    glad_debug_glXWaitGL = glad_debug_impl_glXWaitGL;
    glad_debug_glXWaitX = glad_debug_impl_glXWaitX;
}

void gladUninstallGLXDebug(void) {
    glad_debug_glXChooseFBConfig = glad_glXChooseFBConfig;
    glad_debug_glXChooseVisual = glad_glXChooseVisual;
    glad_debug_glXCopyContext = glad_glXCopyContext;
    glad_debug_glXCreateContext = glad_glXCreateContext;
    glad_debug_glXCreateGLXPixmap = glad_glXCreateGLXPixmap;
    glad_debug_glXCreateNewContext = glad_glXCreateNewContext;
    glad_debug_glXCreatePbuffer = glad_glXCreatePbuffer;
    glad_debug_glXCreatePixmap = glad_glXCreatePixmap;
    glad_debug_glXCreateWindow = glad_glXCreateWindow;
    glad_debug_glXDestroyContext = glad_glXDestroyContext;
    glad_debug_glXDestroyGLXPixmap = glad_glXDestroyGLXPixmap;
    glad_debug_glXDestroyPbuffer = glad_glXDestroyPbuffer;
    glad_debug_glXDestroyPixmap = glad_glXDestroyPixmap;
    glad_debug_glXDestroyWindow = glad_glXDestroyWindow;
    glad_debug_glXGetClientString = glad_glXGetClientString;
    glad_debug_glXGetConfig = glad_glXGetConfig;
    glad_debug_glXGetCurrentContext = glad_glXGetCurrentContext;
    glad_debug_glXGetCurrentDisplay = glad_glXGetCurrentDisplay;
    glad_debug_glXGetCurrentDrawable = glad_glXGetCurrentDrawable;
    glad_debug_glXGetCurrentReadDrawable = glad_glXGetCurrentReadDrawable;
    glad_debug_glXGetFBConfigAttrib = glad_glXGetFBConfigAttrib;
    glad_debug_glXGetFBConfigs = glad_glXGetFBConfigs;
    glad_debug_glXGetProcAddress = glad_glXGetProcAddress;
    glad_debug_glXGetSelectedEvent = glad_glXGetSelectedEvent;
    glad_debug_glXGetVisualFromFBConfig = glad_glXGetVisualFromFBConfig;
    glad_debug_glXIsDirect = glad_glXIsDirect;
    glad_debug_glXMakeContextCurrent = glad_glXMakeContextCurrent;
    glad_debug_glXMakeCurrent = glad_glXMakeCurrent;
    glad_debug_glXQueryContext = glad_glXQueryContext;
    glad_debug_glXQueryDrawable = glad_glXQueryDrawable;
    glad_debug_glXQueryExtension = glad_glXQueryExtension;
    glad_debug_glXQueryExtensionsString = glad_glXQueryExtensionsString;
    glad_debug_glXQueryServerString = glad_glXQueryServerString;
    glad_debug_glXQueryVersion = glad_glXQueryVersion;
    glad_debug_glXSelectEvent = glad_glXSelectEvent;
    glad_debug_glXSwapBuffers = glad_glXSwapBuffers;
    glad_debug_glXUseXFont = glad_glXUseXFont;
    glad_debug_glXWaitGL = glad_glXWaitGL;
    glad_debug_glXWaitX = glad_glXWaitX;
}


#ifdef __cplusplus
}
#endif
