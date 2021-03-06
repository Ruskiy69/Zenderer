#include "Zenderer/Graphics/RenderTarget.hpp"

using namespace zen;

using util::zLog;
using util::LogMode;
using gfx::zRenderTarget;

zRenderTarget::zRenderTarget(const math::rect_t& Dimensions) :
    zGLSubsystem("RenderTarget"), m_Log(zLog::GetEngineLog()),
    m_Viewport(Dimensions.w, Dimensions.h), m_texture(0),
    m_fbo(0), m_rbos(nullptr), m_rbo_count(0)
{
}

zRenderTarget::zRenderTarget(const uint16_t w, const uint16_t h) :
    zGLSubsystem("RenderTarget"), m_Log(zLog::GetEngineLog()),
    m_Viewport(w, h), m_texture(0), m_fbo(0), m_rbos(nullptr), m_rbo_count(0)
{
}

zRenderTarget::~zRenderTarget()
{
    this->Destroy();
    if(m_rbos != nullptr) delete[] m_rbos;
}

bool zRenderTarget::Init()
{
    if(m_init) this->Destroy();

    // Store current render target's viewport.
    GLint view[4];
    GL(glGetIntegerv(GL_VIEWPORT, view));
    m_OldViewport.x = view[2];
    m_OldViewport.y = view[3];

    // Create frame buffer.
    GL(glGenFramebuffers(1, &m_fbo));
    GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

    // Create render target texture.
    GL(glGenTextures(1, &m_texture));
    GL(glBindTexture(GL_TEXTURE_2D, m_texture));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Viewport.x,
            m_Viewport.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

    // Attach texture to frame buffer.
    GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, m_texture, 0));

    // Check status.
    GLuint status = GL(glCheckFramebufferStatus(GL_FRAMEBUFFER));

    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        m_error_str = "The frame buffer set up did not complete.";
        m_Log   << m_Log.SetMode(LogMode::ZEN_ERROR)
                << m_Log.SetSystem("RenderTarget") << m_error_str
                << " Error code: " << status << zLog::endl;
    }

    // Unbind our things.
    GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL(glBindTexture(GL_TEXTURE_2D, 0));

    m_Main = gfxcore::zRenderer::s_ProjMatrix;
    m_ProjMatrix = math::matrix4x4_t::Projection2D(
        m_Viewport.x, m_Viewport.y, 16, 1);

    return m_init = (status == GL_FRAMEBUFFER_COMPLETE);
}

bool zRenderTarget::Destroy()
{
    if(!m_init) return false;

    m_init = false;
    this->Unbind();
    GL(glDeleteFramebuffers(1, &m_fbo));
    GL(glDeleteTextures(1, &m_texture));
    if(m_rbo_count > 0) GL(glDeleteRenderbuffers(m_rbo_count, m_rbos));
    m_rbo_count = 0;
    delete[] m_rbos;
    m_rbos = nullptr;
    return true;
}

bool zRenderTarget::Bind() const
{
    // Bind the framebuffer and set our view port.
    GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    GL(glViewport(0, 0, m_Viewport.x, m_Viewport.y));
    gfxcore::zRenderer::s_ProjMatrix = m_ProjMatrix;

    return true;
}

bool zRenderTarget::Unbind() const
{
    // Unbind the framebuffer and reset the view port.
    gfxcore::zRenderer::s_ProjMatrix = m_Main;
    GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL(glViewport(0, 0, m_OldViewport.x, m_OldViewport.y));
    return true;
}

bool zRenderTarget::BindTexture() const
{
    if(m_texture == 0) return false;
    return gfxcore::zRenderer::EnableTexture(m_texture);
}

bool zRenderTarget::Clear(const color4f_t C)
{
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL(glClearColor(C.r, C.g, C.b, C.a));

    return true;
}

bool zRenderTarget::AttachDepthBuffer()
{
    // Create the depth buffer.
    if(!this->Bind()) return false;

    GLuint rb = 0;
    GL(glGenRenderbuffers(1, &rb));
    GL(glBindRenderbuffer(GL_RENDERBUFFER, rb));
    GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
            m_Viewport.x, m_Viewport.y));
    GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER,  GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, rb));

    // Check status.
    GLuint status = GL(glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        m_error_str = "The depth buffer could not be attached.";
        m_Log   << m_Log.SetMode(LogMode::ZEN_ERROR)
                << m_Log.SetSystem("RenderTarget") << m_error_str
                << " Error code: " << status << zLog::endl;
    }
    else
    {
        // Expand array
        GLuint* tmp = new GLuint[m_rbo_count + 1];
        if(m_rbos != nullptr)
        {
            memcpy(tmp, m_rbos, m_rbo_count * sizeof(GLuint));
            delete[] m_rbos;
        }
        m_rbos = tmp;

        // Add to internal render buffer array.
        m_rbos[m_rbo_count++] = rb;
    }

    GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    return (this->Unbind() && status == GL_FRAMEBUFFER_COMPLETE);
}

GLuint zRenderTarget::GetObjectHandle() const
{
    return m_fbo;
}

GLuint zRenderTarget::GetTexture() const
{
    return m_texture;
}
