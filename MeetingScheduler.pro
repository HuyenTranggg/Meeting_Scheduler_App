QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Thư mục chứa các file
INCLUDEPATH += src \
               src/controllers \
               src/models \
               src/repository \
               src/utils \
               src/views \
               src/views/userviews \
               src/views/teacherviews \
               src/views/studentviews \
               data \
               /usr/include/cppconn

# Thư viện MySQL Connector C++
LIBS += -lmysqlcppconn -lpthread

# Client sources
SOURCES += \
    src/client/Client1.cpp

# UI Files - User views
FORMS += \
    src/views/userviews/loginwidget.ui \
    src/views/userviews/registerwidget.ui \
    src/views/userviews/mainmenu.ui

# UI Files - Teacher views
FORMS += \
    src/views/teacherviews/TeacherMenuWidget.ui \
    src/views/teacherviews/timeslotdialog.ui \
    src/views/teacherviews/viewtimeslotsdialog.ui \
    src/views/teacherviews/update_timeslot_dialog.ui \
    src/views/teacherviews/teacherviewmeetingsdialog.ui \
    src/views/teacherviews/meetingdetaildialog.ui \
    src/views/teacherviews/meetinghistorydialog.ui \
    src/views/teacherviews/viewmeetinghistorydialog.ui \
    src/views/teacherviews/viewweeklymeetingsdialog.ui

# UI Files - Student views
FORMS += \
    src/views/studentviews/StudentMenu.ui \
    src/views/studentviews/ViewAllTeacherWidget.ui \
    src/views/studentviews/teacherlistviewer.ui \
    src/views/studentviews/timeslotcalendar.ui \
    src/views/studentviews/meetingbooking.ui \
    src/views/studentviews/meetingcalendarviewer.ui \
    src/views/studentviews/MeetingDialog.ui

# CPP Files - User views
SOURCES += \
    src/views/userviews/loginwidget.cpp \
    src/views/userviews/registerwidget.cpp \
    src/views/userviews/mainmenu.cpp

# CPP Files - Teacher views
SOURCES += \
    src/views/teacherviews/TeacherMenuWidget.cpp \
    src/views/teacherviews/timeslotdialog.cpp \
    src/views/teacherviews/viewtimeslotsdialog.cpp \
    src/views/teacherviews/update_timeslot_dialog.cpp \
    src/views/teacherviews/teacherviewmeetingsdialog.cpp \
    src/views/teacherviews/meetingdetaildialog.cpp \
    src/views/teacherviews/meetinghistorydialog.cpp \
    src/views/teacherviews/viewmeetinghistorydialog.cpp \
    src/views/teacherviews/viewweeklymeetingsdialog.cpp \
    src/views/teacherviews/meetingcalendarwidget.cpp

# CPP Files - Student views
SOURCES += \
    src/views/studentviews/StudentMenu.cpp \
    src/views/studentviews/ViewAllTeacherWidget.cpp \
    src/views/studentviews/teacherlistviewer.cpp \
    src/views/studentviews/timeslotcalendar.cpp \
    src/views/studentviews/meetingbooking.cpp \
    src/views/studentviews/meetingcalendarviewer.cpp \
    src/views/studentviews/MeetingDialog.cpp

# Header Files - User views
HEADERS += \
    src/views/userviews/loginwidget.h \
    src/views/userviews/registerwidget.h \
    src/views/userviews/mainmenu.h

# Header Files - Teacher views
HEADERS += \
    src/views/teacherviews/TeacherMenuWidget.h \
    src/views/teacherviews/timeslotdialog.h \
    src/views/teacherviews/viewtimeslotsdialog.h \
    src/views/teacherviews/update_timeslot_dialog.h \
    src/views/teacherviews/teacherviewmeetingsdialog.h \
    src/views/teacherviews/meetingdetaildialog.h \
    src/views/teacherviews/meetinghistorydialog.h \
    src/views/teacherviews/viewmeetinghistorydialog.h \
    src/views/teacherviews/viewweeklymeetingsdialog.h \
    src/views/teacherviews/meetingcalendarwidget.h

# Header Files - Student views
HEADERS += \
    src/views/studentviews/StudentMenu.h \
    src/views/studentviews/ViewAllTeacherWidget.h \
    src/views/studentviews/teacherlistviewer.h \
    src/views/studentviews/timeslotcalendar.h \
    src/views/studentviews/meetingbooking.h \
    src/views/studentviews/meetingcalendarviewer.h \
    src/views/studentviews/MeetingDialog.h

# Header Files - Old console views (not used in Qt version)
HEADERS += \
    src/views/UserView.h \
    src/views/TeacherView.h \
    src/views/StudentView.h

# Header Files - Controllers
HEADERS += \
    src/controllers/UserController.h \
    src/controllers/TeacherController.h \
    src/controllers/StudentController.h \
    src/controllers/TeacherResponseController.h \
    src/controllers/StudentResponseController.h

# Header Files - Models
HEADERS += \
    src/models/User.h \
    src/models/Meeting.h \
    src/models/Timeslot.h \
    src/models/Response.h \
    src/models/Attendance.h

# Header Files - Repository
HEADERS += \
    src/repository/UserRepository.h \
    src/repository/MeetingRepository.h \
    src/repository/TimeslotRepository.h \
    src/repository/AttendanceRepository.h

# Header Files - Utils
HEADERS += \
    src/utils/Utils.h \
    src/utils/MessageUtils.h \
    src/Status.h

# Header Files - Data
HEADERS += \
    data/Database.h \
    environment.h

# Thư mục output
DESTDIR = build
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/rcc

# Tên file thực thi
TARGET = MeetingScheduler

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
