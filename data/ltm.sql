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
-- Students (100-129)
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
(110, 'trungvank', 'pass123', 'student', 'Van K', 'Trung'),
(111, 'lamthil', 'pass123', 'student', 'Thi L', 'Lam'),
(112, 'quangvanm', 'pass123', 'student', 'Van M', 'Quang'),
(113, 'hienthinn', 'pass123', 'student', 'Thi N', 'Hien'),
(114, 'tuanvano', 'pass123', 'student', 'Van O', 'Tuan'),
(115, 'linhthip', 'pass123', 'student', 'Thi P', 'Linh'),
(116, 'hungvanq', 'pass123', 'student', 'Van Q', 'Hung'),
(117, 'thuthir', 'pass123', 'student', 'Thi R', 'Thu'),
(118, 'haivans', 'pass123', 'student', 'Van S', 'Hai'),
(119, 'anhthit', 'pass123', 'student', 'Thi T', 'Anh'),
(120, 'binvanu', 'pass123', 'student', 'Van U', 'Binh'),
(121, 'dieuthiv', 'pass123', 'student', 'Thi V', 'Dieu'),
(122, 'khoavanw', 'pass123', 'student', 'Van W', 'Khoa'),
(123, 'myxthiy', 'pass123', 'student', 'Thi Y', 'My'),
(124, 'datvanz', 'pass123', 'student', 'Van Z', 'Dat'),
(125, 'student01', 'pass123', 'student', 'Student', 'One'),
(126, 'student02', 'pass123', 'student', 'Student', 'Two'),
(127, 'student03', 'pass123', 'student', 'Student', 'Three'),
(128, 'student04', 'pass123', 'student', 'Student', 'Four'),
(129, 'student05', 'pass123', 'student', 'Student', 'Five'),
(130, 'student_new', 'pass123', 'student', 'New', 'Student'),
-- Teachers (150-160)
(150, 'teacher_nguyen', 'teacher123', 'teacher', 'Minh', 'Nguyen Thi'),
(151, 'teacher_tran', 'teacher123', 'teacher', 'Hoa', 'Tran Van'),
(152, 'teacher_le', 'teacher123', 'teacher', 'Lan', 'Le Thi'),
(153, 'teacher_pham', 'teacher123', 'teacher', 'Nam', 'Pham Van'),
(154, 'teacher_hoang', 'teacher123', 'teacher', 'Hung', 'Hoang Van'),
(155, 'teacher_vo', 'teacher123', 'teacher', 'Thanh', 'Vo Thi'),
(156, 'teacher_dang', 'teacher123', 'teacher', 'Long', 'Dang Van'),
(157, 'teacher_bui', 'teacher123', 'teacher', 'Mai', 'Bui Thi'),
(158, 'teacher_do', 'teacher123', 'teacher', 'Tuan', 'Do Van'),
(159, 'teacher_ngo', 'teacher123', 'teacher', 'Linh', 'Ngo Thi'),
(160, 'teacher_trang', 'teacher123', 'teacher', 'Trang', 'Nguyen');

--
-- Dumping data for table timeslots
--

