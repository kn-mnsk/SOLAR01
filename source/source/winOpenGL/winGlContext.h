
// https://gist.github.com/mmozeiko/6825cb94d393cb4032d250b8e7cc9d14
// important extension functionality used here:
// (4.3) KHR_debug:                     https://www.khronos.org/registry/OpenGL/extensions/KHR/KHR_debug.txt
// (4.5) ARB_direct_state_access:       https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_direct_state_access.txt
// (4.1) ARB_separate_shader_objects:   https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_separate_shader_objects.txt
// (4.2) ARB_shading_language_420pack:  https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_420pack.txt
// (4.3) ARB_explicit_uniform_location: https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_explicit_uniform_location.txt


///////////////////////////////////////////////////////////////////////////////
// Based on the Article of http://www.songho.ca/opengl/gl_mvc.html
// 
// ViewGL.h, ViewGL.cpp
// View component of OpenGL window
//
//////////////////////////////////////////////////////////////////////////////
//  winGlContext.h
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
///////////////////////////////////////////////////////////////////////////////


#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>
#include <thread>
#include "wglext.h"

#include <string>

#include <glad/glad.h>

#include <source/main/resource.h>
#include <mylibrary/tools.h>

namespace Win
{

    #define GL_VERSION_MAJOR 4
    #define GL_VERSION_MINOR 5

    static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;// = NULL;
    static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;// = NULL;
    static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;// = NULL;

    class GlContext
    {
    public:
       // GlContext();
        GlContext(int idchild);
        ~GlContext();

        void  createContext(HWND handle, int colorBits, int depthBits, int stencilBits);  // create OpenGL rendering context
        void  makeGlContextCurrent(thread::id id);
        void  makeGlContextUnCurrent(thread::id id);
        void  glLoader();
        void closeContext(HWND handle);
        void setIdChild(int id) { idChild = id; };
        void setOnInGlContextUse(bool b) { glContextInUse = b; };
        bool isGlContextInUse() { return glContextInUse; };

        static void GetWglFunctions(void);
        static void createDummyContext(HWND& dummy, HDC& dc, HGLRC& rc);
        void createDummyContext2(HWND dummy);
        void deleteDummyContext(HWND& dummy, HDC& dc, HGLRC& rc);
        // compares src string with dstlen characters from dst, returns 1 if they are equal, 0 if not
        static int StringsAreEqual(const char* src, const char* dst, size_t dstlen)
        {
            while (*src && dstlen-- && *dst)
            {
                if (*src++ != *dst++)
                {
                    return 0;
                }
            }

            return (dstlen && *src == *dst) || (!dstlen && *src == 0);
        };

        HDC getDC() const { return hdc; };
        HGLRC getRC() const { return hglrc; };


    protected:

    private:
        //// member functions
        HDC hdc;                                        // handle to device context
        HGLRC hglrc;                                    // handle to OpenGL rendering context
        GLuint glVersionMajor;
        GLuint glVersionMinor;
        int idChild;
        bool glContextInUse{false};
    };

