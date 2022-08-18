-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Get my bearings with the crime description
SELECT * FROM crime_scene_reports WHERE street = "Humphrey Street";

-- Look at interviews from the day of the theft
SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28;

-- One interview said to check parking lot security cameras within 10 mins of the theft (10:15am)
SELECT * FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 26;

-- Another interview said to look into phone logs as thief called someone right after the theft and the call was less than a minute long
SELECT * FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

-- Look for people with a caller phone number and license plate that match the previous two query results
SELECT * FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 26);

-- Need to narrow results down more (4 people left), need to check ATM transactions for more evidence
SELECT * FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";

-- Search bank accounts with the list of people from the phone and license plate check
SELECT * FROM bank_accounts JOIN people ON bank_accounts.person_id = people.id WHERE bank_accounts.person_id IN (SELECT id FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 26));

-- Suspect list is narrowed to just 2 people (Bruce, Diana) - Need to check passport numbers with flights the next day. First, need info about airports and flights
SELECT * FROM airports;

--Fiftyville airport id = 8 (abbr = CSF, nice one hehe)
SELECT * FROM flights WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour;

--Earliest Flight out of Fiftyville is headed to airport 4, which is LGA with a flight ID of 36. Now let's search the passengers
SELECT * FROM passengers WHERE flight_id = 36;

--Now compare the passport numbers of our passengers with the previous narrow list of 3 suspects (phone, license, and atm info)
SELECT * FROM bank_accounts JOIN people ON bank_accounts.person_id = people.id WHERE bank_accounts.person_id IN (SELECT id FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 26)) AND passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36);

--FOUND THE THIEF! Bruce is the Culprit and he was flying to LGA (New York)! Now to find his accomplice
SELECT name FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE caller IN (SELECT phone_number FROM people WHERE name = "Bruce") AND year = 2021 AND month = 7 AND day = 28 AND duration < 60);

-- GOT IT! The Accomplice was Robin!

