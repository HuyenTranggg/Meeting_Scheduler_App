// meetingcalendarwidget.h
#ifndef MEETINGCALENDARWIDGET_H
#define MEETINGCALENDARWIDGET_H

#include <QCalendarWidget>
#include <QMap>
#include <QVector>
#include "../../models/Meeting.h"

class MeetingCalendarWidget : public QCalendarWidget
{
    Q_OBJECT

public:
    explicit MeetingCalendarWidget(QWidget *parent = nullptr);
    void setMeetings(const QMap<QString, QVector<Meeting>>& meetings);

protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const override;

private:
    QMap<QString, QVector<Meeting>> meetings;
};

#endif // MEETINGCALENDARWIDGET_H