#pragma once
#include "JuceHeader.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cmath>
#include "../../Common/Point3d.h"

// ============================= Design Tokens ==============================
struct TLTheme
{
    static juce::Colour bg() { return juce::Colour::fromRGB(18, 20, 23); }
    static juce::Colour bgAlt() { return juce::Colour::fromRGB(24, 26, 30); }
    static juce::Colour panel() { return juce::Colour::fromRGB(28, 30, 35); }
    static juce::Colour gridMinor() { return juce::Colours::white.withAlpha(0.04f); }
    static juce::Colour gridMedium() { return juce::Colours::white.withAlpha(0.07f); }
    static juce::Colour gridMajor() { return juce::Colours::white.withAlpha(0.10f); }
    static juce::Colour textDim() { return juce::Colours::white.withAlpha(0.65f); }
    static juce::Colour textBright() { return juce::Colours::white.withAlpha(0.90f); }
    static juce::Colour selection() { return juce::Colours::yellow; }
    static juce::Colour marqueeFill() { return juce::Colours::yellow.withAlpha(0.25f); }
    static juce::Colour marqueeEdge() { return juce::Colours::yellow.withAlpha(0.85f); }
    static juce::Colour header() { return juce::Colour::fromRGB(34, 37, 42); }
};

// ============================= Model (ms) =================================
using ms_t = int64_t;

struct Clip
{
    juce::String id;
    ms_t start = 0;      // ms
    ms_t length = 250;    // ms
    juce::Colour colour = juce::Colours::cornflowerblue;
    
    ms_t end() const { return start + length; }
};

struct MovementClip : public Clip
{
    Point3D<double> startPointGroup;
    Point3D<double> endPointGroup;
    bool useStartPoint;
    Array<Point3D<double>> startPointsRel;
    Array<Point3D<double>> endPointsRel;
};

struct MovementLayer
{
    Array<MovementClip> clips;
};

enum class ActionType
{
    None,
    RotationX,
    RotationY,
    RotationZ,
    Stretch
};

enum class TimingType
{
    None,
    AbsoluteTarget,
    RelativeDuringClip,
    AbsolutePerSecond
};

struct ActionDefinition
{
    ActionType action;
    TimingType timing;
    double value;
};

struct ActionClip : public Clip
{
    Array<ActionDefinition> actions;
};

struct ActionLayer
{
    String name;
    Array<ActionClip> clips;
};

struct TimeLineModel2
{
    MovementLayer movement;
    Array<ActionLayer> actions;
};

struct Layer
{
    juce::String name;
    juce::Array<Clip> clips;
};

struct TimelineModel
{
    juce::Array<Layer> layers;

    int findClipAt(int layerIndex, ms_t t) const
    {
        if (!juce::isPositiveAndBelow(layerIndex, layers.size())) return -1;
        const auto& L = layers[(int)layerIndex];
        for (int i = 0; i < L.clips.size(); ++i)
            if (t >= L.clips[i].start && t <= L.clips[i].end())
                return i;
        return -1;
    }
    
    // Im TimelineModel
    std::unique_ptr<juce::XmlElement> toXml() const
    {
        auto xml = std::make_unique<juce::XmlElement>("Timeline");

        for (const auto& L : layers)
        {
            auto* xLayer = new juce::XmlElement("Layer");
            xLayer->setAttribute("name", L.name);

            for (const auto& c : L.clips)
            {
                auto* xClip = new juce::XmlElement("Clip");
                xClip->setAttribute("id",     c.id);
                xClip->setAttribute("start",  juce::String((juce::int64)c.start));
                xClip->setAttribute("length", juce::String((juce::int64)c.length));
                xClip->setAttribute("colour",
                    juce::String::toHexString((juce::uint32) c.colour.getARGB()).paddedLeft('0', 8));
                xLayer->addChildElement(xClip);
            }

            xml->addChildElement(xLayer);
        }

        return xml;
    }

    bool fromXml (const juce::XmlElement& xml)
    {
        if (!xml.hasTagName("Timeline"))
            return false;

        layers.clearQuick();

        for (auto* xLayer = xml.getFirstChildElement(); xLayer != nullptr; xLayer = xLayer->getNextElement())
        {
            if (!xLayer->hasTagName("Layer"))
                continue;

            Layer L;
            L.name = xLayer->getStringAttribute("name");

            for (auto* xClip = xLayer->getFirstChildElement(); xClip != nullptr; xClip = xClip->getNextElement())
            {
                if (!xClip->hasTagName("Clip"))
                    continue;

                Clip c;
                c.id     = xClip->getStringAttribute("id");
                c.start  = (ms_t) xClip->getStringAttribute("start").getLargeIntValue();
                c.length = (ms_t) xClip->getStringAttribute("length").getLargeIntValue();

                const auto colourHex = xClip->getStringAttribute("colour");
                c.colour = colourHex.isNotEmpty()
                         ? juce::Colour((juce::uint32) colourHex.getHexValue32())
                         : juce::Colours::cornflowerblue;

                L.clips.add(c);
            }

            layers.add(L);
        }

        return true;
    }

};

// ============================= Helpers ====================================
static inline juce::String fmtTime(ms_t t)
{
    if (t < 0) t = 0;
    ms_t ms = t % 1000;
    ms_t s = (t / 1000) % 60;
    ms_t m = (t / 60000);
    return juce::String::formatted("%02lld:%02lld.%03lld",
        (long long)m, (long long)s, (long long)ms);
}

struct TimeTransform
{
    double pixelsPerMs = 0.12;   // 120 px / s
    double timeOffset = 0.0;    // linke sichtbare Kante in ms (double für Präzision)

    // Nur Logik/Hit-Tests – Canvas zeichnet in Weltkoordinaten!
    float  toX(ms_t t)     const { return float(((double)t - timeOffset) * pixelsPerMs); }
    ms_t   toTime(float x) const { return (ms_t)std::llround(timeOffset + (double)x / pixelsPerMs); }
};

// ============================= Selection ==================================
struct Selection
{
    struct Ref { int layer = -1; int clip = -1; bool operator==(const Ref& o) const { return layer == o.layer && clip == o.clip; } };
    juce::Array<Ref> items;

