#include "MSIMSlaveItemDelegate.h"

#include <QPainter>
#include <QColor>
#include <QTextDocument>

#include "MSIMUIConstants.h"

MSIMSlaveItemDelegate::MSIMSlaveItemDelegate(QObject *parent) :
	QItemDelegate(parent)
{
}

void MSIMSlaveItemDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
	if (index.column() != 0) {
		QItemDelegate::paint(painter, option, index);
		return;
	}

	QString text = index.data().toString();
	QVariant data = index.data(Qt::UserRole);
	QColor col = data.value<QColor>();
	QRect r(option.rect);

	if (text.isEmpty()) {
		// color background when there is no text
		painter->setBrush(col);
		painter->drawRect(r);
	}
	else {
		// write colored text otherwise
		QStyleOptionViewItemV4 options = option;

		painter->save();

		QTextDocument doc;
		QFont f;
		f.setPointSize(TABLE_FONT_SIZE);
		doc.setDefaultFont(f);
		doc.setDocumentMargin(0);
		QString htmlText = QString("<div style=\"margin:0; color:%1\">%2</div>").arg(col.name()).arg(text);
		doc.setHtml(htmlText);

		options.text = "";
		options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

		painter->translate(r.left()+2, r.top());
		QRect clip(0, 0, r.width(), r.height());
		doc.drawContents(painter, clip);

		painter->restore();
	}
}