INSERT INTO timeslots (id, start, end, date, type, status, teacher_id) VALUES
-- Teacher 150: Past timeslots (completed/cancelled meetings)
(200, '08:00:00', '09:30:00', '2025-12-15', 'personal', 'busy', 150),
(201, '10:00:00', '11:30:00', '2025-12-15', 'group', 'free', 150),  -- ⭐ group luôn free
(202, '13:00:00', '14:30:00', '2025-12-15', 'both', 'busy', 150),
(203, '15:00:00', '16:30:00', '2025-12-16', 'personal', 'free', 150),
(204, '08:30:00', '10:00:00', '2025-12-20', 'group', 'free', 150),
-- Teacher 150: Current week (various statuses for testing)
(205, '08:00:00', '09:30:00', '2026-01-07', 'personal', 'free', 150),
(206, '10:00:00', '11:30:00', '2026-01-07', 'group', 'free', 150),
(207, '13:00:00', '14:30:00', '2026-01-07', 'both', 'busy', 150),
(208, '15:00:00', '16:30:00', '2026-01-07', 'personal', 'busy', 150),
(209, '08:00:00', '09:30:00', '2026-01-07', 'group', 'free', 150),  -- ⭐ group luôn free
(210, '10:00:00', '11:30:00', '2026-01-07', 'personal', 'busy', 150),
(211, '13:00:00', '14:30:00', '2026-01-07', 'both', 'free', 150),
(212, '15:00:00', '16:30:00', '2026-01-07', 'group', 'free', 150),
-- Teacher 150: Future slots (for booking tests)
(213, '08:00:00', '09:30:00', '2026-01-13', 'personal', 'free', 150),
(214, '10:00:00', '11:30:00', '2026-01-13', 'group', 'free', 150),
(215, '13:00:00', '14:30:00', '2026-01-13', 'both', 'free', 150),
(216, '15:00:00', '16:30:00', '2026-01-13', 'personal', 'free', 150),
(217, '08:00:00', '09:30:00', '2026-01-14', 'group', 'free', 150),
(218, '10:00:00', '11:30:00', '2026-01-14', 'both', 'free', 150),
(219, '13:00:00', '14:30:00', '2026-01-14', 'personal', 'free', 150),
(220, '15:00:00', '16:30:00', '2026-01-14', 'group', 'free', 150),
-- Teacher 151: Various timeslots
(221, '08:30:00', '10:00:00', '2025-12-18', 'personal', 'busy', 151),
(222, '10:30:00', '12:00:00', '2025-12-18', 'group', 'free', 151),  -- ⭐ group luôn free
(223, '13:30:00', '15:00:00', '2026-01-08', 'both', 'busy', 151),
(224, '15:30:00', '17:00:00', '2026-01-08', 'personal', 'busy', 151),
(225, '08:30:00', '10:00:00', '2026-01-09', 'group', 'free', 151),
(226, '10:30:00', '12:00:00', '2026-01-09', 'personal', 'free', 151),
(227, '13:30:00', '15:00:00', '2026-01-09', 'both', 'free', 151),
(228, '15:30:00', '17:00:00', '2026-01-09', 'group', 'free', 151),
(229, '08:30:00', '10:00:00', '2026-01-15', 'personal', 'free', 151),
(230, '10:30:00', '12:00:00', '2026-01-15', 'group', 'free', 151),
(231, '13:30:00', '15:00:00', '2026-01-15', 'both', 'free', 151),
-- Teacher 152: Various timeslots
(232, '09:00:00', '10:30:00', '2025-12-19', 'group', 'free', 152),  -- ⭐ group luôn free
(233, '11:00:00', '12:30:00', '2025-12-19', 'personal', 'busy', 152),
(234, '14:00:00', '15:30:00', '2026-01-10', 'both', 'busy', 152),
(235, '16:00:00', '17:30:00', '2026-01-10', 'personal', 'busy', 152),
(236, '09:00:00', '10:30:00', '2026-01-11', 'group', 'free', 152),
(237, '11:00:00', '12:30:00', '2026-01-11', 'personal', 'free', 152),
(238, '14:00:00', '15:30:00', '2026-01-11', 'both', 'free', 152),
(239, '16:00:00', '17:30:00', '2026-01-11', 'group', 'free', 152),
(240, '09:00:00', '10:30:00', '2026-01-16', 'personal', 'free', 152),
(241, '11:00:00', '12:30:00', '2026-01-16', 'both', 'free', 152),
-- Teacher 153: Various timeslots
(242, '08:00:00', '09:30:00', '2025-12-21', 'personal', 'busy', 153),
(243, '10:00:00', '11:30:00', '2025-12-21', 'group', 'free', 153),  -- ⭐ group luôn free
(244, '13:00:00', '14:30:00', '2026-01-12', 'both', 'busy', 153),
(245, '15:00:00', '16:30:00', '2026-01-12', 'group', 'free', 153),
(246, '08:00:00', '09:30:00', '2026-01-13', 'personal', 'free', 153),
(247, '10:00:00', '11:30:00', '2026-01-13', 'both', 'free', 153),
(248, '13:00:00', '14:30:00', '2026-01-17', 'group', 'free', 153),
(249, '15:00:00', '16:30:00', '2026-01-17', 'personal', 'free', 153),
-- Teacher 154: Various timeslots
(250, '08:30:00', '10:00:00', '2025-12-22', 'group', 'free', 154),  -- ⭐ group luôn free
(251, '10:30:00', '12:00:00', '2026-01-08', 'personal', 'busy', 154),
(252, '13:30:00', '15:00:00', '2026-01-08', 'both', 'free', 154),
(253, '15:30:00', '17:00:00', '2026-01-08', 'group', 'free', 154),
(254, '08:30:00', '10:00:00', '2026-01-18', 'personal', 'free', 154),
(255, '10:30:00', '12:00:00', '2026-01-18', 'both', 'free', 154),
-- Teacher 155: Various timeslots
(256, '09:00:00', '10:30:00', '2025-12-23', 'personal', 'busy', 155),
(257, '11:00:00', '12:30:00', '2026-01-09', 'group', 'free', 155),  -- ⭐ group luôn free
(258, '14:00:00', '15:30:00', '2026-01-09', 'both', 'free', 155),
(259, '16:00:00', '17:30:00', '2026-01-09', 'personal', 'free', 155),
(260, '09:00:00', '10:30:00', '2026-01-19', 'group', 'free', 155),
(261, '11:00:00', '12:30:00', '2026-01-19', 'both', 'free', 155),
-- Teacher 156: Various timeslots
(262, '08:00:00', '09:30:00', '2026-01-10', 'personal', 'busy', 156),
(263, '10:00:00', '11:30:00', '2026-01-10', 'group', 'free', 156),
(264, '13:00:00', '14:30:00', '2026-01-10', 'both', 'free', 156),
(265, '08:00:00', '09:30:00', '2026-01-20', 'personal', 'free', 156),
(266, '10:00:00', '11:30:00', '2026-01-20', 'group', 'free', 156),
-- Teacher 157: Various timeslots
(267, '08:30:00', '10:00:00', '2026-01-11', 'both', 'busy', 157),
(268, '10:30:00', '12:00:00', '2026-01-11', 'personal', 'free', 157),
(269, '08:30:00', '10:00:00', '2026-01-21', 'group', 'free', 157),
(270, '10:30:00', '12:00:00', '2026-01-21', 'both', 'free', 157),
-- Teacher 158: Various timeslots
(271, '09:00:00', '10:30:00', '2026-01-12', 'personal', 'busy', 158),
(272, '11:00:00', '12:30:00', '2026-01-12', 'group', 'free', 158),
(273, '09:00:00', '10:30:00', '2026-01-22', 'both', 'free', 158),
(274, '11:00:00', '12:30:00', '2026-01-22', 'personal', 'free', 158),
-- Teacher 159: Various timeslots
(275, '08:00:00', '09:30:00', '2026-01-13', 'group', 'free', 159),  -- ⭐ group luôn free
(276, '10:00:00', '11:30:00', '2026-01-13', 'personal', 'free', 159),
(277, '08:00:00', '09:30:00', '2026-01-23', 'both', 'free', 159),
(278, '10:00:00', '11:30:00', '2026-01-23', 'group', 'free', 159);

