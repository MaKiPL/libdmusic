#include "MusicMessages.h"
#include <dmusic/Structs.h>
#include <dmusic/PlayingContext.h>
#include <assert.h>
#include <cstdlib>

using namespace DirectMusic;

void MusicMessage::changeTempo(PlayingContext& ctx, double tempo) {
    ctx.m_tempo = tempo;
}

std::shared_ptr<InstrumentPlayer> MusicMessage::createInstrument(PlayingContext& ctx, std::uint8_t bank_lo, std::uint8_t bank_hi, std::uint8_t patch, const DirectMusic::DLS::DownloadableSound& dls, float volume, float pan) {
    return ctx.m_instrumentFactory(bank_lo, bank_hi, patch, dls, ctx.m_sampleRate, ctx.m_audioChannels, volume, pan);
}

void MusicMessage::setInstrument(PlayingContext& ctx, std::uint32_t channel, std::shared_ptr<InstrumentPlayer> instr) {
    ctx.m_performanceChannels[channel] = instr;
}

void MusicMessage::setGrooveLevel(PlayingContext& ctx, std::uint8_t level) {
    ctx.m_grooveLevel = level;
}

const std::map<std::uint32_t, std::shared_ptr<InstrumentPlayer>>& MusicMessage::getChannels(PlayingContext& ctx) {
    return ctx.m_performanceChannels;
}

void MusicMessage::changeChord(PlayingContext& ctx, std::uint32_t chord, const std::vector<DMUS_IO_SUBCHORD>& subchords) {
    ctx.m_chord = chord;
    ctx.m_subchords = subchords;
}

void TempoChangeMessage::Execute(PlayingContext& ctx) {
    this->changeTempo(ctx, m_tempo);
}

BandChangeMessage::BandChangeMessage(PlayingContext& ctx, std::uint32_t time, const BandForm& form)
    : MusicMessage(time)
{
    for (const auto& instr : form.getInstruments()) {
        const auto& header = instr.getHeader();
        const auto ref = instr.getReference();
        if (ref != nullptr) {
            std::uint8_t bankHi = (header.dwPatch & 0x00FF0000) >> 0x10;
            std::uint8_t bankLo = (header.dwPatch & 0x0000FF00) >> 0x8;
            std::uint8_t patch = (header.dwPatch & 0x000000FF);
            float volume = header.bVolume / 255.0f;
            float pan = ((float)(header.bPan) - 63.0f) / 64.0f;

            auto dls = ctx.loadInstrumentCollection(ref->getFile());
            
            assert(dls != nullptr);
            instruments[header.dwPChannel] = createInstrument(ctx, bankLo, bankHi, patch, *dls, volume, pan);
        }
    }
}

void BandChangeMessage::Execute(PlayingContext& ctx) {
    for (const auto& kvpair : instruments) {
        setInstrument(ctx, kvpair.first, kvpair.second);
    }
}

void GrooveLevelMessage::Execute(PlayingContext& ctx) {
    if (m_range == 0) {
        setGrooveLevel(ctx, m_level);
    } else {
        std::int8_t offset = (std::rand() % m_range) - (m_range / 2);
        setGrooveLevel(ctx, m_level - offset);
    }
}

void ChordMessage::Execute(PlayingContext& ctx) {
    changeChord(ctx, this->m_chord, this->m_subchords);
}

void NoteOnMessage::Execute(PlayingContext& ctx) {
    const auto& channels = getChannels(ctx);
    assert(channels.find(m_channel) != channels.end());
    if (m_velRange == 0) {
        channels.at(m_channel)->noteOn(m_note, m_vel);
    } else {
        std::int8_t offset = (std::rand() % m_velRange) - (m_velRange / 2);
        channels.at(m_channel)->noteOn(m_note, m_vel - offset);
    }
}

void NoteOffMessage::Execute(PlayingContext& ctx) {
    const auto& channels = getChannels(ctx);
    assert(channels.find(m_channel) != channels.end());
    channels.at(m_channel)->noteOff(m_note, 0);
}