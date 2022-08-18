user_text = input("Text: ")

letters = 0
# Since the first word doesn't start with a space, need to count it here
words = 1
sentences = 0
for char in user_text:
    if char.isalpha():
        letters += 1
    elif char.isspace():
        words += 1
    elif ord(char) in [33, 46, 63]:
        sentences += 1

L = (letters / words) * 100
S = (sentences / words) * 100
index = round(0.0588 * L - 0.296 * S - 15.8)

if index < 1:
    print("Before Grade 1")
elif index > 15:
    print("Grade 16+")
else:
    print(f"Grade {index}")