    bool empty()   const { return items.isEmpty(); }
    void clear() { items.clear(); }
    bool contains(int l, int c) const { return items.contains({ l,c }); }
    void addUnique(int l, int c) { if (!contains(l, c)) items.add({ l,c }); }
};

// ============================= Header (Ruler) ==============================
class TimelineHeader : public juce::Component
{
public:
    TimeTransform* tx = nullptr;
    float height = 26.0f;
    float gutterWidth = 160.0f;

    // Pixelgenaue horizontale Scrollposition (vom Viewport)
    double scrollX = 0.0;

    void paint(juce::Graphics& g) override
    {
        g.fillAll(TLTheme::header());
        if (!tx) return;

        // linker Streifen (fix, optisch wie der Gutter)
        g.setColour(TLTheme::panel().darker(0.2f));
        g.fillRect(juce::Rectangle<float>(0, 0, gutterWidth, (float)getHeight()));

        const double p = tx->pixelsPerMs;
        const int    W = getWidth();
        const int    H = getHeight();
        const int    tlX0 = (int)gutterWidth;         // Timeline-Start im Header (Pixel)
        const int    tlW = W - tlX0;                 // Timeline-Breite im Header

        // Tick-Schritt in Pixel (Major nahe 110px)
        auto chooseStepMs = [](double pxPerMs, int targetPx)->ms_t {
            static const ms_t steps[] = { 1,2,5,10,20,25,50,100,200,250,500,1000,2000,5000,10000,15000,30000,60000 };
            const double targetMs = targetPx / pxPerMs;
            ms_t best = steps[0]; double bestDiff = 1e18;
            for (ms_t s : steps) { double d = std::abs((double)s - targetMs); if (d < bestDiff) { best = s; bestDiff = d; } }
            return best;
            };
        const ms_t majorMs = chooseStepMs(p, 110);
        const ms_t minorMs = juce::jmax<ms_t>(1, majorMs / 4);

        const double majorPx = (double)majorMs * p;
        const double minorPx = (double)minorMs * p;

        auto firstTickX = [](double scroll, double stepPx)->double
            {
                double r = std::fmod(scroll, stepPx);
                if (r < 0) r += stepPx;
                return stepPx - r; // Abstand bis zum nächsten Tick >= scroll
            };

        // Minor
        g.setColour(TLTheme::gridMinor());
        for (double x = tlX0 + firstTickX(scrollX, minorPx); x <= tlX0 + tlW; x += minorPx)
            g.drawVerticalLine((int)std::llround(x), 0.0f, (float)H);

        // Major + Labels
        g.setColour(TLTheme::gridMajor());
        for (double x = tlX0 + firstTickX(scrollX, majorPx); x <= tlX0 + tlW; x += majorPx)
        {
            g.drawVerticalLine((int)std::llround(x), 0.0f, (float)H);

            // Zeitbeschriftung aus Pixeln berechnen (kein Drift)
            const double xRel = x - tlX0;              // Pixel ab Timeline-Start im Header
            const double tMsD = (scrollX + xRel) / p;  // ms als double
            const ms_t   tMs = (ms_t)std::llround(tMsD);

            g.setColour(TLTheme::textBright()); g.setFont(12.0f);
            g.drawFittedText(fmtTime(tMs), (int)std::llround(x) + 4, 2, 80, H - 4,
                juce::Justification::left, 1);
            g.setColour(TLTheme::gridMajor());
        }
    }
};

// ============================= Gutter (fix links) =========================
class TimelineGutter : public juce::Component
{
public:
    TimelineModel* model = nullptr;
    float rowHeight = 40.0f;
    float rowGap = 5.0f;

    void setScrollY(int y) { scrollY = y; repaint(); }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(TLTheme::panel());
        if (!model) return;

        const int h = getHeight();
        const float startY = (float)scrollY;
        const float endY = startY + (float)h;

        for (int i = 0; i < model->layers.size(); ++i)
        {
            const float rowTop = rowY(i);
            const float rowBottom = rowTop + rowHeight;
            if (rowBottom < startY || rowTop > endY) continue;

            const juce::Rectangle<float> r(0.0f, rowTop - startY, (float)getWidth(), rowHeight);
            g.setColour(TLTheme::panel());
            g.fillRect(r);

            g.setColour(TLTheme::textDim());
            g.drawText(model->layers[i].name, r.reduced(8.0f).toNearestInt(), juce::Justification::left);
        }

        g.setColour(juce::Colours::black.withAlpha(0.5f));
        g.fillRect(getWidth() - 1, 0, 1, getHeight());
    }

    int getContentHeight() const
    {
        if (!model) return 0;
        return (int)(model->layers.size() * (rowHeight + rowGap) + rowGap);
    }

private:
    int scrollY = 0;
    float rowY(int layerIndex) const { return rowGap + (rowHeight + rowGap) * (float)layerIndex; }
};

// ============================= Canvas (im Viewport) =======================
class TimelineCanvas : public juce::Component
{
public:
    struct ClipState
    {
        int layer; int index; ms_t start; ms_t length;
        ms_t end() const { return start + length; }
    };
    struct MoveResizeAction; // fwd

    ms_t playheadMs = -1; // <0 = aus
    bool  loopEnabled = false;
    ms_t  loopStartMs = -1, loopEndMs = -1;
    
    void setPlayheadMs(ms_t t) { playheadMs = t; repaint(); }
    void setLoopRegion (bool enabled, ms_t startMs, ms_t endMs)
    {
        loopEnabled = enabled;
        loopStartMs = startMs;
        loopEndMs   = endMs;
        repaint();
    }
    
    TimelineModel* model = nullptr;
    TimeTransform* tx = nullptr;

    float rowHeight = 40.0f;
    float rowGap = 5.0f;
    ms_t  snap = 10; // 10 ms Snap
    static constexpr ms_t kMinLen = 10;

    // echte Viewport-X-Position (Pixel) für Hit-Test & Zoom
    double viewX = 0.0;

    Selection        selection;
    juce::UndoManager undo;

    // Zoom → gewünschte Viewport-X-Position (Pixel)
    std::function<void(int newViewX)> onViewportChange;
    // Optional: wird bei Skalenänderung immer getriggert (Repaint)
    std::function<void()> onScaleChanged;

