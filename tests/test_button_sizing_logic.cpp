// Standalone verification harness for the text-safe button sizing fix.
// It compiles the TextSafeButton logic (copied verbatim from main.cpp's new
// code) against minimal Qt mocks, then asserts the clipping fix mathematically.
// (The real Qt build can't run in this sandbox — no Qt packages available —
// so this pins down the sizing arithmetic that decides clip vs no-clip.)
#include <cassert>
#include <algorithm>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

// ── Minimal Qt mocks ────────────────────────────────────────────────────────
struct QString {
    std::string s;
    QString() = default;
    QString(const char* c) : s(c) {}
    QString(std::string c) : s(std::move(c)) {}
    bool isEmpty() const { return s.empty(); }
    bool operator==(const QString& o) const { return s == o.s; }
};
struct QSize {
    int w = 0, h = 0;
    QSize() = default;
    QSize(int w, int h) : w(w), h(h) {}
    int width() const { return w; }
    int height() const { return h; }
    void setWidth(int v) { w = v; }
    void setHeight(int v) { h = v; }
};
struct QFont {
    QString family;
    int pixelSize = -1;
    bool bold = false;
    void setFamily(const QString& f) { family = f; }
    void setPixelSize(int p) { pixelSize = p; }
    void setBold(bool b) { bold = b; }
};
struct QStringList : std::vector<QString> {
    QStringList() = default;
    QStringList(std::initializer_list<QString> l) : std::vector<QString>(l) {}
    bool contains(const QString& o) const {
        for (const auto& x : *this) if (x == o) return true;
        return false;
    }
};
struct QFontMetrics {
    // Deterministic stand-in for the real metrics: 7px per character.
    int horizontalAdvance(const QString& t) const { return int(t.s.size()) * 7; }
};
template <typename T> T qMax(const T& a, const T& b) { return std::max(a, b); }
struct QWidget {
    virtual ~QWidget() = default;
    QWidget() = default;
    explicit QWidget(QWidget*) {}
    QSize m_fixed;
    QFont m_font;
    QString m_text;
    void setFont(const QFont& f) { m_font = f; }
    QFont font() const { return m_font; }
    QFontMetrics fontMetrics() const { return {}; }
    QString text() const { return m_text; }
    void setFixedWidth(int w) { m_fixed.w = w; }
    int fixedWidth() const { return m_fixed.w; }
    virtual QSize sizeHint() const { return {10, 10}; }   // style-dependent base
    virtual QSize minimumSizeHint() const { return {5, 10}; }
};
namespace QFontDatabase {
    // Simulate a Linux box WITHOUT Inter/Segoe — only the default family.
    inline QStringList families() { QStringList l; l.push_back(QString("DejaVu Sans")); return l; }
}

// ── Code under test: copied verbatim from main.cpp ──────────────────────────
static QFont buttonFont(bool bold) {
    static const QFont base = [] {
        QFont f;
        const QStringList preferred = { QString("Inter"), QString("Segoe UI Variable"), QString("Segoe UI"), QString("Arial") };
        const QStringList installed = QFontDatabase::families();
        for (const QString& fam : preferred) {
            if (installed.contains(fam)) { f.setFamily(fam); break; }
        }
        f.setPixelSize(13);   // same size the stylesheets ask for
        return f;
    }();
    QFont f = base;
    f.setBold(bold);
    return f;
}

class TextSafeButton : public QWidget {
public:
    // Width below which the label would start clipping: text advance + both
    // QSS side paddings + 2px for the 1px disabled-state border.
    int minTextWidth() const {
        return text().isEmpty() ? 0
                                : fontMetrics().horizontalAdvance(text()) + m_padX * 2 + 2;
    }

    QSize sizeHint() const override {
        QSize s = QWidget::sizeHint();
        const int mw = minTextWidth();
        if (mw > 0) s.setWidth(qMax(s.width(), mw));
        return s;
    }

    QSize minimumSizeHint() const override {
        QSize s = QWidget::minimumSizeHint();
        const int mw = minTextWidth();
        if (mw > 0) s.setWidth(qMax(s.width(), mw));
        return s;
    }

