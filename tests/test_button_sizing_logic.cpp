#include <cassert>
#include <algorithm>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

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
    virtual QSize sizeHint() const { return {10, 10}; }
    virtual QSize minimumSizeHint() const { return {5, 10}; }
};
namespace QFontDatabase {

    inline QStringList families() { QStringList l; l.push_back(QString("DejaVu Sans")); return l; }
}

static QFont buttonFont(bool bold) {
    static const QFont base = [] {
        QFont f;
        const QStringList preferred = { QString("Inter"), QString("Segoe UI Variable"), QString("Segoe UI"), QString("Arial") };
        const QStringList installed = QFontDatabase::families();
        for (const QString& fam : preferred) {
            if (installed.contains(fam)) { f.setFamily(fam); break; }
        }
        f.setPixelSize(13);
        return f;
    }();
    QFont f = base;
    f.setBold(bold);
    return f;
}

class TextSafeButton : public QWidget {
public:

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

    void fitWidth(int designW) {
        m_designW = qMax(designW, 0);
        setFixedWidth(qMax(m_designW, sizeHint().width()));
    }

    void refit() {
        if (m_designW >= 0) setFixedWidth(qMax(m_designW, sizeHint().width()));
    }

protected:
    explicit TextSafeButton(int padX, const QString& text, QWidget* parent = nullptr)
        : QWidget(parent), m_padX(padX) { (void)parent; m_text = text; }

    int m_padX;

private:
    int m_designW = -1;
};

#define B(p, t) struct B_##__LINE__ {}
int main() {

    const QFont f = buttonFont(true);
    assert(f.family.s.empty());
    assert(f.pixelSize == 13);
    assert(f.bold);

    struct Btn : TextSafeButton { Btn(int p, const QString& t) : TextSafeButton(p, t) {} };

    {

        Btn b(16, "Clear");
        b.fitWidth(58);
        assert(b.fixedWidth() == 69);
    }
    {
        Btn b(16, "None");
        b.fitWidth(58);
        assert(b.fixedWidth() == 62);
    }
    {
        Btn b(16, "All");
        b.fitWidth(52);
        assert(b.fixedWidth() == 55);
    }
    {
        Btn b(16, "Copy");
        b.fitWidth(58);
        assert(b.fixedWidth() == 62);
    }

    {
        Btn b(16, "Browse");
        b.fitWidth(80);
        assert(b.fixedWidth() == 80);
    }
    {
        Btn b(20, "Look Up");
        b.fitWidth(110);
        assert(b.fixedWidth() == 110);
    }

    {

        struct SyncBtn : TextSafeButton {
            SyncBtn(int p, const QString& t) : TextSafeButton(p, t) {}
            void setLabelText(const QString& t) { m_text = t; }
        } b(20, "Sync Entire Library");
        b.fitWidth(210);
        assert(b.fixedWidth() == 210);
        b.setLabelText(QString("Sync Entire Library (33333)"));
        b.refit();
        assert(b.fixedWidth() == 27 * 7 + 40 + 2);
        b.setLabelText(QString("Sync Entire Library"));
        b.refit();
        assert(b.fixedWidth() == 210);
    }

    {
        Btn b(16, "None");
        assert(b.sizeHint().width() == 62);
        assert(b.minimumSizeHint().width() == 62);
    }

    {
        Btn b(16, QString());
        b.fitWidth(40);
        assert(b.minTextWidth() == 0);
        assert(b.fixedWidth() == 40);
    }

    std::cout << "All text-safe sizing tests passed.\n";
    return 0;
}