---------------------
--Crime Report
--| 295 | 2021 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
--
---------------------
--Witnesses
--| 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
--| 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
--| 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.' |
--
--------------------
--Parking Lot
--Possible License Plates for Thief
--+-----+------+-------+-----+------+--------+----------+---------------+
--| id  | year | month | day | hour | minute | activity | license_plate |
--+-----+------+-------+-----+------+--------+----------+---------------+
--| 260 | 2021 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
--| 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
--| 262 | 2021 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
--| 263 | 2021 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
--| 264 | 2021 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
--| 265 | 2021 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
--| 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
--| 267 | 2021 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
--| 268 | 2021 | 7     | 28  | 10   | 35     | exit     | 1106N58
--
---------------------
--Call Logs
--+-----+----------------+----------------+------+-------+-----+----------+
--| id  |     caller     |    receiver    | year | month | day | duration |
--+-----+----------------+----------------+------+-------+-----+----------+
--| 221 | (130) 555-0289 | (996) 555-8899 | 2021 | 7     | 28  | 51       |
--| 224 | (499) 555-9472 | (892) 555-8872 | 2021 | 7     | 28  | 36       |
--| 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
--| 251 | (499) 555-9472 | (717) 555-1342 | 2021 | 7     | 28  | 50       |
--| 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       |
--| 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       |
--| 261 | (031) 555-6622 | (910) 555-3251 | 2021 | 7     | 28  | 38       |
--| 279 | (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       |
--| 281 | (338) 555-6650 | (704) 555-2131 | 2021 | 7     | 28  | 54
--
--------------
--Phone Caller Number and License Plate Results
--+--------+--------+----------------+-----------------+---------------+
--|   id   |  name  |  phone_number  | passport_number | license_plate |
--+--------+--------+----------------+-----------------+---------------+
--| 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
--| 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
--| 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       |
--| 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
--| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X
--
----------------
--ATM Withdraw Info
--+-----+----------------+------+-------+-----+----------------+------------------+--------+
--| id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
--+-----+----------------+------+-------+-----+----------------+------------------+--------+
--| 246 | 28500762       | 2021 | 7     | 28  | Leggett Street | withdraw         | 48     |
--| 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
--| 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     |
--| 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     |
--| 269 | 16153065       | 2021 | 7     | 28  | Leggett Street | withdraw         | 80     |
--| 288 | 25506511       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
--| 313 | 81061156       | 2021 | 7     | 28  | Leggett Street | withdraw         | 30     |
--| 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     |
--
------------------------
--Call logs, License Plates, and ATM transaction Match
--+----------------+-----------+---------------+--------+--------+----------------+-----------------+---------------+
--| account_number | person_id | creation_year |   id   |  name  |  phone_number  | passport_number | license_plate |
--+----------------+-----------+---------------+--------+--------+----------------+-----------------+---------------+
--| 49610011       | 686048    | 2010          | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
--| 26013199       | 514354    | 2012          | 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       |
--| 76054385       | 449774    | 2015          | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
--+----------------+-----------+---------------+--------+--------+----------------+-----------------+---------------+
--
-----------------
--Airports
--+----+--------------+-----------------------------------------+---------------+
--| id | abbreviation |                full_name                |     city      |
--+----+--------------+-----------------------------------------+---------------+
--| 1  | ORD          | O'Hare International Airport            | Chicago       |'
--| 2  | PEK          | Beijing Capital International Airport   | Beijing       |
--| 3  | LAX          | Los Angeles International Airport       | Los Angeles   |
--| 4  | LGA          | LaGuardia Airport                       | New York City |
--| 5  | DFS          | Dallas/Fort Worth International Airport | Dallas        |
--| 6  | BOS          | Logan International Airport             | Boston        |
--| 7  | DXB          | Dubai International Airport             | Dubai         |
--| 8  | CSF          | Fiftyville Regional Airport             | Fiftyville    |
--| 9  | HND          | Tokyo International Airport             | Tokyo         |
--| 10 | CDG          | Charles de Gaulle Airport               | Paris         |
--| 11 | SFO          | San Francisco International Airport     | San Francisco |
--| 12 | DEL          | Indira Gandhi International Airport     | Delhi         |
--
-----------------
--Flights the next day ordered so earliest is first
--+----+-------------------+------------------------+------+-------+-----+------+--------+
--| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
--+----+-------------------+------------------------+------+-------+-----+------+--------+
--| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
--| 43 | 8                 | 1                      | 2021 | 7     | 29  | 9    | 30     |
--| 23 | 8                 | 11                     | 2021 | 7     | 29  | 12   | 15     |
--| 53 | 8                 | 9                      | 2021 | 7     | 29  | 15   | 20     |
--| 18 | 8                 | 6                      | 2021 | 7     | 29  | 16   | 0      |
--+----+-------------------+------------------------+------+-------+-----+------+--------+
--
---------------
--Passenger Info for Earliest FLight out of Fiftyville
--+-----------+-----------------+------+
--| flight_id | passport_number | seat |
--+-----------+-----------------+------+
--| 36        | 7214083635      | 2A   |
--| 36        | 1695452385      | 3B   |
--| 36        | 5773159633      | 4A   |
--| 36        | 1540955065      | 5C   |
--| 36        | 8294398571      | 6C   |
--| 36        | 1988161715      | 6D   |
--| 36        | 9878712108      | 7A   |
--| 36        | 8496433585      | 7B   |
--+-----------+-----------------+------+
--
---------------
--On Flight 36 and Other matching data
--+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+
--| account_number | person_id | creation_year |   id   | name  |  phone_number  | passport_number | license_plate |
--+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+
--| 49610011       | 686048    | 2010          | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
--+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+
--
--Accomplice
--+-------+
--| name  |
--+-------+
--| Robin |
--+-------+