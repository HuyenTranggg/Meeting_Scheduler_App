-- phpMyAdmin SQL Dump
-- version 5.1.1deb5ubuntu1
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Dec 31, 2024 at 07:09 PM
-- Server version: 8.0.40-0ubuntu0.22.04.1
-- PHP Version: 8.1.2-1ubuntu2.20

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `ltm`
--

-- --------------------------------------------------------

--
-- Table structure for table `attendances`
--

CREATE TABLE `attendances` (
  `id` bigint UNSIGNED NOT NULL,
  `meeting_id` bigint UNSIGNED NOT NULL,
  `student_id` bigint UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Dumping data for table `attendances`
--

INSERT INTO `attendances` (`id`, `meeting_id`, `student_id`) VALUES
(2, 2, 5),
(3, 2, 1),
(4, 2, 4),
(5, 3, 2),
(6, 4, 5),
(7, 5, 5),
(8, 5, 3),
(9, 6, 1),
(10, 7, 5),
(11, 8, 5),
(12, 9, 5),
(13, 10, 5),
(15, 11, 5),
(18, 2, 5),
(20, 2, 5),
(21, 2, 5),
(23, 18, 5),
(24, 19, 3),
(27, 20, 26),
(28, 21, 26),
(29, 22, 26),
(30, 23, 26),
(31, 24, 26),
(32, 25, 26),
(33, 20, 27),
(34, 26, 27),
(35, 27, 27),
(36, 20, 29),
(37, 28, 29),
(38, 26, 29),
(39, 20, 28),
(40, 29, 28),
(41, 30, 28),
(42, 20, 30),
(43, 31, 30),
(44, 32, 30);

-- --------------------------------------------------------

--
-- Table structure for table `meetings`
--

CREATE TABLE `meetings` (
  `id` bigint UNSIGNED NOT NULL,
  `timeslot_id` bigint UNSIGNED NOT NULL,
  `status` enum('pending','confirmed','canceled','doing','completed') CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'pending',
  `type` enum('personal','group') COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'personal',
  `report` longtext COLLATE utf8mb4_unicode_ci
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Dumping data for table `meetings`
--

INSERT INTO `meetings` (`id`, `timeslot_id`, `status`, `type`, `report`) VALUES
(2, 2, 'pending', 'group', 'helo anh em\naa'),
(3, 3, 'doing', 'personal', NULL),
(4, 4, 'confirmed', 'personal', NULL),
(5, 5, 'completed', 'group', '“Meo...meo...meo...” Mới sáng sớm, chú mèo Mimi nhà tôi đã cất tiếng kêu không ngớt. Hẳn là chú ta mới tỉnh giấc và đói bụng do đêm qua không rình được con mồi nào.\n\nMimi |là món quà mẹ tặng tôi nhân dịp sinh nhật lần thứ chín. Ngày mới về, nó bé bằng chai nước lọc, lại lấm lét vẻ e sợ. Ấy thế mà, gần một năm qua, chú ta lớn nhanh như thổi, bỗng chốc đã thành một chàng mèo đỏm dáng, lớn chừng gấp đôi ngày trước. Mimi là giống mèo tam thể nên chú có bộ lông khá sặc sỡ. Trên nền lông vàng ươm điểm những vệt lông đen và xám. Bộ lông của chú mềm mượt, trông chú y hệt như một cục bông, khiến ai thấy chú cũng muốn vuốt ve, âu yếm. Biết được mọi người cưng yêu, Mimi rất thích cọ cọ mũi hoặc rúc đầu vào tay người vuốt, như muốn đáp lại. Xem ra, cậu nhóc này sống tính cảm đấy! Chiếc đầu hình tam giác, thon nhỏ như một quả cầu bông xinh xắn. Quả cầu này lúc nào cũng ngó nghiêng với đôi mắt hấp háy. Đôi mắt Mimi mới đẹp làm sao! Đôi mắt chú to, tròn, màu xanh ngọc bích, long lanh như hai viên bi để dưới ánh mặt trời.\n\nCả ngày dài, Mimi hết bước ra lại chạy vào. Cái đuôi dài mượt cứ ngoe nguẩy, chẳng khác nào chiếc chổi lông đang treo trên thân mèo. Đêm về, sau hồi meo meo meo kêu đói như ban sáng, mèo ta chui vào ngôi nhà nhỏ xinh bằng gỗ của mình, nằm im. Những tưởng chú đã ngủ khì, nhưng không, chú ngẩng đầu, đảo đôi mắt liên hồi quan sát. Đôi mắt chính là chiếc đèn soi sáng để mèo có thể quan sát mọi vật trong đêm. Đôi tai trên đỉnh đầu như hai chiếc tam giác khẽ giật giật, bộ ria mép trắng như cước cũng khẽ động đậy, vẻ như mèo nghe ngóng rất kĩ càng. Sột soạt… nghe thấy tiếng, Mimi đứng thẳng dậy, bước ra ngoài. Dưới ánh trăng mờ đang khẽ rọi qua khe cửa, mèo chẳng còn bóng dáng lười biếng ban chiều, chú ta trở nên oai phong kì lạ. Lũ chuột vẻ biết mèo đang rình mình, đua nhau kêu chít bỏ chạy. Nhanh như cắt, mèo lùi bốn chi lại lấy đà, rồi phi thẳng về chỗ lũ chuột. Một chú chuột xấu số đã yên vị trong bốn chi cứng cáp của mèo. Mèo ta chúi chiếc mũi đo đỏ xuống để ngửi như thể chuẩn bị thưởng thức món ngon. Rồi cũng nhanh như cách chú bắt chuột, chú dùng cái miệng ngoạm vài miếng ngon lành.\n\nĐêm nào cũng vậy, Mimi lặng lẽ đi tiêu diệt “lũ giặc”. Nếu thu được nhiều chiến lợi phẩm, sáng hôm sau chú ta chỉ nằm dài mà ngủ, chẳng meo meo đòi ăn nữa. Tôi rất thích chú mèo này, tôi sẽ chăm sóc chú thật tốt. '),
(6, 6, 'completed', 'personal', 'src/client/../controllers/ClientController.h:76:18: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<std::__cxx11::basic_string<char> >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n   76 |         while (i < tokens.size()) {\n      |                ~~^~~~~~~~~~~~~~~\nsrc/client/../controllers/ClientController.h: In member function ‘std::vector<Meeting> ClientController::parseMeetingsFromResponse(const string&)’:\nsrc/client/../controllers/ClientController.h:94:18: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<std::__cxx11::basic_string<char> >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n   94 |         while (i < tokens.size()) {\n      |                ~~^~~~~~~~~~~~~~~\nIn file included from src/client/Client1.cpp:2:\nsrc/client/../controllers/ResponseController.h: In member function ‘Response ResponseController::viewTimeslots(const int&)’:\nsrc/client/../controllers/ResponseController.h:52:39: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<Timeslot>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n   52 |                     for (int i = 0; i < tss.size(); i++) {\n      |                                     ~~^~~~~~~~~~~~\nIn file included from src/client/Client1.cpp:8:\nsrc/client/../views/StudentView.h: In member function ‘std::pair<std::__cxx11::basic_string<char>, int> StudentView::showAllTeacher(const std::vector<User>&)’:\nsrc/client/../views/StudentView.h:46:27: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<User>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n   46 |         for (int i = 0; i < teachers.size(); i++) {\n      |                         ~~^~~~~~~~~~~~~~~~~\nsrc/client/../views/StudentView.h:58:38: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<User>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n   58 |             if (choice > 0 && choice <= teachers.size()) {\n      |                               ~~~~~~~^~~~~~~~~~~~~~~~~~\nsrc/client/../views/StudentView.h: In member function ‘Timeslot StudentView::showTimeslots(const std::map<std::__cxx11::basic_string<char>, std::vector<Timeslot> >&, const string&)’:\nsrc/client/../views/StudentView.h:83:31: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<Timeslot>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n   83 |             for (int i = 0; i < tss.size(); i++) {\n      |                             ~~^~~~~~~~~~~~\nsrc/client/../views/StudentView.h:96:38: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::map<int, Timeslot>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n   96 |             if (choice > 0 && choice <= editTimeslots.size()) {\n      |                               ~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~\nsrc/client/../views/StudentView.h: In member function ‘int StudentView::selectMeeting(const std::vector<Meeting>&)’:\nsrc/client/../views/StudentView.h:166:38: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<Meeting>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  166 |             if (choice > 0 && choice <= meetings.size()) {\n      |                               ~~~~~~~^~~~~~~~~~~~~~~~~~\nsrc/client/../views/StudentView.h: In member function ‘Meeting StudentView::showMeetingsInWeeks(const std::map<std::__cxx11::basic_string<char>, std::map<std::__cxx11::basic_string<char>, std::vector<std::pair<Meeting, User> > > >&)’:\nsrc/client/../views/StudentView.h:225:35: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<std::pair<Meeting, User> >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  225 |                 for (int i = 0; i < currentMeetings.size(); i++) {\n      |                                 ~~^~~~~~~~~~~~~~~~~~~~~~~~\nsrc/client/../views/StudentView.h:243:38: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::map<int, Meeting>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  243 |             if (choice > 0 && choice <= editMeetings.size()) {\n      |                               ~~~~~~~^~~~~~~~~~~~~~~~~~~~~~\nIn file included from src/client/Client1.cpp:9:\nsrc/client/../views/TeacherView.h: In member function ‘Timeslot TeacherView::showDeclareTimeSlots(const int&)’:\nsrc/client/../views/TeacherView.h:51:14: warning: unused variable ‘done’ [-Wunused-variable]\n   51 |         bool done = false, typechosen = false;\n      |              ^~~~\nsrc/client/../views/TeacherView.h: In member function ‘Timeslot TeacherView::showTimeslots(const std::map<std::__cxx11::basic_string<char>, std::vector<Timeslot> >&)’:\nsrc/client/../views/TeacherView.h:157:31: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<Timeslot>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  157 |             for (int i = 0; i < tss.size(); i++) {\n      |                             ~~^~~~~~~~~~~~\nsrc/client/../views/TeacherView.h:170:38: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::map<int, Timeslot>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  170 |             if (choice > 0 && choice <= editTimeslots.size()) {\n      |                               ~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h: In member function ‘Timeslot TeacherView::showUpdateTimeslot(const Timeslot&)’:\nsrc/client/../views/TeacherView.h:200:13: warning: unused variable ‘id’ [-Wunused-variable]\n  200 |         int id = timeslot.getId();\n      |             ^~\nsrc/client/../views/TeacherView.h: In member function ‘Meeting TeacherView::showMeetings(const std::map<std::__cxx11::basic_string<char>, std::vector<Meeting> >&)’:\nsrc/client/../views/TeacherView.h:290:31: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<Meeting>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  290 |             for (int i = 0; i < currentMeetings.size(); i++) {\n      |                             ~~^~~~~~~~~~~~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h:303:38: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::map<int, Meeting>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  303 |             if (choice > 0 && choice <= editMeetings.size()) {\n      |                               ~~~~~~~^~~~~~~~~~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h: In member function ‘Meeting TeacherView::showMeetingsInWeeks(const std::map<std::__cxx11::basic_string<char>, std::map<std::__cxx11::basic_string<char>, std::vector<std::pair<Meeting, std::vector<User> > > > >&)’:\nsrc/client/../views/TeacherView.h:329:35: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<std::pair<Meeting, std::vector<User> > >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  329 |                 for (int i = 0; i < currentMeetings.size(); i++) {\n      |                                 ~~^~~~~~~~~~~~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h:354:38: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::map<int, Meeting>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  354 |             if (choice > 0 && choice <= editMeetings.size()) {\n      |                               ~~~~~~~^~~~~~~~~~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h: In member function ‘Meeting TeacherView::showHistory(const std::map<std::__cxx11::basic_string<char>, std::map<std::__cxx11::basic_string<char>, std::vector<std::pair<Meeting, std::vector<User> > > > >&)’:\nsrc/client/../views/TeacherView.h:405:35: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<std::pair<Meeting, std::vector<User> > >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  405 |                 for (int i = 0; i < currentMeetings.size(); i++) {\n      |                                 ~~^~~~~~~~~~~~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h:430:38: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::map<int, Meeting>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  430 |             if (choice > 0 && choice <= editMeetings.size()) {\n      |                               ~~~~~~~^~~~~~~~~~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h: In member function ‘std::string TeacherView::showEditReport()’:\nsrc/client/../views/TeacherView.h:478:27: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<std::__cxx11::basic_string<char> >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  478 |         for (int i = 0; i < lines.size(); i++) {\n      |                         ~~^~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h:479:19: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<std::__cxx11::basic_string<char> >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  479 |             if (i == lines.size() - 1) {\n      |                 ~~^~~~~~~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h: In member function ‘int TeacherView::showStudentList(const std::vector<User>&)’:\nsrc/client/../views/TeacherView.h:526:27: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<User>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  526 |         for (int i = 0; i < students.size(); i++) {\n      |                         ~~^~~~~~~~~~~~~~~~~\nsrc/client/../views/TeacherView.h:538:38: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<User>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  538 |             if (choice > 0 && choice <= students.size()) {\n      |                               ~~~~~~~^~~~~~~~~~~~~~~~~~\nsrc/client/Client1.cpp: In function ‘void handleViewAllTeacher()’:\nsrc/client/Client1.cpp:544:14: warning: structured bindings only available with ‘-std=c++17’ or ‘-std=gnu++17’\n  544 |         auto [teacherName, teacherId] = viewer.showTeacherList(teachers);\n      |              ^\ng++ -Wl,-O1 -o client1 Client1.o timeslotdialog.o viewtimeslotsdialog.o update_timeslot_dialog.o teacherviewmeetingsdialog.o meetingdetaildialog.o meetingcalendarwidget.o meetinghistorydialog.o viewmeetinghistorydialog.o viewweeklymeetingsdialog.o loginwidget.o mainmenu.o registerwidget.o TeacherMenuWidget.o ViewAllTeacherWidget.o teacherlistviewer.o timeslotcalendar.o meetingbooking.o meetingcalendarviewer.o MeetingDialog.o StudentMenu.o studentlistdialog.o moc_timeslotdialog.o moc_viewtimeslotsdialog.o moc_update_timeslot_dialog.o moc_teacherviewmeetingsdialog.o moc_meetingdetaildialog.o moc_meetingcalendarwidget.o moc_meetinghistorydialog.o moc_viewmeetinghistorydialog.o moc_viewweeklymeetingsdialog.o moc_loginwidget.o moc_mainmenu.o moc_registerwidget.o moc_TeacherMenuWidget.o moc_ViewAllTeacherWidget.o moc_teacherlistviewer.o moc_timeslotcalendar.o moc_meetingbooking.o moc_meetingcalendarviewer.o moc_MeetingDialog.o moc_StudentMenu.o moc_studentlistdialog.o   -lmysqlcppconn /usr/lib/x86_64-linux'),
(7, 7, 'confirmed', 'personal', 'van\nban\ncuoc\nhop'),
(8, 8, 'pending', 'personal', ''),
(9, 9, 'pending', 'personal', ''),
(10, 10, 'doing', 'group', 'src/client/../controllers/TeacherController.h: In member function ‘std::map<std::__cxx11::basic_string<char>, std::map<std::__cxx11::basic_string<char>, std::vector<std::pair<Meeting, std::vector<User> > > > > TeacherController::getMeetingsInWeeksFromResponse(const string&)’:\nsrc/client/../controllers/TeacherController.h:138:18: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<std::__cxx11::basic_string<char> >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]\n  138 '),
(11, 16, 'pending', 'personal', NULL),
(12, 16, 'pending', 'personal', NULL),
(15, 2, 'pending', 'group', NULL),
(18, 19, 'doing', 'personal', '“Meo...meo...meo...” Mới sáng sớm, chú mèo Mimi nhà tôi đã cất tiếng kêu không ngớt. Hẳn là chú ta mới tỉnh giấc và đói bụng do đêm qua không rình được con mồi nào.\n\nMimi là món quà mẹ tặng tôi nhân dịp sinh nhật lần thứ chín. Ngày mới về, nó bé bằng chai nước lọc, lại lấm lét vẻ e sợ. Ấy thế mà, gần một năm qua, chú ta lớn nhanh như thổi, bỗng chốc đã thành một chàng mèo đỏm dáng, lớn chừng gấp đôi ngày trước. Mimi là giống mèo tam thể nên chú có bộ lông khá sặc sỡ. Trên nền lông vàng ươm điểm những vệt lông đen và xám. Bộ lông của chú mềm mượt, trông chú y hệt như một cục bông, khiến ai thấy chú cũng muốn vuốt ve, âu yếm. Biết được mọi người cưng yêu, Mimi rất thích cọ cọ mũi hoặc rúc đầu vào tay người vuốt, như muốn đáp lại. Xem ra, cậu nhóc này sống tính cảm đấy! Chiếc đầu hình tam giác, thon nhỏ như một quả cầu bông xinh xắn. Quả cầu này lúc nào cũng ngó nghiêng với đôi mắt hấp háy. Đôi mắt Mimi mới đẹp làm sao! Đôi mắt chú to, tròn, màu xanh ngọc bích, long lanh như hai viên bi để dưới ánh mặt trời.\n\nCả ngày dài, Mimi hết bước ra lại chạy vào. Cái đuôi dài mượt cứ ngoe nguẩy, chẳng khác nào chiếc chổi lông đang treo trên thân mèo. Đêm về, sau hồi meo meo meo kêu đói như ban sáng, mèo ta chui vào ngôi nhà nhỏ xinh bằng gỗ của mình, nằm im. Những tưởng chú đã ngủ khì, nhưng không, chú ngẩng đầu, đảo đôi mắt liên hồi quan sát. Đôi mắt chính là chiếc đèn soi sáng để mèo có thể quan sát mọi vật trong đêm. Đôi tai trên đỉnh đầu như hai chiếc tam giác khẽ giật giật, bộ ria mép trắng như cước cũng khẽ động đậy, vẻ như mèo nghe ngóng rất kĩ càng. Sột soạt… nghe thấy tiếng, Mimi đứng thẳng dậy, bước ra ngoài. Dưới ánh trăng mờ đang khẽ rọi qua khe cửa, mèo chẳng còn bóng dáng lười biếng ban chiều, chú ta trở nên oai phong kì lạ. Lũ chuột vẻ biết mèo đang rình mình, đua nhau kêu chít bỏ chạy. Nhanh như cắt, mèo lùi bốn chi lại lấy đà, rồi phi thẳng về chỗ lũ chuột. Một chú chuột xấu số đã yên vị trong bốn chi cứng cáp của mèo. Mèo ta chúi chiếc mũi đo đỏ xuống để ngửi như thể chuẩn bị thưởng thức món ngon. Rồi cũng nhanh như cách chú bắt chuột, chú dùng cái miệng ngoạm vài miếng ngon lành.\n\nĐêm nào cũng vậy, Mimi lặng lẽ đi tiêu diệt “lũ giặc”. Nếu thu được nhiều chiến lợi phẩm, sáng hôm sau chú ta chỉ nằm dài mà ngủ, chẳng meo meo đòi ăn nữa. Tôi rất thích chú mèo này, tôi sẽ chăm sóc chú thật tốt. '),
(19, 14, 'completed', 'group', NULL),
(20, 42, 'confirmed', 'group', NULL),
(21, 39, 'pending', 'personal', NULL),
(22, 34, 'pending', 'personal', NULL),
(23, 24, 'pending', 'group', NULL),
(24, 23, 'pending', 'personal', 'I. Mở bài: Giới thiệu về con vật mà em dự định miêu tả (con mèo).\n\nII. Thân bài\n\n- Tả hình dáng con mèo\n\n+ Mèo loài động vật có 4 chân.\n\n+ Mèo tam thể có 3 màu đặc trưng đó là màu đen, màu trắng và màu vàng xen lẫn nhau, bộ lông mượt.\n\n+ Cái đầu tròn, hai mắt tròn và long lanh.\n\n+ Hai tai lúc nào cũng vểnh lên nên chúng rất thính khi nghe ngóng âm thanh.\n\n+ Mũi có màu hồng, lúc nào cũng ướt át.\n\n+ Ria mép gồm nhiều sợ râu dài và cứng.\n\n+ Dưới chân có miếng đệm giúp chúng di chuyển nhẹ nhàng. Xung quanh có móng vuốt.Mèo là loài đẻ con, mỗi lứa 3- 5 con.\n\n- Tả hoạt động con mèo\n\n+ Mèo rất thích đuổi bắt chuột.\n\n+ Mèo thích được phơi nắng, sưởi ấm vào buổi sáng.\n\n+ Di chuyển nhanh nhẹn, khéo léo, khi di chuyển thường ít nghe tiếng động.\n\n+ Mèo và chó thường rất hay xung đột và cắn nhau.\n\n+ Trong đêm tối mắt mèo sáng giúp nhìn thấy rõ mọi vật.\n\n+ Mèo rất thích được âu yếm, vuốt ve.\n\n+ Mèo nhà em rất thích ăn, món khoái khẩu là cá.\n\nIII. Kết bài: Cảm nghĩ về con mèo nhà em.\n\n- Mèo là vật nuôi trong nhà rất gần gũi, thân thiết và như một người bạn trong gia đình.\n\n- Mèo thích bắt chuột và là loài vật hữu ích với con người.\n\n- Em sẽ chăm sóc chu đáo và cẩn thận.'),
(25, 35, 'pending', 'personal', NULL),
(26, 49, 'completed', 'group', NULL),
(27, 28, 'completed', 'personal', NULL),
(28, 25, 'pending', 'personal', NULL),
(29, 41, 'pending', 'group', NULL),
(30, 31, 'pending', 'personal', NULL),
(31, 51, 'pending', 'group', NULL),
(32, 20, 'pending', 'personal', NULL);

-- --------------------------------------------------------

--
-- Table structure for table `timeslots`
--

CREATE TABLE `timeslots` (
  `id` bigint UNSIGNED NOT NULL,
  `start` time NOT NULL,
  `end` time NOT NULL,
  `date` date NOT NULL,
  `type` enum('personal','group','both') CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'personal',
  `status` enum('busy','free') COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'free',
  `teacher_id` bigint UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Dumping data for table `timeslots`
--

INSERT INTO `timeslots` (`id`, `start`, `end`, `date`, `type`, `status`, `teacher_id`) VALUES
(1, '07:50:00', '09:15:00', '2024-11-15', 'both', 'free', 6),
(2, '03:30:00', '06:00:00', '2025-01-20', 'group', 'free', 6),
(3, '06:32:00', '07:10:00', '2024-11-05', 'both', 'free', 6),
(4, '15:50:12', '16:30:00', '2024-11-05', 'group', 'free', 6),
(5, '06:20:00', '06:45:00', '2024-11-05', 'both', 'busy', 6),
(6, '15:20:00', '15:30:00', '2024-11-05', 'personal', 'busy', 6),
(7, '15:25:00', '15:40:00', '2024-11-05', 'group', 'free', 6),
(8, '16:00:00', '16:20:00', '2024-11-05', 'group', 'free', 6),
(9, '15:35:00', '15:50:00', '2024-11-05', 'group', 'free', 6),
(10, '05:12:00', '06:13:00', '2024-12-03', 'personal', 'busy', 6),
(11, '09:10:00', '10:00:00', '2024-12-03', 'personal', 'busy', 6),
(12, '05:10:00', '07:20:00', '2024-12-03', 'group', 'free', 11),
(13, '10:10:00', '11:11:00', '2024-12-10', 'personal', 'free', 11),
(14, '05:15:00', '07:12:00', '2024-12-16', 'group', 'free', 6),
(15, '08:20:00', '10:10:00', '2024-12-18', 'personal', 'free', 6),
(16, '07:00:00', '09:00:00', '2025-01-10', 'personal', 'busy', 6),
(17, '07:00:00', '08:00:00', '2025-12-30', 'personal', 'busy', 6),
(18, '07:00:00', '08:00:00', '2025-12-29', 'personal', 'busy', 6),
(19, '07:00:00', '08:00:00', '2024-12-30', 'personal', 'busy', 6),
(20, '08:10:00', '10:00:00', '2024-12-30', 'personal', 'busy', 25),
(21, '17:00:00', '18:00:00', '2024-12-31', 'personal', 'free', 25),
(22, '11:45:00', '12:30:00', '2024-12-30', 'group', 'free', 25),
(23, '14:00:00', '15:00:00', '2024-12-31', 'personal', 'busy', 25),
(24, '14:00:00', '15:00:00', '2024-12-30', 'group', 'free', 25),
(25, '07:00:00', '09:00:00', '2025-01-01', 'personal', 'busy', 25),
(26, '06:45:00', '08:00:00', '2024-12-29', 'group', 'free', 25),
(27, '11:00:00', '12:00:00', '2025-01-01', 'group', 'free', 25),
(28, '10:00:00', '10:30:00', '2024-12-29', 'personal', 'busy', 25),
(29, '15:00:00', '16:00:00', '2024-12-29', 'both', 'free', 25),
(30, '07:00:00', '08:00:00', '2025-01-02', 'both', 'free', 25),
(31, '13:00:00', '14:00:00', '2024-12-28', 'personal', 'busy', 25),
(32, '14:05:00', '15:00:00', '2024-12-28', 'personal', 'free', 25),
(33, '09:00:00', '10:00:00', '2025-01-02', 'group', 'free', 25),
(34, '15:15:00', '16:15:00', '2024-12-28', 'personal', 'busy', 25),
(35, '15:00:00', '16:00:00', '2025-01-02', 'personal', 'busy', 25),
(36, '06:30:00', '07:30:00', '2024-12-27', 'group', 'free', 25),
(37, '08:00:00', '09:00:00', '2024-12-27', 'personal', 'free', 25),
(38, '10:00:00', '11:45:00', '2024-12-27', 'personal', 'free', 25),
(39, '10:20:00', '11:00:00', '2024-12-26', 'personal', 'busy', 25),
(40, '14:00:00', '15:00:00', '2024-12-26', 'personal', 'free', 25),
(41, '08:00:00', '09:00:00', '2025-01-03', 'group', 'free', 25),
(42, '08:00:00', '09:00:00', '2024-12-25', 'group', 'free', 25),
(43, '14:00:00', '15:00:00', '2024-12-25', 'personal', 'free', 25),
(44, '12:00:00', '13:00:00', '2025-01-03', 'personal', 'free', 25),
(45, '09:30:00', '11:00:00', '2024-12-12', 'group', 'free', 25),
(46, '08:00:00', '09:00:00', '2025-01-05', 'personal', 'free', 25),
(47, '08:00:00', '09:00:00', '2024-12-24', 'personal', 'free', 25),
(48, '14:00:00', '15:15:00', '2024-12-24', 'personal', 'free', 25),
(49, '14:00:00', '15:00:00', '2025-01-05', 'group', 'free', 25),
(50, '11:00:00', '12:00:00', '2025-05-08', 'both', 'free', 25),
(51, '08:00:00', '09:00:00', '2025-01-08', 'group', 'free', 25),
(52, '08:00:00', '09:00:00', '2025-01-12', 'personal', 'free', 25);

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `id` bigint UNSIGNED NOT NULL,
  `username` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `password` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `role` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'student',
  `first_name` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `last_name` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`id`, `username`, `password`, `role`, `first_name`, `last_name`) VALUES
(1, 'haha', '123', 'student', 'Viet', 'Hoan'),
(2, 'haha', '123', 'student', 'Viet', 'Hoan'),
(3, 'haha', '123', 'student', 'Viet oxiro', 'Hoan'),
(4, 'haha123', '123', 'student', 'Viet ox', 'Hoan oy'),
(5, 'kuwado', '123', 'student', 'Tanashi', 'Kuwado'),
(6, 'giaoviencu', '123456', 'teacher', 'Giao', 'Vien'),
(7, 'giaovien23', '123', 'teacher', 'Giao', 'Vien2'),
(8, 'giaovienmoi', '123', 'teacher', 'giao', 'vien'),
(9, 'giaovien22', '123', 'teacher', 'le', 'le'),
(10, 'giaovien123', '123', 'teacher', 'll', 'ss'),
(11, 'giaovien1234', '123', 'teacher', 'giao', 'vien'),
(12, 'vhvh', '123', 'student', 'll', 'vv'),
(13, 'ha189', '123', 'teacher', 'cao', 'ha'),
(14, 'hanoi123', '123', 'teacher', 'cao', 'hoan'),
(15, 'haha125', '123', 'student', 'nam', 'ha'),
(16, 'hoannuheoia', '123', 'teacher', 'hoan', 'liu'),
(17, 'hoanhuiraiahu', '123', 'teacher', 'haon', 'uho'),
(18, 'giaouhaiuh', '123456', 'student', 'ghuah', 'faifhoia'),
(19, 'hrioaejo', '123', 'teacher', 'uugh', 'uhryei'),
(20, 'jj', '123\'', 'student', '2s', 'dd'),
(21, 'hoaohfuorha', '123', 'teacher', 'hyeiw', 'hriha'),
(22, 'hahaih', '123456', 'student', 'nam', 'ha'),
(23, 'rhaf', 'uih', 'teacher', 'fvhi', 'ẳfeư'),
(24, 'fgaiuuifa', 'rdidue', 'teacher', 'reghhi', 'rua'),
(25, 'giaovien', '123456', 'teacher', 'Giao', 'Vien'),
(26, 'sinhvien', '123456', 'student', 'Hai', 'Lam'),
(27, 'sinhvien1', '123456', 'student', 'Luu', 'Viet Hoan'),
(28, 'sinhvien3', '123456', 'student', 'Nguyen', 'Giang'),
(29, 'sinhvien2', '123456', 'student', 'Le', 'Van Sinh'),
(30, 'sinhvien4', '123456', 'student', 'Hai', 'San'),
(31, 'giaovien1', '123456', 'teacher', 'Co', 'Linh'),
(32, 'giaovien2', '123456', 'teacher', 'Co', 'Hoan');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `attendances`
--
ALTER TABLE `attendances`
  ADD UNIQUE KEY `id` (`id`),
  ADD KEY `fk_attendances_student_id_users` (`student_id`),
  ADD KEY `fk_attendances_meeting_id_meetings` (`meeting_id`);

--
-- Indexes for table `meetings`
--
ALTER TABLE `meetings`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`),
  ADD KEY `fk_meetings_timeslot_id_users` (`timeslot_id`);

--
-- Indexes for table `timeslots`
--
ALTER TABLE `timeslots`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`),
  ADD KEY `fk_teacher_id_users` (`teacher_id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `attendances`
--
ALTER TABLE `attendances`
  MODIFY `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=45;

--
-- AUTO_INCREMENT for table `meetings`
--
ALTER TABLE `meetings`
  MODIFY `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;

--
-- AUTO_INCREMENT for table `timeslots`
--
ALTER TABLE `timeslots`
  MODIFY `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=53;

--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `attendances`
--
ALTER TABLE `attendances`
  ADD CONSTRAINT `fk_attendances_meeting_id_meetings` FOREIGN KEY (`meeting_id`) REFERENCES `meetings` (`id`) ON DELETE CASCADE ON UPDATE RESTRICT,
  ADD CONSTRAINT `fk_attendances_student_id_users` FOREIGN KEY (`student_id`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE RESTRICT;

--
-- Constraints for table `meetings`
--
ALTER TABLE `meetings`
  ADD CONSTRAINT `fk_meetings_timeslot_id_users` FOREIGN KEY (`timeslot_id`) REFERENCES `timeslots` (`id`) ON DELETE CASCADE ON UPDATE RESTRICT;

--
-- Constraints for table `timeslots`
--
ALTER TABLE `timeslots`
  ADD CONSTRAINT `fk_teacher_id_users` FOREIGN KEY (`teacher_id`) REFERENCES `users` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
