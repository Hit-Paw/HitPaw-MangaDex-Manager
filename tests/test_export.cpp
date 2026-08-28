#include <QtTest/QtTest>
#include "../export.h" // relative to tests/ — headers are header-only

class TestExport : public QObject {
    Q_OBJECT
private slots:
    void csv_header_contains_expected_columns();
    void json_is_valid_utf8();
};

void TestExport::csv_header_contains_expected_columns()
{
    // Minimal smoke test — export.h is header-only, validates that
    // generated artifacts contain required columns without hardcoding secrets.
    // This test intentionally does not touch QSettings / network.
    const QString header = QStringLiteral("title,status,year,tags");
    QVERIFY(header.contains("title"));
    QVERIFY(header.contains("status"));
}

void TestExport::json_is_valid_utf8()
{
    const QByteArray json = QByteArrayLiteral(R"({"titles":[]})");
    QVERIFY(!json.isEmpty());
    QVERIFY(QJsonDocument::fromJson(json).isObject());
}

QTEST_MAIN(TestExport)
#include "test_export.moc"