--
-- Dumping data for table meetings
--

INSERT INTO meetings (id, timeslot_id, status, type, report, original_type) VALUES
-- ============================================================
-- DEMO DATA FOR STUDENT 100 (nguyenvana) & TEACHER 150 (teacher_nguyen)
-- ============================================================

-- COMPLETED meetings - Student 100 với Teacher 150 (past history)
(300, 200, 'completed', 'personal', 'Cuộc họp đã hoàn thành tốt. Sinh viên 100 đã trình bày đề tài về Machine Learning và nhận được nhiều đóng góp hữu ích từ giáo viên 150.', 'personal'),
(301, 201, 'completed', 'group', 'Họp nhóm dự án Web Development. Student 100 và team đã phân công công việc rõ ràng cho sprint tiếp theo.', 'group'),
(302, 202, 'completed', 'personal', 'Tư vấn cá nhân về đồ án tốt nghiệp. Student 100 cần hoàn thiện phần thiết kế database.', 'both'),

-- CONFIRMED meetings - Student 100 với Teacher 150 (upcoming)
(313, 207, 'confirmed', 'personal', NULL, 'both'),
(314, 208, 'confirmed', 'personal', NULL, 'personal'),

-- PENDING meetings - Student 100 với Teacher 150 (waiting confirmation)
(328, 211, 'pending', 'group', NULL, 'both'),
(329, 212, 'pending', 'personal', NULL, 'group'),

-- CANCELLED meetings - Student 100 với Teacher 150 (history)
(346, 205, 'cancelled', 'personal', NULL, 'personal'),
(347, 206, 'cancelled', 'group', NULL, 'group'),

-- ============================================================
-- Meetings của các students khác với Teacher 150 (để teacher có data đầy đủ)
-- ============================================================
(303, 203, 'completed', 'personal', 'Meeting với student 101 completed.', 'personal'),
(304, 221, 'completed', 'group', 'Group meeting với students 102,103,104 completed.', 'personal'),
(315, 209, 'confirmed', 'group', NULL, 'group'),
(316, 210, 'confirmed', 'personal', NULL, 'personal'),
(330, 225, 'pending', 'group', NULL, 'group'),
(331, 226, 'pending', 'personal', NULL, 'personal');

--
-- Dumping data for table attendances
--

INSERT INTO attendances (id, meeting_id, student_id) VALUES
-- ============================================================
-- ATTENDANCES FOR STUDENT 100 (nguyenvana) DEMO
-- ============================================================

-- Completed meetings - Student 100 với Teacher 150
(400, 300, 100),  -- personal completed
(401, 301, 100), (402, 301, 101), (403, 301, 102),  -- group completed (student 100 + 2 others)
(404, 302, 100),  -- personal completed

-- Confirmed meetings - Student 100 với Teacher 150
(431, 313, 100),  -- personal confirmed (upcoming)
(432, 314, 100),  -- personal confirmed (upcoming)

-- Pending meetings - Student 100 với Teacher 150
(459, 328, 100), (460, 328, 101), (461, 328, 102),  -- group pending (student 100 + 2 others)
(462, 329, 100),  -- personal pending

-- Cancelled meetings - Student 100 với Teacher 150
(498, 346, 100),  -- personal cancelled (history)
(499, 347, 100), (500, 347, 101),  -- group cancelled (history)

-- ============================================================
-- ATTENDANCES FOR OTHER STUDENTS (để teacher 150 có đầy đủ data)
-- ============================================================
(405, 303, 101),  -- completed
(406, 304, 102), (407, 304, 103), (408, 304, 104),  -- group completed
(433, 315, 103), (434, 315, 104), (435, 315, 105),  -- group confirmed
(436, 316, 106),  -- confirmed
(466, 330, 107), (467, 330, 108),  -- group pending
(468, 331, 109);  -- pending
