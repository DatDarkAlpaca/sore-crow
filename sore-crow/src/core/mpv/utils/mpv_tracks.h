#pragma once
#include <vector>
#include <QVariantMap>
#include "mpv_node.h"

namespace sore
{
	struct Track
	{
        int64_t id;
        QString type;
        int64_t srcID = 0;
        QString title;
        QString lang;
        bool image;
        bool albumArt;
        bool defaultFlag;
        bool forced;
        bool selected;
        int64_t mainSelection;
        bool external;
        QString externalFilename;
        QString codec;
        int64_t ffIndex;
        QString decoderDesc;
        int64_t demuxWidth;
        int64_t demuxHeight;
        int64_t demuxCropX;
        int64_t demuxCropY;
        int64_t demuxCropWidth;
        int64_t demuxCropHeight;
        int64_t demuxChannelCount;
        QString demuxChannels;
        int64_t demuxSamplerate;
        double demuxFPS;
        int64_t demuxBitrate;
        int64_t demuxRotation;
        double demuxPar;
        int64_t audioChannels;
        double replaygainTrackPeak;
        double replaygainTrackGain;
        double replaygainAlbumPeak;
        double replaygainAlbumGain;
	};

    static inline Track parseTrack(const QVariantMap& trackMap)
    {
        Track track;

        track.id = trackMap.value("id", 0).toInt();
        track.type = trackMap.value("type", "").toString();
        track.srcID = trackMap.value("src-id", 0).toInt();
        track.title = trackMap.value("title", "").toString();
        track.lang = trackMap.value("lang", "").toString();
        track.image = trackMap.value("image", false).toBool();
        track.albumArt = trackMap.value("albumart", false).toBool();
        track.defaultFlag = trackMap.value("default-flag", false).toBool();
        track.forced = trackMap.value("forced", false).toBool();
        track.selected = trackMap.value("selected", false).toBool();
        track.mainSelection = trackMap.value("main-selection", 0).toInt();
        track.external = trackMap.value("external", false).toBool();
        track.externalFilename = trackMap.value("external-filename", "").toString();
        track.codec = trackMap.value("codec", "").toString();
        track.ffIndex = trackMap.value("ff-index", 0).toInt();
        track.decoderDesc = trackMap.value("decoder-desc", "").toString();
        track.demuxWidth = trackMap.value("demux-w", 0).toInt();
        track.demuxHeight = trackMap.value("demux-h", 0).toInt();
        track.demuxCropX = trackMap.value("demux-crop-x", 0).toInt();
        track.demuxCropY = trackMap.value("demux-crop-y", 0).toInt();
        track.demuxCropWidth = trackMap.value("demux-crop-width", 0).toInt();
        track.demuxCropHeight = trackMap.value("demux-crop-height", 0).toInt();
        track.demuxChannelCount = trackMap.value("demux-channel-count", 0).toInt();
        track.demuxChannels = trackMap.value("demux-channels", "").toString();
        track.demuxSamplerate = trackMap.value("demux-crop-height", 0).toInt();
        track.demuxFPS = trackMap.value("demux-fps", 0).toDouble();
        track.demuxBitrate = trackMap.value("demux-fps", 0).toInt();
        track.demuxRotation = trackMap.value("demux-fps", 0).toInt();
        track.demuxPar = trackMap.value("demux-fps", 0).toDouble();
        track.audioChannels = trackMap.value("audio-channels", 0).toInt();
        track.replaygainTrackPeak = trackMap.value("replaygain-track-peak", 0).toDouble();
        track.replaygainTrackGain = trackMap.value("replaygain-track-gain", 0).toDouble();
        track.replaygainAlbumPeak = trackMap.value("replaygain-album-peak", 0).toDouble();
        track.replaygainAlbumGain = trackMap.value("replaygain-album-gain", 0).toDouble();

        return track;
    }

	inline std::vector<Track> getTracks(mpv_handle* handle)
	{
        std::vector<Track> tracks;
        QVariantList trackList = mpv::getProperty(handle, "track-list").toList();
        
        for (const auto& internalTrack : trackList)
        {
            QVariantMap trackMap = internalTrack.toMap();
            tracks.push_back(parseTrack(trackMap));
        }

        return tracks;
	}
}