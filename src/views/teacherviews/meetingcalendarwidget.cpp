// meetingcalendarwidget.cpp
#include "meetingcalendarwidget.h"
#include <QPainter>

MeetingCalendarWidget::MeetingCalendarWidget(QWidget *parent)
    : QCalendarWidget(parent)
{
}

void MeetingCalendarWidget::setMeetings(const QMap<QString, QVector<Meeting>>& meetings)
{
    this->meetings = meetings;
    updateCells();
}

void MeetingCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    painter->save();
    
    // Vẽ cell mặc định
    QCalendarWidget::paintCell(painter, rect, date);
    
    QString dateStr = date.toString("dd/MM/yyyy");
    if (meetings.contains(dateStr)) {
        const QVector<Meeting>& dayMeetings = meetings[dateStr];
        
        // Vẽ thông tin cuộc họp
        int y = rect.top() + 25;
        for (const Meeting& meeting : dayMeetings) {
            if (y + 15 > rect.bottom()) break;
            
            QRect meetingRect = QRect(rect.left() + 2, y, rect.width() - 4, 15);
            
            // Màu nền theo loại cuộc họp
            QColor bgColor;
            QString type = QString::fromStdString(meeting.getType());
            if (type.contains("Họp Hội đồng", Qt::CaseInsensitive))
                bgColor = QColor(255, 200, 200);
            else if (type.contains("Họp Tổ", Qt::CaseInsensitive))
                bgColor = QColor(200, 255, 200);
            else
                bgColor = QColor(200, 200, 255);
                
            painter->fillRect(meetingRect, bgColor);
            
            QString meetingText = QString("%1 - %2")
                                .arg(QString::fromStdString(meeting.getStart()).mid(0, 5))
                                .arg(QString::fromStdString(meeting.getType()));
            
            painter->drawText(meetingRect, Qt::AlignLeft | Qt::AlignVCenter, 
                            painter->fontMetrics().elidedText(meetingText, Qt::ElideRight, meetingRect.width() - 4));
            
            y += 20;
        }
    }
    
    painter->restore();
}