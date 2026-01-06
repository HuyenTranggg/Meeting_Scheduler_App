-- ============================================
-- Meeting Scheduler Database Schema
-- Database: ltm
-- ============================================

-- Tạo database
CREATE DATABASE IF NOT EXISTS ltm;
USE ltm;

-- ============================================
-- Table: users
-- Mô tả: Lưu thông tin người dùng (sinh viên và giáo viên)
-- ============================================
CREATE TABLE IF NOT EXISTS users (
    id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    role ENUM('student', 'teacher') NOT NULL,
    first_name VARCHAR(50),
    last_name VARCHAR(50),
    INDEX idx_username (username),
    INDEX idx_role (role)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ============================================
-- Table: timeslots
-- Mô tả: Lưu các khung giờ rảnh của giáo viên
-- ============================================
CREATE TABLE IF NOT EXISTS timeslots (
    id INT PRIMARY KEY AUTO_INCREMENT,
    start TIME NOT NULL,
    end TIME NOT NULL,
    date DATE NOT NULL,
    type ENUM('personal', 'group', 'both') NOT NULL,
    status ENUM('free', 'busy') NOT NULL DEFAULT 'free',
    teacher_id INT NOT NULL,
    FOREIGN KEY (teacher_id) REFERENCES users(id) ON DELETE CASCADE,
    INDEX idx_teacher_date (teacher_id, date),
    INDEX idx_status (status)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ============================================
-- Table: meetings
-- Mô tả: Lưu thông tin cuộc họp
-- ============================================
CREATE TABLE IF NOT EXISTS meetings (
    id INT PRIMARY KEY AUTO_INCREMENT,
    timeslot_id INT NOT NULL,
    status ENUM('pending', 'confirmed', 'completed', 'cancelled') NOT NULL DEFAULT 'pending',
    type ENUM('personal', 'group') NOT NULL,
    report TEXT,
    original_type ENUM('personal', 'group', 'both') DEFAULT 'both',
    FOREIGN KEY (timeslot_id) REFERENCES timeslots(id) ON DELETE CASCADE,
    INDEX idx_timeslot (timeslot_id),
    INDEX idx_status (status)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ============================================
-- Table: attendances
-- Mô tả: Lưu danh sách sinh viên tham dự cuộc họp
-- ============================================
CREATE TABLE IF NOT EXISTS attendances (
    id INT PRIMARY KEY AUTO_INCREMENT,
    meeting_id INT NOT NULL,
    student_id INT NOT NULL,
    FOREIGN KEY (meeting_id) REFERENCES meetings(id) ON DELETE CASCADE,
    FOREIGN KEY (student_id) REFERENCES users(id) ON DELETE CASCADE,
    UNIQUE KEY unique_attendance (meeting_id, student_id),
    INDEX idx_meeting (meeting_id),
    INDEX idx_student (student_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ============================================
-- Hiển thị cấu trúc các bảng
-- ============================================
SHOW TABLES;
DESC users;
DESC timeslots;
DESC meetings;
DESC attendances;
