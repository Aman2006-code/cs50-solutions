-- Keep a log of any SQL queries you execute as you solve the mystery.


-- Understanding the database --
.table
.schema crime_scene_reports
.schema interviews
.schema bakery_security_logs
.schema people
.schema atm_transactions
.schema bank_accounts
.schema flights
.schema passengers
.schema airports
.schema phone_calls


-- Crime scene reports --
-- Seeking description of the crime


SELECT description FROM crime_scene_reports 
WHERE year = 2024
  AND month = 7
  AND day = 28 
  AND street = 'Humphrey Street';
-- NOTICE : Crime occured at 10:15 am; Interviews were taken.



-- Interviews --
-- Checking all the interviews on that day

SELECT location, description FROM crime_scene_reports 
WHERE year = 2024
  AND month = 7
  AND day = 28;

-- sorting all relevant interviews from the list.

SELECT id, transcript FROM interviews 
WHERE transcript LIKE '%I saw the thief get into a car in the bakery parking lot and drive away.%'
   OR transcript LIKE '% saw the thief there withdrawing some money%'
   OR transcript LIKE '%I heard the thief say that they were planning to take the earliest flight out of Fiftyville%'
-- These are the interviews that may take our case forward


-- Bakery Parking lot --
-- Every car exited from 10:00 to 11:00

SELECT * FROM bakery_security_logs 
WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit';
-- All cars left within 10 minitues of crime are noted(i.e, from 10:15 to 10:25) as per the testimonial.


-- Plate identification --

SELECT * FROM people
WHERE license_plate IN ('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55');


-- ATM Analysis --
-- Withdrawal from Leggett Street in the morning as per testimonial

SELECT account_number FROM atm_transactions 
WHERE year = 2024 AND month = 7 AND day = 28 
AND atm_location = 'Leggett Street' 
AND transaction_type = 'withdraw';

-- Phone call analysis --
-- Had a less than a minitue call after crime mentioned about next day morning first flight


SELECT name FROM people
WHERE phone_number IN (
    SELECT caller FROM phone_calls
    WHERE year = 2024 AND month = 7 AND day = 28 AND duration < 70
);


-- Preparing prime suspect list --

SELECT name FROM people
WHERE id IN (
    SELECT id FROM people 
    WHERE license_plate IN ('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55')
)
AND id IN (
    SELECT person_id FROM bank_accounts    
    WHERE account_number IN (
        SELECT account_number FROM atm_transactions 
        WHERE year = 2024 AND month = 7 AND day = 28
        AND atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
    )
)
AND name IN (
    SELECT name FROM people
    WHERE phone_number IN (
        SELECT caller FROM phone_calls
        WHERE year = 2024 AND month = 7 AND day = 28 AND duration < 70
    )
);
--Shortlisted two


-- Passport activity --


SELECT name, passport_number FROM people 
WHERE name IN ('Diana', 'Bruce');
-- Got passport number
SELECT flight_id FROM passengers
WHERE passport_number IN ('3592750733', '5773159633');
-- Got flight id
SELECT * FROM flights
WHERE year = 2024 AND month = 7 AND day = 29 
AND id IN ('18', '24', '36', '54');
-- Found the early morning flight and its destination code
SELECT city FROM airports
WHERE id = 4;
-- Found the destination city
SELECT name
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)
AND passport_number IN ('3592750733', '5773159633');
-- Found the theif.


-- Phone call analysis -- 
-- Theif made a less than a minitue call with his accomplice to facilitate his journey next day after theft

-- Number of thief
SELECT phone_number FROM people
WHERE name = 'Bruce';

-- Number of accomplice
SELECT receiver 
FROM phone_calls
WHERE caller = '(367) 555-5533'
  AND year = 2024
  AND month = 7
  AND day = 28
  AND duration < 60;

-- Name of accomplice 
SELECT name 
FROM people 
WHERE phone_number = '(375) 555-8161';

-- FOUND THE THIEF, CITY HE ESCAPED TO AND HIS ACCOMPLICE





