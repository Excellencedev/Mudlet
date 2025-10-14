#ifndef MUDLET_TMXPSTUBCLIENT_H
#define MUDLET_TMXPSTUBCLIENT_H

#include "TMxpClient.h"

class TMxpStubClient : public TMxpClient {
public:
    bool displayMxpErrorCalled = false;
    QString lastErrorMessage;

    void displayMxpError(const QString& message) override {
        displayMxpErrorCalled = true;
        lastErrorMessage = message;
    }

    // Other TMxpClient methods are not implemented as they are not needed for this test
    QString getVersion() override { return QString(); }
    void sendToServer(QString& str) override {}
    void setLinkMode(bool val) override {}
    void setFlag(const QString& elementName, const QMap<QString, QString>& params, const QString& content) override {}
    void publishEntity(const QString& name, const QString& value) override {}
    void setVariable(const QString& name, const QString& value) override {}
    void pushColor(const QString& fgColor, const QString& bgColor) override {}
    void popColor() override {}
    void pushFont(const QString& fontFace, const QString& fontSize) override {}
    void popFont() override {}
    void setBold(bool val) override {}
    void setItalic(bool val) override {}
    void setUnderline(bool val) override {}
    void setStrikeOut(bool val) override {}
    bool bold() override { return false; }
    bool italic() override { return false; }
    bool underline() override { return false; }
    bool strikeOut() override { return false; }
    void resetTextProperties() override {}
    void setStyle(const QString& val) override {}
    QString getStyle() override { return QString(); }
    int setLink(const QStringList& hrefs, const QStringList& hints) override { return 0; }
    bool getLink(int id, QStringList** hrefs, QStringList** hints) override { return false; }
    void playMedia(TMediaData& mediaData) override {}
    void stopMedia(TMediaData& mediaData) override {}
    void promptToDisableMxp() override {}
};

#endif //MUDLET_TMXPSTUBCLIENT_H
