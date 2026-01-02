-- Migration: Add status field to attendances table
-- Purpose: Allow teachers to accept/reject individual students in group meetings

USE ltm;

-- Add status column to attendances table
ALTER TABLE attendances 
ADD COLUMN status ENUM('pending', 'confirmed', 'rejected') DEFAULT 'pending' AFTER student_id;

-- Update existing records with default value 'pending'
UPDATE attendances SET status = 'pending' WHERE status IS NULL;

-- Display the updated structure
DESC attendances;