    TimelineCanvas()
    {
        setInterceptsMouseClicks(true, true);
        setWantsKeyboardFocus(true);
    }

    TimelineCanvas(TimelineCanvas&&) = delete;
    TimelineCanvas& operator=(TimelineCanvas&&) = delete;
    
    int getContentHeight() const
    {
        if (!model) return 0;
        return (int)(model->layers.size() * (rowHeight + rowGap) + rowGap);
    }

    // ---------- Painting ----------
    void paint(juce::Graphics& g) override
    {
        g.fillAll(TLTheme::bg());
        if (!model || !tx) return;

        paintGrid(g);

        for (int i = 0; i < model->layers.size(); ++i)
        {
            const float y = rowY(i);
            g.setColour((i % 2 == 0) ? TLTheme::bgAlt() : TLTheme::bg());
            g.fillRect(juce::Rectangle<float>(0, y, (float)getWidth(), rowHeight));

            for (int c = 0; c < model->layers[i].clips.size(); ++c)
                paintClip(g, i, c, model->layers[i].clips.getReference(c));
        }

        if (marqueeActive)
        {
            g.setColour(TLTheme::marqueeFill());
            g.fillRect(marquee.getSmallestIntegerContainer());
            g.setColour(TLTheme::marqueeEdge());
            g.drawRect(marquee.getSmallestIntegerContainer(), 1);
        }
        
        if (loopEnabled && loopStartMs >= 0 && loopEndMs > loopStartMs && tx)
        {
            const double p = tx->pixelsPerMs;
            const float x0 = (float)((double)loopStartMs * p);
            const float x1 = (float)((double)loopEndMs   * p);
            const float w  = x1 - x0;

            // halbtransparentes Band
            g.setColour(juce::Colours::yellow.withAlpha(0.12f));
            g.fillRect(juce::Rectangle<float>(x0, 0.0f, w, (float)getHeight()));

            // Kanten hervorheben
            g.setColour(juce::Colours::yellow.withAlpha(0.85f));
            g.drawLine(x0, 0.0f, x0, (float)getHeight(), 1.5f);
            g.drawLine(x1, 0.0f, x1, (float)getHeight(), 1.5f);
        }
        
        if (playheadMs >= 0 && tx)
        {
            const float x = (float)((double)playheadMs * tx->pixelsPerMs); // Weltkoordinate
            g.setColour(juce::Colours::red.withAlpha(0.9f));
            g.drawLine(x, 0.0f, x, (float)getHeight(), 2.0f);
            juce::Path tri; tri.addTriangle(x - 5.f, 0.f, x + 5.f, 0.f, x, 8.f);
            g.fillPath(tri);
        }
    }

    void resized() override
    {
        setSize(getWidth(), getContentHeight());
    }

    // ---------- Mouse ----------
    void mouseDown(const juce::MouseEvent& e) override
    {
        grabKeyboardFocus();
        dragMode = DragMode::None;
        dragStartMouse = e.position;
        dragAccum = 0;

        const int li = layerAt(e.position.y);
        if (li < 0 || !tx || !model)
        {
            if (!e.mods.isAnyModifierKeyDown()) selection.clear();
            repaint(); return;
        }

        const ms_t t = timeAtLocalX(e.position.x); // präzise aus viewX
        const int ci = model->findClipAt(li, t);

        if (e.mods.isShiftDown())
        {
            marqueeActive = true;
            marquee = juce::Rectangle<float>(e.position.x, e.position.y, 1.f, 1.f);
            return;
        }

        if (ci < 0)
        {
            if (!e.mods.isAnyModifierKeyDown()) selection.clear();
            repaint(); return;
        }

        selectedLayer = li;
        selectedClip = ci;

        if (!e.mods.isCommandDown() && !selection.contains(li, ci))
        {
            selection.clear();
            selection.addUnique(li, ci);
        }
        else if (e.mods.isCommandDown())
        {
            if (selection.contains(li, ci))
            {
                for (int k = 0; k < selection.items.size(); ++k)
                    if (selection.items.getReference(k) == Selection::Ref{ li,ci })
                        selection.items.remove(k);
            }
            else selection.addUnique(li, ci);
        }

        // Drag-Anchor (Zeit an der Maus bei mouseDown)
        dragAnchorTime = timeAtLocalX(e.position.x);

        // Originalzustände der aktuell selektierten Clips sichern (Map wird Quelle der Wahrheit)
        storeOriginals();

        const auto& clip = model->layers[li].clips[ci];
        const auto r = clipBounds(li, clip);

        const float edge = 6.0f;
        if (std::abs(e.x - r.getX()) < edge)          dragMode = DragMode::ResizeLeft;
        else if (std::abs(e.x - r.getRight()) < edge) dragMode = DragMode::ResizeRight;
        else                                           dragMode = DragMode::Move;

        repaint();
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (marqueeActive)
        {
            marquee = marquee.getUnion(juce::Rectangle<float>(dragStartMouse, e.position));
            updateMarqueeSelection();
            repaint(); return;
        }

        if (selection.empty() || !tx || !model) return;

        // --- NEU: Safety gegen "Drag ohne mouseDown/storeOriginals" ---
        if (originals.empty())
            storeOriginals();

        const ms_t mouseT = timeAtLocalX(e.position.x);
        const ms_t dT = mouseT - dragAnchorTime;
        dragAccum = dT;

        if (dragMode == DragMode::Move)             applyMove(dT, e.mods.isAltDown());
        else if (dragMode == DragMode::ResizeLeft)  applyResizeLeft(dT, e.mods.isAltDown());
        else if (dragMode == DragMode::ResizeRight) applyResizeRight(dT, e.mods.isAltDown());

        repaint();
    }