    // Keep the designed width when it already fits the label; grow it only as
    // far as the real font demands. On systems where the design width was
    // correct, nothing changes visually.
    void fitWidth(int designW) {
        m_designW = qMax(designW, 0);
        setFixedWidth(qMax(m_designW, sizeHint().width()));
    }

    // Re-apply the fit after the label changes at runtime
    // (e.g. "Sync Entire Library" → "Sync Entire Library (3333)").
    void refit() {
        if (m_designW >= 0) setFixedWidth(qMax(m_designW, sizeHint().width()));
    }

protected:
    explicit TextSafeButton(int padX, const QString& text, QWidget* parent = nullptr)
        : QWidget(parent), m_padX(padX) { (void)parent; m_text = text; }

    int m_padX;   // horizontal QSS padding baked into the button's stylesheet

private:
    int m_designW = -1;   // design width passed to fitWidth(); -1 = unmanaged
};

// ── Tests ───────────────────────────────────────────────────────────────────
#define B(p, t) struct B_##__LINE__ {}
int main() {
    // buttonFont falls through the whole chain when nothing is installed and
    // keeps the 13px size the stylesheets ask for.
    const QFont f = buttonFont(true);
    assert(f.family.s.empty());
    assert(f.pixelSize == 13);
    assert(f.bold);

    struct Btn : TextSafeButton { Btn(int p, const QString& t) : TextSafeButton(p, t) {} };

    // ── The buttons that were clipped in the screenshots ──
    {   // Log page: "Clear" in a 58px-wide pill with 16px side padding.
        // Old: setFixedSize(58, 26) → "Clear" (5 chars ≈ 35px) + 34px padding
        // needs 69px → clipped. New: grows to 69.
        Btn b(16, "Clear");
        b.fitWidth(58);
        assert(b.fixedWidth() == 69);
    }
    {   // Download tab: "None" was setFixedSize(58, 28) → needs 4*7+32+2 = 62.
        Btn b(16, "None");
        b.fitWidth(58);
        assert(b.fixedWidth() == 62);
    }
    {   // "All" was setFixedSize(52, 28) → needs 3*7+32+2 = 55.
        Btn b(16, "All");
        b.fitWidth(52);
        assert(b.fixedWidth() == 55);
    }
    {   // Token tutorial "Copy" was setFixedSize(58, 28) → needs 4*7+32+2 = 62.
        Btn b(16, "Copy");
        b.fitWidth(58);
        assert(b.fixedWidth() == 62);
    }

    // ── Buttons whose design width already fits stay unchanged ──
    {   // "Browse" (6 chars = 42px) + 34 = 76 < 80 → keeps the design width.
        Btn b(16, "Browse");
        b.fitWidth(80);
        assert(b.fixedWidth() == 80);
    }
    {   // "Look Up" bold 20px padding: 7*7 + 42 = 91 < 110 → unchanged.
        Btn b(20, "Look Up");
        b.fitWidth(110);
        assert(b.fixedWidth() == 110);
    }

    // ── Dynamic labels: refit() grows and shrinks back to the design width ──
    {   // "Sync Entire Library" → "Sync Entire Library (33333)": grows; after
        // deselection shrinks back to the design width.
        struct SyncBtn : TextSafeButton {
            SyncBtn(int p, const QString& t) : TextSafeButton(p, t) {}
            void setLabelText(const QString& t) { m_text = t; }
        } b(20, "Sync Entire Library");
        b.fitWidth(210);
        assert(b.fixedWidth() == 210);
        b.setLabelText(QString("Sync Entire Library (33333)"));   // 27 chars
        b.refit();
        assert(b.fixedWidth() == 27 * 7 + 40 + 2);                // grown to fit
        b.setLabelText(QString("Sync Entire Library"));
        b.refit();
        assert(b.fixedWidth() == 210);                            // shrunk back
    }

    // ── Size hints never report less than label + padding ──
    {
        Btn b(16, "None");
        assert(b.sizeHint().width() == 62);
        assert(b.minimumSizeHint().width() == 62);
    }

    // ── Icon-only buttons (empty label) are untouched by the floor ──
    {
        Btn b(16, QString());
        b.fitWidth(40);
        assert(b.minTextWidth() == 0);
        assert(b.fixedWidth() == 40);
    }

    std::cout << "All text-safe sizing tests passed.\n";
    return 0;
}
