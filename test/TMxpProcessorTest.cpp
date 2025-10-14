#include "TMxpProcessor.h"
#include "TMxpStubClient.h"
#include <QObject>
#include <QTest>

class TMxpProcessorTest : public QObject {
    Q_OBJECT

private slots:
    void testMalformedTagProducesError() {
        // Arrange
        TMxpStubClient client;
        TMxpProcessor processor(&client);
        processor.enable();
        QString malformedTag = "<send href=\"look\nsword\">";

        // Act
        for (const auto& ch : malformedTag) {
            char c = ch.toLatin1();
            processor.processMxpInput(c, false);
        }

        // Assert
        QVERIFY(client.displayMxpErrorCalled);
        QCOMPARE(client.lastErrorMessage, "Malformed tag found: '<send href=\"look'");
    }
};

QTEST_MAIN(TMxpProcessorTest)
#include "TMxpProcessorTest.moc"
