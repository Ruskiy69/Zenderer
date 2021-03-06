#include "Zenderer/Audio/Sound2D.hpp"

using namespace zen;

using util::zLog;
using util::LogMode;

using sfx::zSound2D;

zSound2D::zSound2D(const void* const owner) : zAudio2D(owner) {}

zSound2D::~zSound2D()
{
    this->UnloadSource();
}

/// @todo Use some standardized WAV loading library.
bool zSound2D::LoadFromFile(const string_t& filename)
{
    ZEN_ASSERT(!filename.empty());

    AL(alGenBuffers(1, &m_AL.buffers[0]));

    // #lazylyfe
    m_AL.buffers[0] = alutCreateBufferFromFile(filename.c_str());

    // This doesn't use the `AL` macro because we need
    // to do some buffer clean up.
    if(m_AL.buffers[0] == AL_NONE)
    {
        m_error_code = alutGetError();
        m_error_str  = alutGetErrorString(m_error_code);
        alDeleteBuffers(1, &m_AL.buffers[0]);

        m_Log   << m_Log.SetMode(LogMode::ZEN_ERROR)
                << m_Log.SetSystem("Audio")
                << "Failed to load '"  << filename << "' ("
                << m_error_str << ")." << zLog::endl;

        return (m_loaded = false);
    }

    m_filename = filename;
    return (m_loaded = true);
}

void zSound2D::Play()
{
    if(!this->IsLoaded())
    {
        m_Log   << m_Log.SetMode(LogMode::ZEN_DEBUG)
                << m_Log.SetSystem("Audio")
                << "No file loaded" << zLog::endl;

        return;
    }

    // Check to see if we are already playing; if so,
    // do nothing. We cannot play from the start since
    // it's a stream.
    if(this->GetAudioState() == AL_PLAYING)
    {
        m_Log   << m_Log.SetMode(LogMode::ZEN_DEBUG)
                << m_Log.SetSystem("Audio")
                << "Already playing '" << m_filename << "'"
                << zLog::endl;

        return;
    }

    this->UnloadSource();
    m_AL.source = zAudioManager::CreateSource();
    if(!m_AL.source) return;

    m_Log   << m_Log.SetMode(LogMode::ZEN_DEBUG)
            << m_Log.SetSystem("Audio")
            << "Playing '" << m_filename << "' ("
            << m_AL.source << ")" << zLog::endl;

    AL(alSourcei(m_AL.source, AL_BUFFER, m_AL.buffers[0]));
    AL(alSourcef(m_AL.source, AL_GAIN, m_AL.volume));
    AL(alSourcePlay(m_AL.source));
}
