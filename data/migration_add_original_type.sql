-- Migration: Add original_type field to meetings table
-- Purpose: Store the original type of timeslot before booking to properly restore it on cancellation

USE ltm;

-- Add original_type column to meetings table
ALTER TABLE meetings 
ADD COLUMN original_type ENUM('personal', 'group', 'both') DEFAULT 'both' AFTER type;

-- Update existing records with default value 'both'
-- (This is a safe assumption for existing data)
UPDATE meetings SET original_type = 'both' WHERE original_type IS NULL;

-- Display the updated structure
DESC meetings;