    void mouseUp(const juce::MouseEvent&) override
    {
        if (marqueeActive) { marqueeActive = false; repaint(); return; }
        if (selection.empty() || !model) return;

        if (dragMode != DragMode::None && dragAccum != 0)
        {
            const auto before = captureFromMap();          // Snapshots vor Drag
            const auto after = captureCurrent(selection); // nach Drag
            undo.perform(new MoveResizeAction(*model, selection, before, after));
        }
        dragMode = DragMode::None;
        dragAccum = 0;
        originals.clear();
    }

    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override
    {
        if (!tx) return;

        if (e.mods.isCommandDown()) // Zoom um den Mauspunkt
        {
            const double pOld = tx->pixelsPerMs;
            const double factor = (wheel.deltaY > 0 ? 1.12 : 0.89);
            const double pNew = juce::jlimit(0.01, 2.0, pOld * factor);

            const ms_t   mouseT = timeAtLocalX(e.position.x); // aus viewX
            const double newScrollX = (double)mouseT * pNew - (double)e.position.x;

            tx->pixelsPerMs = pNew;
            tx->timeOffset = std::max(0.0, newScrollX / pNew); // double, nicht runden

            if (onScaleChanged) onScaleChanged();
            if (onViewportChange) onViewportChange((int)std::llround(newScrollX));
        }
        else
        {
            // horizontales Scrollen übernimmt der Viewport
        }
    }

    // ---------- Keyboard ----------
    bool keyPressed(const juce::KeyPress& key) override
    {
        if (selection.empty()) return false;

        auto nudge = [&](ms_t delta)
            {
                storeOriginals();
                applyMove(delta, false);
                const auto before = captureFromMap();
                const auto after = captureCurrent(selection);
                undo.perform(new MoveResizeAction(*model, selection, before, after));
                originals.clear();
                repaint();
            };

        if (key == juce::KeyPress(juce::KeyPress::leftKey)) { nudge(-snap); return true; }
        if (key == juce::KeyPress(juce::KeyPress::rightKey)) { nudge(+snap); return true; }

        if (key == juce::KeyPress('z', juce::ModifierKeys::commandModifier, 0)) { undo.undo(); repaint(); return true; }
        if (key == juce::KeyPress('y', juce::ModifierKeys::commandModifier, 0)
            || key == juce::KeyPress('Z', juce::ModifierKeys::shiftModifier | juce::ModifierKeys::commandModifier, 0)) {
            undo.redo(); repaint(); return true;
        }

        if (key == juce::KeyPress(juce::KeyPress::deleteKey))
        {
            std::vector<std::pair<int, int>> toRemove;
            toRemove.reserve((size_t)selection.items.size());
            for (auto r : selection.items) toRemove.emplace_back(r.layer, r.clip);

            std::sort(toRemove.begin(), toRemove.end(),
                [](auto a, auto b) { return a.first != b.first ? a.first<b.first : a.second>b.second; });

            for (auto pr : toRemove) model->layers[pr.first].clips.remove(pr.second);
            selection.clear();
            repaint();
            return true;
        }

        return false;
    }

    // ---------- JSON ----------
    juce::var toJson() const
    {
        juce::DynamicObject* root = new juce::DynamicObject();
        juce::Array<juce::var> jLayers;
        for (const auto& L : model->layers)
        {
            juce::DynamicObject* oL = new juce::DynamicObject();
            oL->setProperty("name", L.name);
            juce::Array<juce::var> jClips;
            for (const auto& c : L.clips)
            {
                juce::DynamicObject* oC = new juce::DynamicObject();
                oC->setProperty("id", c.id);
                oC->setProperty("start", (int64)c.start);
                oC->setProperty("length", (int64)c.length);
                oC->setProperty("colour", (int)c.colour.getARGB());
                jClips.add(juce::var(oC));
            }
            oL->setProperty("clips", juce::var(jClips));
            jLayers.add(juce::var(oL));
        }
        root->setProperty("layers", juce::var(jLayers));
        return juce::var(root);
    }

    void fromJson(const juce::var& v)
    {
        if (!v.isObject()) return;
        model->layers.clear();

        auto* root = v.getDynamicObject();
        auto jLayers = root->getProperty("layers");
        if (jLayers.isArray())
        {
            for (auto jl : *jLayers.getArray())
            {
                Layer L;
                auto* oL = jl.getDynamicObject();
                L.name = oL->getProperty("name").toString();

                auto jClips = oL->getProperty("clips");
                if (jClips.isArray())
                    for (auto jc : *jClips.getArray())
                    {
                        Clip c;
                        auto* oC = jc.getDynamicObject();
                        c.id = oC->getProperty("id").toString();
                        c.start = (ms_t)(int64)oC->getProperty("start");
                        c.length = (ms_t)(int64)oC->getProperty("length");
                        c.colour = juce::Colour((uint32)(int)oC->getProperty("colour"));
                        L.clips.add(c);
                    }
                model->layers.add(L);
            }
        }
        resized();
        repaint();
    }

private:
    enum class DragMode { None, Move, ResizeLeft, ResizeRight };

    DragMode dragMode = DragMode::None;
    juce::Point<float> dragStartMouse{};
    ms_t dragAccum = 0;
    ms_t dragAnchorTime = 0; // Zeit an der Maus bei mouseDown

    int selectedLayer = -1;
    int selectedClip = -1;

    bool marqueeActive = false;
    juce::Rectangle<float> marquee;

    // Originalwerte der selektierten Clips (Key = (layer<<32)|clip)
    std::unordered_map<uint64_t, ClipState> originals;

    static inline uint64_t keyOf(int layer, int clip)
    {
        return ((uint64_t)(uint32_t)layer << 32) | (uint64_t)(uint32_t)clip;
    }

    // ---- Helpers ----
    inline ms_t timeAtLocalX(float px) const
    {
        // Welt-X = viewX + lokales Pixel; Zeit = Welt-X / pixelsPerMs
        return (ms_t)std::llround((viewX + (double)px) / tx->pixelsPerMs);
    }

