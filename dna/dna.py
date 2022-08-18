import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py your_database.csv sequence_to_check.txt")
        sys.exit(1)
    # TODO: Read database file into a variable
    database = []
    STR_list = []
    test_results = {}
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            # print(f"Row: {row}")
            database.append(dict(row))
        for key, value in database[0].items():
            if key != "name":
                STR_list.append(key)
                test_results[key] = 0
    # print(STR_list)
    # print(f"Database: {database}\n")
    # TODO: Read DNA sequence file into a variable
    dna_seq = ""
    with open(sys.argv[2], "r") as file:
        reader = csv.reader(file)
        for row in reader:
            dna_seq += row[0]
    # print(dna_seq)

    # TODO: Find longest match of each STR in DNA sequence
    for seq in STR_list:
        result = longest_match(dna_seq, seq)
        test_results[seq] = str(result)

    # TODO: Check database for matching profiles
    match = False
    matched_person = ""
    for person in database:
        # print(f"Person: {person}\nTest: {test_results}")
        for seq in STR_list:
            # print(seq)
            if person[seq] != test_results[seq]:
                match = False
                break
            match = True
            matched_person = person["name"]
        if match:
            print(matched_person)
            break
    if match == False:
        print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
