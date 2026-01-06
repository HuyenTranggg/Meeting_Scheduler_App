SET FOREIGN_KEY_CHECKS = 0;
TRUNCATE TABLE attendances;
TRUNCATE TABLE meetings;
TRUNCATE TABLE timeslots;
TRUNCATE TABLE users;
SET FOREIGN_KEY_CHECKS = 1;
-- Sample data for ltm database
-- Insert this data after creating the database structure
-- This file contains completely new sample data, different from existing data

USE ltm;

--
-- Dumping data for table users
--

INSERT INTO users (id, username, password, role, first_name, last_name) VALUES
(100, 'nguyenvana', 'pass123', 'student', 'Van A', 'Nguyen'),
(101, 'tranthib', 'pass123', 'student', 'Thi B', 'Tran'),
(102, 'levanc', 'pass123', 'student', 'Van C', 'Le'),
(103, 'phamthid', 'pass123', 'student', 'Thi D', 'Pham'),
(104, 'hoangvane', 'pass123', 'student', 'Van E', 'Hoang'),
(105, 'vuthif', 'pass123', 'student', 'Thi F', 'Vu'),
(106, 'dangvang', 'pass123', 'student', 'Van G', 'Dang'),
(107, 'buithih', 'pass123', 'student', 'Thi H', 'Bui'),
(108, 'dovani', 'pass123', 'student', 'Van I', 'Do'),
(109, 'ngothij', 'pass123', 'student', 'Thi J', 'Ngo'),
(110, 'teacher_nguyen', 'teacher123', 'teacher', 'AB', 'Nguyen Thi'),
(111, 'teacher_tran', 'teacher123', 'teacher', 'B', 'Tran Van'),
(112, 'teacher_le', 'teacher123', 'teacher', 'C', 'Le Thi'),
(113, 'teacher_pham', 'teacher123', 'teacher', 'D', 'Pham Van'),
(114, 'teacher_hoang', 'teacher123', 'teacher', 'H', 'Hoang Van');

--
-- Dumping data for table timeslots
--

INSERT INTO timeslots (id, start, end, date, type, status, teacher_id) VALUES
(200, '08:00:00', '09:30:00', '2026-01-15', 'personal', 'free', 110),
(201, '10:00:00', '11:30:00', '2026-01-15', 'group', 'free', 110),
(202, '13:00:00', '14:30:00', '2026-01-15', 'both', 'free', 110),
(203, '15:00:00', '16:30:00', '2025-01-15', 'personal', 'busy', 110),
(204, '08:30:00', '10:00:00', '2026-01-16', 'group', 'free', 111),
(205, '10:30:00', '12:00:00', '2025-01-16', 'personal', 'busy', 111),
(206, '13:30:00', '15:00:00', '2026-01-16', 'both', 'free', 111),
(207, '15:30:00', '17:00:00', '2026-01-16', 'group', 'free', 111),
(208, '09:00:00', '10:30:00', '2026-01-17', 'personal', 'free', 112),
(209, '11:00:00', '12:30:00', '2025-01-17', 'group', 'free', 112),
(210, '14:00:00', '15:30:00', '2026-01-17', 'personal', 'free', 112),
(211, '16:00:00', '17:30:00', '2026-01-17', 'both', 'free', 112),
(212, '08:00:00', '09:30:00', '2026-01-18', 'group', 'free', 113),
(213, '10:00:00', '11:30:00', '2025-01-18', 'personal', 'busy', 113),
(214, '13:00:00', '14:30:00', '2026-01-18', 'group', 'free', 113),
(215, '15:00:00', '16:30:00', '2026-01-18', 'both', 'free', 113),
(216, '08:30:00', '10:00:00', '2026-01-20', 'personal', 'free', 114),
(217, '10:30:00', '12:00:00', '2025-01-20', 'group', 'free', 114),
(218, '13:30:00', '15:00:00', '2026-01-20', 'personal', 'free', 114),
(219, '15:30:00', '17:00:00', '2025-01-20', 'both', 'free', 114),
(220, '09:00:00', '10:30:00', '2025-01-21', 'group', 'free', 110),
(221, '11:00:00', '12:30:00', '2025-01-21', 'personal', 'free', 110),
(222, '14:00:00', '15:30:00', '2025-01-21', 'both', 'free', 110),
(223, '16:00:00', '17:30:00', '2025-01-21', 'group', 'free', 110),
(224, '08:00:00', '09:30:00', '2025-01-22', 'personal', 'free', 111),
(225, '10:00:00', '11:30:00', '2025-01-22', 'group', 'free', 111),
(226, '13:00:00', '14:30:00', '2025-01-22', 'personal', 'busy', 111),
(227, '15:00:00', '16:30:00', '2025-01-22', 'both', 'free', 111);

--
-- Dumping data for table meetings
--

INSERT INTO meetings (id, timeslot_id, status, type, report, original_type) VALUES
(300, 203, 'completed', 'personal', 'Cuộc họp đã hoàn thành tốt. Sinh viên đã trình bày đề tài về Machine Learning và nhận được nhiều đóng góp hữu ích từ giáo viên.', 'personal'),
(301, 205, 'completed', 'personal', 'Thảo luận về đồ án tốt nghiệp. Sinh viên cần hoàn thiện phần thiết kế database và cải thiện giao diện người dùng.', 'personal'),
(302, 209, 'completed', 'group', 'Họp nhóm dự án Web Development. Các thành viên đã phân công công việc rõ ràng cho sprint tiếp theo.', 'group'),
(303, 213, 'completed', 'personal', 'Tư vấn về hướng nghiên cứu cho luận văn thạc sĩ. Đề xuất sinh viên tập trung vào Deep Learning cho xử lý ảnh y khoa.', 'personal'),
(304, 217, 'completed', 'group', 'Review tiến độ dự án Mobile App. Team đang đúng kế hoạch, cần tăng cường testing trong tuần tới.', 'group'),
(305, 222, 'completed', 'group', 'Hướng dẫn sinh viên về cách viết báo cáo khoa học. Đã cung cấp template và các tài liệu tham khảo cần thiết.', 'both'),
(306, 226, 'completed', 'personal', 'Thảo luận về đề tài nghiên cứu AI trong giáo dục. Sinh viên có ý tưởng tốt nhưng cần cụ thể hóa phương pháp nghiên cứu.', 'personal');

--
-- Dumping data for table attendances
--

INSERT INTO attendances (id, meeting_id, student_id) VALUES
(400, 300, 100),
(401, 301, 101),
(402, 302, 102),
(403, 302, 103),
(404, 302, 104),
(405, 303, 105),
(406, 304, 106),
(407, 304, 107),
(408, 304, 108),
(409, 305, 109),
(410, 305, 100),
(411, 306, 101);