    void paintClip(juce::Graphics& g, int layerIndex, int clipIndex, const Clip& c)
    {
        auto r = clipBounds(layerIndex, c);
        const bool sel = selection.contains(layerIndex, clipIndex);

        juce::Colour base = c.colour.interpolatedWith(juce::Colours::black, 0.25f);
        g.setGradientFill(juce::ColourGradient(base.brighter(0.2f), r.getTopLeft(),
            base.darker(0.2f), r.getBottomLeft(), false));
        g.fillRoundedRectangle(r, 5.0f);

        const float hw = 6.0f;
        g.setColour(juce::Colours::black.withAlpha(0.35f));
        auto left = r.removeFromLeft(hw);
        auto right = r.removeFromRight(hw);
        g.fillRect(left);
        g.fillRect(right);

        g.setColour(sel ? TLTheme::selection() : juce::Colours::black.withAlpha(0.6f));
        g.drawRoundedRectangle(clipBounds(layerIndex, c), 5.0f, sel ? 2.0f : 1.0f);

        juce::Rectangle<int> bounds = clipBounds(layerIndex, c).reduced(8, 2).toNearestInt();

        // Zwei Zeilen vorbereiten
        juce::String line1 = c.id;                                 // Erste Zeile
        juce::String line2 = fmtTime(c.start) + " - " + fmtTime(c.end()); // Zweite Zeile

        g.setColour(juce::Colours::white);

        // Höhe für jede Zeile aufteilen
        int halfHeight = bounds.getHeight() / 2;
        auto topBounds = bounds.removeFromTop(halfHeight);
        auto bottomBounds = bounds; // Rest

        // Erste Zeile oben, fitted
        g.drawFittedText(line1, topBounds, juce::Justification::topLeft, 1);

        // Zweite Zeile unten, fitted
        g.drawFittedText(line2, bottomBounds, juce::Justification::bottomLeft, 1);
    }

    void paintGrid(juce::Graphics& g)
    {
        // Clip-basiert zeichnen (nur sichtbarer Teil) in Weltkoordinaten
        const auto clip = g.getClipBounds();
        const int x0 = clip.getX();
        const int x1 = clip.getRight();
        const int rh = juce::jmax(1, getHeight());

        const double p = tx->pixelsPerMs;
        const ms_t t0 = (ms_t)std::floor((double)x0 / p);
        const ms_t t1 = (ms_t)std::ceil((double)x1 / p);

        auto drawLines = [&](ms_t step, juce::Colour col)
            {
                const ms_t first = ((t0 + step - 1) / step) * step; // erster Tick >= t0
                g.setColour(col);
                for (ms_t t = first; t < t1; t += step)
                {
                    const int x = (int)std::llround((double)t * p); // WELTKOORDINATE
                    g.drawVerticalLine(x, 0.0f, (float)rh);
                }
            };

        g.setColour(TLTheme::bg()); g.fillAll();
        drawLines(50, TLTheme::gridMinor());
        drawLines(100, TLTheme::gridMedium());
        drawLines(200, TLTheme::gridMajor());
    }

    float rowY(int layerIndex) const { return rowGap + (rowHeight + rowGap) * (float)layerIndex; }

    juce::Rectangle<float> clipBounds(int layerIndex, const Clip& c) const
    {
        const double p = tx->pixelsPerMs;
        const float x = (float)((double)c.start * p);         // WELTKOORDINATE
        const float w = (float)((double)c.length * p);
        return juce::Rectangle<float>(x, rowY(layerIndex) + 2.0f, w, rowHeight - 4.0f);
    }

    int layerAt(float y) const
    {
        if (!model) return -1;
        for (int i = 0; i < model->layers.size(); ++i)
        {
            auto r = juce::Rectangle<float>(0, rowY(i), (float)getWidth(), rowHeight);
            if (r.contains(0.0f, y)) return i;
        }
        return -1;
    }

    static ms_t snapTo(ms_t v, ms_t step) { if (step <= 1) return v; return step * ((v + step / 2) / step); }

    // Liefert nur echte Originalwerte aus 'originals'. Kein Stub!
    inline bool getOriginalStrict (int layer, int clip, ClipState& out) const
    {
        const auto it = originals.find(keyOf(layer, clip));
        if (it == originals.end())
            return false;
        out = it->second;
        return true;
    }


    inline ClipState getOriginal(int layer, int clip) const
    {
        auto it = originals.find(keyOf(layer, clip));
        if (it != originals.end())
            return it->second;

        // Fallback: aktuellen Zustand nehmen (defensiv)
        jassert (model != nullptr);
        if (model != nullptr
            && juce::isPositiveAndBelow(layer, model->layers.size())
            && juce::isPositiveAndBelow(clip,  model->layers[layer].clips.size()))
        {
            const auto& c = model->layers[layer].clips.getReference(clip);
            return { layer, clip, c.start, c.length };
        }

        // Worst case: neutraler Stub, damit es nicht crasht
        return { layer, clip, 0, kMinLen };
    }


    void applyMove(ms_t dT, bool noSnap)
    {
        if (!model || selection.empty())
            return;

        // --- Anchor aus ORIGINALEN bestimmen: bevorzugt der aktiv gezogene Clip
        ms_t anchor = std::numeric_limits<ms_t>::max();
        bool haveAnchor = false;

        if (selection.contains(selectedLayer, selectedClip))
        {
            ClipState oSel;
            if (getOriginalStrict(selectedLayer, selectedClip, oSel))
            {
                anchor = oSel.start;
                haveAnchor = true;
            }
        }

        if (!haveAnchor)
        {
            for (auto s : selection.items)
            {
                ClipState o;
                if (getOriginalStrict(s.layer, s.clip, o))
                {
                    anchor = std::min(anchor, o.start);
                    haveAnchor = true;
                }
            }
        }

        ms_t delta = dT;
        if (!noSnap && haveAnchor && snap > 1)
        {
            const ms_t snapped = snapTo(anchor + dT, snap);
            delta = snapped - anchor;
        }

        // --- Anwenden NUR auf gültige Clips (aus ORIGINALEN), Länge nicht ändern
        for (auto s : selection.items)
        {
            if (!juce::isPositiveAndBelow(s.layer, model->layers.size())) continue;
            auto& layer = model->layers.getReference(s.layer);
            if (!juce::isPositiveAndBelow(s.clip, layer.clips.size()))     continue;

            ClipState o;
            if (!getOriginalStrict(s.layer, s.clip, o))
                continue; // niemals Stub-Werte erfinden

            auto& c = layer.clips.getReference(s.clip);

            const ms_t newStart = std::max<ms_t>(0, o.start + delta);
            c.start  = newStart;
            // Länge exakt beibehalten; nur nach oben clampen, falls Original kaputt war
            c.length = (o.length >= kMinLen ? o.length : kMinLen);
        }
    }



