#include "user_level_logging_messages.h"
#include <QApplication>
#include <QErrorMessage>
#include <cstring>



namespace UserLevelMessageHandling {

    Q_LOGGING_CATEGORY(PLUGINS, "plugins")

    const char * const CUSTOM_CATEGORIES[] = {
        "plugins"
    };

    bool initialized = false;
    QtMessageHandler prevHandler;
    QErrorMessage* msgBox;

    void handler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        /* todo: rn with QErrorMessage, this will show up with a check box
         * asking if the user wants to see it again, but that only applies for exactly the same error string,
         * sometimes you have more than one string but that all are the same basic error,
         * so we should replace this with a checkbox that is based on something else,
         * maybe put in a pull request that makes all this relly on a hash of the string by default(probably what already happens),
         * but have an optional hash parameter to override this.
         */
        for (auto c : CUSTOM_CATEGORIES)
            if (0 == strcmp(c, context.category))
                msgBox->showMessage(msg);

        prevHandler(type, context, msg);
    }

    void init() {
        if (initialized)
            return;

        prevHandler = qInstallMessageHandler(0);

        //sets up message handler that swallows everything, but we already have the old handler stored
        //todo: get rid of this and manually set this to something, the default one is too small
        msgBox = QErrorMessage::qtHandler();

        qInstallMessageHandler(handler);

        initialized = true;
    }

}
