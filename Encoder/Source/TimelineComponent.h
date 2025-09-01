#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_core/juce_core.h>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cmath>

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

        // robust: dT aus Maus-Zeit, nicht aus dx
        const ms_t mouseT = timeAtLocalX(e.position.x);
        const ms_t dT = mouseT - dragAnchorTime;
        dragAccum = dT;

        if (dragMode == DragMode::Move)        applyMove(dT, e.mods.isAltDown());
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

        juce::String label = c.id + "  " + fmtTime(c.start) + " - " + fmtTime(c.end());
        g.setColour(juce::Colours::white);
        g.drawFittedText(label, clipBounds(layerIndex, c).reduced(8).toNearestInt(),
            juce::Justification::centredLeft, 1);
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

    inline ClipState getOriginal(int layer, int clip) const
    {
        auto it = originals.find(keyOf(layer, clip));
        jassert(it != originals.end()); // MUSS existieren – setzten wir in storeOriginals()
        return it->second;
    }

    void applyMove(ms_t dT, bool noSnap)
    {
        for (auto s : selection.items)
        {
            auto& c = model->layers[s.layer].clips.getReference(s.clip);
            const auto o = getOriginal(s.layer, s.clip);

            ms_t target = o.start + dT;
            if (!noSnap) target = snapTo(target, snap);
            c.start = juce::jmax<ms_t>(0, target);
            c.length = juce::jmax<ms_t>(kMinLen, o.length); // Länge bleibt, Safety
        }
    }

    void applyResizeLeft(ms_t dT, bool noSnap)
    {
        for (auto s : selection.items)
        {
            auto& c = model->layers[s.layer].clips.getReference(s.clip);
            const auto o = getOriginal(s.layer, s.clip);

            ms_t newStart = o.start + dT;
            if (!noSnap) newStart = snapTo(newStart, snap);
            newStart = juce::jlimit<ms_t>(0, o.end() - kMinLen, newStart);

            c.start = newStart;
            c.length = juce::jmax<ms_t>(kMinLen, o.end() - newStart);
        }
    }

    void applyResizeRight(ms_t dT, bool noSnap)
    {
        for (auto s : selection.items)
        {
            auto& c = model->layers[s.layer].clips.getReference(s.clip);
            const auto o = getOriginal(s.layer, s.clip);

            ms_t newEnd = o.end() + dT;
            if (!noSnap) newEnd = snapTo(newEnd, snap);
            newEnd = juce::jmax<ms_t>(o.start + kMinLen, newEnd);

            c.start = o.start;                    // Start fix wie im Original
            c.length = juce::jmax<ms_t>(kMinLen, newEnd - o.start);
        }
    }

    void storeOriginals()
    {
        originals.clear();
        for (auto s : selection.items)
        {
            const auto& c = model->layers[s.layer].clips.getReference(s.clip);
            originals.emplace(keyOf(s.layer, s.clip), ClipState{ s.layer, s.clip, c.start, c.length });
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

// ============================= Container ==================================
class TimelineWidgetMS : public juce::Component
{
public:
    TimelineWidgetMS()
    {
        // Demo-Daten
        model.layers.add({ "Drums", { Clip{ "Kick",   0,    120, juce::Colours::orange },
                                      Clip{ "Snare",  400,  100, juce::Colours::orangered },
                                      Clip{ "Hat",    200,   50, juce::Colours::goldenrod } } });
        model.layers.add({ "Bass",  { Clip{ "Bass A", 250,  800, juce::Colours::mediumseagreen } } });
        model.layers.add({ "Pads",  { Clip{ "Pad",   1200, 1500, juce::Colours::slateblue } } });

        header.tx = &tx;
        header.gutterWidth = gutterWidth;

        gutter.model = &model;
        gutter.rowHeight = canvas.rowHeight;
        gutter.rowGap = canvas.rowGap;

        canvas.model = &model;
        canvas.tx = &tx;

        addAndMakeVisible(header);
        addAndMakeVisible(gutter);
        viewport.setViewedComponent(&canvas, false);
        addAndMakeVisible(viewport);

        viewport.setScrollBarsShown(true, true);
        viewport.setScrollOnDragEnabled(false); // WICHTIG: Canvas-Drag nicht vom Viewport abfangen lassen!

        // Zoom aus Canvas → Viewport positionieren (pixelgenau)
        canvas.onViewportChange = [this](int newViewX)
            {
                viewport.setViewPosition(newViewX, viewport.getViewPositionY());
                // Falls Position unverändert bleibt, trotzdem frisch zeichnen:
                header.scrollX = (double)viewport.getViewPositionX();
                header.repaint();
                canvas.repaint();
            };

        // bei Scale-Änderung (Ctrl+Wheel) ohne Positionsänderung -> Repaint forcieren
        canvas.onScaleChanged = [this]
            {
                header.scrollX = (double)viewport.getViewPositionX();
                header.repaint();
                canvas.repaint();
            };

        // Scrollbar/Trackpad → Header pixelgenau, Canvas.viewX und tx.timeOffset ableiten
        viewport.onVisibleAreaChanged = [this](const juce::Rectangle<int>& vis)
            {
                const int x = vis.getX();
                const int y = vis.getY();

                header.scrollX = (double)x;
                canvas.viewX = (double)x;

                const double newOffsetMs = (double)x / tx.pixelsPerMs; // präzise, ohne Rundung
                if (newOffsetMs != tx.timeOffset)
                    tx.timeOffset = std::max(0.0, newOffsetMs);

                gutter.setScrollY(y);
                header.repaint();
                canvas.repaint();
            };

        setSize(1000, 480);
    }

    void resized() override
    {
        auto r = getLocalBounds();

        auto headerArea = r.removeFromTop(headerHeight);
        header.setBounds(headerArea);

        auto gutterArea = r.removeFromLeft(gutterWidth);
        gutter.setBounds(gutterArea);

        viewport.setBounds(r);

        const ms_t maxEnd = computeMaxEndMs();
        const int contentWidth = juce::jmax(r.getWidth(),
            (int)std::ceil((maxEnd + 1000) * tx.pixelsPerMs));
        canvas.setSize(contentWidth, canvas.getContentHeight());
    }

    TimelineModel& getModel() { return model; }
    TimelineCanvas& getCanvas() { return canvas; }
    TimeTransform& getTx() { return tx; }

private:
    TimelineModel  model;
    TimeTransform  tx;
    TimelineHeader header;
    TimelineGutter gutter;
    TimelineCanvas canvas;
    SyncViewport   viewport;

    const int headerHeight = 28;
    const int gutterWidth = 160;

    ms_t computeMaxEndMs() const
    {
        ms_t m = 0;
        for (const auto& L : model.layers)
            for (const auto& c : L.clips)
                m = std::max(m, c.end());
        return m;
    }
};
