while True:
    try:
        user_num = input("Number: ")
        card_num = int(user_num)
        card_type = user_num[0:2]
        break
    except ValueError:
        print("Enter numbers only; no hyphens, no spaces, just numbers")
sum = 0
for number in user_num[-2::-2]:
    doubled = int(number) * 2
    if doubled < 10:
        sum += doubled
    else:
        string = str(doubled)
        sum += int(string[0]) + int(string[1])
for number in user_num[-1::-2]:
    sum += int(number)
if sum % 10 != 0:
    print("INVALID")
else:
    c_len = len(user_num)
    if card_type[0] == "4" and c_len in [13, 16]:
        print("VISA")
    elif card_type in ["34", "37"] and c_len == 15:
        print("AMEX")
    elif (int(card_type) > 50 and int(card_type) < 56) and c_len == 16:
        print("MASTERCARD")
    else:
        print("INVALID")