    void applyResizeLeft(ms_t dT, bool noSnap)
    {
        if (!model || selection.empty())
            return;

        // Anchor = linke Kante des gezogenen Clips, sonst kleinstes ORIGINAL-Start
        ms_t anchorStart = std::numeric_limits<ms_t>::max();
        bool haveAnchor = false;

        if (selection.contains(selectedLayer, selectedClip))
        {
            ClipState oSel;
            if (getOriginalStrict(selectedLayer, selectedClip, oSel))
            {
                anchorStart = oSel.start;
                haveAnchor = true;
            }
        }

        if (!haveAnchor)
        {
            for (auto s : selection.items)
            {
                ClipState o;
                if (getOriginalStrict(s.layer, s.clip, o))
                {
                    anchorStart = std::min(anchorStart, o.start);
                    haveAnchor = true;
                }
            }
        }

        ms_t delta = dT;
        if (!noSnap && haveAnchor && snap > 1)
        {
            const ms_t snapped = snapTo(anchorStart + dT, snap);
            delta = snapped - anchorStart;
        }

        for (auto s : selection.items)
        {
            if (!juce::isPositiveAndBelow(s.layer, model->layers.size())) continue;
            auto& layer = model->layers.getReference(s.layer);
            if (!juce::isPositiveAndBelow(s.clip, layer.clips.size()))     continue;

            ClipState o;
            if (!getOriginalStrict(s.layer, s.clip, o))
                continue;

            auto& c = layer.clips.getReference(s.clip);

            ms_t newStart = o.start + delta;
            newStart = juce::jlimit<ms_t>(0, o.end() - kMinLen, newStart);

            c.start  = newStart;
            c.length = std::max<ms_t>(kMinLen, o.end() - newStart);
        }
    }


    void applyResizeRight(ms_t dT, bool noSnap)
    {
        if (!model || selection.empty())
            return;

        // --- Anker bestimmen: bevorzugt der aktiv gezogene Clip, sonst rechtester Endpunkt
        ms_t anchorEnd = std::numeric_limits<ms_t>::min();
        bool haveAnchor = false;

        const bool selectedIsValid =
            juce::isPositiveAndBelow(selectedLayer, model->layers.size()) &&
            juce::isPositiveAndBelow(selectedClip,  model->layers[selectedLayer].clips.size()) &&
            selection.contains(selectedLayer, selectedClip);

        if (selectedIsValid)
        {
            anchorEnd = getOriginal(selectedLayer, selectedClip).end();
            haveAnchor = true;
        }
        else
        {
            for (auto s : selection.items)
            {
                if (!juce::isPositiveAndBelow(s.layer, model->layers.size())) continue;
                const auto& layer = model->layers.getReference(s.layer);
                if (!juce::isPositiveAndBelow(s.clip, layer.clips.size()))   continue;

                const auto o = getOriginal(s.layer, s.clip);
                anchorEnd = std::max(anchorEnd, o.end());
                haveAnchor = true;
            }
        }

        if (!haveAnchor)
            return;

        // --- gemeinsames Delta (gesnapped an den Anker)
        ms_t delta = dT;
        if (!noSnap && snap > 1)
        {
            const ms_t snapped = snapTo(anchorEnd + dT, snap);
            delta = snapped - anchorEnd;
        }

        // --- anwenden (pro Clip clampen)
        for (auto s : selection.items)
        {
            if (!juce::isPositiveAndBelow(s.layer, model->layers.size())) continue;
            auto& layer = model->layers.getReference(s.layer);
            if (!juce::isPositiveAndBelow(s.clip, layer.clips.size()))     continue;

            auto& c = layer.clips.getReference(s.clip);
            const auto o = getOriginal(s.layer, s.clip);

            ms_t newEnd = o.end() + delta;
            newEnd = juce::jmax<ms_t>(o.start + kMinLen, newEnd);

            c.start  = o.start; // Start bleibt wie im Original
            c.length = juce::jmax<ms_t>(kMinLen, newEnd - o.start);
        }
    }


    void storeOriginals()  // nur am Drag-Beginn aufrufen!
    {
        originals.clear();
        if (!model) return;

        for (auto s : selection.items)
        {
            if (!juce::isPositiveAndBelow(s.layer, model->layers.size())) continue;
            auto& layer = model->layers.getReference(s.layer);   // L-Value!
            if (!juce::isPositiveAndBelow(s.clip, layer.clips.size()))     continue;

            const Clip& c = layer.clips.getReference(s.clip);    // L-Value!
            const ms_t lenSafe = juce::jmax<ms_t>(kMinLen, c.length);

            originals.emplace(keyOf(s.layer, s.clip),
                              ClipState{ s.layer, s.clip, c.start, lenSafe });
        }
    }


    juce::Array<ClipState> captureFromMap() const
    {
        juce::Array<ClipState> out;
        out.ensureStorageAllocated((int)originals.size());
        for (const auto& kv : originals) out.add(kv.second);
        return out;
    }

    void updateMarqueeSelection()
    {
        if (!model) return;
        selection.clear();

        for (int li = 0; li < model->layers.size(); ++li)
            for (int ci = 0; ci < model->layers[li].clips.size(); ++ci)
                if (marquee.intersects(clipBounds(li, model->layers[li].clips[ci])))
                    selection.addUnique(li, ci);
    }

    juce::Array<ClipState> captureCurrent(const Selection& sel) const
    {
        juce::Array<ClipState> out;
        if (!model) return out;

        for (auto r : sel.items)
        {
            if (!juce::isPositiveAndBelow(r.layer, model->layers.size())) continue;
            if (!juce::isPositiveAndBelow(r.clip, model->layers[r.layer].clips.size())) continue;
            const auto& c = model->layers[r.layer].clips.getReference(r.clip);
            out.add({ r.layer, r.clip, c.start, c.length });
        }
        return out;
    }

public:
    struct MoveResizeAction : public juce::UndoableAction
    {
        TimelineModel& model;
        Selection sel;
        juce::Array<ClipState> before, after;

