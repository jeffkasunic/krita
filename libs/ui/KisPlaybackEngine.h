#ifndef KISPLAYBACKENGINE_H
#define KISPLAYBACKENGINE_H

#include <QObject>
#include "KoCanvasObserverBase.h"
#include <kritaui_export.h>

#include <QScopedPointer>
#include <QFileInfo>
#include <boost/optional.hpp>


enum PlaybackMode {
    PLAYBACK_PUSH, // MLT is being pushed to, used during pause and stop state for scrubbing.
    PLAYBACK_PULL // MLT is updating itself, we are getting regular updates from it about when we need to show our next frame.
};

enum SeekOption {
    SEEK_NONE = 0,
    SEEK_PUSH_AUDIO = 1, // Whether we should be pushing audio or not. Used to prevent double-takes on scrubbing.
    SEEK_FORCE_RECACHE = 1 << 1,
    SEEK_FINALIZE = 1 << 2 // Force reload of KisImage to specific frame, ignore caching ability.
};
Q_DECLARE_FLAGS(SeekFlags, SeekOption);
Q_DECLARE_OPERATORS_FOR_FLAGS(SeekFlags);

class KRITAUI_EXPORT KisPlaybackEngine : public QObject, public KoCanvasObserverBase
{
    Q_OBJECT
public:
    explicit KisPlaybackEngine(QObject *parent = nullptr);
    ~KisPlaybackEngine();

Q_SIGNALS:
    void sigChangeActiveCanvasFrame(int p_frame);

public Q_SLOTS:
    void play();
    void pause();
    void playPause();
    void stop();

    void seek(int frameIndex, SeekFlags flags = SEEK_FINALIZE | SEEK_PUSH_AUDIO);
    void previousFrame();
    void nextFrame();
    void previousKeyframe();
    void nextKeyframe();

    /**
     * @brief previousMatchingKeyframe && nextMatchingKeyframe
     * Navigate to the next keyframe that has the same color-label
     * as the current keyframe. Useful to quickly navigate to user-specified
     * 'similar' keyframes. E.g. Contact points in an animation might have
     * a specific color to specify importance and be quickly swapped between.
     */
    void previousMatchingKeyframe();
    void nextMatchingKeyframe();

    /**
     * @brief previousUnfilteredKeyframe && nextUnfilteredKeyframe
     * Navigate to keyframes based on the current onion skin filtration.
     * This lets users easily navigate to the next visible "onion-skinned"
     * keyframe on the active layer.
     */
    void previousUnfilteredKeyframe();
    void nextUnfilteredKeyframe();

    void setPlaybackSpeedPercent(int value);
    void setPlaybackSpeedNormalized(double value);

    void setMute(bool val);
    bool isMute();

protected Q_SLOTS:
    void setCanvas(KoCanvasBase* canvas) override;
    void unsetCanvas() override;

    /**
     * @brief throttledShowFrame
     * @param frame
     *
     * In order to throttle calls from MLT to respect our
     * playback mode, we need to redirect `showFrame` calls
     * to this thread and enforce that we only allow MLT to
     * show frames when we are in PULL mode.
     */
    void throttledShowFrame(const int frame);

    /**
     * @brief setAudioVolume
     * @param volume (normalized)
     */
    void setAudioVolume(qreal volumeNormalized);

private:
    void nextKeyframeWithColor(int color);
    void nextKeyframeWithColor(const QSet<int> &validColors);
    void previousKeyframeWithColor(int color);
    void previousKeyframeWithColor(const QSet<int> &validColors);

    void setupProducer(boost::optional<QFileInfo> file);

private:
    struct Private;
    struct StopAndResume;
    QScopedPointer<Private> m_d;
};

#endif // KISPLAYBACKENGINE_H