    ///////////////////////////////////////////////////////////////////////////////
    // default ctor
    ///////////////////////////////////////////////////////////////////////////////
    inline GlContext::GlContext(int idchild) :
        hdc(0), hglrc(0), glVersionMajor(GL_VERSION_MAJOR), glVersionMinor(GL_VERSION_MINOR)
    {
        setIdChild(idchild);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // default dtor
    ///////////////////////////////////////////////////////////////////////////////
    inline GlContext::~GlContext()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////////
    inline void GlContext::closeContext(HWND handle)
    {
        if (!hdc || !hglrc)
            return;

        // delete DC and RC
        wglMakeCurrent(0, 0);
        wglDeleteContext(hglrc);
        ReleaseDC(handle, hdc);

        hdc = 0;
        hglrc = 0;

        WIN_LOG("GL Context Successfully to Close: child window id=" << idChild);

    }

    ///////////////////////////////////////////////////////////////////////////////
    // create OpenGL rendering context
    ///////////////////////////////////////////////////////////////////////////////
    inline void  GlContext::createContext(HWND handle, int colorBits, int depthBits, int stencilBits)
    {
        //refer to https://gist.github.com/mmozeiko/ed2ad27f75edf9c26053ce332a1f6647
        GetWglFunctions();  

        hdc = GetDC(handle);
        if (!hdc)
        {
            ERROR_EXIT("Can not get Device Control");
        }

        // set pixel format for OpenGL context

        int attribA[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, true, //GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, true, //GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  true, //GL_TRUE,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,     colorBits, // 32
            WGL_DEPTH_BITS_ARB,     depthBits,
            WGL_STENCIL_BITS_ARB,   stencilBits,

            // uncomment for sRGB framebuffer, from WGL_ARB_framebuffer_sRGB extension
            // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_framebuffer_sRGB.txt
            //WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,

            // uncomment for multisampeld framebuffer, from WGL_ARB_multisample extension
            // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_multisample.txt
            //WGL_SAMPLE_BUFFERS_ARB, 1,
            //WGL_SAMPLES_ARB,        4, // 4x MSAA

            0,
        };

        int format;
        UINT formats;
        if (!wglChoosePixelFormatARB(hdc, attribA, NULL, 1, &format, &formats) || formats == 0)
        {
            ERROR_EXIT(NULL);
        }

        PIXELFORMATDESCRIPTOR pfd{};
        pfd.nSize = sizeof(pfd);
        if (!DescribePixelFormat(hdc, format, sizeof(pfd), &pfd))
        {
            ERROR_EXIT(NULL);
        }

        if (!SetPixelFormat(hdc, format, &pfd))
        {
            ERROR_EXIT(NULL);
        }

        // create modern OpenGL context

        int attribB[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, int(glVersionMajor),
            WGL_CONTEXT_MINOR_VERSION_ARB, int(glVersionMinor), //set highest version for multiple gl contexts
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    #ifndef NDEBUG
            // ask for debug context for non "Release" builds
            // this is so we can enable debug callback
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
    #endif
                0,
        };

        hglrc = wglCreateContextAttribsARB(hdc, NULL, attribB);

        if (!hglrc) {
            ERROR_EXIT(NULL);
        }

       WIN_LOG("GL Context Successfully Created");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // make the rendering context active for this window
    ///////////////////////////////////////////////////////////////////////////////
    inline void  GlContext::makeGlContextCurrent(thread::id id)
    {
        bool result = wglMakeCurrent(hdc, hglrc);
        if (!result)
        {
            ERROR_EXIT("thread Id=" << id << " Child Window(" << idChild << ")");
        }

        WIN_LOG("GL Context Successfully made Current: thread Id=" << id << " Child Window(" << idChild << ")");
    }

    inline void  GlContext::makeGlContextUnCurrent(thread::id id)
    {
        bool result = wglMakeCurrent(NULL, NULL);
        if (!result)
        {
            ERROR_EXIT("thread Id=" << id << " Child Window(" << idChild << ")");
        }

        WIN_LOG("GL Context Successfully made UnCurrentcreated: thread Id=" << id << " Child Window(" << idChild << ")");
    }


    ///////////////////////////////////////////////////////////////////////////////
    // Load OpenGL API with Glad
    ///////////////////////////////////////////////////////////////////////////////
    inline void  GlContext::glLoader() {

        if (!gladLoadGL())
        {
            ERROR_EXIT(NULL);
        }
        else
        {
            WIN_LOG("Glad Successfully Loaded: Version=" << GLVersion.major << "." << GLVersion.minor);
        }
    }

    //refer to https://gist.github.com/mmozeiko/ed2ad27f75edf9c26053ce332a1f6647
    inline void GlContext::GetWglFunctions(void)
    {
        // to get WGL functions we need valid GL context, so create dummy window for dummy GL contetx
        HWND dummy = CreateWindowExW(
            0, L"STATIC", L"DummyWindow", WS_OVERLAPPED,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, NULL, NULL);
        if (!dummy)
        {
            WIN_LOG_ERROR("Dummy Window Failed to be Created");
        }

        HDC dc = GetDC(dummy);
        if (!dc)
        {
            WIN_LOG_ERROR("FAILED  to get device context for dummy window");
        }

        PIXELFORMATDESCRIPTOR desc{};
        ZeroMemory(&desc, sizeof(desc));
        desc.nSize = sizeof(desc);
        desc.nVersion = 1;
        desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        desc.iPixelType = PFD_TYPE_RGBA;
        desc.cColorBits = 32; //24; made same as createContext colorbits: not sure

        int format = ChoosePixelFormat(dc, &desc);
        if (!format)
        {
            ERROR_EXIT(NULL);
        }

        int ok = DescribePixelFormat(dc, format, sizeof(desc), &desc);
        if (!ok)
        {
            WIN_LOG_ERROR("Failed to describe OpenGL pixel format ");
        }

        // reason to create dummy window is that SetPixelFormat can be called only once for the window
        if (!SetPixelFormat(dc, format, &desc))
        {
            ERROR_EXIT(NULL);
        }

        HGLRC rc = wglCreateContext(dc);
        if (!rc)
        {
            WIN_LOG_ERROR("FAILED to create OpenGL context for dummy window ");
        }

        ok = wglMakeCurrent(dc, rc);
        if (!ok)
        {
            WIN_LOG_ERROR("FAILED to make current OpenGL context for dummy window");
        }

        // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_extensions_string.txt
        PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
            (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

        if (!wglGetExtensionsStringARB)
        {
            ERROR_EXIT(NULL);
        }

        const char* ext = wglGetExtensionsStringARB(dc);
        if (!ext)
        {
            WIN_LOG_ERROR("FAILED to get OpenGL WGL extension string");
        }

        const char* start = ext;
        for (;;)
        {
            while (*ext != 0 && *ext != ' ')
            {
                ext++;
            }

            size_t length = ext - start;
            if (StringsAreEqual("WGL_ARB_pixel_format", start, length))
            {
                // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
                wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
            }
            else if (StringsAreEqual("WGL_ARB_create_context", start, length))
            {
                // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
                wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
            }
            else if (StringsAreEqual("WGL_EXT_swap_control", start, length))
            {
                // https://www.khronos.org/registry/OpenGL/extensions/EXT/WGL_EXT_swap_control.txt
                wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
            }

            if (*ext == 0)
            {
                break;
            }

            ext++;
            start = ext;
        }

        if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB || !wglSwapIntervalEXT)
        {
            // inline void ERROR_EXIT((GET_FILE_NAME.c_str(), GET_FUNCTION.c_str(), GET_LINE.c_str());
            ERROR_EXIT(NULL);
        }

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(rc);
        ReleaseDC(dummy, dc);
        DestroyWindow(dummy);

        //Win::log("At line %s, %s of %s: \tProcess Successfully completed", GET_LINE.c_str(), GET_FUNCTION.c_str(), GET_FILE_NAME.c_str()); 
        WIN_LOG("Process Successfully completed");
    }

    inline void GlContext::createDummyContext(HWND &dummy, HDC &dc, HGLRC & rc)
    {
        // to get WGL functions we need valid GL context, so create dummy window for dummy GL contetx
        dummy = CreateWindowExW(
            0, L"STATIC", L"DummyWindow", WS_OVERLAPPED,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, NULL, NULL);
        if (!dummy)
        {
            WIN_LOG_ERROR("Dummy Window Failed to be Created");
        }

        dc = GetDC(dummy);
        if (!dc)
        {
            WIN_LOG_ERROR("FAILED  to get device context for dummy window");
        }

        PIXELFORMATDESCRIPTOR desc{};
        ZeroMemory(&desc, sizeof(desc));
        desc.nSize = sizeof(desc);
        desc.nVersion = 1;
        desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        desc.iPixelType = PFD_TYPE_RGBA;
        desc.cColorBits = 24;

        int format = ChoosePixelFormat(dc, &desc);
        if (!format)
        {
            ERROR_EXIT(NULL);
        }

        int ok = DescribePixelFormat(dc, format, sizeof(desc), &desc);
        if (!ok)
        {
            WIN_LOG_ERROR("Failed to describe OpenGL pixel format ");
        }

        // reason to create dummy window is that SetPixelFormat can be called only once for the window
        if (!SetPixelFormat(dc, format, &desc))
        {
            ERROR_EXIT(NULL);
        }

        rc = wglCreateContext(dc);
        if (!rc)
        {
            WIN_LOG_ERROR("FAILED to create OpenGL context for dummy window ");
        }

        ok = wglMakeCurrent(dc, rc);
        if (!ok)
        {
            WIN_LOG_ERROR("FAILED to make current OpenGL context for dummy window");
        }

        // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_extensions_string.txt
        PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
            (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

        if (!wglGetExtensionsStringARB)
        {
            ERROR_EXIT(NULL);
        }

        const char* ext = wglGetExtensionsStringARB(dc);
        if (!ext)
        {
            WIN_LOG_ERROR("FAILED to get OpenGL WGL extension string");
        }

        const char* start = ext;
        for (;;)
        {
            while (*ext != 0 && *ext != ' ')
            {
                ext++;
            }

            size_t length = ext - start;
            if (StringsAreEqual("WGL_ARB_pixel_format", start, length))
            {
                // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
                wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
            }
            else if (StringsAreEqual("WGL_ARB_create_context", start, length))
            {
                // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
                wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
            }
            else if (StringsAreEqual("WGL_EXT_swap_control", start, length))
            {
                // https://www.khronos.org/registry/OpenGL/extensions/EXT/WGL_EXT_swap_control.txt
                wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
            }

            if (*ext == 0)
            {
                break;
            }

            ext++;
            start = ext;
        }

        if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB || !wglSwapIntervalEXT)
        {
            // inline void ERROR_EXIT((GET_FILE_NAME.c_str(), GET_FUNCTION.c_str(), GET_LINE.c_str());
            ERROR_EXIT(NULL);
        }

      


        //Win::log("At line %s, %s of %s: \tProcess Successfully completed", GET_LINE.c_str(), GET_FUNCTION.c_str(), GET_FILE_NAME.c_str()); 
        WIN_LOG("Dummy Process Successfully completed");
    }

    inline void GlContext::createDummyContext2(HWND dummy)
    {
        // to get WGL functions we need valid GL context, so create dummy window for dummy GL contetx
       /* dummy = CreateWindowExW(
            0, L"STATIC", L"DummyWindow", WS_OVERLAPPED,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, NULL, NULL);
        if (!dummy)
        {
            WIN_LOG_ERROR("Dummy Window Failed to be Created");
        }*/

        hdc = GetDC(dummy);
        if (!hdc)
        {
            WIN_LOG_ERROR("FAILED  to get device context for dummy window");
        }

        PIXELFORMATDESCRIPTOR desc{};
        ZeroMemory(&desc, sizeof(desc));
        desc.nSize = sizeof(desc);
        desc.nVersion = 1;
        desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        desc.iPixelType = PFD_TYPE_RGBA;
        desc.cColorBits = 24;

        int format = ChoosePixelFormat(hdc, &desc);
        if (!format)
        {
            ERROR_EXIT(NULL);
        }

        int ok = DescribePixelFormat(hdc, format, sizeof(desc), &desc);
        if (!ok)
        {
            WIN_LOG_ERROR("Failed to describe OpenGL pixel format ");
        }

        // reason to create dummy window is that SetPixelFormat can be called only once for the window
        if (!SetPixelFormat(hdc, format, &desc))
        {
            ERROR_EXIT(NULL);
        }

        hglrc = wglCreateContext(hdc);
        if (!hglrc)
        {
            WIN_LOG_ERROR("FAILED to create OpenGL context for dummy window ");
        }

        ok = wglMakeCurrent(hdc, hglrc);
        if (!ok)
        {
            WIN_LOG_ERROR("FAILED to make current OpenGL context for dummy window");
        }

        // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_extensions_string.txt
        PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
            (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

        if (!wglGetExtensionsStringARB)
        {
            ERROR_EXIT(NULL);
        }

        const char* ext = wglGetExtensionsStringARB(hdc);
        if (!ext)
        {
            WIN_LOG_ERROR("FAILED to get OpenGL WGL extension string");
        }

        const char* start = ext;
        for (;;)
        {
            while (*ext != 0 && *ext != ' ')
            {
                ext++;
            }

            size_t length = ext - start;
            if (StringsAreEqual("WGL_ARB_pixel_format", start, length))
            {
                // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
                wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
            }
            else if (StringsAreEqual("WGL_ARB_create_context", start, length))
            {
                // https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
                wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
            }
            else if (StringsAreEqual("WGL_EXT_swap_control", start, length))
            {
                // https://www.khronos.org/registry/OpenGL/extensions/EXT/WGL_EXT_swap_control.txt
                wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
            }

            if (*ext == 0)
            {
                break;
            }

            ext++;
            start = ext;
        }

        if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB || !wglSwapIntervalEXT)
        {
            // inline void ERROR_EXIT((GET_FILE_NAME.c_str(), GET_FUNCTION.c_str(), GET_LINE.c_str());
            ERROR_EXIT(NULL);
        }




        //Win::log("At line %s, %s of %s: \tProcess Successfully completed", GET_LINE.c_str(), GET_FUNCTION.c_str(), GET_FILE_NAME.c_str()); 
        WIN_LOG("Dummy2 Process Successfully completed");
    }

    inline void GlContext::deleteDummyContext(HWND& dummy, HDC& dc, HGLRC& rc)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(rc);
        ReleaseDC(dummy, dc);
        DestroyWindow(dummy);
    }

   
}
