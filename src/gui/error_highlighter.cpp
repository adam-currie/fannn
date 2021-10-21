#include "error_highlighter.h"
#include "q_governor_error.h"
#include <QTextBlock>

ErrorHighlighter::ErrorHighlighter(QObject* parent) : QObject(parent) {}

void ErrorHighlighter::formatErrors(QVariantList errors) {
    if (!_document) return;

    QTextDocument* doc = _document->textDocument();
    QTextCursor cursor = QTextCursor(doc);
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::MoveOperation::End, QTextCursor::KeepAnchor);

    for (QTextBlock block = doc->begin(); block.isValid(); block = block.next()) {
        QTextLayout * layout = block.layout();
        layout->clearFormats();

        QVector<QTextLayout::FormatRange> ranges;
        QTextLayout::FormatRange range;
        range.format.setForeground(Qt::red);
/* todo: tooltip for error msgs, red underline instead of text color, so text color can be used for syntax highlighting instead
        range.format.setToolTip(QString("test"));//waiting for this to be implemented, go here and search "todo" -> https://code.woboq.org/qt6/qtbase/src/gui/text/qtextodfwriter.cpp.html
        range.format.setUnderlineColor(Qt::red);
        range.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
        range.format.setFontUnderline(true);//this gets reset to false if we set underline style after this, also, style and color do nothing
*/

        auto sortedInsertRange = [&](){
            for (auto it = ranges.begin(); it != ranges.end(); ++it) {
                if (range.start < it->start) {
                    ranges.insert(it, range);
                    return;
                }
            }
            ranges.append(range);
        };

        for (const auto & e : errors) {
            auto err = e.value<QGovernorError>();

            for (const auto [start, end] : err.ranges) {
                int length = end-start + 1;

                if (block.contains(start)) {
                    range.format.setToolTip(err.errMsg);
                    range.start = start - block.position();
                    range.length = block.contains(end) ?
                        length : block.length() - block.position() - start;
                    sortedInsertRange();
                } else if (block.contains(end)) {
                    range.format.setToolTip(err.errMsg);
                    range.start = 0;
                    range.length = end - block.position() + 1;
                    sortedInsertRange();
                }
            }
        }

        layout->setFormats(ranges);
    }

    cursor.endEditBlock();
    doc->markContentsDirty(0, doc->characterCount());
}


