#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];



int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_cycle(int end_num, int start_num);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }
    //     for (int i = 0; i < (MAX * (MAX - 1) / 2); i++)
    //{
    //    printf("Pair%i: \nwinner: %i \nloser: %i\n", i, pairs[i].winner, pairs[i].loser);
    // }
    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        int choice = ranks[i];
        // Loops through all subsequent candidates in voter's ranks list
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[choice][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // COME BACK LATER AND TRY TO OPTIMIZE ALGORITHM TO MERGE SORT!!!!!!!
    int counter = 0;
    int pair_counter = 0;
    int k = 0;
    while (pairs[k].winner || pairs[k].loser)
    {
        k++;
        pair_counter++;
    }
    for (int i = 0; i < pair_counter - 1; i++)
    {
        pair x = pairs[i];
        pair y = pairs[i + 1];
        int x_strength = preferences[x.winner][x.loser];
        int y_strength = preferences[y.winner][y.loser];
        if (y_strength > x_strength)
        {
            pairs[i] = y;
            pairs[i + 1] = x;
            counter++;
        }
    }
    if (counter == 0)
    {
        return;
    }
}


//Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    int pair_counter = 0;
    int k = 0;
    while (pairs[k].winner || pairs[k].loser)
    {
        k++;
        pair_counter++;
    }
    // Loops through every pair in pairs
    for (int i = 0; i < pair_counter; i++)
    {
        bool not_cycle = true;
        int start_num = pairs[i].winner;
        int end_num = pairs[i].loser;
        if (check_cycle(end_num, start_num))
        {
            not_cycle = false;
        }
        if (not_cycle)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int left[candidate_count];
    int right[candidate_count];
    int counter = 0;
    int win_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                left[counter] = i;
                right[counter] = j;
                counter++;
            }
        }
    }
    for (int i = 0; i < counter; i++)
    {
        for (int j = 0; j < counter; j++)
        {
            if (left[i] == right[j])
            {
                left[i] = 100;
            }
        }
    }
    int winner;
    for (int i = 0; i < counter; i++)
    {
        if (left[i] < 100)
        {
            winner = left[i];
            win_count++;
        }
    }
    printf("%s\n", candidates[winner]);
    return;
}


bool check_cycle(int end_num, int start_num)
{
    // End num changes as we continue on the path, start num stays same (root start)
    if (end_num == start_num)
    {
        return true;
    }
    // This continues the cycle checking
    // Key is to use the existing locked pairs, checking for the end [] in the array
    for (int i = 0; i < candidate_count; i++)
    {
        // Checks to see if loser was a winner over someone and to follow the path
        if (locked[end_num][i])
        {
            // Uses the original start number, but the new end_num
            if (check_cycle(i, start_num))
            {
                return true;
            }
        }
    }
    return false;
}




//MY OLD CHECK
//bool check_cycle(int pair_counter, int start_num, int a)
//    for (int i = 0; i < pair_counter; i++)
//    {
//        for (int j = 0; j < i + 1; j++)
//        {
//            //Checks is a winner was the previous loser
//            if (pairs[j].winner == a)
//            {
                  //Protects against empty
//                if (pairs[j].winner != pairs[j].loser)
//                {
                        // Set new loser
//                    a = pairs[j].loser;
//                }
//                else
//                {
//                    return false;
//                }
                    // 
//                if (a == start_num)
//                {
//                    return true;
//                }
//                else
//                {
//                    if (check_cycle(pair_counter, start_num, a))
//                    {
//                        return true;
//                    }
//                }
//            }
//        }
//    }
//    return false;
//}
//
//




//OLD Lock
//{
//    // TODO
//    // Calculates the length of pairs list
//    int pair_counter = 0;
//    int k = 0;
//    while (pairs[k].winner || pairs[k].loser)
//    {
//        k++;
//        pair_counter++;
//    }
//    int winners[pair_counter];
//    int losers[pair_counter];
//    int counter = 0;
//
//    // Loops through every pair in pairs
//    for (int i = 0; i < pair_counter; i++)
//    {
//        int start_num = pairs[i].winner;
//        int a = pairs[i].loser;
//        bool not_cycle = true;
//        bool not_end = true;
//        while (not_end)
//        {
//            for (int j = 0; j < i + 1; j++)
//            {
//                if (pairs[j].winner == a)
//                {
//                    // Helps prevent loops with empty pairs (0, 0)
//                    // Otherwise, continues in the next step of searching for loop
//                    if (pairs[j].winner != pairs[j].loser)
//                    {
//                        a = pairs[j].loser;
//                    }
//                    else
//                    {
//                        not_end = false;
//                    }
//                }
//                else if (a == start_num)
//                {
//                    not_end = false;
//                    not_cycle = false;
//                }
//                else
//                {
//                    not_end = false;
//                }
//            }
//        }
//        winners[i] = pairs[i].winner;
//        losers[i] = pairs[i].loser;
//
//        if (not_cycle)
//        {
//            locked[pairs[i].winner][pairs[i].loser] = true;
//        }
//    }
//    return;
//}