        MoveResizeAction(TimelineModel& m, Selection s,
            juce::Array<ClipState> b, juce::Array<ClipState> a)
            : model(m), sel(std::move(s)), before(std::move(b)), after(std::move(a)) {
        }

        bool perform() override { apply(after);  return true; }
        bool undo() override { apply(before); return true; }
        int getSizeInUnits() override { return 64; }

        void apply(const juce::Array<ClipState>& states)
        {
            for (auto st : states)
            {
                auto& c = model.layers[st.layer].clips.getReference(st.index);
                c.start = st.start; c.length = st.length;
            }
        }
    };
};

// ============================= Viewport-Subclass ===========================
class SyncViewport : public juce::Viewport
{
public:
    std::function<void(const juce::Rectangle<int>&)> onVisibleAreaChanged;

private:
    void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea) override
    {
        if (onVisibleAreaChanged) onVisibleAreaChanged(newVisibleArea);
        juce::Viewport::visibleAreaChanged(newVisibleArea);
    }
};

// -------------------- Horizontaler Scroll-Sync --------------------
struct ViewSyncHub
{
    void registerViewport (juce::Viewport* vp)
    {
        if (vp != nullptr) viewports.addIfNotAlreadyThere(vp);
    }

    void broadcastX (int x, juce::Viewport* source)
    {
        if (isBroadcasting) return;
        isBroadcasting = true;
        for (auto* vp : viewports)
            if (vp != source)
                vp->setViewPosition (x, vp->getViewPositionY());
        isBroadcasting = false;
    }

private:
    juce::Array<juce::Viewport*> viewports;
    bool isBroadcasting = false;
};

// ============================= TimelineRow ================================
class TimelineRow : public juce::Component
{
public:
    TimelineRow (TimelineModel& modelRef, TimeTransform& sharedTx, ViewSyncHub& hubRef, int gutterW)
        : tx (sharedTx), hub (hubRef), gutterWidth (gutterW)
    {
        // Gutter & Canvas verdrahten
        gutter.model    = &modelRef;
        gutter.rowHeight = canvas.rowHeight;
        gutter.rowGap    = canvas.rowGap;

        canvas.model = &modelRef;
        canvas.tx    = &tx;

        vp.setViewedComponent (&canvas, false);
        vp.setScrollBarsShown(false /*vertical*/, false /*horizontal*/);
        vp.setScrollOnDragEnabled(false); // Drag-Gesten gehören der Canvas

        // Horizontal-Scroll → Header & andere Rows syncen
        vp.onVisibleAreaChanged = [this](const juce::Rectangle<int>& vis)
        {
            const int x = vis.getX();
            headerScrollX = x;

            // tx.timeOffset global halten
            const double newOffsetMs = (double) x / tx.pixelsPerMs;
            if (newOffsetMs != tx.timeOffset)
                tx.timeOffset = std::max(0.0, newOffsetMs);

            gutter.setScrollY (vis.getY());
            hub.broadcastX (x, &vp);
            repaint();
        };

        hub.registerViewport (&vp);

        addAndMakeVisible (gutter);
        addAndMakeVisible (vp);
    }

    void resized() override
    {
        auto r = getLocalBounds();
        auto gutterArea = r.removeFromLeft (gutterWidth);
        gutter.setBounds (gutterArea);

        vp.setBounds (r);

        const int contentWidth = (int) std::ceil ((computeMaxEndMs() + 1000) * tx.pixelsPerMs);
        canvas.setSize (juce::jmax (r.getWidth(), contentWidth), canvas.getContentHeight());
    }

    // Weiterreichen für globale Aktionen (Playhead/Loop etc.)
    TimelineCanvas& getCanvas() { return canvas; }
    int             getGutterWidth() const { return gutterWidth; }
    int             getContentHeight() const { return canvas.getContentHeight(); }
    int             getHeaderScrollX() const { return headerScrollX; } // nützlich für Header
    juce::Viewport& getViewport()             { return vp; }
    const juce::Viewport& getViewport() const { return vp; }

private:
    ms_t computeMaxEndMs() const
    {
        ms_t m = 0;
        if (!canvas.model) return 0;
        for (const auto& L : canvas.model->layers)
            for (const auto& c : L.clips)
                m = std::max (m, c.end());
        return m;
    }

    TimeTransform&   tx;
    ViewSyncHub&     hub;
    const int        gutterWidth;

    TimelineGutter   gutter;
    TimelineCanvas   canvas;
    SyncViewport     vp;

    int headerScrollX = 0;
};

// ============================= Container ==================================
class TimelineWidgetMS : public juce::Component
{
public:
    struct PlayheadSnapshot
    {
        bool   valid   = false;
        bool   playing = false;
        ms_t   timeMs  = -1;   // <0 => nicht zeichnen
        double bpm     = 120.0;
        
        bool   looping = false;
        ms_t   loopStartMs = -1;
        ms_t   loopEndMs   = -1;
    };

    using PlayheadProvider = std::function<PlayheadSnapshot()>;

    void setPlayheadProvider(PlayheadProvider p) { playheadProvider = std::move(p); }

    // Optional: Auto-Follow (Cursor im Viewport halten)
    void setAutoFollow(bool shouldFollow) { autoFollow = shouldFollow; }

    TimelineWidgetMS(TimelineWidgetMS&&) = delete;
    TimelineWidgetMS& operator=(TimelineWidgetMS&&) = delete;
    
    TimelineWidgetMS()
    {
        // Header konfigurieren (geteilt für alle Canvas)
        header.tx = &tx;
        header.gutterWidth = gutterWidth; // optische Ausrichtung mit Guttern
        addAndMakeVisible(header);
    
        // Vertikal-Viewport für die Rows (falls viele Timelines)
        rowsViewport.setViewedComponent (&rowsContainer, false);
        rowsViewport.setScrollBarsShown (false, true); // nur vertikal sichtbar
        addAndMakeVisible (rowsViewport);
        setSize (1000, 600);
    }

    void resized() override
    {
        auto r = getLocalBounds();
        header.setBounds (r.removeFromTop (headerHeight));
        rowsViewport.setBounds (r);
        
        layoutRowsContainer();
    }

    TimeTransform& getTx() { return tx; }

    void setModels (juce::OwnedArray<TimelineModel>& modelsRef)
    {
        models = &modelsRef;   // nur Referenz speichern
        rebuildRows();
    }
    
    void rebuildRows()
    {
        rows.clear(true);
        if (!models) return;

        for (auto* tm : *models)
        {
            auto row = std::make_unique<TimelineRow>(*tm, tx, syncHub, gutterWidth);
            row->getCanvas().onViewportChange = [this](int newViewX)
            {
                // irgendeine Row hat gescrollt (per Canvas-Zoom): Header nachziehen
                header.scrollX = (double) newViewX;
                header.repaint();
            };
            row->getCanvas().onScaleChanged = [this]
            {
                // bei Scale-Änderung ohne Positionsänderung -> Header neu
                header.repaint();
                repaint();
            };

            rowsContainer.addAndMakeVisible(row.get());
            rows.add(row.release());
        }

        layoutRowsContainer();
        repaint();
    }
    
private:
    TimeTransform  tx;
    TimelineHeader header;
    ViewSyncHub    syncHub;
    juce::OwnedArray<TimelineModel>* models = nullptr;
    
    class RowsContainer : public juce::Component
    {
    public:
        juce::OwnedArray<TimelineRow> rows;
        int interRowGap = 8;
        void resized() override
        {
            auto r = getLocalBounds();
            int y = 0;
            for (auto* row : rows)
            {
                const int h = row->getContentHeight();
                row->setBounds (r.getX(), y, r.getWidth(), h);
                y += h + interRowGap;
            }
        }
        int computeTotalHeight() const
        {
            int total = 0;
            for (auto* row : rows) total += row->getContentHeight() + interRowGap;
            return juce::jmax (0, total - interRowGap);
        }
    } rowsContainer;
    juce::Viewport rowsViewport;
    
    juce::OwnedArray<TimelineRow>& rows { rowsContainer.rows };
    
    const int headerHeight = 28;
    const int gutterWidth = 160;

    void layoutRowsContainer()
    {
        // Breite: Viewport-Innenbreite, Höhe: Summe aller Row-Höhen
        const auto viewArea = rowsViewport.getLocalBounds();
        const int totalH = rowsContainer.computeTotalHeight();

        // Content-Breite abhängig vom maximalen Clip-Ende aller Models
        const ms_t maxEnd = computeGlobalMaxEndMs();
        const int contentW = juce::jmax (viewArea.getWidth(),
                                            (int) std::ceil ((maxEnd + 1000) * tx.pixelsPerMs));
        rowsContainer.setSize (contentW, juce::jmax (totalH, viewArea.getHeight()));
        rowsContainer.resized(); // jetzt die Rows positionieren

        // Header-ScrollX an eine (erste) Row koppeln:
        if (rows.size() > 0)
            header.scrollX = (double) rows[0]->getHeaderScrollX();
        header.repaint();
    }

    ms_t computeGlobalMaxEndMs() const
    {
        ms_t m = 0;
        for (auto* row : rows)
        {
            const auto& canvas = row->getCanvas();
            if (!canvas.model) continue;
            for (const auto& L : canvas.model->layers)
                for (const auto& c : L.clips)
                    m = std::max (m, c.end());
        }
        return m;
    }
    
    PlayheadProvider playheadProvider;
    bool autoFollow = true;

    // ---- Timer integrieren:
    class PlayheadTimer : private juce::Timer
    {
    public:
        PlayheadTimer(TimelineWidgetMS& ownerRef) : owner(ownerRef) { startTimerHz(60); }
        void setEnabled(bool on) { on ? startTimerHz(60) : stopTimer(); }

    private:
        TimelineWidgetMS& owner;
        void timerCallback() override { owner.tickPlayhead(); }
    } playheadTimer { *this };

    // ---- Tick: Provider abfragen, Canvas updaten, optional folgen
    void tickPlayhead()
    {
        if (!playheadProvider) return;

        const auto s = playheadProvider();

        // Alle Canvas updaten
        for (auto* row : rows)
        {
            auto& cv = row->getCanvas();
            cv.setPlayheadMs (s.valid ? s.timeMs : -1);
            cv.setLoopRegion (s.looping && s.loopEndMs > s.loopStartMs,
                                s.loopStartMs, s.loopEndMs);
        }

        if (autoFollow && s.valid && s.timeMs >= 0)
            keepPlayheadVisible (s.timeMs,
                                    s.looping ? std::make_optional(std::pair<ms_t,ms_t>{s.loopStartMs, s.loopEndMs})
                                            : std::nullopt);
        
    }

    void keepPlayheadVisible (ms_t tMs, std::optional<std::pair<ms_t,ms_t>> loop)
    {
        const double p = tx.pixelsPerMs;
        const int xWorld = (int) std::llround((double) tMs * p);

        if (rows.size() == 0) return;

        // use the first row as reference
        auto& refRow = *rows[0];
        auto& refVP  = refRow.getViewport();
        auto  vis    = refVP.getViewArea();
        const int margin  = 40;
        const int canvasW = refRow.getCanvas().getWidth();

        if (loop && loop->second > loop->first)
        {
            const int loopX0 = (int) std::llround((double) loop->first  * p);
            const int loopX1 = (int) std::llround((double) loop->second * p);
            const int loopW  = loopX1 - loopX0;

            if (loopW <= vis.getWidth())
            {
                const int wantX = juce::jlimit(0, std::max(0, canvasW - vis.getWidth()),
                                               loopX0 - margin);
                if (loopX0 < vis.getX() + margin || loopX1 > vis.getRight() - margin)
                    refVP.setViewPosition(wantX, vis.getY());
            }
        }

        // standard follow
        vis = refVP.getViewArea();
        if (xWorld < vis.getX() + margin || xWorld > vis.getRight() - margin)
        {
            const int newX = juce::jlimit(0, std::max(0, canvasW - vis.getWidth()),
                                          xWorld - vis.getWidth() / 2);
            refVP.setViewPosition(newX, vis.getY());
        }

        // sync header/timeOffset
        header.scrollX = (double) refVP.getViewPositionX();
        tx.timeOffset  = std::max(0.0, header.scrollX / tx.pixelsPerMs);
        header.repaint();
    }
};
